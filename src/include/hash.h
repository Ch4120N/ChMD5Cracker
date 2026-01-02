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

    void processBlock(const uint8 *block)
    {
        uint32 a = h0;
        uint32 b = h1;
        uint32 c = h2;
        uint32 d = h3;
        uint32 f, g, temp;

        uint32 M[16];
        for (int i = 0; i < 16; i++)
        {
            M[i] = toInt32(block + i * 4);
        }

        // Main loop
        for (uint32 i = 0; i < 64; i++)
        {
            if (i < 16)
            {
                f = (b & c) | ((~b) & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            }
            else
            {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + leftRotate((a + f + K[i] + M[g]), s[i]);
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }

};