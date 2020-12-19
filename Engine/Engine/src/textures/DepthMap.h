#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#include "ITexture.h"

class DepthMap : public ITexture {

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
};

#endif


