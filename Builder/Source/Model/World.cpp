#include "Model/World.h"

World::World(int32 width, int32 height) : m_Width(width), m_Height(height)
{
    m_Tiles = new Tile*[m_Width * m_Height];

    for (uint32 y = 0; y < m_Height; y++)
    {
        for (uint32 x = 0; x < m_Width; x++)
        {
            m_Tiles[x + y * m_Width] = new Tile(x, y, Tile::Type::EMPTY);
        }
    }
}

World::~World()
{
    if (m_Tiles)
    {
        for (uint32 i = 0; i < m_Width * m_Height; i++)
        {
            delete m_Tiles[i];
            m_Tiles[i] = nullptr;
        }

        delete[] m_Tiles;
        m_Tiles = nullptr;
    }
}

Tile* World::GetTileAt(int32 x, int32 y)
{
    if (x >= 0 && y < m_Width)
    {
        if (x >= 0 && y < m_Height)
        {
            return m_Tiles[x + y * m_Width];
        }
    }

    return nullptr;
}