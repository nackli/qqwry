#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>
#ifdef _WIN32
#include <WS2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#else
    #include <arpa/inet.h> // 用于字节序转换
#endif
#include "qqwry.h"

class QQWry {
public:
    QQWry(const std::string& filename) {
        file.open(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("无法打开文件");
        }

        // 读取前8字节获取索引范围
        file.read(reinterpret_cast<char*>(&first_index_offset), 4);
        file.read(reinterpret_cast<char*>(&last_index_offset), 4);
        first_index_offset = le32toh(first_index_offset);
        last_index_offset = le32toh(last_index_offset);
        index_count = (last_index_offset - first_index_offset) / 7 + 1;
    }

    ~QQWry() {
        if (file.is_open()) {
            file.close();
        }
    }

    std::string query(const std::string& ip_str) {
        uint32_t ip = convertIP(ip_str);
        uint32_t index = findIndex(ip);
        if (index == (uint32_t)-1) return "未找到IP";

        uint32_t record_offset = getRecordOffset(index);
        return readRecord(record_offset, ip);
    }

private:
    std::ifstream file;
    uint32_t first_index_offset;
    uint32_t last_index_offset;
    uint32_t index_count;

    // 将点分十进制IP转换为大端序的32位整数
    uint32_t convertIP(const std::string& ip_str) {
        struct in_addr addr;
        inet_pton(AF_INET, ip_str.c_str(), &addr);
        return ntohl(addr.s_addr);
    }

    // 二分查找定位索引
    uint32_t findIndex(uint32_t ip) {
        uint32_t low = 0, high = index_count - 1;
        uint32_t mid, start_ip;
        uint32_t pos;

        while (low <= high) {
            mid = (low + high) / 2;
            pos = first_index_offset + mid * 7;
            file.seekg(pos);
            file.read(reinterpret_cast<char*>(&start_ip), 4);
            start_ip = ntohl(start_ip);

            if (start_ip > ip) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        if (high < 0) return -1;
        return high;
    }

    // 获取记录偏移量
    uint32_t getRecordOffset(uint32_t index) {
        uint32_t pos = first_index_offset + index * 7 + 4;
        file.seekg(pos);

        unsigned char buf[3];
        file.read(reinterpret_cast<char*>(buf), 3);
        return (buf[2] << 16) | (buf[1] << 8) | buf[0];
    }

    // 读取记录信息
    std::string readRecord(uint32_t offset, uint32_t ip) {
        file.seekg(offset);

        // 读取结束IP
        uint32_t end_ip;
        file.read(reinterpret_cast<char*>(&end_ip), 4);
        end_ip = ntohl(end_ip);
        if (ip > end_ip) return "IP范围无效";

        // 读取国家地区信息
        std::string country = readString(true);
        std::string area = readString(true);

        return country + " " + area;
    }

    // 读取字符串（处理重定向）
    std::string readString(bool readMode) {
        unsigned char mode;
        if (readMode) {
            file.read(reinterpret_cast<char*>(&mode), 1);
            if (mode == 0x01 || mode == 0x02) {
                unsigned char buf[3];
                file.read(reinterpret_cast<char*>(buf), 3);
                uint32_t new_offset = (buf[2] << 16) | (buf[1] << 8) | buf[0];

                uint32_t backup = file.tellg();
                file.seekg(new_offset);
                std::string result = readString(false);
                file.seekg(backup);
                return result;
            }
            file.seekg(-1, std::ios::cur); // 回退模式字节
        }

        // 直接读取字符串
        std::string str;
        char c;
        while (file.get(c) && c != '\0') {
            str += c;
        }
        return str;
    }

    // 小端转换（可根据平台调整）
    uint32_t le32toh(uint32_t val) {
        return (val >> 24) |
            ((val << 8) & 0x00FF0000) |
            ((val >> 8) & 0x0000FF00) |
            ((val >> 24) & 0x000000FF);
    }
};

// 示例用法
int main() {
    FILE* fr = nullptr;
    fopen_s(&fr,"qqwry.dat", "rb");
    char szLoca[64] = { 0 };
    char szLoca1[64] = { 0 };
    qqwry_get_location(szLoca, szLoca1, "222.83.21.122", fr);
    return 0;
}