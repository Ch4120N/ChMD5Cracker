#include <string>
#include <cstring>
#include <cstdint>
#include <sstream>
#include <iomanip>

class MD5{
private:
    typedef uint8_t uint8;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    const static uint32 s[64];
    const static uint32 K[64];

    uint32 h0, h1, h2, h3;

    static uint32 leftRotate(uint32 x, uint32 c)
    {
        return (x << c) | (x >> (32 - c));
    }

    static void toBytes(uint32 val, uint8 *bytes)
    {
        bytes[0] = (uint8)val;
        bytes[1] = (uint8)(val >> 8);
        bytes[2] = (uint8)(val >> 16);
        bytes[3] = (uint8)(val >> 24);
    }

    static uint32 toInt32(const uint8 *bytes)
    {
        return (uint32)bytes[0] | ((uint32)bytes[1] << 8) | ((uint32)bytes[2] << 16) | ((uint32)bytes[3] << 24);
    }

};