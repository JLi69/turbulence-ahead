#include "world.h"

World::World(unsigned int seed)
{
	mSeed = seed;
	createPermutation();
	generateWorld();
}

World::~World()
{
	for(int i = 0; i < WORLD_SIZE; i++)
		delete mTiles[i];
	delete mTiles;
}

World::World()
{ }

std::pair<int, int> World::getCurrentGoal()
{
	return mCities.at(mCurrentGoal);
}

bool World::updateGoal()
{
	if(mCurrentGoal == mCities.size() - 1)
		return true; //Player won the game
	mCurrentGoal++;
	return false;
}
