#include "Engine.h"
#include <iostream>
#include "apps/TreeGrowth.h"
#include "apps/ShadowMapping.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

int main(int argc, char* argv[])
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "../Engine/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}


	//Engine* eng = new ShadowMapping();
	//Engine* eng = new TreeGrowth();
	//eng->run();
	//delete eng;
	exit(EXIT_SUCCESS);
}