#pragma once

#ifndef StaticActor_H
#define StaticActor_H

#include "Actor.h"

class StaticActor : public Actor
{
    friend class ActorFactory;

private:
    StaticActor(uint64_t id);
};

#endif