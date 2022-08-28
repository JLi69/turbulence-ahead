#include "shader.h"
#include <glad/glad.h>
#include "gl-func.h"

void Shader::use()
{
	useProgram(mId);	
}

int Shader::getUniformLocation(std::string uniformName)
{
	if(mUniformLocations.count(uniformName) == 0)
		mUniformLocations[uniformName] = glGetUniformLocation(mId, uniformName.c_str());
	return mUniformLocations[uniformName];
}

Shader::Shader(const char *vertpath, const char *fragpath)
{
	mId = createShaderProgram(vertpath, fragpath);
}

Shader::Shader()
{ }
