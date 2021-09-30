#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Obstacle.h"
#include "Label.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;

	Obstacle* m_pProjectile;
	bool movementEnabled = false;
	
	glm::vec2 m_mousePosition;

	// UI Items
	Label* m_pInstructionsLabel;

	float* startingX;
	float* startingY;
	float* launchElevationAngle;
	float* launchSpeed;
	float* accelerationGravity;

	glm::vec2 lineEnd, startingPos;
};

#endif /* defined (__PLAY_SCENE__) */