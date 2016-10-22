#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	p2List<PhysBody*> board;

	PhysBody* sensor1;
	PhysBody* sensor2;
	PhysBody* sensor3;
	PhysBody* sensor4;
	PhysBody* sensor5;
	PhysBody* sensor6;
	PhysBody* sensor7;
	PhysBody* sensorballpassedexit;

	bool sensor1triggered = false;
	bool sensor2triggered = false;
	bool sensor3triggered = false;
	bool sensor4triggered = false;
	bool sensor5triggered = false;
	bool sensor6triggered = false;
	bool sensor7triggered = false;
	bool sensorballpassedexittriggered = false;

	PhysBody* sensor100points;
	PhysBody* sensor500points1;
	PhysBody* sensor500points2;
	PhysBody* sensor500points3;
	PhysBody* sensor1000points;
	PhysBody* sensorreset;

	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* main_board;
	SDL_Texture* sprites;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int realtime = 0;
	int currenttime = 0;
	SDL_Rect springrect_1;
	SDL_Rect springrect_2;
	SDL_Rect springrect_3;
	SDL_Rect springrect_4;
	SDL_Rect springrect_5;

	bool newball = false;
	bool reset = false;

	uint points = 0;
	uint maxpoints = 0;
	int numballs = 3;
};
