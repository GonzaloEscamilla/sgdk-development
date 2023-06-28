__________________________________________________

16TILE

Version: 1.0.1
   Date: 08/22/2022
__________________________________________________

CREDITS

    DEVELOPED BY IURI NERY


16Tile is a tool designed to help creating tilemaps for the Sega Genesis/Mega Drive. The tool features a simple user interface focused on tilemap editing as well as different paint modes: Tile Mode, Palette Mode, Priority Mode and the Tilemap Painter, which allows the user to paint directly into the tilemap.

This software is meant to be used with SGDK, a free development kit made by Stephane Dallongeville (https://github.com/Stephane-D/SGDK). The palettes, tilesets and tilemap can all be exported into the 8-Bit PNG format and then imported into SGDK through Rescomp.

16Tile comes with an SGDK sample to show all the main features of the tool. The ROM can be found in the Sample\bin folder and the source code in the Sample\source folder (which also includes the 16Tile project).
__________________________________________________

HOW TO INSTALL

There is no need to install, just unzip the files into any folder of your choice.
__________________________________________________

HOW TO USE

1. Click on Tilesets and import the tileset of your choice;
2. Open the Palette Manager by clicking on the palette button (next to the import button of the tileset) to import/edit the colors of all the palettes;
3. Click on Editor, go to the Tile Mode (Hotkey: T) and assign a tileset to the tilemap by clicking on the button next to the tileset name (top-right);
4. Paint the tilemap by selecting the tiles on the right side of the screen and clicking on the tilemap on the center of the grid;
5. You can also paint the Palette and Priority bits of the tilemap by going into the Palette Mode (Hotkey: 0-3) and Priority Mode (Hotkey: P);
6. Export all the assets by clicking in the Export button.
__________________________________________________

HOTKEYS

EDITOR 
CTRL + S = Saves the project.
F = Toggles the tile frame ON/OFF.
G = Toggles the grid ON/OFF.
SPACE + LMB = Drags the workspace.
SPACE + RMB = Centers the grid.
MOUSE WHEEL = Zoom IN/OUT.

TOOLBAR 
T = Tile mode.
D = Drawing Mode (opens the Tilemap Painter).
0-3 = Palette Mode.
P = Priority Mode.

TILE MODE
UP / DOWN / Z = Flips the tile vertically.
LEFT / RIGHT / X = Flips the tile horizontally.

TILEMAP PAINTER 
CTRL + LMB = Picks a color from the tilemap.
CTRL + MOUSE WHEEL = Scrolls through the palette colors.
__________________________________________________

CHANGELOGS

1.0.1
Release Date: 08/22/2022
------------------------------
• Added the support for the JASC Palette format (.PAL text files).
• Updated the Mega Drive RGB color conversion algorithm. Now it should properly convert the colors when importing tilesets and palettes.

1.0.0
Release Date: 07/29/2022
------------------------------
• 1st version.
__________________________________________________

LINKS

Twitter: http://twitter.com/IuriNery
Website: https://www.allone-works.com
Itch.io: https://allone-works.itch.io/16tile
__________________________________________________