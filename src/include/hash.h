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

};