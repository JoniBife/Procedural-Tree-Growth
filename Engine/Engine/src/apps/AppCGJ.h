#ifndef APP_CGJ_H
#define APP_CGJ_H

#include "../Engine.h"

class AppCGJ : public Engine {

public:
	AppCGJ() {}

	void start() override;

	void update() override;

	void end() override;

};

#endif;
