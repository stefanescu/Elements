#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class Player : public Object
{

public:
        //FIELDS
        enum State {
        Standing, Walking, Jumping
    };

        State state;

        bool facingRight;
        bool grounded;
        float stateTime;

        Point position;
        Point velocity;

        Player(void);
        virtual ~Player(void);
};
