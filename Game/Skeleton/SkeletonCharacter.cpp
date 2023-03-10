#include "SkeletonCharacter.h"

#include "../../Engine/Singelton.h"
#include "../../Engine/Pathfinding.h"
#include "../../Engine/Core/MeasurePerformance.h"
#include "../../Engine/Core/CollisionObject.h"
#include "../../Engine/Debuging/DebugRectangle.h"

SkeletonCharacter::SkeletonCharacter(Vector& SpawnPosition)
{
    //Set spawn Position
    position = SpawnPosition;

    //Load sprite sheet
    spriteSheet.loadTexture("assets/textures/enemies/skeleton.png");
    spriteSheet.setDynamicPosition(&position);

    //Create Animation
    animation.addSpriteSheet(&spriteSheet, 4, 36, spriteSheet.getHeight());
    animation.enable();
    animation.setFrameRate(8.f);

    collision = new CollisionObject(this);
    collision->createCollisionShape(spriteSheet.getHeight(), spriteSheet.getWidth(), &position);

    name = typeid(this).name();

    PLAYER->addObserver(this);
}

SkeletonCharacter::~SkeletonCharacter()
{
}

void SkeletonCharacter::update()
{
    lastValidPos = position;

    auto timer = new MeasurePerformance;
    if (MEASURE_PERFORMANCE) { timer->start(); }
    BaseCharacter::update();
    if (MEASURE_PERFORMANCE) { timer->end("     Base Character Constructor: "); }

    //Search new Path whenever cooldown is 0
    if (MEASURE_PERFORMANCE) { timer->start(); }
    if (pfCurrentCooldown <= 0)
    {
        pfCurrentCooldown = pfCooldown;
        if (!SINGLETON->pathfindingGrid->findPath(position, PLAYER->position, path, this))
        {
            printf("no path found in: %s \n", name.c_str());
        }
    }
    else
    {
        pfCurrentCooldown -= DELTA_TIME;
    }
    if (MEASURE_PERFORMANCE) { timer->end("     Pathfinding: "); }

    //Set direction to next path node
    if (setDirToPath())
    {
        velocity = moveDir * movementSpeed;
    }
    else
    {
        velocity.Zero();
    }
    lastValidPos = position;
    move();
    if (checkForCollision())
    {
        move();
    }


    if (MEASURE_PERFORMANCE) { timer->start(); }
    animation.update();
    if (MEASURE_PERFORMANCE) { timer->end("     Animation: "); }
}

void SkeletonCharacter::move()
{
    position = position + velocity * DELTA_TIME;
}

void SkeletonCharacter::close()
{
    BaseEnemy::close();

    PLAYER->removeObserver(this);
}

bool SkeletonCharacter::checkForCollision()
{
    for (int enemy = 0; enemy < SINGLETON->sizeEnemiesList; ++enemy)
    {
        if (SINGLETON->gEnemiesList[enemy] != this)
        {
            if (Vector::dist(SINGLETON->gEnemiesList[enemy]->position, position) <= 100.f)
            {
                if (collision->checkForIntersection(SINGLETON->gEnemiesList[enemy]->collision))
                {
                    // if (collision->calculateCollisionPoint(SINGLETON->gEnemiesList[enemy]->collision, hit))
                    // {
                    //     moveDir += hit.normal;
                    //     return true;
                    // }
                    position = lastValidPos;
                    collision->collisionResponse(SINGLETON->gEnemiesList[enemy]->collision);
                    return true;
                }
            }
            if (Vector::dist(PLAYER->position, position) <= 100.f)
            {
                if (collision->checkForIntersection(PLAYER->collision))
                {
                    // if (collision->calculateCollisionPoint(SINGLETON->gEnemiesList[enemy]->collision, hit))
                    // {
                    //     moveDir += hit.normal;
                    //     return true;
                    // }
                    PLAYER->disablePlayer();
                    return true;
                }
            }
        }
    }
    return false;
}

bool SkeletonCharacter::setDirToPath()
{
    moveDir.Zero();

    if (!path.empty())
    {
        if (Vector::dist(path[0], position) <= 20.f)
        {
            path.erase(path.begin());
        }
        if (!path.empty())
        {
            moveDir += (path[0] - position).normalize();
            return true;
        }
    }
    return false;
}

void SkeletonCharacter::onNotify(const Event event)
{
    if (event == PLAYER_DIED)
    {
        shouldUpdate = false;
    }
}
