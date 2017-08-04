#pragma once
#include<vector>
#include<string>
#include<BENGINE/spriteBatch.h>
const int TILE_WIDTH = 64;

class Map
{
public:
	Map(const std::string &fileName);
	~Map();

	void draw();

	//getters
	int getWidth() const { return _mapData[0].size(); }
	int getHeight() const { return _mapData.size(); }
	glm::vec2 getPlayerStart() const { return _startBotPos; }
	const std::vector<std::string> &getLevelData() const { return _mapData; }

private:
	std::vector<std::string> _mapData;
	bengine::spriteBatch _spriteBatch;
	glm::vec2 _startBotPos;
};

