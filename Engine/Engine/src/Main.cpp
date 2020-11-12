#include "Engine.h"
#include <iostream>
#include "apps/AppCGJ.h"
#include "apps/AppAVT.h"

int main(int argc, char* argv[])
{
	Engine* eng = &AppCGJ();
	eng->run();
	exit(EXIT_SUCCESS);
}