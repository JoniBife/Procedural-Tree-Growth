#ifndef MORPHOSPACE_H
#define MORPHOSPACE_H

#include <iostream>
#include "BranchModule.h"


class Morphospace {

	static void init();

	static BranchModule* selectModule(float apicalControl, float determinacy);
};

#endif

