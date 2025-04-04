/**
 * @author nackli <nackli@163.com>
 * @version 1.0
 * @copyright 2025 nackli. All rights reserved.
 * @License: MIT (https://opensource.org/licenses/MIT)
 * @Created: 2025-03-20
 */

#include "QqwryParse.h"
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <iostream>
#ifndef _WIN32
#include <iconv.h>
#include <cstring>
#include <assert.h>
#include <unistd.h>
#endif
// 示例用法


int main(){
    // std::locale::global(std::locale("en_US.utf8")); // 或者使用适当的locale，例如"zh_CN.utf8"
    // std::cout.imbue(std::locale()); // 应用loqale设置到cout
    char szLoca[64] = { 0 };
    char szTelecom[64] = { 0 };
    QqwryParse qwry;
   // setlocale(LC_ALL, "chs");
    qwry.loadQqwryData("qqwry.dat");
    qwry.getLocateAddr("222.83.21.122", szLoca, szTelecom);
  //  mbstowcs()
    std::cout << szLoca << " --" << szTelecom<< std::endl;
#ifdef _WIN32
    system("pause");
#else
    pause();
#endif
    return 0;
}