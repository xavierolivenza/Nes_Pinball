#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	//debug = false;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(69), PIXEL_TO_METERS(0));

	b2Body* main_board = world->CreateBody(&body);

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

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[72 / 2];

	for (uint i = 0; i < 72 / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(Pinball_MainBoard_1_coords[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(Pinball_MainBoard_1_coords[i * 2 + 1]);
	}
	shape.CreateLoop(p, 72 / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	main_board->CreateFixture(&fixture);

	/*kickerjoint = CreateRectangleKickerPoint(104, 429, 2, 1);
	kicker = CreateRectangleKicker(104, 429, 16, 8);

	b2RevoluteJointDef revolutedef;
	revolutedef.bodyA = kickerjoint->body;
	revolutedef.bodyB = kicker->body;
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 4);
	revolutedef.upperAngle = (3.14 / 4);
	revolutedef.collideConnected = false;

	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);
	*/


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

//PADDLE L
PhysBody* ModulePhysics::CreatePaddleL(int x, int y, float angd, float angu, uint16 categbts, uint16 mskbts)
{
	//cirlce
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(0);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 5;
	//polygon
	b2BodyDef body2;
	body2.type = b2_dynamicBody;
	body2.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b2 = world->CreateBody(&body2);

	b2PolygonShape shape2;

	int points[16] =
	{
		1, 10,
		1, 6,
		3, 3,
		33, 6,
		33, 8,
		12, 11,
		6, 12,
		2, 11
	};

	b2Vec2* p = new b2Vec2[8];

	for (uint i = 0; i < 8; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, 8);

	//shape2.SetAsBox(PIXEL_TO_METERS(25) * 0.5f, PIXEL_TO_METERS(4) * 0.5f);
	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 2.0f;

	b2->CreateFixture(&fixture2);

	PhysBody* pbody2 = new PhysBody();
	pbody->body = b2;
	b2->SetUserData(pbody2);
	pbody->width = pbody->height = 0;
	//REVOLUTION JOINT
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = b;
	revoluteJointDef.bodyB = b2;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(6));
	revoluteJointDef.referenceAngle = 0;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.enableMotor = false;
	revoluteJointDef.maxMotorTorque = 500;
	revoluteJointDef.motorSpeed = -1000 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = angu;
	revoluteJointDef.upperAngle = angd;
	//paddles = (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
	paddleList.add((b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef));
	return pbody;
}

//MOVE PADDLE L
void ModulePhysics::PaddleMoveL()
{
	//p2List_item<PhysBody*>*temp = App->scene_intro->paddles.getFirst()->data->body->GetJointList;
	p2List_item<b2RevoluteJoint*> *temp = paddleList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(true);
		temp = temp->next;
	}
	//paddles->EnableMotor(true);

}

//STOP PADDLE L
void ModulePhysics::PaddleStopL()
{
	p2List_item<b2RevoluteJoint*> *temp = paddleList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(false);
		temp = temp->next;
	}
}

//PADDLE R
PhysBody* ModulePhysics::CreatePaddleR(int x, int y, float angd, float angu, uint16 categbts, uint16 mskbts)
{
	//cirlce
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(0);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 50;
	//polygon
	b2BodyDef body2;
	body2.type = b2_dynamicBody;
	body2.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b2 = world->CreateBody(&body2);

	b2PolygonShape shape2;

	int points[14] =
	{
		-3, 2,
		27, 3,
		29, 4,
		29, 8,
		27, 12,
		21, 12,
		-3, 3
	};

	b2Vec2* p = new b2Vec2[7];

	for (uint i = 0; i < 7; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, 7);

	//shape2.SetAsBox(PIXEL_TO_METERS(25) * 0.5f, PIXEL_TO_METERS(4) * 0.5f);
	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 2.0f;

	b2->CreateFixture(&fixture2);

	PhysBody* pbody2 = new PhysBody();
	pbody->body = b2;
	b2->SetUserData(pbody2);
	pbody->width = pbody->height = 0;
	//REVOLUTION JOINT
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = b;
	revoluteJointDef.bodyB = b2;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(25), PIXEL_TO_METERS(6));
	revoluteJointDef.referenceAngle = -120 * DEGTORAD;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.enableMotor = false;
	revoluteJointDef.maxMotorTorque = 500;
	revoluteJointDef.motorSpeed = 1000 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = angu;
	revoluteJointDef.upperAngle = angd;
	//paddles = (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
	paddleListR.add((b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef));
	return pbody;
}

//MOVE PADDLE R
void ModulePhysics::PaddleMoveR()
{
	//p2List_item<PhysBody*>*temp = App->scene_intro->paddles.getFirst()->data->body->GetJointList;
	p2List_item<b2RevoluteJoint*> *temp = paddleListR.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(true);
		temp = temp->next;
	}
	//paddles->EnableMotor(true);

}

//STOP PADDLE R
void ModulePhysics::PaddleStopR()
{
	p2List_item<b2RevoluteJoint*> *temp = paddleListR.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(false);
		temp = temp->next;
	}
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, float radius, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, float angle, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = angle;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

/*
PhysBody* ModulePhysics::CreateRectangleKicker(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}


PhysBody* ModulePhysics::CreateRectangleKickerPoint(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}
*/

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = angle;

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels


	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		//TODO 1-2
		bool obj_ins_mouse = false;
		b2Body* b_mouse = b;
		b2Vec2 mouse_pos;
		mouse_pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
		mouse_pos.y = PIXEL_TO_METERS(App->input->GetMouseY());

		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x) * SCREEN_SIZE, METERS_TO_PIXELS(pos.y) * SCREEN_SIZE, METERS_TO_PIXELS(shape->m_radius) * SCREEN_SIZE, 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			if (obj_ins_mouse == false)
			{
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					mouse_pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
					mouse_pos.y = PIXEL_TO_METERS(App->input->GetMouseY());
					if (f->TestPoint(mouse_pos))
					{
						obj_ins_mouse = true;
						break;
					}
				}
				// test if the current body contains mouse position
			}
		}
		b2MouseJointDef def;
		// If a body was selected we will attach a mouse joint to it
		// so we can pull it around
		// TODO 2: If a body was selected, create a mouse joint
		// using mouse_joint class property
		if (obj_ins_mouse == true)
		{
			def.bodyA = ground;
			def.bodyB = b_mouse;
			def.target = mouse_pos;
			def.dampingRatio = 0.5f;
			def.frequencyHz = 2.0f;
			def.maxForce = 100.0f * b_mouse->GetMass();			

			mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
		}		

		// TODO 3: If the player keeps pressing the mouse button, update
		// target position and draw a red line between both anchor points
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint)
		{
			mouse_joint->SetTarget(mouse_pos);
			App->renderer->DrawLine(METERS_TO_PIXELS(mouse_pos.x), METERS_TO_PIXELS(mouse_pos.y), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 100, 100);
		}

		// TODO 4: If the player releases the mouse button, destroy the joint
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint)
		{
			obj_ins_mouse = false;
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}