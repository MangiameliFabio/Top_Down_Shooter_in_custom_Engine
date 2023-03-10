#pragma once
#include "Animator.h"
#include <typeinfo>

class Animator;

class BaseState
{
public:
    virtual ~BaseState()
    {
        delete animation;
    }

    virtual void startState()
    {
        if (!animation)
        {
            printf("Can't start State, animation is invalid -> %s", typeid(*animation).name());
            return;
        }
        animation->enable();
    }

    virtual void updateState()
    {
        if (!animation)
        {
            printf("Can't update State, animation is invalid -> %s", typeid(*animation).name());
            return;
        }
        animation->update();
    }

    virtual void endState()
    {
        if (!animation)
        {
            printf("Can't end State, animation is invalid -> %s", typeid(*animation).name());
            return;
        }
        animation->disable();
    }

    Animator* animation = nullptr;
};
