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
	TextField* vRootMax = nullptr; // Maximum plant vigour (The maximum vigour of the root)
	TextField* gP = nullptr; // Growth rate of the whole plant
	TextField* apicalControl = nullptr; // Lambda in the paper, represents the growth suppression of an existing subdominant branch by a higher dominanting shoot thta functions to maintain crown dominance by a central stem
	TextField* determinacy = nullptr; // D in the paper
	TextField* gravityDirX = nullptr; // Alpha in the paper
	TextField* gravityDirY = nullptr; // Alpha in the paper
	TextField* gravityDirZ = nullptr; // Alpha in the paper
	//TextField* w1, w2 = nullptr; //  Weights that controll the impact of optimization criteria in gradient descent
	TextField* g1 = nullptr; // Controls how fast the tropism effect decreases with time
	TextField* g2 = nullptr; // Controls the overall strength of the tropism
	TextField* thickeningFactor = nullptr; // Phi in the paper
	TextField* scalingCoefficient = nullptr; // Its a scaling coefficient used in the paper for calculating the segment lengt
	TextField* vMax = nullptr; // Maximum module vigour, not clear in the paper whether this is global or module specific
	TextField* vMin = nullptr; // Minimum module vigour, not clear in the paper whether this is global or module specific
	TextField* maxModules = nullptr;

	int cn = 0;

	double initialTime;
	double currTime;

	const float height = 484.0f;
	const float width = 200.0f;
	const float heightPanel2 = 285.0f;
	const float widthPanel2 = 210.0f;

	const float padding = 10.0f;
	const float offSetTextField = 10.0f;

	bool paused = false;

	std::function<void(GrowthParameters growthParameters)> onStart;
	std::function<void()> onPause;
	std::function<void()> onUpdate;

	void buildFPSCounter(GUI* gui, float windowWidth, float windowHeight) {
		Text* fpsName = new Text("FPS: ", { 0.0f, 0.0f });
		{
			fpsName->setColor(ColorRGBA::RED);
			fpsName->setSize(4.0f);
			fpsName->setDepth(1);
			gui->addComponent(fpsName);
		}
		fps = new Text("", { 0.0f, 0.0f });
		{
			fps->setColor(ColorRGBA::RED);
			fps->setSize(4.0f);
			fps->setDepth(1);
			gui->addComponent(fps);
		}

		float fpsOffset = 2.0f;
		fpsName->setPosition({ windowWidth - fpsName->getWidth() - fpsOffset - widthPanel2 - padding, windowHeight - fpsName->getHeight() / 2 });
		fps->setPosition({ windowWidth - fpsName->getWidth() * 0.3f - widthPanel2 - padding, windowHeight - fpsName->getHeight() / 2 });
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

	float buildTitleTextFieldPairR(GUI* gui, float windowWidth, float windowHeight, const std::string& title, float yStart, TextField*& builtTextField) {

		const float textFieldHeight = 25.0f;

		Text* textFieldName = new Text(title, { 0.0f,0.0f });
		{
			textFieldName->setSize(2.5f);
			textFieldName->setDepth(1);
			textFieldName->setPosition({ windowWidth - ((textFieldName->getWidth() * 0.5f) + padding),  yStart - (textFieldName->getHeight() * 0.5f) - padding });
			gui->addComponent(textFieldName);
		}

		Vec2 textFieldPosition = { windowWidth - ((width * 0.9f) / 2 + padding), yStart - (textFieldHeight * 0.5f) - offSetTextField - padding - (textFieldName->getHeight() * 0.5f) };
		builtTextField = new TextField(textFieldPosition, width * 0.9f, textFieldHeight);
		{
			builtTextField->setDepth(1);
			gui->addComponent(builtTextField);
		}

		return textFieldPosition.y - textFieldHeight / 2;
	}

	void buildTreeGrowthUI(GUI* gui, float windowWidth, float windowHeight) {

		Panel* panel2 = new Panel({ windowWidth - widthPanel2 / 2, windowHeight - (heightPanel2) / 2 }, widthPanel2, heightPanel2);
		{
			panel2->setDepth(0); // Panel is behind the text fields and buttons
			gui->addComponent(panel2);

			Text* panelTitle = new Text("Adaptation parameters", { 0.0f, 0.0f });
			{
				panelTitle->setSize(4);
				panelTitle->setDepth(1);
				panelTitle->setPosition({ windowWidth - (panelTitle->getWidth() * 0.5f) - padding,  windowHeight - (panelTitle->getHeight() * 0.5f) - padding });
				gui->addComponent(panelTitle);
			}

			GrowthParameters* growthParameters = GrowthParameters::instance;

			// Gravity Dir
			float startY = buildTitleTextFieldPairR(gui, windowWidth, windowHeight, "Gravity Dir X", windowHeight - 25, gravityDirX);
			gravityDirX->setInput(growthParameters ? std::to_string(growthParameters->gravityDir.x) : "0.0");
			startY = buildTitleTextFieldPairR(gui, windowWidth, windowHeight, "Gravity Dir Y", startY, gravityDirY);
			gravityDirY->setInput(growthParameters ? std::to_string(growthParameters->gravityDir.y) : "0.0");
			startY = buildTitleTextFieldPairR(gui, windowWidth, windowHeight, "Gravity Dir Z", startY, gravityDirZ);
			gravityDirZ->setInput(growthParameters ? std::to_string(growthParameters->gravityDir.z) : "0.0");
			startY = buildTitleTextFieldPairR(gui, windowWidth, windowHeight, "Tropism Decrease", startY, g1);
			g1->setInput(growthParameters ? std::to_string(growthParameters->g1) : "0.0");
			startY = buildTitleTextFieldPairR(gui, windowWidth, windowHeight, "Tropism Strength", startY, g2);
			g2->setInput(growthParameters ? std::to_string(growthParameters->g2) : "0.0");
		}

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

			float startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Root Vigour", windowHeight - 25, vRootMax);
			vRootMax->setInput(growthParameters ? std::to_string(growthParameters->vRootMax) : "0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Growth Rate", startY, gP);
			gP->setInput(growthParameters ? std::to_string(growthParameters->gP) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Apical Control", startY, apicalControl);
			apicalControl->setInput(growthParameters ? std::to_string(growthParameters->apicalControl) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Determinacy", startY, determinacy);
			determinacy->setInput(growthParameters ? std::to_string(growthParameters->determinacy) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Thickening Factor", startY, thickeningFactor);
			thickeningFactor->setInput(growthParameters ? std::to_string(growthParameters->thickeningFactor) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Scaling coeficient", startY, scalingCoefficient);
			scalingCoefficient->setInput(growthParameters ? std::to_string(growthParameters->scalingCoefficient) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Vigour", startY, vMax);
			vMax->setInput(growthParameters ? std::to_string(growthParameters->vMax) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Min Vigour", startY, vMin);
			vMin->setInput(growthParameters ? std::to_string(growthParameters->vMin) : "0.0");
			startY = buildTitleTextFieldPair(gui, windowWidth, windowHeight, "Max Modules", startY, maxModules);
			maxModules->setInput(growthParameters ? std::to_string(growthParameters->maxModules) : "0");

			Button* pause = new Button({ windowWidth * 0.5f, windowHeight - 30.0f * 0.5f - padding });
			{
				pause->setDepth(1);
				pause->setButtonText("Pause", ColorRGBA::BLACK, 4.0f);
				pause->setOnClickListener([=]() {

					if (!paused) {
						pause->setButtonText("Resume", ColorRGBA::BLACK, 4.0f);
						paused = true;
					}
					else {
						pause->setButtonText("Pause", ColorRGBA::BLACK, 4.0f);
						paused = false;
					}

					onPause();
				});
				gui->addComponent(pause);
			}

			Button* start = new Button({ windowWidth * 0.5f - 90.0f - padding, windowHeight - 30.0f * 0.5f - padding });
			{
				start->setDepth(1);
				start->setButtonText("Start", ColorRGBA::BLACK, 4.0f);
				start->setOnClickListener([=]() {
					if (paused) {
						pause->setButtonText("Pause", ColorRGBA::BLACK, 4.0f);
						paused = false;
					}
					
					onStart(getGrowthParameters());
				});
				gui->addComponent(start);
			}

			Button* stop = new Button({ windowWidth * 0.5f + 90.0f + padding, windowHeight - 30.0f * 0.5f - padding });
			{
				stop->setDepth(1);
				stop->setButtonText("Update", ColorRGBA::BLACK, 4.0f);
				stop->setOnClickListener([=]() {
					onUpdate();
				});
				gui->addComponent(stop);
			}
		}

		
	}

public:
	TreeGrowthUI(GUI* gui, float windowWidth, float windowHeight, std::function<void(GrowthParameters growthParameters)> onStart,
	std::function<void()> onPause,
	std::function<void()> onUpdate) {
		this->onStart = onStart;
		this->onPause = onPause,
		this-> onUpdate = onUpdate;
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
		growthParameters.vRootMax = unsigned int(std::stoul(vRootMax->getInput()));
		growthParameters.gP = std::stof(gP->getInput());
		growthParameters.apicalControl = std::stof(apicalControl->getInput());
		growthParameters.determinacy = std::stof(determinacy->getInput());
		growthParameters.gravityDir = Vec3(std::stof(gravityDirX->getInput()), std::stof(gravityDirY->getInput()), std::stof(gravityDirZ->getInput()));
		growthParameters.g1 = std::stof(g1->getInput());
		growthParameters.g2 = std::stof(g2->getInput());
		growthParameters.thickeningFactor = std::stof(thickeningFactor->getInput());
		growthParameters.scalingCoefficient = std::stof(scalingCoefficient->getInput());
		growthParameters.vMax = std::stof(vMax->getInput());
		growthParameters.vMin = std::stof(vMin->getInput());
		growthParameters.maxModules = unsigned int(std::stoul(maxModules->getInput()));
		return growthParameters;
	}

};

#endif
