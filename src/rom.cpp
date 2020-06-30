#include "rom.hpp"

int Cartridge::loadRom(char *romfile)
{
    unsigned int nb_read;

    SDL_RWops *rw = SDL_RWFromFile(romfile, "rb");
    assert(rw != nullptr);
    size_ = (unsigned int)SDL_RWsize(rw);
    nb_read = SDL_RWread(rw, (uint8_t*)&rom_, sizeof(char), size_);
    if (nb_read == size_)
        return 0;
    return -1;
}

void Cartridge::loadHeader()
{
    memcpy(&(romheader_), &rom_[header_start], header_end - header_start);
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
    uint8_t ret = loadRom(filename);
    assert(ret == 0);
    loadHeader();
    displayHeader();
    return 0;
}

s_romHeader &Cartridge::getHeader()
{
    return romheader_;
}
