#include "particle.h"
#include <raylib.h>
#include "raymath.h"

Particle::Particle(Vector2 pos, int width, int height)
{

    _pos.x = pos.x + GetRandomValue(-width, width);
    _pos.y = pos.y + GetRandomValue(-height, height);
    _vel.x = GetRandomValue(-100, 100) / 100.f;
    _vel.y = GetRandomValue(-100, 100) / 100.f;
    _color = {static_cast<unsigned char>(GetRandomValue(40,255)),
        static_cast<unsigned char>(GetRandomValue(40,255)),
        static_cast<unsigned char>(GetRandomValue(0,255)),255};

    //color = { 0,0,0,255 };
}

Particle::Particle(Vector2 pos, Vector2 vel, Color color)
{
    _pos = pos;
    _vel = vel;
    _color = color;
}

float Particle::GetDistance(Vector2 otherPos) {
    const float dx = _pos.x - otherPos.x;
    const float dy = _pos.y - otherPos.y;
    return sqrt((dx * dx) + (dy * dy));
}

Vector2 Particle::GetNormal(Vector2 otherPos) {
    float dist = GetDistance(otherPos);
    if (dist == 0.0f) dist = 1;
    const float dx = _pos.x - otherPos.x;
    const float dy = _pos.y - otherPos.y;
    Vector2 normal = { dx * (1 / dist), dy * (1 / dist) };
    return normal;
}

Vector2 Particle::GetPos() {
    return _pos;
}

Color Particle::GetColor() {
    return _color;
}

void Particle::AttractToPos(Vector2 posToAttract, float multiplier)
{
    float dist = fmax(GetDistance(posToAttract), 0.5);
    Vector2 normal = GetNormal(posToAttract);

    _vel.x -= (normal.x / dist) * multiplier;
    _vel.y -= (normal.y / dist) * multiplier;
}

void Particle::DoFriction(float amount) {
    _vel.x *= amount;
    _vel.y *= amount;
}

void Particle::Step(float deltaTime) {
    _pos.x += _vel.x * deltaTime;
    _pos.y += _vel.y * deltaTime;
}