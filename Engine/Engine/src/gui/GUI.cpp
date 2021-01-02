#include "GUI.h"

#define ever (;;)

GUI::GUI() {

}
GUI::~GUI() {
	for (GUIComponent* comp : guiComponents)
		delete comp;
}

void GUI::drawUI() {
	/*
	* We are using the painters algorithm to draw the GUI.
	* Each component is associated with a depth and the components
	* are drawn from the furthest to the closest.
	* Since the insertion is sorted we don't need to sort the list each
	* time we are drawing.
	*/
	for (GUIComponent* comp : guiComponents) {
		comp->draw();
	}
}

void GUI::removeComponent(GUIComponent* component) {
	guiComponents.remove(component);
}

void GUI::addComponentSort(GUIComponent* component) {

	// If the list is empty we insert in the front
	if (guiComponents.empty()) {
		guiComponents.push_front(component);
		return;
	}

	// Otherwise we find the adequate position to insert
	std::list<GUIComponent*>::iterator curr = guiComponents.begin();

	for ever {

		// If we reached the end of the list we insert in the end
		if (curr == guiComponents.end()) {
			guiComponents.push_back(component);
			return;
		}

		// If the depth is smaller or equal to the curr then we insert before it
		if (component->getDepth() <= (**curr).getDepth()) {
			guiComponents.insert(curr, component);
			return;
		}
;
		++curr;
	}
}

