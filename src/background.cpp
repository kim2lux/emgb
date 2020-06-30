#include "GB.h"
#include "gpu.h"

int getRealPosition(struct s_gb *s_gb)
{
    int yPos = s_gb->gb_io.scrollY + s_gb->gb_gpu.scanline;
    int yDataLine = yPos / 8;
    if (yDataLine > 0x1f)
        yDataLine -= 0x20;
    int lineOffset = (yDataLine)* 32;
    int dataOffset = s_gb->gb_io.lcd.BgTileMapSelect + lineOffset + s_gb->gb_io.scrollX;

    return (dataOffset);
}

void renderingBg(struct s_gb *s_gb)
{
    unsigned short line;
    int color = 0;
    int dec;
    int posx, x;
    unsigned char tileindex;
    signed char stileindex;
    int baseaddr = s_gb->gb_io.lcd.BgWindowTileData;
    int dataOffset = getRealPosition(s_gb);
    posx = 0;
    for (int index = 0; index < 20; index += 1)
    {
        int tileAddr;
        if (s_gb->gb_io.lcd.BgWindowTileData == 0x8800)
        {
            stileindex = (signed)(read8bit(index + dataOffset, s_gb));
            tileAddr = baseaddr + ((stileindex + 128) * 16);
        }
        else
        {
            tileindex = read8bit(index + dataOffset, s_gb);
            tileAddr = baseaddr + (tileindex * 16);
        }
        dec = 15;
        line = read16bit(tileAddr + ((s_gb->gb_gpu.scanline % 8) * 2), s_gb);
        for (x = 0; x < 8; x++)
        {
            color = ((line >> dec) & 0x01);
            if ((line >> (dec - 8)) & 0x01)
                color += 2;
            if (color == 0)
                color = 0x00ffffff;
            else if (color == 1)
                color = 0x00444444;
            else if (color == 2)
                color = 0x00aaaaaa;
            else if (color == 3)
                color = 0x00000000;
            if ((160 * (s_gb->gb_gpu.scanline) + (posx + x)) < (160 * 144))
                s_gb->gb_gpu.pixels[(160 * s_gb->gb_gpu.scanline) + (posx + x)] = color;
            dec--;
        }
        posx += 8;
    }
}