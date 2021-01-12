#include "TextRendering.h"
#include "../gui/Text.h"
#include "../gui/Button.h"
#include "../gui/Panel.h"
#include "../gui/TextField.h"
#include "../utils/ColorRGBA.h"

Text* fpsName;
Text* fps;
TextField* pMax = nullptr; // Maximum plant age
TextField* vRootMax = nullptr; // Maximum plant vigour (The maximum vigour of the root)
TextField* gP = nullptr; // Growth rate of the whole plant
TextField* apicalControl = nullptr; // Lambda in the paper, represents the growth suppression of an existing subdominant branch by a higher dominanting shoot thta functions to maintain crown dominance by a central stem
TextField* determinacy = nullptr; // D in the paper
TextField* tropismAngle = nullptr; // Alpha in the paper
//TextField* w1, w2 = nullptr; //  Weights that controll the impact of optimization criteria in gradient descent
TextField* g1 = nullptr; // Controls how fast the tropism effect decreases with time
TextField* g2 = nullptr; // Controls the overall strength of the tropism
TextField* thickeningFactor = nullptr; // Phi in the paper
TextField* scalingCoefficient = nullptr; // Its a scaling coefficient used in the paper for calculating the segment lengt
TextField* vMax = nullptr; // Maximum module vigour, not clear in the paper whether this is global or module specific
TextField* vMin = nullptr; // Minimum module vigour, not clear in the paper whether this is global or module specific

int cn = 0;

double initialTime;
double currTime;

const float height = 635.0f;
const float width = 200.0f;
const float padding = 10.0f;
const float offSetTextField = 10.0f;

float buildTitleTextFieldPair(GUI* gui, float windowWidth, float windowHeight, const std::string& title, float yStart, TextField* builtTextField) {

	const float textFieldHeight = 25.0f;

	Text* textFieldName = new Text(title, { 0.0f,0.0f });
	{
		textFieldName->setSize(2.5f);
		textFieldName->setDepth(1);
		textFieldName->setPosition({ (textFieldName->getWidth()*0.5f) + padding,  yStart - (textFieldName->getHeight()*0.5f) - padding});
		gui->addComponent(textFieldName);
	}
	
	Vec2 textFieldPosition = { (width * 0.9f) / 2 + padding, yStart - (textFieldHeight *0.5f)- offSetTextField - padding - (textFieldName->getHeight() *0.5f)};
	builtTextField = new TextField(textFieldPosition, width * 0.9f, textFieldHeight);
	{
		builtTextField->setInput("0.0");
		builtTextField->setDepth(1);
		gui->addComponent(builtTextField);
	}
	
	return textFieldPosition.y - textFieldHeight / 2;
}

void buildUI(GUI* gui, float windowWidth, float windowHeight) {
	Panel* panel = new Panel({ 0.0f + width / 2, windowHeight - height / 2 }, width, height);
	{
		panel->setDepth(0); // Panel is behind the text fields and buttons
		gui->addComponent(panel);

		Text* panelTitle = new Text("Growth Parameters", {0.0f, 0.0f});
		{
			panelTitle->setSize(4);
			panelTitle->setDepth(1);
			panelTitle->setPosition({ (panelTitle->getWidth() * 0.5f) + padding,  windowHeight - (panelTitle->getHeight() * 0.5f) - padding });
			gui->addComponent(panelTitle);
		}


		float startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Plant Age", windowHeight - 25, pMax);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Root Vigour", startY, vRootMax);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Growth Rate", startY, gP);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Apical Control", startY, apicalControl);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Determinacy", startY, determinacy);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Tropism Angle", startY, tropismAngle);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Tropism Decrease", startY, g1);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Tropism Strength", startY, g2);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Thickening Factor", startY, thickeningFactor);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Scaling coeficient", startY, scalingCoefficient);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Vigour", startY, vMax);
		startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Min Vigour", startY, vMin);

		Button* start = new Button({ windowWidth * 0.5f - 90.0f - padding, windowHeight - 30.0f * 0.5f - padding});
		{
			start->setDepth(1);
			start->setButtonText("Start", ColorRGBA::BLACK, 4.0f);
			start->setOnClickListener([=]() {

			});
			gui->addComponent(start);
		}

		Button* pause = new Button({ windowWidth * 0.5f, windowHeight - 30.0f * 0.5f - padding });
		{
			pause->setDepth(1);
			pause->setButtonText("Pause", ColorRGBA::BLACK, 4.0f);
			pause->setOnClickListener([=]() {

			});
			gui->addComponent(pause);
		}

		Button* stop = new Button({ windowWidth * 0.5f + 90.0f + padding, windowHeight - 30.0f * 0.5f - padding });
		{
			stop->setDepth(1);
			stop->setButtonText("Stop", ColorRGBA::BLACK, 4.0f);
			stop->setOnClickListener([=]() {

			});
			gui->addComponent(stop);
		}
	}
}

void TextRendering::start() {
	buildUI(getGui(), getWindowWidth(), getWindowHeight());

	fpsName = new Text("FPS: ", { 0.0f, 0.0f }); 
	{
		fpsName->setColor(ColorRGBA::YELLOW);
		fpsName->setSize(4.0f);
		fpsName->setDepth(0.5f);
		getGui()->addComponent(fpsName);
	}
	fps = new Text("", { 0.0f, 0.0f });
	{
		fps->setColor(ColorRGBA::YELLOW);
		fps->setSize(4.0f);
		fps->setDepth(1);
		getGui()->addComponent(fps);
	}

	float fpsOffset = 2.0f;
	fpsName->setPosition({ getWindowWidth() - fpsName->getWidth()  - fpsOffset, getWindowHeight() -  fpsName->getHeight() / 2 });
	fps->setPosition({ getWindowWidth() - fpsName->getWidth() * 0.3f , getWindowHeight() - fpsName->getHeight() / 2 });

	initialTime = getElapsedTime();
	currTime = getElapsedTime();
}

void TextRendering::update() {
	++cn;
	currTime += getElapsedTime();
	fps->setContent(std::to_string(int(cn/(currTime - initialTime))));
}

void TextRendering::end() {

}