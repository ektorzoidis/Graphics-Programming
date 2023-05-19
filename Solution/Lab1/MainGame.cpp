#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	// Initialise display
	Display* _gameDisplay = new Display();
	// Initialise shaders
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	// Initialise textures
	Texture texture();
	Texture texture1();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	
	// Load models
	plane.loadModel("..\\res\\Plane.obj");
	cube.loadModel("..\\res\\Cube.obj");
	sphere.loadModel("..\\res\\Sphere.obj");
	owl.loadModel("..\\res\\Owl.obj");
	lion.loadModel("..\\res\\Lion.obj");
	ball.loadModel("..\\res\\Ball.obj");
	platform.loadModel("..\\res\\Platform.obj");
	// Load shaders
	eMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	glowShader.init("..\\res\\glow.vert", "..\\res\\glow.frag");
	lavaShader.init("..\\res\\lava.vert", "..\\res\\lava.frag");
	marbleShader.init("..\\res\\marble.vert", "..\\res\\marble.frag");
	darkMarbleShader.init("..\\res\\darkMarble.vert", "..\\res\\darkMarble.frag");
	geoShader.initGeo();
	// Load textures
	wood.init("..\\res\\owl.jpg");
	lava.init("..\\res\\lava.png");
	normal1.init("..\\res\\normal.jpg");
	normal2.init("..\\res\\normal.png");
	sky.init("..\\res\\sky.jpg");

	// Initialise camera
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	// Set counter
	counter = 1.0f;

	// Set skybox textures
	vector<std::string> faces
	{
		"..\\res\\skybox\\LancellottiChapel\\posx.jpg",
		"..\\res\\skybox\\LancellottiChapel\\negx.jpg",
		"..\\res\\skybox\\LancellottiChapel\\posy.jpg",
		"..\\res\\skybox\\LancellottiChapel\\negy.jpg",
		"..\\res\\skybox\\LancellottiChapel\\posz.jpg",
		"..\\res\\skybox\\LancellottiChapel\\negz.jpg"
	};
	// Initialise skybox
	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		// Process events
		processInput();
		// Draw window
		drawGame();
		// Generate collision
		//collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;
	float speed = 0.5f;

	// Get and process events
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		//case SDL_MOUSEBUTTONDOWN:
		//	break;
		//// On key pressed
		//case SDL_KEYDOWN:
		//	switch (evnt.key.keysym.sym)
		//	{
		//		// A pressed
		//	case SDLK_a:
		//		myCamera.MoveRight(speed);
		//		break;
		//		// D pressed
		//	case SDLK_d:
		//		myCamera.MoveRight(-speed);
		//		break;
		//		// W pressed
		//	case SDLK_w:
		//		myCamera.MoveForward(speed);
		//		break;
		//		// S pressed
		//	case SDLK_s:
		//		myCamera.MoveForward(-speed);
		//		break;
		//		// E pressed
		//	case SDLK_e:
		//		myCamera.RotateY(-speed / 2);
		//		break;
		//		// Q pressed
		//	case SDLK_q:
		//		myCamera.RotateY(speed / 2);
		//		break;
		//		// Z pressed
		//	case SDLK_z:
		//		myCamera.Pitch(-speed);
		//		break;
		//		// X pressed
		//	case SDLK_x:
		//		myCamera.Pitch(speed);
		//		break;
		//	}
		//	break;
		//// On mouse wheel
		//case SDL_MOUSEWHEEL:
		//	// Scroll up
		//	if (evnt.wheel.y > 0)
		//	{
		//		myCamera.MoveForward(speed);
		//		break;
		//	}
		//	// Scroll down
		//	else if (evnt.wheel.y < 0)
		//	{
		//		myCamera.MoveForward(-speed);
		//		break;
		//	}
		//	break;
		// On quit
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		}
	}
}

bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::linkGeo()
{
	glEnable(GL_CULL_FACE);

	// Set uniforms
	geoShader.setFloat("time", counter);
}

void MainGame::linkEmapping()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set uniforms
	eMapping.setMat4("model", transform1.GetModel());
	eMapping.setVec3("cameraPos", myCamera.getPos());

	// Set textures
	GLuint t1L = glGetUniformLocation(eMapping.getID(), "diffuse");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sky.getID());
	glUniform1i(t1L, 1);
}

void MainGame::linkLava(bool change)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	// Set uniforms
	lavaShader.setMat4("model", transform3.GetModel());
	lavaShader.setVec3("cameraPos", myCamera.getPos());
	lavaShader.setFloat("time", counter);
	//phong.setVec2("resolution", glm::vec2(355, 355));

	// Set textures
	GLuint t1L = glGetUniformLocation(lavaShader.getID(), "lava");
	GLuint t2L = glGetUniformLocation(lavaShader.getID(), "normal");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lava.getID());
	glUniform1i(t1L, 0);
	glActiveTexture(GL_TEXTURE1);
	if (change)
		glBindTexture(GL_TEXTURE_2D, normal1.getID());
	else
		glBindTexture(GL_TEXTURE_2D, normal2.getID());
	glUniform1i(t2L, 1);
}

void MainGame::linkGlow()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set uniforms
	glowShader.setMat4("model", transform3.GetModel());
	glowShader.setFloat("time", counter);
}

void MainGame::drawGame()
{
	// Set background colour
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f);

	//---------------Skybox---------------
	// Draw Skybox
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	skybox.draw(&myCamera);

	//---------------Enviroment mapping---------------
	// Set enviroment mapping shader
	eMapping.Bind();
	linkEmapping();
	// Bind texture
	sky.Bind(0);
	// Set transform
	transform1.SetPos(glm::vec3(0.0, 5 + cubePos, 0.0));
	transform1.SetRot(glm::vec3(0.0, counter / 2, 0.0));
	transform1.SetScale(glm::vec3(1.0, 1.0, 1.0));
	// Update shader
	eMapping.Update(transform1, myCamera);
	// Draw mesh
	cube.draw();
	// Update collision
	cube.updateSphereData(*transform1.GetPos(), 0.62f);

	//---------------Geo shader---------------
	// Set geo shader
	geoShader.Bind();
	linkGeo();
	// Bind texture
	wood.Bind(0);
	// Set transform
	transform2.SetPos(glm::vec3(0.0, 5.0 + owlPos, 0.0));
	transform2.SetRot(glm::vec3(0.0, -sinf(counter * 2) - 180 + 45, 0.0));
	transform2.SetScale(glm::vec3(7.0, 7.0, 7.0));
	// Update shader
	geoShader.Update(transform2, myCamera);
	// Draw mesh
	owl.draw();
	// Update collision
	owl.updateSphereData(*transform2.GetPos(), 0.62f);

	//---------------Marble shader---------------
	// Set marble shader
	marbleShader.Bind();
	linkLava(false);
	// Set transform
	transform3.SetPos(glm::vec3(0.0, 5.0 + lionPos, 0.0));
	transform3.SetRot(glm::vec3(0.0, counter * 1.2, 0.0));
	transform3.SetScale(glm::vec3(1.5, 1.5, 1.5));
	// Update shader
	marbleShader.Update(transform3, myCamera);
	// Draw mesh
	lion.draw();
	// Update collision
	lion.updateSphereData(*transform3.GetPos(), 0.62f);

	//---------------Dark Marble shader---------------
	// Set marble shader
	darkMarbleShader.Bind();
	linkLava(true);
	// Set transform
	transform3.SetPos(glm::vec3(0.0, 5.0 + lionPos, 0.0));
	transform3.SetRot(glm::vec3(0.0, counter * 1.2, 0.0));
	transform3.SetScale(glm::vec3(1.5, 1.5, 1.5));
	// Update shader
	darkMarbleShader.Update(transform3, myCamera);
	// Draw mesh
	platform.draw();
	// Update collision
	platform.updateSphereData(*transform3.GetPos(), 0.62f);

	//---------------Lava shader---------------
	// Set lava shader
	lavaShader.Bind();
	linkLava(true);
	// Set transform
	transform3.SetPos(glm::vec3(0.0, 5.0 + lionPos, 0.0));
	transform3.SetRot(glm::vec3(0.0, counter * 1.2, 0.0));
	transform3.SetScale(glm::vec3(1.5, 1.5, 1.5));
	// Update shader
	lavaShader.Update(transform3, myCamera);
	// Draw mesh
	ball.draw();
	// Update collision
	ball.updateSphereData(*transform3.GetPos(), 0.62f);

	//---------------Glow shader---------------
	// Set glow shader
	glowShader.Bind();
	linkGlow();
	// Set transform
	transform4.SetPos(glm::vec3(0.0, 5.0 + lionPos, 0.05));
	transform4.SetRot(glm::vec3(0.0, counter * 1.2, 0.0));
	transform4.SetScale(glm::vec3(1.55, 1.55, 1.55));
	// Update shader
	glowShader.Update(transform4, myCamera);
	// Draw mesh
	ball.draw();
	// Update collision
	ball.updateSphereData(*transform4.GetPos(), 0.62f);

	//---------------Movement---------------
	// Update counter
	counter = counter + 0.01f;
	// Move cube
	if (counter >= 5 && cubePos >= -10)
		cubePos -= counter * 0.05;
	else if (cubePos >= -5)
		cubePos -= counter * 0.05;
	// Move skull
	if (counter >= 10 && owlPos >= -10)
		owlPos -= counter * 0.05;
	else if (counter >= 5 && owlPos >= -5)
		owlPos -= counter * 0.05;
	// Move monkey
	if (counter >= 20 && lionPos >= -10)
		lionPos -= counter * 0.007;
	else if (counter >= 10 && lionPos >= -5)
		lionPos -= counter * 0.007;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();
}
