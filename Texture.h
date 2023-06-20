#pragma once
#include <glad/glad.h>
#include <iostream>


class Texture
{


public:

	unsigned int texture;
	bool isPng;

	void buildTexture(const char* filePath, bool _isPng);

private:

	void genTexture();
	void setParameters();
	void load(const char* filePath);

};

