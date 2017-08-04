#pragma once
#include<BENGINE\InputManager.h>
#include<BENGINE\camera2D.h>
#include"Agent.h"
class Bot : public Agent
{
public:
	Bot();
	~Bot();

	void init(float speed, glm::vec2 position, bengine::InputManager *inputManager, bengine::camera2D *camera);
	void update(const std::vector<std::string> &levelData,
		float deltaTime) override;
	void setId(int x) { _id = x; }
	int getId() const { return _id; }
	/*void addGun(Gun *gun);*/
	
private:
	
	bengine::InputManager *_inputManager;
	/*std::vector<Gun*> _guns;*/
	bengine::camera2D *_camera;
	int _id;
};

