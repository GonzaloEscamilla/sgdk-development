#include "genesis.h"
#include <resources.h>

u16 scrooll;

int main(bool hardReset)
{
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, background.palette->data, DMA);
    VDP_drawImageEx(BG_B, &background, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);

    ind += background.tileset->numTile; 


    PAL_setPalette(PAL1, foreground.palette->data, DMA);
    VDP_drawImageEx(BG_A, &foreground, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    while(TRUE)
    {
        // nothing to do here
        // ...

       // VDP_setHorizontalScroll(BG_B, scrooll++);

        // always call this method at the end of the frame
        SYS_doVBlankProcess();
    }

    return 0;
}
