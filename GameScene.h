#pragma once

#include "cocos2d.h"
#include "Level.h"
#include "Player.h"
#include "enemy.h"
#include <algorithm>
#include <string>
#include <vector>
#include "SimpleAudioEngine.h"
#include "globals.h"
#include "mainmenuscene.h"
#include "gameoverscene.h"

using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
private:
    Level *level;
    Player *player;
    Sprite *ship, *powerNotification, *cameraTarget;
    Label *playerScore, *playerScoreText;
    vector<Enemy*> enemyList;
    vector<Sprite*> projectileList;
    int tapCount, shotCount, score;
    float shotInterval;;

    bool powerAvailable, invulnerable, collidesX, constantTouch;
    Point lastTouch, moveTouchLocation, shootTouchLocation;
    Follow *camera;
    DrawNode *rectWithBorder;
    vector<EventKeyboard::KeyCode> heldKeys;

public:
    typedef Layer super;
    typedef GameScene self;

    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    virtual void onTouchesBegan(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesCancelled(const vector<Touch*>& touches, Event* event);
    void touch(const Point& location);

    void updateScene(float interval);
    void updatePlayer(float interval);

    void addEnemies();
    void resetTapCount(float t);
    void resetShotCount(float t);

    void gameLogic(float dt);
    void powerCheck();
    void usePower();
    void activateInvulnerability();
    void disableInvulnerability(float t);

    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(GameScene);

    GameScene(void);
    virtual ~GameScene(void);
};
