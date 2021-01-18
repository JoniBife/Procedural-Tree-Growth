#ifndef TREE_GROWTH_UI
#define TREE_GROWTH_UI

#include "../gui/GUI.h"
#include "../gui/TextField.h"
#include "../gui/Button.h"
#include "../Engine.h"
#include <functional>
#include "GrowthParameters.h"

class TreeGrowthUI {

private:
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

	std::function<void(GrowthParameters growthParameters)> onStart;
	std::function<void()> onPause;
	std::function<void()> onStop;

	void buildFPSCounter(GUI* gui, float windowWidth, float windowHeight) {
		Text* fpsName = new Text("FPS: ", { 0.0f, 0.0f });
		{
			fpsName->setColor(ColorRGBA::YELLOW);
			fpsName->setSize(4.0f);
			fpsName->setDepth(0.5f);
			gui->addComponent(fpsName);
		}
		fps = new Text("", { 0.0f, 0.0f });
		{
			fps->setColor(ColorRGBA::YELLOW);
			fps->setSize(4.0f);
			fps->setDepth(1);
			gui->addComponent(fps);
		}

		float fpsOffset = 2.0f;
		fpsName->setPosition({ windowWidth - fpsName->getWidth() - fpsOffset, windowHeight - fpsName->getHeight() / 2 });
		fps->setPosition({ windowWidth - fpsName->getWidth() * 0.3f , windowHeight - fpsName->getHeight() / 2 });
	}

	float buildTitleTextFieldPair(GUI* gui, float windowWidth, float windowHeight, const std::string& title, float yStart, TextField*& builtTextField) {

		const float textFieldHeight = 25.0f;

		Text* textFieldName = new Text(title, { 0.0f,0.0f });
		{
			textFieldName->setSize(2.5f);
			textFieldName->setDepth(1);
			textFieldName->setPosition({ (textFieldName->getWidth() * 0.5f) + padding,  yStart - (textFieldName->getHeight() * 0.5f) - padding });
			gui->addComponent(textFieldName);
		}

		Vec2 textFieldPosition = { (width * 0.9f) / 2 + padding, yStart - (textFieldHeight * 0.5f) - offSetTextField - padding - (textFieldName->getHeight() * 0.5f) };
		builtTextField = new TextField(textFieldPosition, width * 0.9f, textFieldHeight);
		{
			builtTextField->setDepth(1);
			gui->addComponent(builtTextField);
		}

		return textFieldPosition.y - textFieldHeight / 2;
	}

	void buildTreeGrowthUI(GUI* gui, float windowWidth, float windowHeight) {
		Panel* panel = new Panel({ 0.0f + width / 2, windowHeight - height / 2 }, width, height);
		{
			panel->setDepth(0); // Panel is behind the text fields and buttons
			gui->addComponent(panel);

			Text* panelTitle = new Text("Growth Parameters", { 0.0f, 0.0f });
			{
				panelTitle->setSize(4);
				panelTitle->setDepth(1);
				panelTitle->setPosition({ (panelTitle->getWidth() * 0.5f) + padding,  windowHeight - (panelTitle->getHeight() * 0.5f) - padding });
				gui->addComponent(panelTitle);
			}

			GrowthParameters* growthParameters = GrowthParameters::instance;

			float startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Plant Age", windowHeight - 25, pMax);
			pMax->setInput(growthParameters ? std::to_string(growthParameters->pMax) : "0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Root Vigour", startY, vRootMax);
			vRootMax->setInput(growthParameters ? std::to_string(growthParameters->vRootMax) : "0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Growth Rate", startY, gP);
			gP->setInput(growthParameters ? std::to_string(growthParameters->gP) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Apical Control", startY, apicalControl);
			apicalControl->setInput(growthParameters ? std::to_string(growthParameters->apicalControl) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Determinacy", startY, determinacy);
			determinacy->setInput(growthParameters ? std::to_string(growthParameters->determinacy) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Tropism Angle", startY, tropismAngle);
			tropismAngle->setInput(growthParameters ? std::to_string(growthParameters->tropismAngle) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Tropism Decrease", startY, g1);
			g1->setInput(growthParameters ? std::to_string(growthParameters->g1) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Tropism Strength", startY, g2);
			g2->setInput(growthParameters ? std::to_string(growthParameters->g2) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Thickening Factor", startY, thickeningFactor);
			thickeningFactor->setInput(growthParameters ? std::to_string(growthParameters->thickeningFactor) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Scaling coeficient", startY, scalingCoefficient);
			scalingCoefficient->setInput(growthParameters ? std::to_string(growthParameters->scalingCoefficient) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Vigour", startY, vMax);
			vMax->setInput(growthParameters ? std::to_string(growthParameters->vMax) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Min Vigour", startY, vMin);
			vMin->setInput(growthParameters ? std::to_string(growthParameters->vMin) : "0.0");

			Button* start = new Button({ windowWidth * 0.5f - 90.0f - padding, windowHeight - 30.0f * 0.5f - padding });
			{
				start->setDepth(1);
				start->setButtonText("Start", ColorRGBA::BLACK, 4.0f);
				start->setOnClickListener([=]() {
					GrowthParameters growthParameters;
					growthParameters.pMax = unsigned int(std::stoul(pMax->getInput()));
					growthParameters.vRootMax = unsigned int(std::stoul(vRootMax->getInput()));
					growthParameters.gP = std::stof(gP->getInput());
					growthParameters.apicalControl = std::stof(apicalControl->getInput());
					growthParameters.determinacy = std::stof(determinacy->getInput());
					growthParameters.tropismAngle = std::stof(tropismAngle->getInput());
					growthParameters.g1 = std::stof(g1->getInput());
					growthParameters.g2 = std::stof(g2->getInput());
					growthParameters.thickeningFactor = std::stof(thickeningFactor->getInput());
					growthParameters.scalingCoefficient = std::stof(scalingCoefficient->getInput());
					growthParameters.vMax = std::stof(vMax->getInput());
					growthParameters.vMin = std::stof(vMin->getInput());
					
					onStart(growthParameters);
				});
				gui->addComponent(start);
			}

			Button* pause = new Button({ windowWidth * 0.5f, windowHeight - 30.0f * 0.5f - padding });
			{
				pause->setDepth(1);
				pause->setButtonText("Pause", ColorRGBA::BLACK, 4.0f);
				pause->setOnClickListener([=]() {
					onPause();
				});
				gui->addComponent(pause);
			}

			Button* stop = new Button({ windowWidth * 0.5f + 90.0f + padding, windowHeight - 30.0f * 0.5f - padding });
			{
				stop->setDepth(1);
				stop->setButtonText("Stop", ColorRGBA::BLACK, 4.0f);
				stop->setOnClickListener([=]() {
					onStop();
				});
				gui->addComponent(stop);
			}
		}
	}

public:
	TreeGrowthUI(GUI* gui, float windowWidth, float windowHeight, std::function<void(GrowthParameters growthParameters)> onStart,
	std::function<void()> onPause,
	std::function<void()> onStop) {
		this->onStart = onStart;
		this->onPause = onPause,
		this-> onStop = onStop;
		buildTreeGrowthUI(gui, windowWidth, windowHeight);
		buildFPSCounter(gui, windowWidth, windowHeight);
	}

	void updateFPSCounter(float elapsedTime) {
		++cn;
		currTime += elapsedTime;
		fps->setContent(std::to_string(int(cn / (currTime - initialTime))));
	}

	GrowthParameters getGrowthParameters() {
		GrowthParameters growthParameters;
		growthParameters.pMax = unsigned int(std::stoul(pMax->getInput()));
		growthParameters.vRootMax = unsigned int(std::stoul(vRootMax->getInput()));
		growthParameters.gP = std::stof(gP->getInput());
		growthParameters.apicalControl = std::stof(apicalControl->getInput());
		growthParameters.determinacy = std::stof(determinacy->getInput());
		growthParameters.tropismAngle = std::stof(tropismAngle->getInput());
		growthParameters.g1 = std::stof(g1->getInput());
		growthParameters.g2 = std::stof(g2->getInput());
		growthParameters.thickeningFactor = std::stof(thickeningFactor->getInput());
		growthParameters.scalingCoefficient = std::stof(scalingCoefficient->getInput());
		growthParameters.vMax = std::stof(vMax->getInput());
		growthParameters.vMin = std::stof(vMin->getInput());
		return growthParameters;
	}

};

#endif
