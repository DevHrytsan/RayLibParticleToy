#pragma once
#include <raylib.h>

class Particle
{
private:
	Vector2 _pos;
	Vector2 _vel;
	Color _color;

	float GetDistance(Vector2 pos);
	Vector2 GetNormal(Vector2 otherPos);
public:
	Particle(Vector2 pos, int width, int height);
	Particle(Vector2 pos, Vector2 vel, Color color);

	void AttractToPos(Vector2 posToAttract, float multiplier);
	Vector2 GetPos();
	Color GetColor();

	void DoFriction(float amount);
	void Step(float deltaTime);
};
