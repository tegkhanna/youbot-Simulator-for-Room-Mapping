#include "stdafx.h"
#include "Main.h"

#include<random>
#include<ctime>
#include<algorithm>
#include <Bengine/Bengine.h>
#include<BENGINE\Timings.h>
#include<BENGINE\Error.h>
#include <SDL/SDL.h>
#include <iostream>
#include"Bot.h"
int ids = 1; // to give individual ids to each human(had a sudden need for this ...just initialized it globally because i was being a lazy ass)
Main::Main() :
	_screenW(800),
	_screenH(800),
	_gameState(GameState::PLAY),
	_fps(0.0f),
	_level(nullptr)
{

}


Main::~Main()
{
}

void Main::run()
{
	initSystems();
	initLevel();
	gameLoop();
}

void Main::initSystems()
{
	// IMPLEMENT THIS!
	bengine::init();
	_window.create("try", _screenW, _screenH, 0);
	glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
	initShaders();
	_agentSB.init();
	_hudSpriteBatch.init();
	_pauseSpriteBatch.init();
	//inititalize sprite font
	_spriteFont = new bengine::SpriteFont("Fonts/OpenSans-Semibold.ttf", 64);

	//initialize cam
	_hudCamera.init(_screenW, _screenH);
	_hudCamera.setPos(glm::vec2(_screenW / 2.0f, _screenH / 2.0f));
	_camera.init(_screenW, _screenH);
	_camera.setPos(glm::vec2(_screenW / 2, (_screenH / 2) - 100));
	_camera.setScale(0.50f);

}

void Main::initLevel()
{
	_level = new Map("Levels/level6.txt");
	_player = new Bot;
	_player->init(10.0f, _level->getPlayerStart(), &_inputManager, &_camera);
	_player->setId(ids++);

}


void Main::initShaders()
{
	_textureProgram.compileShaders("Shaders/textureShading.ver", "Shaders/textureShading.frag");
	_textureProgram.addAttrib("vertexPosition");
	_textureProgram.addAttrib("vertexColor");
	_textureProgram.addAttrib("vertexUV");
	_textureProgram.linkShaders();
}

void Main::gameLoop()
{

	bengine::FpsLimiter fpsLimitter;
	fpsLimitter.setMaxFps(60.0f);

	const int MAX_STEPS = 6;
	const float MS_PER_SEC = 1000.0f;
	const float DESIRED_FPS = 60.0f;
	const float DESIRED_FRAME_TIME = MS_PER_SEC / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;
	float previousTicks = SDL_GetTicks();
	while (_gameState != GameState::EXIT)
	{
		///>SUPER SMARTTTTTTTTT SUPER DUPER :P // MAINTAINS A CONSTANT GAME SPEED IRRESPECTIVE OF THE FPS
		fpsLimitter.begin();
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;
		_inputManager.update();
		processInput();

		if (_inputManager.isKeyPressed(SDLK_p))
		{
			_gameState = GameState::PAUSE;

		}

		if (_gameState != GameState::PAUSE)
		{

			int i = 0;
			while (totalDeltaTime > 0 && i++ < MAX_STEPS)
			{
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
				updatePlayer(deltaTime);
				updateTime(deltaTime);
				if (_timeRemain <= 0)
				{
					_loose = true;
					_gameState = GameState::PAUSE;
				}
				totalDeltaTime -= deltaTime;
			}
		}

		_camera.setPos(glm::vec2(_player->getPos().x, _player->getPos().y));

		_camera.update();
		_hudCamera.update();
		drawGame();
		_fps = fpsLimitter.end();
		static int frames = 0;
		if (frames == 1000)
		{
			std::cout << _fps << std::endl;
			frames = 0;
		}
		else
			frames++;
	}
	if (_gameState == GameState::EXIT)
	{
		bengine::fatalError("bye bye .. thanks for playing :D :D :D", 1);
	}

}
void Main::updateTime(float deltaTime)
{
	static int see = 0;
	see++;
	if (see == _MAX_FPS)
	{
		_timeRemain = _timeRemain - deltaTime;
		see = 0;
	}

}
void Main::updatePlayer(float delta)
{
	_player->update(_level->getLevelData(), delta);

}

void Main::drawGame()
{
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);
	//get uniform from shader:
	//->sampler
	GLint textureLocation = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);
	//->camera matrix set up
	GLuint pLocation = _textureProgram.getUniformLocation("P");//location
															   //grab camera matrix
	glm::mat4 cameraMatrix = _camera.getCamMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//draw

	_level->draw();

	const glm::vec2 agentDim(RADIUS_A * 2.0f);
	//draw agents(including player)
	_agentSB.begin();
	
	
	
	
	_player->draw(_agentSB);
	
	
	//draw humans
	

	_agentSB.end();

	_agentSB.renderBatch();
	
	if (_gameState == GameState::PAUSE && _win == false && _loose == false)
	{
		
		if (_inputManager.isKeyPressed(SDLK_1) || _inputManager.isKeyPressed(SDLK_KP_1))
			_gameState = GameState::PLAY;
		if (_inputManager.isKeyPressed(SDLK_2) || _inputManager.isKeyPressed(SDLK_KP_2))
			_gameState = GameState::EXIT;
		if (_inputManager.isKeyPressed(SDLK_3) || _inputManager.isKeyPressed(SDLK_KP_3))
		{
			_gameState = GameState::PLAY;
			run();
		}
	}
	else if (_gameState == GameState::PAUSE && _win == true)
	{
		
		if (_inputManager.isKeyPressed(SDLK_1) || _inputManager.isKeyPressed(SDLK_KP_1))
			_gameState = GameState::EXIT;
	}
	else if (_gameState == GameState::PAUSE && _loose == true)
	{
		
		if (_inputManager.isKeyPressed(SDLK_1) || _inputManager.isKeyPressed(SDLK_KP_1))
			_gameState = GameState::EXIT;
	}
	_textureProgram.unUse();
	// Swap our buffer and draw everything to the screen!
	_window.swapBuff();
}
void Main::processInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			// Exit the game here!
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}




