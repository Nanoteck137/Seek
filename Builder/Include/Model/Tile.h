#pragma once

#include <Seek.h>

class Tile
{
public:
    enum class Type
    {
        EMPTY,
        FLOOR
    };

public:
    Tile(int32 x, int32 y, Type type);

    inline int32 GetX() const { return m_X; }
    inline int32 GetY() const { return m_Y; }

    inline void SetType(Type type) { m_Type = type; }
    inline Type GetType() const { return m_Type; }

private:
    int32 m_X;
    int32 m_Y;
    Type m_Type;
};