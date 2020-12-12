#ifndef TREE_GROWTH_H
#define TREE_GROWTH_H

#include "../Engine.h"

class TreeGrowth : public Engine {

public:
	TreeGrowth() {}

	void start() override;

	void update() override;

	void end() override;

};

#endif
