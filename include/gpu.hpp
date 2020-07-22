#pragma once
#include "cpu.hpp"
#include "utils.h"
#include <SDL.h>

static constexpr uint16_t OAM_START_ADDR = 0xFE00;
static constexpr uint16_t OAM_TILE_START_ADDR = 0x8000;

static constexpr uint16_t LCD_DISPLAY_CTRL = 0xff40;
static constexpr uint16_t LCDC_STATUS_ADDR = 0xff41;

static constexpr uint16_t SCY_ADDR = 0xff42;
static constexpr uint16_t SCX_ADDR = 0xff43;
static constexpr uint16_t WINY_ADDR = 0xff4a;
static constexpr uint16_t WINX_ADDR = 0xff4b;

static constexpr uint16_t LY_ADDR = 0xff44;
static constexpr uint16_t LYC_ADDR = 0xff45;

const int MAX_VIDEO_CYCLES = 456;
const int MIN_HBLANK_MODE_CYCLES = 204;
const int OAM_MODE_CYCLES = 80;
const int MIN_LCD_TRANSFER_CYCLES = 172;
const int VBLANK_CYCLES = 4560;
static constexpr uint8_t VERTICAL_BLANK_SCAN_LINE = 144;
static constexpr uint8_t VERTICAL_BLANK_SCAN_LINE_MAX = 153;

static constexpr uint8_t gameboy_width = 160;
static constexpr uint8_t gameboy_height = 144;
static constexpr uint16_t bg_tile_map_display_select_low = 0x9800;
static constexpr uint16_t bg_tile_map_display_select_high = 0x9c00;
static constexpr uint16_t window_tile_display_map_select_low = 0x9800;
static constexpr uint16_t window_tile_display_map_select_high = 0x9c00;
static constexpr uint16_t tile_data_addr_low = 0x8000;
static constexpr uint16_t tile_data_addr_high = 0x8800;

enum GpuMode
{
    H_BLANK = 0,
    V_BLANK = 1,
    OAM = 2,
    LCD_TX = 3,
};

enum LcdCTrl : uint8_t
{
    LCD_DISPLAY = 7,
    WIN_TILE_MAP_DISPLAY = 6,
    WIN_DISPLAY = 5,
    BG_WIN_TILE_DATA_SELECT = 4,
    BG_TILE_MAP_DISPLAY = 3,
    OBJ_SIZE = 2,
    OBJ_DISPLAY = 1,
    BG_WIN_PRIORITY = 0,
};

class Gpu
{
public:
    uint16_t backgroudMap_;
    uint16_t windowMap_;
    uint16_t tileData_;
    uint8_t lcdCtrl_;
    uint8_t scY_;
    uint8_t scX_;
    uint8_t winY_;
    uint8_t winX_;
    bool unsign_;
    SDL_Surface *window_surface_;
    SDL_Surface *tmp_surface_;

public:
    void updateWindow(uint32_t width, uint32_t height);
    void simpleRender();
    void rendering();
    void render();
    void initDisplay();
    void renderBackground();
    void renderWindow();
    void renderSprite();
    void renderTile(int32_t idx, uint8_t startY, uint8_t startX,
                    uint16_t mapStartAddr, uint16_t tileDataAddr);
    void updateGpuRegister();

    Gpu(Z80Cpu &cpu) : cpu_(cpu)
    {
        gpuMode_ = GpuMode::H_BLANK;
        lcdEnable_ = true;
        gpuCycle_ = 0;
        scanline_ = 0;
        scX_ = 0;
        scY_ = 0;
        winY_ = 0;
        winX_ = 0;
        scanline_ = 0;
        vblanckCycle_ = 0;
        gpuCycle_ = 0;
        initDisplay();
    }

    bool isLcdEnable()
    {
        uint8_t lcdCtrl = 0;
        lcdCtrl = cpu_.getMemory().read8bit(LCD_DISPLAY_CTRL);
        lcdEnable_ = false;
        if (isBitSet(lcdCtrl, LcdCTrl::LCD_DISPLAY))
        {
            lcdEnable_ = true;
        }
        return (lcdEnable_);
    }

    void disableLcd()
    {
        lcdEnable_ = false;
        gpuCycle_ = 0;
        vblanckCycle_ = 0;
        scanline_ = 0;
        cpu_.getMemory().write8bit(LY_ADDR, 0);

        uint8_t lcdc = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
        cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcdc & 0xFC);
        gpuMode_ = GpuMode::H_BLANK;
    }

    void updateGpu(uint8_t opcodeCycle)
    {
        if (isLcdEnable())
        {
            gpuCycle_ += opcodeCycle;
            switch (gpuMode_)
            {
            case GpuMode::H_BLANK:
                hblank();
                break;
            case GpuMode::V_BLANK:
                vblank(opcodeCycle);
                break;
            case GpuMode::OAM:
                oam();
                break;
            case GpuMode::LCD_TX:
                lcdTransfert();
                break;
            }
        }
        else
        {
            disableLcd();
        }
    }

    void compareScanline()
    {
        scanline_ = cpu_.getMemory().read8bit(LY_ADDR);
        uint8_t scanlineCompare = cpu_.getMemory().read8bit(LYC_ADDR);
        uint8_t lcdStatus = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);

        if (scanline_ == scanlineCompare)
        {
            setBit(lcdStatus, 2); //set coincidence flag
            if (isBitSet(lcdStatus, 6))
            { // request interrupt on coincidence flag ?
                cpu_.requestInterrupt(InterruptType::LCDC);
            }
        }
        else
        {
            clearBit(lcdStatus, 2);
        }
        cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcdStatus);
    }

    void hblank()
    {
        //// std::cout << "h blank: " << scanline_ << std::endl;
        if (gpuCycle_ >= MIN_HBLANK_MODE_CYCLES)
        {
            gpuCycle_ -= MIN_HBLANK_MODE_CYCLES;

            scanline_ = cpu_.getMemory().read8bit(LY_ADDR);
            scanline_ += 1;
            cpu_.getMemory().write8bit(LY_ADDR, scanline_);
            compareScanline();
            if (scanline_ == VERTICAL_BLANK_SCAN_LINE)
            {
                gpuMode_ = GpuMode::V_BLANK;

                //set lcd status to vblank
                uint8_t lcdStatus = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
                //set current vblanck status
                setBit(lcdStatus, 0);
                clearBit(lcdStatus, 1); //should we reset previous status artefacts ?
                cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcdStatus);

                //request vblank interrupt
                cpu_.requestInterrupt(InterruptType::VBLANCK);
                // request interrupt on hblank flag ?
                if (isBitSet(lcdStatus, 4))
                {
                    cpu_.requestInterrupt(InterruptType::LCDC);
                }
            }
            else
            {
                gpuMode_ = GpuMode::OAM;
                uint8_t lcdStatus = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
                setBit(lcdStatus, 1);
                clearBit(lcdStatus, 0);
                cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcdStatus);
                // request interrupt on OAM flag ?
                if (isBitSet(lcdStatus, 5))
                {
                    cpu_.requestInterrupt(InterruptType::LCDC);
                }
            }
        }
    }

    void vblank(uint16_t cycle)
    {
        vblanckCycle_ += cycle;

        scanline_ = cpu_.getMemory().read8bit(LY_ADDR);
        // keep inc scanline and call LCD interrupt
        //// std::cout << "V blank: " << scanline_ << std::endl;
        if (vblanckCycle_ >= MAX_VIDEO_CYCLES)
        {

            vblanckCycle_ = 0;
            if (scanline_ < VERTICAL_BLANK_SCAN_LINE_MAX)
            {
                cpu_.getMemory().write8bit(LY_ADDR, ++scanline_);
            }
            //// std::cout << "current scanline: " << scanline_ << std::endl;
            compareScanline();
        }

        if (gpuCycle_ >= VBLANK_CYCLES)
        {
            vblanckCycle_ = 0;
            gpuCycle_ = 0;
            gpuMode_ = GpuMode::OAM;
            uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);

            cpu_.getMemory().write8bit(LY_ADDR, 0);
            scanline_ = 0;
            compareScanline();
            setBit(lcd, 1);	  //set OAM mode
            clearBit(lcd, 0); //clear vblank
            // request interrupt on OAM flag ?
            if (isBitSet(lcd, 5))
            {
                cpu_.requestInterrupt(InterruptType::LCDC);
            }
            cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);
        }
    }

    void oam()
    {
        //// std::cout << "OAM: " << scanline_ << std::endl;
        if (gpuCycle_ >= OAM_MODE_CYCLES)
        {
            gpuCycle_ -= OAM_MODE_CYCLES;

            gpuMode_ = GpuMode::LCD_TX;
            uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
            setBit(lcd, 0);
            setBit(lcd, 1);
            cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);
        }
    }

    void lcdTransfert()
    {
        //// std::cout << "LCD: " << scanline_ << std::endl;
        if (gpuCycle_ >= MIN_LCD_TRANSFER_CYCLES)
        {
            gpuCycle_ -= MIN_LCD_TRANSFER_CYCLES;
            gpuMode_ = GpuMode::H_BLANK;

            //display rendering ? should be done right after oam transfer ?

                rendering();

            uint8_t lcdStatus = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
            clearBit(lcdStatus, 0);
            clearBit(lcdStatus, 1);
            cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcdStatus);
            if (isBitSet(lcdStatus, 3))
            {
                cpu_.requestInterrupt(InterruptType::LCDC);
            }
        }
    }

    int32_t gpuCycle_ = 0;

    bool &getLcdEnable()
    {
        return lcdEnable_;
    }

    GpuMode &getGpuMode()
    {
        return gpuMode_;
    }

    uint16_t &getScanline()
    {
        return scanline_;
    }

    bool bgDisplayEnable_ = false;
    bool windowsDisplayEnable_ = false;

private:
    Z80Cpu &cpu_;
    SDL_Window *window_;
    SDL_Surface *screenSurface_;
    SDL_Texture *texture_;
    SDL_Renderer *renderer_;
    unsigned int *pixels_;

    bool lcdEnable_ = true;
    enum GpuMode gpuMode_;
    uint16_t scanline_ = 0;
    uint32_t vblanckCycle_ = 0;
};
