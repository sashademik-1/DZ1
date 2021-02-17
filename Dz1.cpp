#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>

struct Block {
    uint64_t shortstr = 0;
};

Block operator^(Block &block, uint64_t key) {
    block.shortstr = block.shortstr ^ key;
    return block;
}

void operator>>(Block &block, unsigned int sdvig) {
    block.shortstr = (block.shortstr << (64 - sdvig)) | (block.shortstr >> sdvig);
}

void operator<<(Block &block, unsigned int sdvig) {
    block.shortstr = (block.shortstr >> (64 - sdvig)) | (block.shortstr << sdvig);
}

void Code(std::string ifs, std::string ofs) {
    std::ifstream in(ifs, std::ios::binary);
    std::string text;
    in.seekg(0, std::ios::end);
    size_t sizebytes = in.tellg();
    in.seekg(0, std::ios::beg);
    text.resize(sizebytes);
    in.read(&text[0], sizebytes);
    in.close();
    while (text.length() % 8 != 0) {
        text.push_back('\0');
    }
    Block block;
    block.shortstr = 0;
    std::ofstream out(ofs, std::ios::binary);
    int a = 0, b = 0;
    for (int i = 0; i < text.length(); i++) {
        block.shortstr += (text[i]);
        block >> 8;
        if (i % 8 == 7) {
            uint64_t shfr = 0;
            for (int i = 0; i < 4; i++) {
                shfr += rand();
                shfr = shfr << 16;
            }
            a = rand() % 65;
            b = rand() % 2;

            if (b == 1) {
                block << a;
            } else {
                block >> a;
            }
            out.write(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t));
            block.shortstr = 0;
        }
    }
    out.close();
}

void Decode(std::string ifs, std::string ofs) {
    std::ifstream in(ifs, std::ios::binary);
    std::ofstream out(ofs, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t sizebytes = in.tellg();
    in.seekg(0, std::ios::beg);
    Block block;
    int a = 0, b = 0;
    for (int i = 1; in.read(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t)); i++) {
        uint64_t shfr = 0;
        for (int b = 0; b < 4; b++) {
            shfr += rand();
            shfr = shfr << 16;
        }
        a = rand() % 65;
        b = rand() % 2;
        if (b == 1) {
            block >> a;
        } else {
            block << a;
        }
        if (i == (sizebytes / 8)) {
            std::string str(reinterpret_cast<char *>(&block.shortstr));
            out.write(reinterpret_cast<char *>(&block.shortstr), str.size());
            continue;
        }
        out.write(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t));
        block.shortstr = 0;
    }
    in.close();
    out.close();
}
int main(int argc, char *argv[]) {
    std::string mode = argv[2];
    if (mode == "code") {
        srand(std::stoi(argv[1]));
        Code(argv[3], argv[4]);
    }

    if (mode == "decode") {

        srand(std::stoi(argv[1]));
        Decode(argv[3], argv[4]);
    }
    return 0;
}