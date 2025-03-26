#pragma once
#ifdef _WIN32
#include <Windows.h>
#lese
#include <sys/mman.h>
#endif
#include <stdint.h>
class QqwryParse
{
public:
	QqwryParse();
	~QqwryParse();
	bool loadQqwryData(const char* szFilePath);
	bool getLocateAddr(const char* szIpAddr, char* szAddr, char* szTelecom);
	bool getLocateAddr(const uint32_t uIpAddr, char* szAddr, char* szTelecom);
	void closeMemHandle();
private:
#ifdef _WIN32
	HANDLE m_hMapFile;
#else
	int m_iMapFile;
#endif
	char* m_pMemData;
};

