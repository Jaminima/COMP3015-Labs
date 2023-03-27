#ifndef __TextureLoading
#define __TextureLoading

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include "helper/glutils.h"

using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "helper/stb/stb_image.h"

const int ColourChanels[]{ 0 , GL_R, GL_RG, GL_RGB, GL_RGBA };

inline void loadTexture(GLuint& texture, std::string texturepath)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturepath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, ColourChanels[nrChannels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		cout << "Loaded Texture " << texturepath.c_str() << std::endl;
	}
	else
	{
		std::cout << "Texture Error " << texturepath.c_str() << std::endl;
	}

	stbi_image_free(data);
}

#endif // !__TextureLoading
