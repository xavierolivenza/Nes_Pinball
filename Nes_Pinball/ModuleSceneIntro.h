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

	PhysBody* wall;
	PhysBody* godball;

	bool springanimation = false;

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

	PhysBody* sensorcard10;
	PhysBody* sensorcardJ;
	PhysBody* sensorcardQ;
	PhysBody* sensorcardK;
	PhysBody* sensorcardA;

	bool sensorcard10triggered = false;
	bool sensorcardJtriggered = false;
	bool sensorcardQtriggered = false;
	bool sensorcardKtriggered = false;
	bool sensorcardAtriggered = false;
	bool cardstriggered = false;
	bool orangemaploaded = false;

	p2List<PhysBody*> paddlesL;
	p2List<PhysBody*> paddlesR;

	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	//SDL_Texture* paddleL;
	//SDL_Texture* paddleR;

	SDL_Texture* main_board;
	SDL_Texture* sprites;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int realtime = 0;
	int currenttime = 0;
	int currenttimeexit = 0;
	int currenttimepenguin = 0;
	SDL_Rect springrect_1;
	SDL_Rect springrect_2;
	SDL_Rect springrect_3;
	SDL_Rect springrect_4;
	SDL_Rect springrect_5;

	bool newball = false;
	bool reset = false;
	bool firstpenguin = true;

	uint points = 0;
	uint maxpoints = 0;
	int numballs = 3;

	// 0=dead
	// 1=egg
	// 2=chicken
	int chicken1state = 1;
	int chicken2state = 1;
	int chicken3state = 1;
	SDL_Rect egg;
	SDL_Rect chicken1;
	SDL_Rect chicken2;
	bool firstchicken = true;
	int currenttimechicken = 0;
	PhysBody* sensorchicken1;
	PhysBody* sensorchicken2;
	PhysBody* sensorchicken3;
	void BlitChickenAnimation(int, int, SDL_Rect&, SDL_Rect&);

	bool minispringsactivated = false;
	PhysBody* sensorminispring1;
	PhysBody* sensorminispring2;
};
