#include "TextRendering.h"
#include "../gui/Text.h"
#include "../gui/Button.h"
#include "../gui/Panel.h"
#include "../gui/TextField.h"
#include "../utils/ColorRGBA.h"

Text* fpsName;
Text* fps;
TextField* textField;
int cn = 0;
double initialTime;
double currTime;
std::string currText = "";

void TextRendering::start() {

	float height = 600.0f;
	float width = 250.0f;

	Panel* panel = new Panel({0.0f + width/2, getWindowHeight() - height/2}, width, height);
	{
		getGui()->addComponent(panel);
	}

	fpsName = new Text("FPS: ", { 0.0f, 0.0f }); 
	{
		fpsName->setSize(5.0f);
		fpsName->setDepth(0.5f);
		getGui()->addComponent(fpsName);
	}
	fps = new Text("", { 0.0f, 0.0f });
	{
		fps->setColor(ColorRGBA::RED);
		fps->setSize(5.0f);
		fps->setDepth(1);
		getGui()->addComponent(fps);
	}

	float fpsOffset = 10.0f;
	fpsName->setPosition({ fpsName->getWidth() / 2 , fpsName->getHeight() / 2 });
	fps->setPosition({ fpsName->getWidth() + fpsOffset, fpsName->getHeight() / 2 });

	Button* button = new Button({100.0f + 45, 300.0f});
	{
		button->setDepth(1);
		button->setButtonText("Click me!", ColorRGBA::BLACK, 3.0f);
		button->setOnClickListener([=]() {std::cout << "Clicked !" << std::endl;});
		getGui()->addComponent(button);
	}
	
	textField = new TextField({ 50.0f, 300.0f }, 90.0f, 30.0f);
	{
		textField->setDepth(1);
		getGui()->addComponent(textField);
	}

	currText = textField->getInput();

	initialTime = getElapsedTime();
	currTime = getElapsedTime();
}
void TextRendering::update() {
	++cn;
	currTime += getElapsedTime();
	fps->setContent(std::to_string(int(cn/(currTime - initialTime))));
	std::string input = textField->getInput();
	if (input != currText) {
		currText = input;
		std::cout << currText << std::endl;
	}
}

void TextRendering::end() {

}