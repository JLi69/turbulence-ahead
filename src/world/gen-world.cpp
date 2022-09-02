#include "world.h"
#include <stdio.h>
#include <math.h>

World::Vector2 World::randVector(int gridX, int gridY)
{	
	float angle = 0.0f;
	
	int index = 0;
	index += gridX * 713;
	index += 413;
	index += gridY * 637;
	index = (index > 0) ? index : -index;
	index %= 256;

	angle = (float)mPermutation.at(index) / 256.0f * 3.14159f * 2.0f;

	return { cosf(angle), sinf(angle) };
}

float World::gradDotProd(int gridX, int gridY, float x, float y)
{
	Vector2 randVec = randVector(gridX, gridY);
	Vector2 dist = { x - gridX, y - gridY };
	return dist.x * randVec.x + dist.y * randVec.y;
}

float World::interpolate(float a, float b, float weight)
{
	return (b - a) * weight + a;
}

float World::perlin(float x, float y)
{
	int lowerX = (int)floorf(x),
		upperX = lowerX + 1,
		lowerY = (int)floorf(y),
		upperY = lowerY + 1;

	float normX = x - lowerX,
		  normY = y - lowerY;

	float dot1, dot2;
	float interpolated1, interpolated2;
	float value; //Final value

	//Lower
	dot1 = gradDotProd(lowerX, lowerY, x, y);
	dot2 = gradDotProd(upperX, lowerY, x, y);
	interpolated1 = interpolate(dot1, dot2, normX);
	//Upper
	dot1 = gradDotProd(lowerX, upperY, x, y);
	dot2 = gradDotProd(upperX, upperY, x, y);
	interpolated2 = interpolate(dot1, dot2, normX);

	value = interpolate(interpolated1, interpolated2, normY);

	return value;
}

//Probably not a great way to create a permutation
//but good enough for this game
void World::createPermutation()
{
	srand(mSeed);
	std::vector<unsigned int> values(256);
	for(int i = 1; i <= 256; i++)
		values.at(i - 1) = i;

	mPermutation.clear();
	while(!values.empty())
	{
		int index = rand() % values.size();
		mPermutation.push_back(values.at(index));
		values.erase(values.begin() + index);	
	}
}

void World::generateWorld()
{
	mTiles = new Tile*[WORLD_SIZE];	
	for(int i = 0; i < WORLD_SIZE; i++)
		mTiles[i] = new Tile[WORLD_SIZE];

	//Generate the world using perlin noise
	for(int i = 0; i < WORLD_SIZE; i++)
	{
		for(int j = 0; j < WORLD_SIZE; j++)
		{
			float x = (float)j / (float)GRID_SIZE,
				  y = (float)i / (float)GRID_SIZE;
			float value = perlin(x, y);
		
			//Water
			if(value < 0.1f)
				mTiles[i][j] = WATER;
			else if(value < 0.15f)
				mTiles[i][j] = SAND;
			//Land
			else if(value < 1.0f)
				mTiles[i][j] = LAND;
		}
	}

	//Randomly place trees around the world
	for(int i = 0; i < WORLD_SIZE; i++)
		for(int j = 0; j < WORLD_SIZE; j++)
			if(rand() % 32 == 0 && mTiles[i][j] == LAND)
				mTiles[i][j] = TREE;

	//Place the cities
	//Starting city
	mTiles[WORLD_SIZE / 2][WORLD_SIZE / 2] = CITY;
	//Clear out the region near the city
	for(int i = WORLD_SIZE / 2 - 4; i <= WORLD_SIZE / 2 + 4; i++)	
		for(int j = WORLD_SIZE / 2 - 4; j <= WORLD_SIZE / 2 + 4; j++)
			if((i - WORLD_SIZE / 2) * (i - WORLD_SIZE / 2) + (j - WORLD_SIZE / 2) * (j - WORLD_SIZE / 2) <= 16 &&
				mTiles[i][j] != CITY)
				mTiles[i][j] = LAND;	
	
	std::vector<std::pair<int, int>> cityLocations;
	cityLocations.push_back({ WORLD_SIZE / 2, WORLD_SIZE / 2 });

	//Place the other cities
	for(int i = 0; i < CITY_COUNT - 1; i++)
	{
		int cityX = rand() % (CITY_MAX - CITY_MIN) + CITY_MIN, 
			cityY = rand() % (CITY_MAX - CITY_MIN) + CITY_MIN;

		bool foundValidLocation = false;
		while(!foundValidLocation)
		{	
			foundValidLocation = true;	
			//Make sure the city isn't too close to other cities	
			for(auto city : cityLocations)
			{	
				if(abs(city.first - cityX) <= 4 && abs(city.second - cityY) <= 4)
				{	
					foundValidLocation = false;
					cityX = rand() % WORLD_SIZE;
					cityY = rand() % WORLD_SIZE;
					break;
				}
			}
		}
		mCities.push_back({ cityX, cityY });	
		cityLocations.push_back({ cityX, cityY });

		mTiles[cityY][cityX] = CITY;	
		for(int j = cityX - 4; j <= cityX + 4; j++)	
		{	
			for(int k = cityY - 4; k <= cityY + 4; k++)
			{
				if(j < 0 || j >= WORLD_SIZE || k < 0 || k >= WORLD_SIZE)
					continue;
				if((j - cityX) * (j - cityX) + (k - cityY) * (k - cityY) <= 16 &&
					mTiles[k][j] != CITY)
					mTiles[k][j] = LAND;
			}
		}
	}
}
