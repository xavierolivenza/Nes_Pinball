#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	sensor500points = App->physics->CreateRectangleSensor(160, 60, 12, 4);
	sensor500points = App->physics->CreateRectangleSensor(127, 60, 12, 4);
	sensor500points = App->physics->CreateRectangleSensor(206, 93, 12, 4);

	sensor1000points = App->physics->CreateRectangleSensor(144, 60, 12, 4);

	sensor100points = App->physics->CreateRectangleSensor(114, 80, 12, 4);

	//Board create
	int Pinball_MainBoard_1_coords[74] = {
		50, 476,
		4, 430,
		4, 372,
		18, 357,
		10, 349,
		10, 283,
		3, 262,
		3, 175,
		19, 160,
		19, 126,
		4, 83,
		4, 64,
		9, 49,
		18, 37,
		30, 28,
		38, 25,
		127, 25,
		141, 31,
		151, 40,
		160, 56,
		163, 69,
		163, 397,
		150, 397,
		150, 317,
		139, 317,
		139, 348,
		132, 357,
		146, 372,
		146, 431,
		100, 476,
		100, 524,
		167, 524,
		167, 1,
		1, 1,
		1, 524,
		50, 524,
		50, 476
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_MainBoard_1_coords, 72));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_MainBoard_2_coords, 72));

	int Pinball_MiniPart_1_Board_coords[14] = {
		34, 381,
		42, 406,
		41, 409,
		36, 406,
		33, 401,
		33, 382,
		34, 381
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_MiniPart_1_Board_coords, 12));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_MiniPart_2_Board_coords, 14));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_Part_1_Board_coords, 26));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_Part_2_Board_coords, 18));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_Part_3_Board_coords, 18));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_Line_1_coords, 20));

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
	board.add(App->physics->CreateChain(69, 0, Pinball_Line_2_coords, 20));

	int Pinball_Separator_1_Board_coords[14] = {
		67, 57,
		68, 58,
		68, 65,
		67, 66,
		66, 65,
		66, 58,
		67, 57
	};
	board.add(App->physics->CreateChain(69, 0, Pinball_Separator_1_Board_coords, 12));

	int Pinball_Separator_2_Board_coords[14] = {
		84, 56,
		85, 58,
		85, 65,
		84, 67,
		83, 65,
		83, 58,
		84, 56
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_2_Board_coords, 12));

	int Pinball_Separator_3_Board_coords[14] = {
		36, 285,
		37, 286,
		37, 294,
		36, 296,
		35, 294,
		35, 286,
		36, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_3_Board_coords, 12));

	int Pinball_Separator_4_Board_coords[14] = {
		52, 285,
		53, 286,
		53, 295,
		52, 296,
		51, 295,
		51, 286,
		52, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_4_Board_coords, 12));

	int Pinball_Separator_5_Board_coords[14] = {
		68, 285,
		69, 286,
		69, 295,
		68, 296,
		67, 295,
		67, 286,
		68, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_5_Board_coords, 12));

	int Pinball_Separator_6_Board_coords[14] = {
		84, 285,
		85, 286,
		85, 295,
		84, 296,
		83, 295,
		83, 286,
		84, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_6_Board_coords, 12));

	int Pinball_Separator_7_Board_coords[14] = {
		100, 285,
		101, 286,
		101, 295,
		100, 296,
		99, 295,
		99, 286,
		100, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_7_Board_coords, 12));

	int Pinball_Separator_8_Board_coords[14] = {
		116, 285,
		117, 286,
		117, 295,
		116, 296,
		115, 295,
		115, 286,
		116, 285
	};
	board.add(App->physics->CreateChain(68, 0, Pinball_Separator_8_Board_coords, 12));

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

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5.5));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_REPEAT)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5.5));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(0, -10), true);
	}

	/*
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
		//App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		/*
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY())) {
			SDL_Rect ballrect;
			ballrect.x = 101;
			ballrect.y = 348;
			ballrect.h = 10;
			ballrect.w = 9;
			App->renderer->Blit(sprites, x, y, &ballrect, 1.0f, c->data->GetRotation());
		}
		*/
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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
