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

public:
    MD5()
    {

    }

    void reset()
    {
        h0 = 0x67452301;
        h1 = 0xEFCDAB89;
        h2 = 0x98BADCFE;
        h3 = 0x10325476;
    }

    std::string hash(const std::string &input)
    {
        reset();

        uint64 inputLen = input.length();
        uint64 bitsLen = inputLen * 8;

        // Padding
        uint64 paddedLen = ((inputLen + 8) / 64 + 1) * 64;
        uint8 *padded = new uint8[paddedLen];
        memcpy(padded, input.c_str(), inputLen);
        padded[inputLen] = 0x80;

        for (uint64 i = inputLen + 1; i < paddedLen - 8; i++)
        {
            padded[i] = 0;
        }

        // Append length
        for (int i = 0; i < 8; i++)
        {
            padded[paddedLen - 8 + i] = (uint8)(bitsLen >> (i * 8));
        }

        // Process blocks
        for (uint64 i = 0; i < paddedLen; i += 64)
        {
            processBlock(padded + i);
        }

        delete[] padded;

        // Convert to hex string
        uint8 digest[16];
        toBytes(h0, digest);
        toBytes(h1, digest + 4);
        toBytes(h2, digest + 8);
        toBytes(h3, digest + 12);

        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 16; i++)
        {
            ss << std::setw(2) << (int)digest[i];
        }

        return ss.str();
    }
};