#ifndef DRAWABLE_H
#define DRAWABLE_H

/*
* A drawable interface that should implemented by all entities
* that want to be drawn onto the screen.
* 
* Usage example:
* 
* std::vector<Drawable> drawables = ...;
* 
* for (Drawable dr : drawables) {
*	dr.bind();
*	dr.draw();
* }
* 
*/
class Drawable {

public:
	virtual ~Drawable() = default;
	virtual void init() = 0;
	virtual void bind() = 0;
	virtual void unBind() = 0;
	virtual void draw() = 0;
};

#endif