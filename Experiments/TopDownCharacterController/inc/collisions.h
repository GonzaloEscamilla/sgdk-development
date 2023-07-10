#pragma once

#include <genesis.h>
#include <resources.h>

typedef struct BoxCollider_
{
    Vect2D_s16 min;
    Vect2D_s16 max;
} BoxCollider;

Vect2D_f16 newVector2D_f16(f16 x, f16 y);
Vect2D_f32 newVector2D_f32(f32 x, f32 y);
Vect2D_s16 newVector2D_s16(s16 x, s16 y);
Vect2D_s32 newVector2D_s32(s32 x, s32 y);
Vect2D_u16 newVector2D_u16(u16 x, u16 y);
Vect2D_u32 newVector2D_u32(u32 x, u32 y);

u16 getTileLeftEdge(u16 x);
u16 getTileRightEdge(u16 x);
u16 getTileTopEdge(u16 y);
u16 getTileBottomEdge(u16 y);

BoxCollider COL_newBoxCollider(s16 xMin, s16 xMax, s16 yMin, s16 yMax);
BoxCollider getTileBounds(s16 x, s16 y);

Vect2D_u16 posToTile(Vect2D_s16 position);