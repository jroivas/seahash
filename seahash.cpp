#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>

#include "seastate.hh"

uint64_t readData(std::ifstream &s, uint64_t &i)
{
    uint64_t res = 0;

    while (!s.eof() && s.good() && i < sizeof(uint64_t)) {
        char c = s.get();
        if (!s.eof()) {
            res |= uint64_t(uint8_t(c)) << (i * 8);
            ++i;
        }
    }
    return res;
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
    std::cout << std::hex << std::setw(16) << std::setfill('0') << state.finalize() << "\n";
    return 0;
}
