#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	main_board = App->textures->Load("pinball/Pinball_Main_Board.png");
	sprites = App->textures->Load("pinball/PinballSheet.png");
	game_over_tex = App->textures->Load("pinball/Game_Over.png");

	//Test board
	//main_board = App->textures->Load("pinball/Pinball_Board_with_score_clean.png");
	
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	paddle_fx = App->audio->LoadFx("pinball/paddle.wav");

	wall = App->physics->CreateRectangle(214, 305, 9, 24, 0, b2_staticBody, 0);

	godball1 = App->physics->CreateCircle(144, 446, 7, b2_staticBody, 0);
	godball1->body->SetActive(false);

	godball2 = App->physics->CreateCircle(144, 217, 7, b2_staticBody, 0);
	godball2->body->SetActive(false);

	sensor1 = App->physics->CreateRectangleSensor(82, 296, 2, 6, 0);
	sensor2 = App->physics->CreateRectangleSensor(82, 304, 2, 6, 0);
	sensor3 = App->physics->CreateRectangleSensor(82, 312, 2, 6, 0);
	sensor4 = App->physics->CreateRectangleSensor(82, 321, 2, 6, 0);
	sensor5 = App->physics->CreateRectangleSensor(82, 328, 2, 6, 0);
	sensor6 = App->physics->CreateRectangleSensor(82, 336, 2, 6, 0);
	sensor7 = App->physics->CreateRectangleSensor(82, 344, 2, 6, 0);
	sensorballpassedexit = App->physics->CreateRectangleSensor(217, 305, 2, 24, 0);

	sensor100points = App->physics->CreateRectangleSensor(114, 80, 12, 4, 172);
	sensor500points1 = App->physics->CreateRectangleSensor(160, 60, 12, 4, 0);
	sensor500points2 = App->physics->CreateRectangleSensor(127, 60, 12, 4, 0);
	sensor1000points = App->physics->CreateRectangleSensor(144, 60, 12, 4, 0);

	sensorcard10 = App->physics->CreateRectangleSensor(113, 291, 12, 4, 0);
	sensorcardJ = App->physics->CreateRectangleSensor(129, 291, 12, 4, 0);
	sensorcardQ = App->physics->CreateRectangleSensor(145, 291, 12, 4, 0);
	sensorcardK = App->physics->CreateRectangleSensor(161, 291, 12, 4, 0);
	sensorcardA = App->physics->CreateRectangleSensor(177, 291, 12, 4, 0);

	sensorchicken1 = App->physics->CreateRectangleSensor(124, 391, 8, 12, 0);
	sensorchicken2 = App->physics->CreateRectangleSensor(145, 391, 8, 12, 0);
	sensorchicken3 = App->physics->CreateRectangleSensor(164, 391, 8, 12, 0);

	sensorminispring1 = App->physics->CreateRectangleSensor(80, 407, 13, 3, 0);
	sensorminispring1->body->SetActive(false);
	sensorminispring2 = App->physics->CreateRectangleSensor(209, 407, 13, 3, 0);
	sensorminispring2->body->SetActive(false);

	sensorcoin1 = App->physics->CreateRectangleSensor(89, 103, 5, 5, 0);
	sensorcoin2 = App->physics->CreateRectangleSensor(86, 95, 5, 5, 0);
	sensorcoin3 = App->physics->CreateRectangleSensor(83, 87, 5, 5, 0);
	sensorcoin4 = App->physics->CreateRectangleSensor(80, 78, 5, 5, 0);
	sensorcoin5 = App->physics->CreateRectangleSensor(80, 68, 5, 5, 0);
	sensorcoin6 = App->physics->CreateRectangleSensor(82, 58, 5, 5, 0);
	sensorcoin7 = App->physics->CreateRectangleSensor(88, 50, 5, 5, 0);
	sensorcoin8 = App->physics->CreateRectangleSensor(93, 43, 5, 5, 0);

	directionalwall1 = App->physics->CreateRectangle(101, 37, 7, 21, 0, b2_staticBody, 0);
	sensordirectionalwallin1 = App->physics->CreateRectangleSensor(95, 37, 2, 21, 0);
	sensordirectionalwallout1 = App->physics->CreateRectangleSensor(109, 35, 2, 21, 0);

	directionalwall2 = App->physics->CreateRectangle(197, 34, 7, 21, 0, b2_staticBody, 0);
	sensordirectionalwallin2 = App->physics->CreateRectangleSensor(225, 75, 10, 2, 0);
	sensordirectionalwallout2 = App->physics->CreateRectangleSensor(188, 34, 2, 21, 0);

	directionalwall3 = App->physics->CreateRectangle(203, 133, 5, 18, 150, b2_staticBody, 0);
	sensordirectionalwallin3 = App->physics->CreateRectangleSensor(206, 100, 10, 2, 0);
	sensordirectionalwallout3 = App->physics->CreateRectangleSensor(200, 136, 2, 21, 150);

	sensorextrapoints1 = App->physics->CreateRectangleSensor(90, 132, 2, 7, 0);
	sensorextrapoints2 = App->physics->CreateRectangleSensor(90, 140, 2, 7, 0);
	sensorextrapoints3 = App->physics->CreateRectangleSensor(90, 148, 2, 7, 0);
	sensorextrapoints4 = App->physics->CreateRectangleSensor(90, 156, 2, 7, 0);

	sensorreset = App->physics->CreateRectangleSensor(145, SCREEN_HEIGHT + 10, 50, 4, 0);

	int Pinball_MainBoard_2_coords[74] = {
		149, 293,
		149, 72,
		146, 62,
		141, 52,
		136, 46,
		125, 41,
		115, 41,
		99, 57,
		99, 71,
		107, 71,
		112, 67,
		117, 65,
		128, 65,
		136, 68,
		141, 75,
		144, 83,
		144, 131,
		141, 139,
		137, 144,
		132, 147,
		132, 160,
		146, 174,
		146, 184,
		142, 188,
		135, 188,
		127, 181,
		116, 192,
		116, 200,
		123, 207,
		123, 245,
		130, 260,
		143, 260,
		146, 265,
		146, 272,
		140, 285,
		140, 293,
		149, 293
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_MainBoard_2_coords, 72, 0));

	int Pinball_MiniPart_1_Board_coords[14] = {
		34, 381,
		42, 406,
		41, 409,
		36, 406,
		33, 401,
		33, 382,
		34, 381
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_MiniPart_1_Board_coords, 12, 1));

	int Pinball_MiniPart_2_Board_coords[16] = {
		118, 382,
		118, 401,
		115, 405,
		110, 409,
		107, 407,
		116, 383,
		117, 381,
		118, 382
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_MiniPart_2_Board_coords, 14, 1));

	int Pinball_Part_1_Board_coords[28] = {
		37, 43,
		51, 57,
		51, 71,
		35, 88,
		35, 101,
		32, 103,
		29, 103,
		24, 94,
		20, 82,
		20, 63,
		23, 56,
		27, 49,
		32, 45,
		37, 43
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Part_1_Board_coords, 26, 0));

	int Pinball_Part_2_Board_coords[20] = {
		116, 88,
		121, 84,
		124, 84,
		130, 88,
		130, 124,
		128, 127,
		125, 127,
		123, 124,
		123, 96,
		116, 88
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Part_2_Board_coords, 18, 0));

	int Pinball_Part_3_Board_coords[20] = {
		23, 181,
		35, 192,
		35, 200,
		27, 208,
		26, 257,
		24, 260,
		21, 260,
		17, 253,
		17, 187,
		23, 181
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Part_3_Board_coords, 18, 0));

	int Pinball_Line_1_coords[22] = {
		19, 381,
		19, 409,
		23, 414,
		30, 419,
		35, 423,
		35, 426,
		29, 421,
		22, 416,
		17, 410,
		17, 381,
		19, 381
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Line_1_coords, 20, 0));

	int Pinball_Line_2_coords[22] = {
		133, 381,
		133, 409,
		130, 414,
		123, 420,
		115, 426,
		115, 423,
		122, 418,
		128, 413,
		131, 409,
		131, 381,
		133, 381
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Line_2_coords, 20, 0));

	int Pinball_Separator_1_Board_coords[14] = {
		67, 57,
		68, 58,
		68, 65,
		67, 66,
		66, 65,
		66, 58,
		67, 57
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Separator_1_Board_coords, 12, 0));

	int Pinball_Separator_2_Board_coords[14] = {
		84, 56,
		85, 58,
		85, 65,
		84, 67,
		83, 65,
		83, 58,
		84, 56
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_2_Board_coords, 12, 0));

	int Pinball_Separator_3_Board_coords[14] = {
		36, 285,
		37, 286,
		37, 294,
		36, 296,
		35, 294,
		35, 286,
		36, 285
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Separator_3_Board_coords, 12, 0));

	int Pinball_Separator_4_Board_coords[14] = {
		52, 285,
		53, 286,
		53, 295,
		52, 296,
		51, 295,
		51, 286,
		52, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_4_Board_coords, 12, 0));

	int Pinball_Separator_5_Board_coords[14] = {
		68, 285,
		69, 286,
		69, 295,
		68, 296,
		67, 295,
		67, 286,
		68, 285
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Separator_5_Board_coords, 12, 0));

	int Pinball_Separator_6_Board_coords[14] = {
		84, 285,
		85, 286,
		85, 295,
		84, 296,
		83, 295,
		83, 286,
		84, 285
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Separator_6_Board_coords, 12, 0));

	int Pinball_Separator_7_Board_coords[14] = {
		100, 285,
		101, 286,
		101, 295,
		100, 296,
		99, 295,
		99, 286,
		100, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_7_Board_coords, 12, 0));

	int Pinball_Separator_8_Board_coords[14] = {
		116, 285,
		117, 286,
		117, 295,
		116, 296,
		115, 295,
		115, 286,
		116, 285
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Separator_8_Board_coords, 12, 0));

	springrect_1.x = 102;
	springrect_1.y = 129;
	springrect_1.h = 44;
	springrect_1.w = 9;
	springrect_2.x = 115;
	springrect_2.y = 129;
	springrect_2.h = 44;
	springrect_2.w = 9;
	springrect_3.x = 127;
	springrect_3.y = 129;
	springrect_3.h = 44;
	springrect_3.w = 10;
	springrect_4.x = 140;
	springrect_4.y = 129;
	springrect_4.h = 44;
	springrect_4.w = 9;
	springrect_5.x = 152;
	springrect_5.y = 129;
	springrect_5.h = 44;
	springrect_5.w = 9;

	egg.x = 125;
	egg.y = 232;
	egg.h = 12;
	egg.w = 8;
	chicken1.x = 103;
	chicken1.y = 232;
	chicken1.h = 12;
	chicken1.w = 8;
	chicken2.x = 114;
	chicken2.y = 232;
	chicken2.h = 12;
	chicken2.w = 8;

	minispring.x = 107;
	minispring.y = 206;
	minispring.h = 6;
	minispring.w = 11;

	coinrect.x = 152;
	coinrect.y = 368;
	coinrect.h = coinrect.w = 5;

	directionalwall1rect.x = 90;
	directionalwall1rect.y = 359;
	directionalwall1rect.h = 14;
	directionalwall1rect.w = 9;
	directionalwall2rect.x = 105;
	directionalwall2rect.y = 359;
	directionalwall2rect.h = 14;
	directionalwall2rect.w = 8;
	directionalwall3rect.x = 116;
	directionalwall3rect.y = 360;
	directionalwall3rect.h = 13;
	directionalwall3rect.w = 12;

	extracoinsrect.x = 137;
	extracoinsrect.y = 366;
	extracoinsrect.h = 7;
	extracoinsrect.w = 2;

	//Create initial ball
	circles.add(App->physics->CreateCircle(225, 390, 5.5, b2_dynamicBody, 0));
	circles.getFirst()->data->body->SetBullet(true);
	circles.getLast()->data->listener = this;

	paddlesL.add(App->physics->CreatePaddleL(108, 429, (40 * DEGTORAD), -30 * DEGTORAD));
	paddlesL.add(App->physics->CreatePaddleL(109, 201, (40 * DEGTORAD), -30 * DEGTORAD));
	paddlesR.add(App->physics->CreatePaddleR(180, 430, (146 * DEGTORAD), 78 * DEGTORAD));
	paddlesR.add(App->physics->CreatePaddleR(179, 203, (146 * DEGTORAD), 78 * DEGTORAD));

	bouncer = App->physics->CreateRectangle(226, 420, 3, 9, 0, b2_dynamicBody, 0);
	bouncerWheel = App->physics->CreateRectangle(225, 450, 10, 0, 0, b2_staticBody, 0);
	App->physics->CreateLineJoint(bouncer->body, bouncerWheel->body, p2Point<float>(0, 0), p2Point<float>(0, 0), 30.0f, 0.0f);

	kincreate = App->physics->CreateRectangle(120, 140, 14, 5, 0, b2_kinematicBody, 1);
	kincreate->body->SetLinearVelocity(b2Vec2(1, 0));

	pinkballs1 = App->physics->CreateCircleSensor(125, 120, 15, b2_staticBody, 25, 2);
	pinkball1 = App->physics->CreateCircle(125, 120, 11, b2_staticBody, 2);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(main_board, 0, 0);

	realtime = SDL_GetTicks();

	kincreate->GetPosition(pinkplatformx, pinkplatformy);
		if (pinkplatformx < 120) {
			kincreate->body->SetLinearVelocity(b2Vec2(0.75, 0));
		}
		else if (pinkplatformx > 153) {
			kincreate->body->SetLinearVelocity(b2Vec2(-0.75, 0));
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		currenttime = realtime;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		springanimation = false;
	}

	if (springanimation == false) {
		App->renderer->Blit(sprites, 220, 401, &springrect_1);
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		uint animation_time = 500;
		springanimation = true;
		if (realtime < currenttime + (animation_time / 4)) {
			App->renderer->Blit(sprites, 220, 401, &springrect_1);
		}
		if ((realtime > currenttime + (animation_time / 4)) && (realtime < currenttime + 2 * (animation_time / 4))) {
            App->renderer->Blit(sprites, 220, 401, &springrect_2);
		}
    	if ((realtime > currenttime + 2 * (animation_time / 4)) && (realtime < currenttime + 3 * (animation_time / 4))) {
			App->renderer->Blit(sprites, 219, 401, &springrect_3);
		}
		if ((realtime > currenttime + 3 * (animation_time / 4)) && (realtime < currenttime + animation_time)) {
			App->renderer->Blit(sprites, 220, 401, &springrect_4);
		}
		if (realtime > currenttime + animation_time) {
			App->renderer->Blit(sprites, 220, 401, &springrect_5);
		}
	}

	//spring
	static float Push = 0.0f;
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		Push += 100.0f;
		bouncer->body->ApplyForceToCenter(b2Vec2(0, (Push)), true);
	}
	else
		Push = 0.0f;

	b2Vec2 pos = bouncer->body->GetPosition();

	//MAKES PADDLES MOVE
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->physics->PaddleMoveL();
		App->audio->PlayFx(paddle_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		App->physics->PaddleStopL();
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->physics->PaddleMoveR();
		App->audio->PlayFx(paddle_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		App->physics->PaddleStopR();
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		App->physics->PaddleMoveL();
		App->audio->PlayFx(paddle_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		App->physics->PaddleStopL();

	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		App->physics->PaddleMoveR();
		App->audio->PlayFx(paddle_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		App->physics->PaddleStopR();

	}
	//END PADDLES MOVES

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5.5, b2_dynamicBody, 0));
		circles.getFirst()->data->body->SetBullet(true);
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_REPEAT)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5.5, b2_dynamicBody, 0));
		circles.getFirst()->data->body->SetBullet(true);
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(0, -10), true);
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		points = 0;
		numballs = 3;
		reset = true;
	}

	if (sensorballpassedexittriggered == true) {
		if (realtime > currenttimeexit + 1000) {
			sensor1triggered = false;
			sensor2triggered = false;
			sensor3triggered = false;
			sensor4triggered = false;
			sensor5triggered = false;
			sensor6triggered = false;
			sensor7triggered = false;
			sensor1triggeredpoints = false;
			sensor2triggeredpoints = false;
			sensor3triggeredpoints = false;
			sensor4triggeredpoints = false;
			sensor5triggeredpoints = false;
			sensor6triggeredpoints = false;
			sensor7triggeredpoints = false;
			sensorballpassedexittriggered = false;
			wall->body->SetActive(true);
		}
	}

	if (numballs <= 0) {
		game_over = true;
	}

	if ((newball == true) || (reset == true)) {
		if ((numballs > 0) || (reset == true)) {

			b2Vec2 BallInitialPos;
			BallInitialPos.x = 4.55;
			BallInitialPos.y = 7.8;
			circles.getLast()->data->body->SetTransform(BallInitialPos, 0);
			reset = false;
			sensor1triggered = false;
			sensor2triggered = false;
			sensor3triggered = false;
			sensor4triggered = false;
			sensor5triggered = false;
			sensor6triggered = false;
			sensor7triggered = false;
			sensor1triggeredpoints = false;
			sensor2triggeredpoints = false;
			sensor3triggeredpoints = false;
			sensor4triggeredpoints = false;
			sensor5triggeredpoints = false;
			sensor6triggeredpoints = false;
			sensor7triggeredpoints = false;
			wall->body->SetActive(true);
			sensorcard10triggered = false;
			sensorcardJtriggered = false;
			sensorcardQtriggered = false;
			sensorcardKtriggered = false;
			sensorcardAtriggered = false;
			cardstriggered = false;
			orangemaploaded = false;
			chicken1state = 1;
			chicken2state = 1;
			chicken3state = 1;
			godball1->body->SetActive(false);
			godball2->body->SetActive(false);
			sensorminispring1->body->SetActive(false);
			sensorminispring2->body->SetActive(false);
			sensorcoin1triggered = false;
			sensorcoin2triggered = false;
			sensorcoin3triggered = false;
			sensorcoin4triggered = false;
			sensorcoin5triggered = false;
			sensorcoin6triggered = false;
			sensorcoin7triggered = false;
			sensorcoin8triggered = false;
			sensorcoin1->body->SetActive(true);
			sensorcoin2->body->SetActive(true);
			sensorcoin3->body->SetActive(true);
			sensorcoin4->body->SetActive(true);
			sensorcoin5->body->SetActive(true);
			sensorcoin6->body->SetActive(true);
			sensorcoin7->body->SetActive(true);
			sensorcoin8->body->SetActive(true);
			sensorextrapoints1triggered = false;
			sensorextrapoints2triggered = false;
			sensorextrapoints3triggered = false;
			sensorextrapoints4triggered = false;
			sensorextrapoints1triggeredpoints = false;
			sensorextrapoints2triggeredpoints = false;
			sensorextrapoints3triggeredpoints = false;
			sensorextrapoints4triggeredpoints = false;
			main_board = App->textures->Load("pinball/Pinball_Main_Board.png");
			game_over = false;
		}
		newball = false;
	}

	if (game_over == true) {
		App->renderer->Blit(game_over_tex, 110, 233);
	}

	/*
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};
		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}
	*/

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	//Moving platform
	b2Vec2 cratepos = kincreate->body->GetPosition();
	SDL_Rect crateposrect;
	crateposrect.h = 5;
	crateposrect.w = 14;
	crateposrect.x = 159;
	crateposrect.y = 368;
	App->renderer->Blit(sprites, METERS_TO_PIXELS(cratepos.x) - (crateposrect.w / 2), METERS_TO_PIXELS(cratepos.y) - (crateposrect.h / 2), &crateposrect);

	//numbers
	SDL_Rect sensornumrect;
	sensornumrect.h = 7;
	sensornumrect.w = 15;
	sensornumrect.x = 84;
	sensornumrect.y = 118;

	int y = 293;
	if (sensor1triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor1triggeredpoints == false) {
			points += 100;
			sensor1triggeredpoints = true;
		}
	}

	sensornumrect.y += 8;
	y += 8;
	if (sensor2triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor2triggeredpoints == false) {
			points += 100;
			sensor2triggeredpoints = true;
		}
	}

	sensornumrect.y += 8;
	y += 8;
	if (sensor3triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor3triggeredpoints == false) {
			points += 100;
			sensor3triggeredpoints = true;
		}
	}

	sensornumrect.y += 8;
	y += 8;
	if (sensor4triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor4triggeredpoints == false) {
			points += 100;
			sensor4triggeredpoints = true;
		}
	}

	sensornumrect.y += 8;
	y += 8;
	if (sensor5triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor5triggeredpoints == false) {
			points += 100;
			sensor5triggeredpoints = true;
		}
	}

	sensornumrect.y += 8;
	y += 8;
	if (sensor6triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor6triggeredpoints == false) {
			points += 100;
			sensor6triggeredpoints = true;
		}
	}

	sensornumrect.y += 8;
	y += 8;
	if (sensor7triggered == false) {
		App->renderer->Blit(sprites, 81, y, &sensornumrect);
	}
	else {
		if (sensor7triggeredpoints == false) {
			points += 100;
			sensor7triggeredpoints = true;
		}
	}

	SDL_Rect wallrect;
	wallrect.h = 24;
	wallrect.w = 13;
	wallrect.x = 62;
	wallrect.y = 349;
	SDL_Rect exitrect;
	exitrect.h = 7;
	exitrect.w = 19;
	exitrect.x = 119;
	exitrect.y = 223;
	if ((sensor1triggered == false) ||
		(sensor2triggered == false) ||
		(sensor3triggered == false) ||
		(sensor4triggered == false) ||
		(sensor5triggered == false) ||
		(sensor6triggered == false) ||
		(sensor7triggered == false)) {
		App->renderer->Blit(sprites, 207, 293, &wallrect);
	}
	else {
		App->renderer->Blit(sprites, 190, 295, &exitrect);
		wall->body->SetActive(false);
	}

	//penguins
	SDL_Rect penguinrect;
	penguinrect.h = 14;
	penguinrect.w = 46;
	penguinrect.x = 95;
	penguinrect.y = 247;
	uint penguinanimationtime = 1000;
	if (firstpenguin == true) {
		currenttimepenguin = realtime;
		firstpenguin = false;
	}
	if (realtime < currenttimepenguin + (penguinanimationtime / 2)) {
		App->renderer->Blit(sprites, 122, 146, &penguinrect);
	}
	penguinrect.y = 264;
	if ((realtime >= currenttimepenguin + (penguinanimationtime / 2)) && (realtime < currenttimepenguin + penguinanimationtime)) {
		App->renderer->Blit(sprites, 121, 146, &penguinrect);
	}
	if (realtime >= currenttimepenguin + penguinanimationtime) {
		firstpenguin = true;
		//first blit of the animation
		//needed for constant blit, if not added, 1 frame is not printed
		penguinrect.y = 247;
		App->renderer->Blit(sprites, 122, 146, &penguinrect);
	}

	//cards
	SDL_Rect carddefaultrect;
	carddefaultrect.h = 24;
	carddefaultrect.w = 14;
	carddefaultrect.x = 76;
	carddefaultrect.y = 88;
	SDL_Rect cardrect;
	cardrect.h = 24;
	cardrect.w = 14;
	cardrect.x = 91;
	cardrect.y = 88;

	if (sensorcard10triggered == false) {
		App->renderer->Blit(sprites, 105, 261, &carddefaultrect);
	}
	else {
		App->renderer->Blit(sprites, 105, 261, &cardrect);
	}
	cardrect.x += 15;
	if (sensorcardJtriggered == false) {
		App->renderer->Blit(sprites, 121, 261, &carddefaultrect);
	}
	else {
		App->renderer->Blit(sprites, 121, 261, &cardrect);
	}
	cardrect.x += 15;
	if (sensorcardQtriggered == false) {
		App->renderer->Blit(sprites, 137, 261, &carddefaultrect);
	}
	else {
		App->renderer->Blit(sprites, 137, 261, &cardrect);
	}
	cardrect.x += 15;
	if (sensorcardKtriggered == false) {
		App->renderer->Blit(sprites, 153, 261, &carddefaultrect);
	}
	else {
		App->renderer->Blit(sprites, 153, 261, &cardrect);
	}
	cardrect.x += 15;
	if (sensorcardAtriggered == false) {
		App->renderer->Blit(sprites, 169, 261, &carddefaultrect);
	}
	else {
		App->renderer->Blit(sprites, 169, 261, &cardrect);
	}
	if ((sensorcard10triggered == true) &&
		(sensorcardJtriggered == true) &&
		(sensorcardQtriggered == true) &&
		(sensorcardKtriggered == true) &&
		(sensorcardAtriggered == true)) {
		cardstriggered = true;
	}
	else {
		cardstriggered = false;
	}
	if (cardstriggered == true) {
		if (orangemaploaded == false) {
			main_board = App->textures->Load("pinball/Pinball_Main_Board_Orange.png");
			points += 5000;
			godball1->body->SetActive(true);
			godball2->body->SetActive(true);
			orangemaploaded = true;
		}
		SDL_Rect godballrect;
		godballrect.x = 77;
		godballrect.y = 361;
		godballrect.h = 12;
		godballrect.w = 12;
		App->renderer->Blit(sprites, 138, 440, &godballrect);
		App->renderer->Blit(sprites, 138, 210, &godballrect);
	}

	//Chickens
	if (chicken1state == 1) {
		App->renderer->Blit(sprites, 120, 385, &egg);
	}
	if (chicken1state == 2) {
		BlitChickenAnimation(120, 385, chicken1, chicken2);
	}
	if (chicken2state == 1) {
		App->renderer->Blit(sprites, 141, 385, &egg);
	}
	if (chicken2state == 2) {
		BlitChickenAnimation(141, 385, chicken1, chicken2);
	}
	if (chicken3state == 1) {
		App->renderer->Blit(sprites, 160, 385, &egg);
	}
	if (chicken3state == 2) {
		BlitChickenAnimation(160, 385, chicken1, chicken2);
	}
	if (minispringsactivated == false) {
		if ((chicken1state == 2) && (chicken2state == 2) && (chicken3state == 2)) {
			minispringsactivated = true;
			minispringsactivatedfirsttime = true;
		}
	}
	if ((chicken1state != 2) || (chicken2state != 2) || (chicken3state != 2)) {
		minispringsactivated = false;
	}

	//Minisprings
	if (minispringsactivatedfirsttime == true) {
		if (sensorminispring1->body->IsActive() == false) {
			sensorminispring1->body->SetActive(true);
		}
		if (sensorminispring2->body->IsActive() == false) {
			sensorminispring2->body->SetActive(true);
		}
		minispringsactivatedfirsttime = false;
	}
	if (sensorminispring1triggered == true) {
		sensorminispring1->body->SetActive(false);
		sensorminispring1triggered = false;
	}
	if (sensorminispring2triggered == true) {
		sensorminispring2->body->SetActive(false);
		sensorminispring2triggered = false;
	}
	if (sensorminispring1->body->IsActive() == true) {
		App->renderer->Blit(sprites, 74, 404, &minispring);
	}
	if (sensorminispring2->body->IsActive() == true) {
		App->renderer->Blit(sprites, 203, 404, &minispring);
	}

	//coins
	if (sensorcoin1triggered == false) {
		App->renderer->Blit(sprites, 87, 101, &coinrect);
	}
	else {
		sensorcoin1->body->SetActive(false);
	}
	if (sensorcoin2triggered == false) {
		App->renderer->Blit(sprites, 84, 93, &coinrect);
	}
	else {
		sensorcoin2->body->SetActive(false);
	}
	if (sensorcoin3triggered == false) {
		App->renderer->Blit(sprites, 81, 85, &coinrect);
	}
	else {
		sensorcoin3->body->SetActive(false);
	}
	if (sensorcoin4triggered == false) {
		App->renderer->Blit(sprites, 78, 76, &coinrect);
	}
	else {
		sensorcoin4->body->SetActive(false);
	}
	if (sensorcoin5triggered == false) {
		App->renderer->Blit(sprites, 78, 66, &coinrect);
	}
	else {
		sensorcoin5->body->SetActive(false);
	}
	if (sensorcoin6triggered == false) {
		App->renderer->Blit(sprites, 80, 56, &coinrect);
	}
	else {
		sensorcoin6->body->SetActive(false);
	}
	if (sensorcoin7triggered == false) {
		App->renderer->Blit(sprites, 86, 48, &coinrect);
	}
	else {
		sensorcoin7->body->SetActive(false);
	}
	if (sensorcoin8triggered == false) {
		App->renderer->Blit(sprites, 91, 41, &coinrect);
		currenttimecoin = realtime;
	}
	else {
		sensorcoin8->body->SetActive(false);
	}

	if (sensorcoin8triggered == true) {
		if (realtime > currenttimecoin + 1000) {
			sensorcoin1triggered = false;
			sensorcoin2triggered = false;
			sensorcoin3triggered = false;
			sensorcoin4triggered = false;
			sensorcoin5triggered = false;
			sensorcoin6triggered = false;
			sensorcoin7triggered = false;
			sensorcoin8triggered = false;
			sensorcoin1->body->SetActive(true);
			sensorcoin2->body->SetActive(true);
			sensorcoin3->body->SetActive(true);
			sensorcoin4->body->SetActive(true);
			sensorcoin5->body->SetActive(true);
			sensorcoin6->body->SetActive(true);
			sensorcoin7->body->SetActive(true);
			sensorcoin8->body->SetActive(true);
		}
	}

	//extra points
	if (sensorextrapoints1triggered == false) {
		App->renderer->Blit(sprites, 89, 129, &extracoinsrect);
	}
	else {
		if (sensorextrapoints1triggeredpoints == false) {
			points += 100;
			sensorextrapoints1triggeredpoints = true;
		}
	}

	if (sensorextrapoints2triggered == false) {
		App->renderer->Blit(sprites, 89, 137, &extracoinsrect);
	}
	else {
		if (sensorextrapoints2triggeredpoints == false) {
			points += 100;
			sensorextrapoints2triggeredpoints = true;
		}
	}

	if (sensorextrapoints3triggered == false) {
		App->renderer->Blit(sprites, 89, 145, &extracoinsrect);
	}
	else {
		if (sensorextrapoints3triggeredpoints == false) {
			points += 100;
			sensorextrapoints3triggeredpoints = true;
		}
	}

	if (sensorextrapoints4triggered == false) {
		App->renderer->Blit(sprites, 89, 153, &extracoinsrect);
	}
	else {
		if (sensorextrapoints4triggeredpoints == false) {
			points += 100;
			sensorextrapoints4triggeredpoints = true;
		}
	}

	//draw ball, keep it at the bottom to draw the ball above all
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		SDL_Rect ballrect;
		ballrect.x = 94;
		ballrect.y = 342;
		ballrect.h = 12;
		ballrect.w = 11;
		App->renderer->Blit(sprites, x, y, &ballrect, 1.0f);
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	if (points > maxpoints) {
		maxpoints = points;
	}

	//title
	//title with score
	/*
	p2SString title("Nes Pinball, Points:%i Maxpoints:%i", points, maxpoints);
	App->window->SetTitle(title.GetString());
	*/

	c = paddlesL.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		SDL_Rect PaddleLrect;
		PaddleLrect.x = 42;
		PaddleLrect.y = 88;
		PaddleLrect.h = 12;
		PaddleLrect.w = 34;
		App->renderer->Blit(sprites, x-1, y+3, &PaddleLrect, 1.0f, c->data->GetRotation(), 0, 0);
		c = c->next;
	}

	c = paddlesR.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		SDL_Rect PaddleRrect;
		PaddleRrect.x = 42;
		PaddleRrect.y = 99;
		PaddleRrect.h = 12;
		PaddleRrect.w = 34;
		App->renderer->Blit(sprites, x-2, y+3, &PaddleRrect, 1.0f, c->data->GetRotation(), 0, 0);
		c = c->next;
	}
	
	//directional walls
	App->renderer->Blit(sprites, 96, 29, &directionalwall1rect);
	if (sensordirectionalwallin1triggered == true) {
		directionalwall1->body->SetActive(false);
		sensordirectionalwallin1triggered = false;
	}
	if (sensordirectionalwallout1triggered == true) {
		directionalwall1->body->SetActive(true);
		sensordirectionalwallout1triggered = false;
	}

	App->renderer->Blit(sprites, 193, 26, &directionalwall2rect);
	if (sensordirectionalwallin2triggered == true) {
		directionalwall2->body->SetActive(false);
		sensordirectionalwallin2triggered = false;
	}
	if (sensordirectionalwallout2triggered == true) {
		directionalwall2->body->SetActive(true);
		sensordirectionalwallout2triggered = false;
	}

	App->renderer->Blit(sprites, 198, 127, &directionalwall3rect);
	if (sensordirectionalwallin3triggered == true) {
		directionalwall3->body->SetActive(false);
		sensordirectionalwallin3triggered = false;
	}
	if (sensordirectionalwallout3triggered == true) {
		directionalwall3->body->SetActive(true);
		sensordirectionalwallout3triggered = false;
	}

	App->window->SetTitle("Nes Pinball (C++, SDL2.0, Box2D)");

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::BlitChickenAnimation(int x, int y, SDL_Rect& rect1, SDL_Rect& rect2) {
	uint time = 500;
	if (firstchicken == true) {
		currenttimechicken = realtime;
		firstchicken = false;
	}
	if (realtime < currenttimechicken + (time / 2)) {
		App->renderer->Blit(sprites, x, y, &rect1);
	}
	if ((realtime >= currenttimechicken + (time / 2)) && (realtime < currenttimechicken + time)) {
		App->renderer->Blit(sprites, x, y, &rect2);
	}
	if (realtime >= currenttimechicken + time) {
		firstchicken = true;
		//first blit of the animation
		//needed for constant blit, if not added, 1 frame is not printed
		App->renderer->Blit(sprites, x, y, &rect1);
	}
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//App->audio->PlayFx(bonus_fx);
	if (bodyA != nullptr && bodyB != nullptr) {
		if (bodyA->body != nullptr && bodyB->body != nullptr)
		{
			if (bodyA->body->GetType() == b2Shape::e_circle || bodyB->body->GetType() == b2Shape::e_circle)
			{
				if (bodyA == sensorreset || bodyB == sensorreset)
				{
					newball = true;
					numballs -= 1;
					if (numballs < 0) {
						numballs = 0;
					}
				}
				if (bodyA == sensor100points || bodyB == sensor100points)
				{
					points += 100;
				}
				if (bodyA == sensor500points1 || bodyB == sensor500points1 ||
					bodyA == sensor500points2 || bodyB == sensor500points2)
				{
					points += 500;
				}
				if (bodyA == sensor1000points || bodyB == sensor1000points)
				{
					points += 1000;
				}
				if (bodyA == sensor1 || bodyB == sensor1)
				{
					sensor1triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensor2 || bodyB == sensor2)
				{
					sensor2triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensor3 || bodyB == sensor3)
				{
					sensor3triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensor4 || bodyB == sensor4)
				{
					sensor4triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensor5 || bodyB == sensor5)
				{
					sensor5triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensor6 || bodyB == sensor6)
				{
					sensor6triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensor7 || bodyB == sensor7)
				{
					sensor7triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensorballpassedexit || bodyB == sensorballpassedexit)
				{
					currenttimeexit = realtime;
					sensorballpassedexittriggered = true;
				}
				if (bodyA == sensorcard10 || bodyB == sensorcard10)
				{
					points += 500;
					sensorcard10triggered = true;
				}
				if (bodyA == sensorcardJ || bodyB == sensorcardJ)
				{
					points += 500;
					sensorcardJtriggered = true;
				}
				if (bodyA == sensorcardQ || bodyB == sensorcardQ)
				{
					points += 500;
					sensorcardQtriggered = true;
				}
				if (bodyA == sensorcardK || bodyB == sensorcardK)
				{
					points += 500;
					sensorcardKtriggered = true;
				}
				if (bodyA == sensorcardA || bodyB == sensorcardA)
				{
					points += 500;
					sensorcardAtriggered = true;
				}
				if (bodyA == sensorchicken1 || bodyB == sensorchicken1)
				{
					chicken1state += 1;
					if (chicken1state >= 3) {
						chicken1state = 0;
					}
				}
				if (bodyA == sensorchicken2 || bodyB == sensorchicken2)
				{
					chicken2state += 1;
					if (chicken2state >= 3) {
						chicken2state = 0;
					}
				}
				if (bodyA == sensorchicken3 || bodyB == sensorchicken3)
				{
					chicken3state += 1;
					if (chicken3state >= 3) {
						chicken3state = 0;
					}
				}
				if (bodyA == sensorminispring1 || bodyB == sensorminispring1)
				{
					sensorminispring1triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(0, -27), true);
				}
				if (bodyA == sensorminispring2 || bodyB == sensorminispring2)
				{
					sensorminispring2triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(0, -27), true);
				}
				if (bodyA == sensorcoin1 || bodyB == sensorcoin1) {
					sensorcoin1triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin2 || bodyB == sensorcoin2) {
					sensorcoin2triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin3 || bodyB == sensorcoin3) {
					sensorcoin3triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin4 || bodyB == sensorcoin4) {
					sensorcoin4triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin5 || bodyB == sensorcoin5) {
					sensorcoin5triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin6 || bodyB == sensorcoin6) {
					sensorcoin6triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin7 || bodyB == sensorcoin7) {
					sensorcoin7triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin8 || bodyB == sensorcoin8) {
					sensorcoin8triggered = true;
					points += 100;
				}
				if (bodyA == sensordirectionalwallin1 || bodyB == sensordirectionalwallin1) {
					sensordirectionalwallin1triggered = true;
				}
				if (bodyA == sensordirectionalwallout1 || bodyB == sensordirectionalwallout1) {
					sensordirectionalwallout1triggered = true;
				}
				if (bodyA == sensordirectionalwallin2 || bodyB == sensordirectionalwallin2) {
					sensordirectionalwallin2triggered = true;
				}
				if (bodyA == sensordirectionalwallout2 || bodyB == sensordirectionalwallout2) {
					sensordirectionalwallout2triggered = true;
				}
				if (bodyA == sensordirectionalwallin3 || bodyB == sensordirectionalwallin3) {
					points += 500;
					sensordirectionalwallin3triggered = true;
				}
				if (bodyA == sensordirectionalwallout3 || bodyB == sensordirectionalwallout3) {
					sensordirectionalwallout3triggered = true;
				}
				if (bodyA == sensorextrapoints1 || bodyB == sensorextrapoints1) {
					sensorextrapoints1triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensorextrapoints2 || bodyB == sensorextrapoints2) {
					sensorextrapoints2triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensorextrapoints3 || bodyB == sensorextrapoints3) {
					sensorextrapoints3triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
				if (bodyA == sensorextrapoints4 || bodyB == sensorextrapoints4) {
					sensorextrapoints4triggered = true;
					circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(3, 0), true);
				}
			}
		}
	}
}
