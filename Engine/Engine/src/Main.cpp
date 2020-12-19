#include "Engine.h"
#include <iostream>
//#include "apps/TreeGrowth.h"
//#include "apps/BoundingSphere.h"
#include "apps/ShadowMapping.h"

int main(int argc, char* argv[])
{
	Engine* eng = &ShadowMapping();
	eng->run();
	exit(EXIT_SUCCESS);
}