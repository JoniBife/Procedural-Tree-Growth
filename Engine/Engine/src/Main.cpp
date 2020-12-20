#include "Engine.h"
#include <iostream>
#include "apps/TreeGrowth.h"
#include "apps/ShadowMapping.h"

int main(int argc, char* argv[])
{
	//Engine* eng = new ShadowMapping();
	Engine* eng = new TreeGrowth();
	eng->run();
	delete eng;
	exit(EXIT_SUCCESS);
}