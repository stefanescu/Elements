#include "enemy.h"

Enemy * Enemy::create()
{
    Enemy* eSprite = new Enemy();

    if (eSprite->initWithSpriteFrameName("p1_stand.png"))
    {
        eSprite->setAnchorPoint(Point::ZERO);
        eSprite->playerSize = Size(eSprite->getBoundingBox().size.width, eSprite->getBoundingBox().size.height);
        return eSprite;
    }
    return NULL;
}

Rect Enemy::getCollisionBox()
{
    Rect bbox = getBoundingBox();

    Rect rect = Rect(
                bbox.getMinX() + velocityX,
                bbox.getMinY() ,//+ 1.0f,
                bbox.size.width,
                bbox.size.height);
    return rect;
}

Rect Enemy::getUpperCollisionBox()
{
    Rect bbox = getBoundingBox();

    Rect rect = Rect(
                bbox.getMinX() + bbox.size.width * 0.33f,
                bbox.getMidY(),
                bbox.size.width * 0.4f,
                bbox.size.height * 1.5f);
    return rect;
}

void Enemy::reverseDirection()
{
    direction = direction * -1;
    velocityX = velocityX * -1;
}

void Enemy::move (int dir)
{
    direction = dir;
    velocityX = dir > 0 ? ENEMY_MAX_VELOCITY : -ENEMY_MAX_VELOCITY;
    moving = true;
}

Enemy::Enemy()
{
    HP = 1;
    killCount = 0;
    velocityX = 0;
    velocityY = 0;
    direction = 0;
    grounded = true;
    moving = false;

    grounded = true;
    moving = false;
    setScale(0.3f);

}

Enemy::~Enemy()
{

}
