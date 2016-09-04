#pragma once

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class Player : public Sprite
{
private:
    float velocityX;
    float velocityY;
    int direction;
    //int facing_left;
    //int facing_right;
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
    void idle();
    void applyGravity();
    void updateState(float delta);
    //void setupAnimation(const char* name);
    Vector<SpriteFrame*> buildAnimation(const char *format, int count);


    Player(void);
    virtual ~Player(void);
};
