#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

class GUIComponent {

private:
	unsigned int depth;

public:

	unsigned int getDepth() const;

	void setDepth(unsigned int depth);

	virtual void draw() = 0;

};

#endif