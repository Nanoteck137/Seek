#pragma once

#include <Seek.h>

#include "Tile.h"

class World
{
public:
    World(int32 width, int32 height);

    Tile& GetTileAt(int32 x, int32 y);

    inline int32 GetWidth() const { return m_Width; }
    inline int32 GetHeight() const { return m_Height; }

private:
    int32 m_Width;
    int32 m_Height;
    Tile** m_Tiles;
};