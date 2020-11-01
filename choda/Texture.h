#pragma once

namespace choda {

	class Texture
	{
	public:
		Texture(const char* imgPath, bool hasAlpha);
		~Texture();

		// getters
		void activate() const;
		int getWidth() const;
		int getHeight() const;
		unsigned short getId() const;

	private:
		// generator for texture units
		static unsigned short TextureUnitCnt;
	private:
		int width, height;
		unsigned int texture;
		unsigned short textureUnit;
	};

}


