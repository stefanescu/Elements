#pragma once

#include "cocos2d.h"
#include "globals.h"
#include <vector>

using namespace std;
using namespace cocos2d;

class Player : public Sprite
{    
protected:
    float velocityX;
    float velocityY;
    int direction, killCount, HP;
    bool grounded;
    bool moving;
    Vector<SpriteFrame*> walkFrames, idleFrames;
    Animate *walking, *idling;
    Size playerSize;

public:
    Rect getCollisionBox();
    Rect getUpperCollisionBox();

    static Player* create();
    float getVelocityX();
    float getVelocityY();
    void setVelocityX(float velX);
    void setVelocityY(float velY);

    int getDirection();
    bool isGrounded();
    void setGrounded(bool);
    bool isMoving();
    Size getPlayerSize();
    void move(int dir);
    void jump();
    void fall();
    Sprite* shootLaser();
    int getKillCount();
    void increaseKillCount();
    void resetKillCount();
    void takeDmg();
    int getHP();
    void die();
    void idle();
    void applyGravity();
    void updateState(float delta);

    Vector<SpriteFrame*> buildAnimation(const char *format, int count);

    Player(void);
    virtual ~Player(void);
};
