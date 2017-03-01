#include "seastate.hh"

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

static uint64_t mulmod(uint64_t a, uint64_t b) {
    return __mulmod(a, b, UINT64_MAX);
    //return ((__uint128_t)a * b) % UINT64_MAX; // FIXME Does not work
}

uint64_t SeaState::finalize() const
{
    return f(a ^ b ^ c ^ d ^ length);
}

void SeaState::update(uint64_t n, uint64_t len)
{
    uint64_t new_a = f(a ^ n);
    a = b;
    b = c;
    c = d;
    d = new_a;
    length += len;
}

uint64_t SeaState::f(const uint64_t x) const
{
    static const uint64_t p = 0x6eed0e9da4d94a4fLLU;
    uint64_t f1 = mulmod(x, p);
    uint64_t fa = (f1 >> 32);
    uint64_t fb = (f1 >> 60);
    uint64_t sh =  fa >> fb;
    uint64_t f2 = f1 ^ sh;
    return mulmod(f2, p);
}
