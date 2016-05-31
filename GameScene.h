#pragma once

#include "cocos2d.h"
#include "Level.h"
#include "Player.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:

    typedef Layer super;
    typedef GameScene self;

    //FIELDS
    Level * level;
    Player *player;
    Sprite *cameraTarget;
    vector<Sprite*> enemyList;

    bool collidesX;
    bool constantTouch;
    Point lastTouch;

    Follow *camera;

    DrawNode *rectWithBorder;

    vector<EventKeyboard::KeyCode> heldKeys;

    //CONSTRUCTOR & METHODS

    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    virtual void onTouchesBegan(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesCancelled(const vector<Touch*>& touches, Event* event);
    void touch(const Point& location);

    void updateScene(float interval);
    void updatePlayer(float interval);

    void loadEnemies();

    //int signum(float x); useful ??

    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(GameScene);

    GameScene(void);
    virtual ~GameScene(void);
};
