#include <iostream>
#include <fstream>
#include <cstdint>

// Multiply can overflow, need to take that in account
static uint64_t __mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t x = 0;
    uint64_t y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1) x = (x + y) % mod;
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}

class SeaState {
public:
    SeaState() :
      a(0x16f11fe89b0d677cLLU),
      b(0xb480a793d8e6c86cLLU),
      c(0x6fe2e5aaf078ebc9LLU),
      d(0x14f994a4c5259381LLU),
      l(0)
    {
    }
    SeaState(uint64_t s1, uint64_t s2, uint64_t s3, uint64_t s4) : l(0) {
        a = s1;
        b = s2;
        c = s3;
        d = s4;
    }

    uint64_t result() const {
        return f(a ^ b ^ c ^ d ^ l);
    }

    void update(uint64_t n, uint64_t len=8) {
        uint64_t na = f(a ^ n);
        a = b;
        b = c;
        c = d;
        d = na;
        l += len;
    }

    uint64_t mulmod(uint64_t a, uint64_t b) const {
        return __mulmod(a, b, UINT64_MAX);
        //return ((__uint128_t)a * b) % UINT64_MAX; // FIXME Does not work
    }

    uint64_t f(uint64_t x) const {
        static const uint64_t p = 0x6eed0e9da4d94a4fLLU;
        uint64_t f1 = mulmod(x, p);
        uint64_t fa = (f1 >> 32);
        uint64_t fb = (f1 >> 60);
        uint64_t sh =  fa >> fb;
        uint64_t f2 = f1 ^ sh;
        return mulmod(f2, p);
    }

private:
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;
    uint64_t l;
};

uint64_t readData(std::ifstream &s, uint64_t &len)
{
    uint64_t res = 0;
    uint64_t i = 0;
    while (!s.eof() && s.good() && i < sizeof(uint64_t)) {
        char c = s.get();
        if (!s.eof()) {
            res <<= 8;
            ++len;
            res |= uint64_t(c);
            ++i;
        }
    }
    // Padding
    while (i < sizeof(uint64_t)) {
        res <<= 8;
        ++i;
    }
    // Reverse
    uint64_t res2 = 0;
    i = 0;
    while (i < sizeof(uint64_t)) {
        res2 <<= 8;
        res2 |= (res & 0xFF);
        res >>= 8;
        ++i;
    }
    return res2;
}

int main(int argc, char **argv)
{
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " fname\n";
        return 1;
    }

    SeaState state;
    std::ifstream src(argv[1], std::ifstream::in | std::ifstream::binary);
    while (!src.eof() && src.good()) {
        uint64_t l = 0;
        uint64_t d = readData(src, l);
        if (l == 0) break;
        state.update(d, l);
    }
    src.close();
    std::cout << std::hex << state.result() << "\n";
    return 0;
}
