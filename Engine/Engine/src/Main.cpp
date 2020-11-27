#include "Engine.h"
#include <iostream>
#include "apps/TreeGrowth.h"

int main(int argc, char* argv[])
{
	Engine* eng = &TreeGrowth();
	eng->run();
	exit(EXIT_SUCCESS);
}