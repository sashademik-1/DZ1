#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <string>
#include <vector>
#include <utility>

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

void Code(std::string ifs, std::string ofs, const std::vector<std::pair<char, int>> vecin) {
    std::ifstream in(ifs, std::ios::binary);
    std::string text;
    in.seekg(0, std::ios::end);
    size_t sizebytes = in.tellg();
    in.seekg(0, std::ios::beg);
    text.resize(sizebytes);
    in.read(&text[0], sizebytes);
    in.close();
    while (text.length() % 8 != 0) {
        text.push_back(static_cast<char >(97));
    }
    Block block;
    uint8_t size = text.length() - sizebytes;
    block.shortstr = 0;
    std::ofstream out(ofs, std::ios::binary);
    auto it = vecin.begin();
    out.write(reinterpret_cast<char *>(&size), sizeof(uint8_t));
    for (int i = 0; i < text.length(); i++) {
        block.shortstr += (text[i]);
        block >> 8;
        if (i % 8 == 7) {
            uint64_t shfr = 0;
            for (int i = 0; i < 4; i++) {
                shfr += rand();
                shfr = shfr << 16;
            }
            if (it == vecin.end()) {
                it = vecin.begin();
            }
            block ^ shfr;
            if (it->first == 62) {
                block >> it->second;
            } else {
                block << it->second;
            }
            it++;
            out.write(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t));
            block.shortstr = 0;
        }
    }
    out.close();
}

void Decode(std::string ifs, std::string ofs, const std::vector<std::pair<char, int>> vecin) {
    std::ifstream in(ifs, std::ios::binary);
    std::ofstream out(ofs, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t sizebytes = in.tellg();
    in.seekg(0, std::ios::beg);
    Block block;
    uint8_t sizeprobe = 0;
    in.read(reinterpret_cast<char *>(&sizeprobe), sizeof(uint8_t));
    auto it = vecin.begin();
    for (int i = 1; in.read(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t)); i++) {
        uint64_t shfr = 0;
        for (int b = 0; b < 4; b++) {
            shfr += rand();
            shfr = shfr << 16;
        }
        if (it == vecin.end()) {
            it = vecin.begin();
        }
        if (it->first == 62) {
            block << it->second;
        } else {
            block >> it->second;
        }
        it++;
        block ^ shfr;
        if (i >= (sizebytes / 8)) {
            block.shortstr = block.shortstr << 8 * sizeprobe;
            block.shortstr = block.shortstr >> 8 * sizeprobe;
            out.write(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t) - sizeprobe);
            continue;
        }
        out.write(reinterpret_cast<char *>(&block.shortstr), sizeof(uint64_t));
        block.shortstr = 0;
    }
    in.close();
    out.close();
}

void stringtovector(std::vector<std::pair<char, int>> &vecin, std::string str) {
    std::string::iterator t = str.begin();
    while (t != str.end()) {
        int integer = 0;
        char oper;
        oper = *t;
        t++;
        while (*t != 62 && *t != 60 && *t != 0) {
            integer *= 10;
            integer += (*t - '0');
            t++;
        }
        auto p = std::make_pair(oper, integer);
        vecin.push_back(p);
    }
}

int main(int argc, char *argv[]) {

    std::vector<std::pair<char, int>> vecin;
    stringtovector(vecin, argv[3]);
    auto start1 = std::chrono::system_clock::now();
    // Code("text.txt", "textout.txt",vecin);
    Decode("textout.txt", "decode.txt", vecin);
    auto end1 = std::chrono::system_clock::now();
    auto time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);
    std::cout << "The time " << time1.count() << std::endl;
//    std::string mode = argv[2];
//    std::vector<std::pair<char, int>> vecin;
//    stringtovector(vecin, argv[3]);
//    if (mode == "code") {
//        Code(argv[4], argv[5], vecin);
//    }
//
//    if (mode == "decode") {
//        Decode(argv[4], argv[5], vecin);
//    }
    return 0;
    //argv[2]-режим работы
    //argv[3]-вектор инициализации
    //argv[4]-файл для чтения
    //argv[5]-файл для записи

    //не уверен что правильно я понял, но мой ключ криптосистемы работает по циклу;
//если блоков больше, чем длинна ключа, то криптосистема начинает цикл сначала;
// (условие на проверку корректности ключа не вводил)
//вводится направление, кол-во битов для сдвига,ключ должен заканчиваться числом;
// например >20<30;

    // текст на котором проверял THE OREL REGION IS SITUATED ON THE CENTRAL RUSSIAN UPLAND, IN EUROPEAN RUSSIA. IT BORDERS ON THE KURSK, BRYANSK, KALUGA, TULA AND LIPETSK REGIONS. THE MAIN RIVER IN THE REGION IS THE OKA WITH ITS TRIBUTARIES. THE CLIMATE IS TEMPERATE CONTINENTAL. JANUARY TEMPERATURES AVERAGE -6.5. JULY IS THE HOTTEST MONTH WITH AVERAGE TEMPERATURES AT 18.4. MODERATE RAINFALL OCCURS DURING A YEAR. THE REGION IS HOME TO A PROTECTED AREA, THE ORLOVSKOYE POLESYE NATIONAL PARK. THE LEGISLATIVE BRANCH IS REPRESENTED BY THE OREL REGIONAL COUNCIL OF PEOPLES DEPUTIES, WHICH IS THE PERMANENT, SUPREME AND ONLY BODY OF LEGISLATIVE AUTHORITY IN THE REGION. THE OREL REGIONAL COUNCIL OF PEOPLES DEPUTIES HAS 50 DEPUTIES ELECTED FOR FIVE YEARS, WITH 25 OF THEM RUNNING IN SINGLE-MANDATE CONSTITUENCIES AND THE OTHER 25 IN THE SINGLE ELECTORAL DISTRICT, WHERE WINNERS ARE IDENTIFIED IN PROPORTION TO THE NUMBER OF VOTES CAST FOR LISTS OF DEPUTIES NOMINATED BY ELECTORAL ASSOCIATIONS. THE CURRENT OREL REGIONAL COUNCIL OF PEOPLES DEPUTIES WAS ELECTED IN SEPTEMBER 2016. ITS TERM EXPIRES IN SEPTEMBER 2021. THE EXECUTIVE BRANCH IS THE GOVERNMENT OF THE OREL REGION HEADED BY THE GOVERNOR. THE GOVERNOR OF THE OREL REGION IS THE REGIONS HIGHEST-RANKING OFFICIAL ELECTED FOR FIVE YEARS. HE FORMS AND HEADS THE GOVERNMENT OF THE OREL REGION. THE TERM OF OFFICE OF THE INCUMBENT GOVERNOR EXPIRES IN SEPTEMBER 2023. THE GOVERNMENT OF THE OREL REGION IS THE TOP PERMANENT EXECUTIVE AUTHORITY IN THE OREL REGION. THE ECONOMY OF THE OREL REGION IS REPRESENTED BY ALL TYPES OF ECONOMIC ACTIVITIES, THE LEADING OF WHICH ARE INDUSTRY, AGRICULTURE, HOUSING CONSTRUCTION, TRADE, TRANSPORT AND COMMUNICATIONS. THE INDUSTRIAL COMPLEX IS THE BACKBONE OF THE REGIONAL ECONOMY. INDUSTRIAL OUTPUT ACCOUNTS FOR OVER 20% OF THE REGIONAL GDP. THE MAJOR INDUSTRY IS MANUFACTURING, REPRESENTED BY THE METAL INDUSTRY AND THE PRODUCTION OF FINISHED METAL PRODUCTS, MACHINERY AND EQUIPMENT, VEHICLES AND AUTOMOTIVE EQUIPMENT, ELECTRIC EQUIPMENT, NON-METAL MINERAL COMMODITIES, RUBBER AND PLASTIC ARTICLES AND FOOD. CURRENTLY, CONSTRUCTION MATERIALS MANUFACTURING IS GROWING RAPIDLY. THE FOOD AND TEXTILE INDUSTRIES ARE ALSO ON THE RISE. THE MAJOR BUSINESSES IN THE REGION ARE THE OREL ENGINEERING PLANT, THE OREL MEDVEDEV FOUNDRY, OREL STEELWORKS, THE OREL METALS GROUP OF COMPANIES, THE ORELSTROY CONSTRUCTION COMPANY AND THE VOSTOK-SERVICE GROUP OF COMPANIES.


}