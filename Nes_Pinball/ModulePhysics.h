#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, float radius, b2BodyType type, float rest);
	PhysBody* CreateRectangle(int x, int y, int width, int height, float angle, b2BodyType type, float rest);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float angle);
	PhysBody* CreateCircleSensor(int x, int y, int radius, b2BodyType type, int density, float rest);
	PhysBody* CreateChain(int x, int y, int* points, int size, float rest);

	//PhysBody* CreateRectangleKicker(int x, int y, int width, int height);
	//PhysBody* CreateRectangleKickerPoint(int x, int y, int width, int height);

	PhysBody* CreatePaddleL(int x, int y, float angd, float angu);
	PhysBody* CreatePaddleR(int x, int y, float angd, float angu);

	b2DistanceJointDef* CreateLineJoint(b2Body* bodyA, b2Body* bodyB, p2Point<float> Local_Anchor_A, p2Point<float> Local_Anchor_B, float frequency, float damping);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	void PaddleMoveL();
	void PaddleStopL();
	void PaddleMoveR();
	void PaddleStopR();

	void CreateTemporaryJoint();
	void DeleteTemporaryJoint();

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2RevoluteJoint* revolute_joint;
	
	b2Body* ground;
	//PhysBody* kicker;
	//PhysBody* kickerjoint;

	p2List<b2RevoluteJoint*> paddleList;
	p2List<b2RevoluteJoint*> paddleListR;

	b2RevoluteJoint* temp_rev_joint;
};