#include "Engine.h"
#include <iostream>
#include "apps/TreeGrowth.h"
#include "apps/ShadowMapping.h"
#include "apps/Blending.h"
#include "apps/TextRendering.h"

int main(int argc, char* argv[])
{
	Engine* eng = new TreeGrowth();
	//Engine* eng = new TextRendering();
	//Engine* eng = new ShadowMapping();
	//Engine* eng = new Blending();
	eng->run();
	delete eng;
	exit(EXIT_SUCCESS);
}
