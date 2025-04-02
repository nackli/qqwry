#pragma once
/**
 * @author nackli <nackli@163.com>
 * @version 1.0
 * @copyright 2025 Your Company Name. All rights reserved.
 * @License: MIT (https://opensource.org/licenses/MIT)
 * @Created: 2025-03-20
 */

#ifdef _WIN32
#include <Windows.h>
#else
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

