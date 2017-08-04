#pragma once

#include <bengine/Window.h>
#include <Bengine/GLSLprog.h>
#include <bengine/Camera2D.h>
#include <bengine/InputManager.h>
#include<BENGINE\spriteBatch.h>
#include<BENGINE\SpriteFont.h>
#include<BENGINE\Sounds.h>
#include"Map.h"
#include"Bot.h"


enum class GameState
{
	PLAY,
	PAUSE,
	EXIT
};


class Main
{
public:
	Main();
	~Main();
	void initSystems();
	void run();
	void gameLoop();
	void initLevel();
	void drawGame();
	/// Initializes the shaders
	void initShaders();
	/// Main game loop for the program

	/// Handles input processing
	void processInput();
	void updateTime(float deltaTime);
private:
	GameState _gameState;
	bengine::Window _window; ///< The game window
	bengine::GLSLprog _textureProgram; ///< The shader program
	bengine::InputManager _inputManager; ///< Handles input
	bengine::camera2D _camera; ///< Main Camera
	bengine::camera2D _hudCamera;
	bengine::spriteBatch _agentSB;
	bengine::spriteBatch _hudSpriteBatch;///heads up display
	bengine::spriteBatch _pauseSpriteBatch;

	//FONT BATCH
	bengine::SpriteFont *_spriteFont;

	int _screenW, _screenH;
	float _fps;
	Bot *_player;
	Map *_level;
	void updatePlayer(float delta);
	
	bool _win = false;
	bool _loose = false;
	int _barafed = 0;
	int _timeRemain = 120;
	const int _MAX_FPS = 60;
};

