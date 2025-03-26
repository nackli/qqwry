#pragma once
#ifdef _WIN32
#include <Windows.h>
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
private:
#ifdef _WIN32
	HANDLE m_hMapFile;
#else
#endif
	char* m_pMemData;
};

