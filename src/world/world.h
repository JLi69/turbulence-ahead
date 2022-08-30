#ifndef WORLD_H
#include <vector>
#include "../GL/shader.h"

#define WORLD_SIZE 1000
#define GRID_SIZE 50
#define TILE_SIZE 48.0f
#define CITY_COUNT 7

class World
{
	enum Tile
	{
		LAND,
		WATER,
		MOUNTAIN,
		TREE,
		CITY,
		NONE
	};

	Tile mTiles[WORLD_SIZE][WORLD_SIZE];
	unsigned int mSeed = 0;
	std::vector<unsigned int> mPermutation;

	std::vector<std::pair<int, int>> mCities;
	int mCurrentGoal = 0;

	//Perlin noise functions
	//Simple 2D vector	
	struct Vector2
	{
		float x, y;
	};
	Vector2 randVector(int gridX, int gridY); 
	float gradDotProd(int gridX, int gridY, float x, float y);
	float interpolate(float a, float b, float weight);
	float perlin(float x, float y);
public:
	//Increments the goal to the next city,
	//but if it is at the last city, return true to represent the player
	//won the game. Otherwise return false
	bool updateGoal();
	void generateWorld();
	void createPermutation();
	void displayWorld(float cameraX, float cameraY, Shader shader, float timePassed);	
	std::pair<int, int> getCurrentGoal();

	World(unsigned int seed);
	World();
};

#define WORLD_H
#endif
