#include "app.h"
#include <glad/glad.h>

void App::display(float timePassed)
{	
	glClear(GL_COLOR_BUFFER_BIT);		

	//Draw the world centered on the player	
	mLevel.displayWorld(mPlayer.getX(),
						mPlayer.getY(),
						mDefaultShader,
						timePassed);


	mDefaultShader.use();
	//Draw the player
	mPlayer.draw(mDefaultShader);
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
}
