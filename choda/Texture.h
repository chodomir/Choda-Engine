#pragma once

namespace choda {

	class Texture
	{
	public:
		Texture(const char* imgPath, bool hasAlpha);

		void activate() const;
		int getWidth() const;
		int getHeight() const;

		~Texture();
	private:
		// generator for texture units
		static unsigned short TextureUnitCnt;
	private:
		int width, height;
		unsigned int texture;
		unsigned short textureUnit;
	};

}


