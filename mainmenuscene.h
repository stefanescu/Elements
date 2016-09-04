#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

using namespace std;
USING_NS_CC;

class MainMenuScene : public cocos2d::Layer
{
public:
    typedef Layer super;
    typedef MainMenuScene self;

    Label *titleLabel, *startLabel;
    //Button *startButton;

    virtual bool init();
    static cocos2d::Scene* createScene();
    void goToGameScene(Ref* pSender);

    CREATE_FUNC(MainMenuScene);

    MainMenuScene();
    virtual ~MainMenuScene();
};


