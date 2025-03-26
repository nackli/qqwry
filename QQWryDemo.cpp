#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include "QqwryParse.h"


// 示例用法
int main() {
    char szLoca[64] = { 0 };
    char szTelecom[64] = { 0 };
    QqwryParse qwry;
    qwry.loadQqwryData("qqwry.dat");
    qwry.getLocateAddr("222.83.21.122", szLoca, szTelecom);
    return 0;
}