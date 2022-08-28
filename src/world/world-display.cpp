#include "world.h"
#include "../GL/gl-func.h"
#include <glad/glad.h>
#include <iostream>
#include <cmath>

void World::displayWorld(float cameraX, float cameraY, Shader shader, float timePassed)
{
	static float animationTime = 0.0f;
	animationTime += timePassed;

	//Display the world
	glUniform2f(shader.getUniformLocation("uScale"), TILE_SIZE, TILE_SIZE);
	glUniform1f(shader.getUniformLocation("uTexFrac"), 1.0f / 16.0f);

	//Draw each tile		
	Tile prevTile = NONE;
	for(int i = (int)cameraY - 600.0f / TILE_SIZE; i < (int)cameraY + 600.0f / TILE_SIZE; i++)
	{
		for(int j = (int)cameraX - 1050.0f / TILE_SIZE; j < (int)cameraX + 1050.0f / TILE_SIZE; j++)
		{
			if(i < 0 || i >= WORLD_SIZE ||
			   j < 0 || j >= WORLD_SIZE)
				continue;

			if(mTiles[i][j] != prevTile)
			{
				switch(mTiles[i][j])
				{
				case LAND:
					glUniform2f(shader.getUniformLocation("uTexOffset"),
								0.0f, 15.0f / 16.0f);
					break;
				case WATER:
					if(animationTime - floorf(animationTime) <= 0.5f)
						glUniform2f(shader.getUniformLocation("uTexOffset"),
									3.0f / 16.0f, 15.0f / 16.0f);
					else if(animationTime - floorf(animationTime) > 0.5f)	
						glUniform2f(shader.getUniformLocation("uTexOffset"),
									4.0f / 16.0f, 15.0f / 16.0f);	
					break;
				case MOUNTAIN:
					glUniform2f(shader.getUniformLocation("uTexOffset"),
								2.0f / 16.0f, 15.0f / 16.0f);	
					break;
				case TREE:
					glUniform2f(shader.getUniformLocation("uTexOffset"),
								1.0f / 16.0f, 15.0f / 16.0f);
				default:
					break;
				}
				prevTile = mTiles[i][j];
			}

			glUniform2f(shader.getUniformLocation("uOffset"),
						(j - cameraX) * TILE_SIZE,
						(i - cameraY) * TILE_SIZE);		
			glDrawArrays(GL_TRIANGLES, 0, 6);		
		}
	}
}
