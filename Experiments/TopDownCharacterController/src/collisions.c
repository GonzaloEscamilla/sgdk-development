#include "../inc/collisions.h"

Vect2D_f16 newVector2D_f16(f16 x, f16 y) {
	return (Vect2D_f16) { x, y };
}
Vect2D_f32 newVector2D_f32(f32 x, f32 y) {
	return (Vect2D_f32) { x, y };
}

Vect2D_s16 newVector2D_s16(s16 x, s16 y) {
	return (Vect2D_s16) { x, y };
}
Vect2D_s32 newVector2D_s32(s32 x, s32 y) {
	return (Vect2D_s32) { x, y };
}

Vect2D_u16 newVector2D_u16(u16 x, u16 y) {
	return (Vect2D_u16) { x, y };
}
Vect2D_u32 newVector2D_u32(u32 x, u32 y) {
	return (Vect2D_u32) { x, y };
}

//Used to get the left edge of a tile by inputting the tile position
u16 getTileLeftEdge(u16 x) {
	return (x << 4);
}
//Used to get the right edge of a tile by inputting the tile position
u16 getTileRightEdge(u16 x) {
	return (x << 4) + 16;
}
//Used to get the top edge of a tile by inputting the tile position
u16 getTileTopEdge(u16 y) {
	return (y << 4);
}
//Used to get the bottom edge of a tile by inputting the tile position
u16 getTileBottomEdge(u16 y) {
	return (y << 4) + 16;
}

BoxCollider COL_newBoxCollider(s16 xMin, s16 xMax, s16 yMin, s16 yMax)
{
    return (BoxCollider) { { xMin, yMin }, { xMax, yMax } };
}

//Used to get the bounds of a tile by inputting the tile position
BoxCollider getTileBounds(s16 x, s16 y) {
	return COL_newBoxCollider((x << 4), (x << 4) + 16, (y << 4), (y << 4) + 16);
}

//Used to get the tile position out of a pixel position
Vect2D_u16 posToTile(Vect2D_s16 position) {
	return newVector2D_u16((position.x >> 4), (position.y >> 4));
}