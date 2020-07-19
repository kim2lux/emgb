#include "gpu.hpp"
#include <stdint.h>
#include "utils.h"
#include <stdint.h>
static constexpr uint16_t bg_map_width = 0x20;

void Gpu::renderTile(int32_t idx, uint8_t startY, uint8_t startX, uint16_t mapStartAddr, uint16_t tileDataAddr)
{
    uint16_t posY = (startY / 8) * bg_map_width;
    if (unsign_ == true)
    {
        uint8_t u_tileIndex = cpu_.getMemory().read8bit(mapStartAddr + posY + (startX / 8));
        tileDataAddr += u_tileIndex * 16;
        assert(tileDataAddr < 0x8FFF);
    }
    else
    {
        int8_t tileIndex = (int8_t)cpu_.getMemory().read8bit(mapStartAddr + posY + (startX / 8));
        tileDataAddr += ((tileIndex + 0x80) * 16);
        assert(tileDataAddr < 0x97FF);
    }

    uint8_t positionInTile = (startY % 8) * 2;
    uint8_t upperByte = cpu_.getMemory().read8bit(tileDataAddr + positionInTile);
    uint8_t lowerByte = cpu_.getMemory().read8bit(tileDataAddr + positionInTile + 1);

    int color = 0;
    uint8_t bitPos = startX % 8;
    bitPos -= 7;
    bitPos = -bitPos;

    color |= ((upperByte >> bitPos) & 0x01);
    color <<= 1;
    color |= ((lowerByte >> bitPos) & 0x01);

    assert(((scanline_ * gameboy_width) + idx) < gameboy_width * gameboy_height);

    if (color == 0)
        pixels_[(scanline_ * gameboy_width) + idx] = SDL_MapRGB(window_surface_->format, 0xff, 0xff, 0xff);
    else if (color == 1)
        pixels_[(scanline_ * gameboy_width) + idx] = SDL_MapRGB(window_surface_->format, 0x44, 0x44, 0x44);
    else if (color == 2)
        pixels_[(scanline_ * gameboy_width) + idx] = SDL_MapRGB(window_surface_->format, 0xaa, 0xaa, 0xaa);
    else if (color == 3)
        pixels_[(scanline_ * gameboy_width) + idx] = SDL_MapRGB(window_surface_->format, 0, 0, 0);

    color = 0x00;
}

void Gpu::updateGpuRegister()
{
    unsign_ = false;
    lcdCtrl_ = cpu_.getMemory().read8bit(LCD_DISPLAY_CTRL);
    lcdEnable_ = isBitSet(lcdCtrl_, 7) ? true: false;
    windowsDisplayEnable_ = isBitSet(lcdCtrl_, 5) ? true: false;
    bgDisplayEnable_ = isBitSet(lcdCtrl_, 0) ? true: false;
    backgroudMap_ = isBitSet(lcdCtrl_, 3) ? bg_tile_map_display_select_high : bg_tile_map_display_select_low;
    windowMap_ = isBitSet(lcdCtrl_, 6) ? window_tile_display_map_select_high : window_tile_display_map_select_low;
    tileData_ = isBitSet(lcdCtrl_, 4) ? tile_data_addr_low : tile_data_addr_high;
    if (isBitSet(lcdCtrl_, 4))
    {
        unsign_ = true;
    }
    scanline_ = cpu_.getMemory().read8bit(LY_ADDR);

    scX_ = cpu_.getMemory().read8bit(SCX_ADDR);
    scY_ = cpu_.getMemory().read8bit(SCY_ADDR);
    winX_ = cpu_.getMemory().read8bit(WINX_ADDR);
    winY_ = cpu_.getMemory().read8bit(WINY_ADDR);
}

void Gpu::renderBackground()
{
    //updateGpuRegister(); update LcdCtrol within rendering func
    uint8_t startY = scanline_ + scY_;
    uint8_t startX = 0;

    for(int32_t idx = 0; idx < 160; ++idx) {
        startX = idx + scX_;
        renderTile(idx, startY, startX, backgroudMap_, tileData_);
    }
}

void Gpu::renderWindow()
{
    if (winX_ > 7) {
        winX_ -= 7;
    }
    else {
        winX_ = 0;
    }
    for (int32_t idx = 0; idx < 160; idx += 1)
    {
        uint8_t startX = idx - winX_;
        uint8_t startY = scanline_ - winY_;
        renderTile(idx, startY, startX, windowMap_, tileData_);
    }
}


void Gpu::renderSprite()
{
    for (uint8_t idx = 0; idx < 40; ++idx)
    {
        uint8_t posY = cpu_.getMemory().read8bit(OAM_START_ADDR + (idx * 4)) - 16;

        if (posY <= scanline_ && posY + 8 >= scanline_)
        {
            uint8_t posX = cpu_.getMemory().read8bit(OAM_START_ADDR + (idx * 4) + 1) - 8;
            uint8_t spriteTileIndex = cpu_.getMemory().read8bit(OAM_START_ADDR + (idx * 4) + 2);
            uint8_t spriteTileAttribute = cpu_.getMemory().read8bit(OAM_START_ADDR + (idx * 4) + 3);
            bool yFlip = (spriteTileAttribute >> 6) & 0x01;
            bool xFlip = (spriteTileAttribute >> 5) & 0x01;
            uint16_t spriteTileAddr = OAM_TILE_START_ADDR + (spriteTileIndex * 16);

            uint8_t positionInSprite = (scanline_ % 8) * 2;
            uint8_t upperByte = cpu_.getMemory().read8bit(spriteTileAddr + positionInSprite);
            uint8_t lowerByte = cpu_.getMemory().read8bit(spriteTileAddr + positionInSprite + 1);

            int color = 0;
            uint8_t dec = 7;
            if (xFlip) {
                dec = 0;
            }

            for (int x = 0; x < 8; ++x)
            {
                color |= ((upperByte >> dec) & 0x01);
                color <<= 1;
                color |= ((lowerByte >> dec) & 0x01);
                if (xFlip) {
                    dec++;
                } else {
                    dec--;
                }
                assert(((scanline_ * gameboy_width) + (x + posX)) < gameboy_width * gameboy_height);

                if (color == 0)
                    pixels_[(scanline_ * gameboy_width) + (x + posX)] = SDL_MapRGBA(window_surface_->format, 0xff, 0xff, 0xff, 0);
                else if (color == 1)
                    pixels_[(scanline_ * gameboy_width) + (x + posX)] = SDL_MapRGBA(window_surface_->format, 0x44, 0x44, 0x44, 0);
                else if (color == 2)
                    pixels_[(scanline_ * gameboy_width) + (x + posX)] = SDL_MapRGBA(window_surface_->format, 0xaa, 0xaa, 0xaa, 0);
                else if (color == 3)
                    pixels_[(scanline_ * gameboy_width) + (x + posX)] = SDL_MapRGBA(window_surface_->format, 0, 0, 0, 0);

                color = 0x00;
            }
        }
    }
}

void Gpu::rendering()
{
    updateGpuRegister();
    uint8_t lcdCtrl = cpu_.getMemory().read8bit(LCD_DISPLAY_CTRL);
    // When Bit 0 is cleared, both background and window become blank (white),
    // and the Window Display Bit is ignored in that case. Only Sprites may still be displayed (if enabled in Bit 1).
    if (isLcdEnable())
    {
        renderBackground();
        // Window enable ?
        if (isBitSet(lcdCtrl, LcdCTrl::WIN_DISPLAY))
        {
            renderWindow();
        }
    } if (isBitSet(lcdCtrl, LcdCTrl::OBJ_DISPLAY)){
        renderSprite();
    }
}

void Gpu::initDisplay()
{
    window_ = SDL_CreateWindow("gameboy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameboy_width, gameboy_height, 0);
    if (window_ == NULL)
        LOG("SDL_CreateWindow Fail", LogStatus::CRITICAL);
    window_surface_ = SDL_GetWindowSurface(window_);
    if (window_surface_->pixels == nullptr) {
        LOG("window_surface_ nullptr", LogStatus::CRITICAL);
    }
    pixels_ = (uint32_t*)window_surface_->pixels;
    std::cout << "bpp: " << (int32_t)window_surface_->format->BytesPerPixel << std::endl;
    std::cout << SDL_GetPixelFormatName(window_surface_->format->format) << std::endl;
}

void Gpu::render()
{
    SDL_UpdateWindowSurface(window_);
}
