#pragma once

#include <Seek.h>

#include "Model/World.h"

class WorldController
{
public:
    WorldController();
    ~WorldController();

    void Update(Seek::Timestep ts);

    inline World& GetWorld() const { return *m_World; }

private:
    World* m_World;
};