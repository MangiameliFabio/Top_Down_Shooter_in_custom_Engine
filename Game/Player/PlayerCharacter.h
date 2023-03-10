#pragma once

#include "../BaseCharacter.h"
#include "../ProjectileSpawner.h"
#include "../../Engine/Core/Patterns/Observer.h"
#include "../../Engine/Core/Vector.h"
#include "../../Engine/Enums.h"
#include "../../Engine/Core/Patterns/Subject.h"
#include "AnimationStateMachine/PlayerASM.h"

class Animator;
struct Vector;

class PlayerCharacter : public BaseCharacter, public Observer, public Subject
{
public:
    void start() override;
    void update() override;
    void close() override;

    void move();
    void onNotify(const Event event) override;
    void addMoveDirection(Vector& v);
    void addAimDirection(Vector& v);
    void disablePlayer();

    PlayerCharacter(Vector& spawnPos);
    ~PlayerCharacter() override;

    ProjectileSpawner projectileSpawner;

    PlayerASM* stateMachine{};

    Vector aimDir;

    float spriteHeight{};
    float spriteWidth{};

    float movementSpeed = 150.f;
    float attackSpeed = 0.5f;
    float attackCooldown = 0.f;

    bool isMoveing = false;
    bool isShooting = false;

    bool died = false;
};
