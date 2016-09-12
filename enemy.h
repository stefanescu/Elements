#pragma once
#include "player.h"

using namespace cocos2d;

class Enemy : public Player
{

public:
    static Enemy* create();

    Rect getCollisionBox();
    Rect getUpperCollisionBox();
    void reverseDirection();
    void move (int dir);
    void updateState (float delta);

    Enemy();
    virtual ~Enemy(void);
};



