#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "gameoverscene.h"
//#include "globals.h"

using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

class GameOverScene : public cocos2d::Layer
{
public:
    typedef Layer super;
    typedef GameOverScene self;

    Label *scoreTextLabel, *scoreLabel, *playAgainLabel, *mainMenuLabel;

    virtual bool init();
    static cocos2d::Scene* createScene();
    void goToGameScene(Ref* pSender);
    void goToMainMenuScene(Ref* pSender);

    CREATE_FUNC(GameOverScene);

    GameOverScene();
    virtual ~GameOverScene();
};


