#include "genesis.h"
#include <resources.h>


int main(bool hardReset)
{
    VDP_drawText("8 bit number",0,0);
    u8 number_8bit = 0;
    char text_8bit[3];

    s8 number_s8bit = 0;
    char text_s8bit[4];

    u16 number_16bit = 0;
    char text_16bit[6];

    fix16 fixed16_number = FIX16(0);
    char text_fixed16bit[10];

    fix32 fixed32_number = FIX32(0);
    char text_fixed32bit[10];

    while(TRUE)
    {
        sprintf(text_8bit, "%3i", number_8bit);
        VDP_drawTextBG(BG_A, text_8bit, 2, 1);

        number_8bit++;
        
        sprintf(text_s8bit, "%4i", number_s8bit);
        VDP_drawTextBG(BG_A, text_s8bit, 1, 2);
        number_s8bit++;
        
        sprintf(text_16bit, "%6i", number_16bit);
        VDP_drawTextBG(BG_A, text_16bit, 0, 3);
        number_16bit += 100;

        sprintf(text_fixed16bit, "%10i", fix16ToInt(fixed16_number));
        VDP_drawTextBG(BG_A, text_fixed16bit, 0, 4);
        fixed16_number += FIX16(0.0157); //Note: Minimum number possible

        sprintf(text_fixed32bit, "%10i", fix32ToInt(fixed32_number));
        VDP_drawTextBG(BG_A, text_fixed32bit, 0, 5);
        fixed32_number += FIX32(0.001); //Note: Minimum number possible

        SYS_doVBlankProcess();
    }

    return 0;
}
