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

	PhysBody* sensor100points;
	PhysBody* sensor500points;
	PhysBody* sensor1000points;
	PhysBody* sensorreset;
	PhysBody* sensorstopballspace;

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

};
