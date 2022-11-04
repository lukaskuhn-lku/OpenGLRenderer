#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>

class Texture {	
public:
	unsigned int ID;

	Texture(const std::string path, bool flip, bool transparency) {

		int width, height, nrChannels;

		if(flip) stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		
		if (data) {
			glGenTextures(1, &ID);

			glBindTexture(GL_TEXTURE_2D, ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (transparency) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "TEXTURE FROM " << path << " NOT LOADED" << std::endl;
			ID = -1;
		}

		stbi_image_free(data);
	}

};

#endif // !TEXTURE_H