#ifndef APP_AVT_H
#define APP_AVT_H

#include "../Engine.h"

class AppAVT : public Engine {

public:
	AppAVT() {}

	void start() override;

	void update() override;

	void end() override;
};

#endif;
