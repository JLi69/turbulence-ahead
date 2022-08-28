#ifndef SHADER_H
#include <map>
#include <string>

class Shader
{	
	unsigned int mId;
	std::map<std::string, int> mUniformLocations;
public:
	void use();
	int getUniformLocation(std::string uniformName);
	Shader(const char *vertpath, const char *fragpath);
	Shader();
};

#define SHADER_H
#endif
