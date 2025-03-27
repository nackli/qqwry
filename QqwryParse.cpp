#include "QqwryParse.h"
#include <iostream>
#ifndef _WIN32
#include<sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#endif
/*********************************************************************************************/
#define BE_32(x) ((((uint8_t*)(x))[0]<<24) |\
                  (((uint8_t*)(x))[1]<<16) |\
                  (((uint8_t*)(x))[2]<<8) |\
                  ((uint8_t*)(x))[3])

#define LE_32(x) ((((uint8_t*)(x))[3]<<24) |\
                  (((uint8_t*)(x))[2]<<16) |\
                  (((uint8_t*)(x))[1]<<8) |\
                  ((uint8_t*)(x))[0])

#define LE_24(x) ((((uint8_t*)(x))[2]<<16) |\
                  (((uint8_t*)(x))[1]<<8) |\
                  ((uint8_t*)(x))[0])

#define REDIRECT_TYPE_ONE			0x01
#define REDIRECT_TYPE_TWO			0x02
#define FLAG_SEVEN					0x07
#ifdef _WIN32
HANDLE OnOpenFile(const char *szFileName, uint32_t dwDesiredAccess,
	uint32_t dwCreationDisposition, uint32_t dwFlagsAndAttributes)
{
	//wstring wstrWriteFile(strFileName.begin(), strFileName.end());
	return CreateFileA(szFileName, dwDesiredAccess, 0, NULL,
		dwCreationDisposition, dwFlagsAndAttributes, NULL);
}
#else
int OnOpenFile(const char *szFileName, uint32_t dwDesiredAccess,
	uint32_t dwCreationDisposition)
{
    int fd = open(szFileName, dwDesiredAccess, dwCreationDisposition);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
	return fd;
}
#endif

static bool OnParseIp(const char* strIpAddr, uint8_t parts[4])
{
	if (!strIpAddr)
		return false;

	char szIpTemp[256];
	strcpy_s(szIpTemp, strIpAddr);
	szIpTemp[strlen(strIpAddr)] = '\0';

	uint8_t uPartCnt = 0;
	char* token = strtok(szIpTemp, ".");
	while (token != NULL) {
		if (uPartCnt >= 4)
			return false; // 超过四个部分

		size_t len = strlen(token);
		if (len == 0 || len > 3)
			return false;

		// 检查是否为纯数字
		for (char* p = token; *p; p++) {
			if (!isdigit((unsigned char)*p))
				return false;
		}

		// 检查前导零
		if (len > 1 && token[0] == '0')
			return false;

		int num = atoi(token);
		if (num < 0 || num > 255)
			return false;

		parts[uPartCnt++] = num;
		token = strtok(NULL, ".");
	}

	if (uPartCnt != 4)
		return false;
	return true;
}

static uint32_t OnIp2Uint32(const char* ip) {
	uint8_t szIpAddr[4];
	OnParseIp(ip, szIpAddr);
	return BE_32(szIpAddr);
}

static uint32_t OnFindIpIndex(const uint32_t uIpAddr,const char *pMemCtx)
{
	uint32_t uIndexIp = 0;
	const char* pMemData = nullptr;
	uint32_t iStartIndex = LE_32(pMemCtx);
	uint32_t iEndIndex = LE_32(pMemCtx + 4);
	uint32_t iMidIndex = 0;
	while (1) {
		if (FLAG_SEVEN == iEndIndex - iStartIndex)
			break;

		iMidIndex = iEndIndex / FLAG_SEVEN - iStartIndex / FLAG_SEVEN;

		iMidIndex = (iMidIndex + iMidIndex % 2) / 2;

		iMidIndex = iStartIndex + iMidIndex * FLAG_SEVEN;
		pMemData = (char *)pMemCtx + iMidIndex;
		uIndexIp = LE_32(pMemData);
		if (uIndexIp == uIpAddr)
			break;

		else if (uIndexIp < uIpAddr)
			iStartIndex = iMidIndex;

		else
			iEndIndex = iMidIndex;
	}
	if (uIndexIp > uIpAddr)
		pMemData = pMemCtx + iStartIndex;
	return (uint32_t)LE_24(pMemData + 4);
}

static bool OnGetTelecomName(char* szTeleName, const char* pData,const uint32_t uDataIndex)
{
	if (!szTeleName || !pData)
		return false;
	uint32_t uDataOffset = 0;

	const char* pTempData = pData;
	if (uDataIndex)
		pTempData += uDataIndex;

	char cFlag = *pTempData;
	pTempData++;
	switch (cFlag){

	case REDIRECT_TYPE_TWO:
	case REDIRECT_TYPE_ONE:
		uDataOffset = LE_24(pTempData);
		pTempData = pData + uDataOffset;
		break;
	default:
		*szTeleName++ = cFlag;
		break;
	}
	if (cFlag) {
		while (*pTempData)
			*szTeleName++ = *pTempData++;
	}
	return true;
}

/*-----------------------------------------
局域网IP地址范围
A类：10.0.0.0-10.255.255.255
B类：172.16.0.0-172.31.255.255
C类：192.168.0.0-192.168.255.255
-------------------------------------------*/
static int OnIsPrivateIpAddr(uint32_t uIpAddr) {
	if((0x0A000000 <= uIpAddr && uIpAddr <= 0x0aFFFFFF) ||
		(0xAC100000 <= uIpAddr && uIpAddr <= 0xAC1FFFFF) ||
		(0xC0A80000 <= uIpAddr && uIpAddr <= 0xC0A8FFFF))
		return true;
	return false;
}
/*********************************************************************************************/

QqwryParse::QqwryParse():m_pMemData(nullptr){
#ifdef _WIN32
	m_hMapFile = nullptr;
#else
	m_iMapFile = 0;
#endif
}

QqwryParse::~QqwryParse() {
	this->closeMemHandle();
}

bool QqwryParse::loadQqwryData(const char* szFilePath) {

	if (!szFilePath || !szFilePath[0])
		return false;
#ifdef _WIN32
	HANDLE hWriteFile = OnOpenFile(szFilePath, GENERIC_READ, OPEN_ALWAYS, 0);
	if (hWriteFile == INVALID_HANDLE_VALUE)
		return false;

	//IP�ļ�δ����4G�����Ժ��Ը�λ
	DWORD dwFileSize = GetFileSize(hWriteFile, nullptr);

	m_hMapFile = CreateFileMappingA(hWriteFile, NULL, PAGE_READONLY, 0, dwFileSize,
		nullptr);

	CloseHandle(hWriteFile);
	hWriteFile = nullptr;

	if (m_hMapFile == NULL) {
		std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
		return false;
	}
	// ӳ���ڴ�
	m_pMemData = (char*)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0,
		dwFileSize);
#else
	m_iMapFile = OnOpenFile(szFilePath, O_RDONLY, 7777);
	struct stat fileStat;
	fstat(m_iMapFile,&fileStat);
    // 将文件映射到内存
    m_pMemData = (char *)mmap(NULL, fileStat.st_size, PROT_READ, MAP_SHARED, m_iMapFile, 0);
    if (m_pMemData == MAP_FAILED) {
        perror("mmap");
        close(m_iMapFile);
        exit(EXIT_FAILURE);
    }

#endif
	if (!m_pMemData)
		return false;

	return true;
}

bool QqwryParse::getLocateAddr(const char* szIpAddr, char* szAddr, char* szTelecom)
{
	if (!szIpAddr || !szIpAddr[0] || !szAddr || !szTelecom)
		return false;
	return getLocateAddr(OnIp2Uint32(szIpAddr), szAddr, szTelecom);;
}

bool QqwryParse::getLocateAddr(const uint32_t uIpAddr, char* szAddr, char* szTelecom)
{
	if (!uIpAddr || !szAddr || !szTelecom)
		return false;
	if (OnIsPrivateIpAddr(uIpAddr))
		return false;
	uint32_t uDataIndex = 0;
	uint32_t uTelecomOffset = 0;
	uint32_t uIndexIp = OnFindIpIndex(uIpAddr, m_pMemData);
	char* pMemData = m_pMemData + uIndexIp + 4;
	char cFlag = *pMemData;
	pMemData++;
	if (cFlag == REDIRECT_TYPE_ONE)	{
		uDataIndex = LE_24(pMemData);
		pMemData = (m_pMemData + uDataIndex);
		cFlag = *pMemData;
		pMemData++;
	}

	if (REDIRECT_TYPE_TWO == cFlag) {
		uTelecomOffset = uDataIndex + 4;
		uDataIndex = LE_24(pMemData);
		pMemData = (m_pMemData + uDataIndex); 
		while (*pMemData)
			*szAddr++ = *pMemData++;
		if (szTelecom)
			OnGetTelecomName(szTelecom, m_pMemData, uTelecomOffset);
	}
	else {
		*szAddr = cFlag;
		while (*pMemData) {
			*(++szAddr) = *pMemData++;
		}
		if (szTelecom)
			OnGetTelecomName(szTelecom, m_pMemData, (uint32_t)(pMemData - m_pMemData + 1));
	}
	return true;
}

void QqwryParse::closeMemHandle()
{
#ifdef _WIN32	
	if(m_pMemData)
	{
		UnmapViewOfFile(m_pMemData);
		m_pMemData = nullptr;
	}
	if(m_hMapFile)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = nullptr;
	}
#else
	struct stat fileStat;
	fstat(m_iMapFile,&fileStat);
	munmap(m_pMemData, fileStat.st_size); 
	close(m_iMapFile);
#endif	
}