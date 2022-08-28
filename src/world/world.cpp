#include "world.h"

World::World(unsigned int seed)
{
	mSeed = seed;
	createPermutation();
	generateWorld();
}

World::World()
{ }
