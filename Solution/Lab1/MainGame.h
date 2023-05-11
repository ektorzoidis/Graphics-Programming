#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "SkyBox.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkGeo();
	void linkEmapping();
	void linkLava();
	void linkGlow();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);

	//void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;
	Mesh plane, cube, sphere, owl, lion;
	Transform transform1, transform2, transform3, transform4;
	Camera myCamera;
	Shader geoShader, shaderSkybox, eMapping, glowShader, lavaShader;
	Texture wood, lava, normal, sky;
	
	Skybox skybox;
	vector<std::string> faces;
	
	float counter, cubePos, owlPos, lionPos;
	bool skyboxChanged;
};
