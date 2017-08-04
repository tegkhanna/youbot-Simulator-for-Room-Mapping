
#include "ImageLoader.h"
#include"picoPNG.h"
#include"IOmanager.h"
#include"Error.h"
namespace bengine
{
	GLtexture ImageLoader::loadPNG(std::string filePath)
	{
		GLtexture texture = {};
		unsigned long width, height;
		std::vector<unsigned char> out;

		std::vector<unsigned char> in;
		if (IOmanager::readFileToBuffer(filePath, in) == false)
		{
			fatalError("Failed to load png in buffer");
		}

		int errorCode = decodePNG(out, width, height, &in[0], in.size());

		if (errorCode != 0)
		{
			fatalError("decodePNG failed with the error " + std::to_string(errorCode));

		}

		glGenTextures(1, &(texture.id));

		glBindTexture(GL_TEXTURE_2D, texture.id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &out[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		texture.width = width;
		texture.height = height;

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;

	}
}