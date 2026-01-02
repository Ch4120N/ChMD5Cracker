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

// MD5 constants
const uint32_t MD5::s[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

const uint32_t MD5::K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
