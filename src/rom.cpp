#include "rom.hpp"

int Cartridge::loadRom(char *romfile)
{
    unsigned int nb_read;

    SDL_RWops *rw = SDL_RWFromFile(romfile, "rb");
    assert(rw != nullptr);
    size_ = (unsigned int)SDL_RWsize(rw);
    nb_read = SDL_RWread(rw, (uint8_t*)&cartridgeType_->rom_, sizeof(char), size_);
    SDL_RWclose(rw);
    if (nb_read == size_)
        return 0;
    return -1;
}

int Cartridge::loadHeader(char *romfile)
{
    unsigned int nb_read;

    std::array<uint8_t, header_end> tmpHeader;

    SDL_RWops *rw = SDL_RWFromFile(romfile, "rb");
    assert(rw != nullptr);
    nb_read = SDL_RWread(rw, (uint8_t *)&tmpHeader, sizeof(char), header_end);
    SDL_RWclose(rw);
    if (nb_read == header_end) {
        memcpy(&romheader_, &tmpHeader[header_start], header_end - header_start);
        return 0;
    }
    return -1;
}

void Cartridge::displayHeader()
{
    printf("rom name: %s\n", romheader_.title);
    printf("cartridge type: %d\n", romheader_.cartridgeType);
    printf("rom size: %dKB\n", 32 << romheader_.romSize);
    printf("ram size: %d\n", romheader_.ramSize << 2);
}

int Cartridge::initRom(char *filename)
{
    uint8_t ret = loadHeader(filename);
    assert(ret == 0);
    loadcartridgeType();
    ret = loadRom(filename);
    assert(ret == 0);
    displayHeader();
    return 0;
}

s_romHeader &Cartridge::getHeader()
{
    return romheader_;
}
