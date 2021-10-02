#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <math.h>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance().draw("bg", 400, 300, 0, 255, true);
	drawDisplayList();

	Util::DrawLine(startingPos, lineEnd, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	if (m_pProjectile->getTransform()->position.y <= *startingY)
	{

		m_pProjectile->getRigidBody()->velocity.y += (*accelerationGravity / 60);
	}
	else
	{
		m_pProjectile->getRigidBody()->velocity *= 0.0f;
	}

	updateDisplayList();

	float leaRadians = *launchElevationAngle * Util::Deg2Rad;

	startingPos = glm::vec2(*startingX, *startingY);
	lineEnd = glm::vec2(
		*startingX + (100.0f * cos(leaRadians)),
		*startingY - (100.0f * sin(leaRadians))
	);

	distanceTravelled = m_pProjectile->getTransform()->position.x - *startingX;
	m_pDistanceLabel->setText("Distance Travelled: " + std::to_string(distanceTravelled) + " Pixels (1 pixel per meter)");
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	float leaRadians = *launchElevationAngle * Util::Deg2Rad;

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				
			}
			else
			{
				
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			
		}
		else
		{
			
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		//reset projectile location and velocity
		m_pProjectile->getTransform()->position = startingPos;
		m_pProjectile->getRigidBody()->velocity *= 0.0f;
		//yeet the projectile
		m_pProjectile->getRigidBody()->velocity.x = ((*launchSpeed * cos(leaRadians)));
		//y velocity must be negated because y coordinate values increase the further down you go
		m_pProjectile->getRigidBody()->velocity.y = -(*launchSpeed * sin(leaRadians));
	}
}

void PlayScene::start()
{
	TextureManager::Instance().load("../Assets/Textures/background.jpg", "bg");

	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	startingX = new float(10.0f);
	startingY = new float (400.0f);
	launchElevationAngle = new float (15.8896328215f);
	launchSpeed = new float (95.0f);
	accelerationGravity = new float (9.8f);

	float leaRadians = *launchElevationAngle * Util::Deg2Rad;

	startingPos = glm::vec2(*startingX, *startingY);
	lineEnd = glm::vec2(
		*startingX + (100.0f * cos(leaRadians)),
		*startingY - (100.0f * sin(leaRadians))
	);

	/* Labels */
	m_pDistanceLabel = new Label("Distance Travelled: " + std::to_string(distanceTravelled) + " Pixels (1 pixel per meter)", "Consolas");
	m_pDistanceLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 100.0f);

	addChild(m_pDistanceLabel);

	m_pInstructionsLabel = new Label("Press Space to throw projectile and ` to open debug view", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);

	addChild(m_pInstructionsLabel);

	/* Projectile to model */
	m_pProjectile = new Obstacle();
	m_pProjectile->getTransform()->position = glm::vec2(*startingX, *startingY);

	addChild(m_pProjectile);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	if (ImGui::SliderFloat("Starting X", startingX, 0.0f, 800.0f))
	{
		m_pProjectile->getRigidBody()->velocity *= 0.0f;
		m_pProjectile->getTransform()->position = startingPos;
	}
	if (ImGui::SliderFloat("Launch Angle - Deg", launchElevationAngle, 0.0f, 360.0f))
	{

	}
	if (ImGui::SliderFloat("Launch Speed - M/s", launchSpeed, 0.0f, 100.0f))
	{

	}
	if (ImGui::SliderFloat("Gravity - M/s", accelerationGravity, -10.0f, 10.0f))
	{

	}

	//ImGui::Separator();

	//ImGui::Text(std::to_string(distanceTravelled));
	
	ImGui::End();
}
