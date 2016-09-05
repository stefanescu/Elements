#include "gameoverscene.h"
#include "mainmenuscene.h"
#include "GameScene.h"

using namespace CocosDenshion;
using namespace std;

Scene *GameOverScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    layer->retain();
    scene->addChild(layer);
    return scene;
}

bool GameOverScene::init()
{
    if (!Layer::init())
        return false;


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);

    auto dispatcher = this->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(2.0f);

    SimpleAudioEngine::getInstance()->playBackgroundMusic("for nines psg.mp3", true);

    scoreTextLabel = Label::createWithTTF("score:", "fonts/space age.ttf", 32);
    scoreTextLabel->setPosition(Point(visibleSize.width * 0.3, visibleSize.height * 0.75));
    this->addChild(scoreLabel,2);
    scoreLabel = Label::createWithTTF("1k", "fonts/space age.ttf", 32);
    scoreLabel->setPosition(Point(visibleSize.width * 0.3 + scoreTextLabel->getWidth(), visibleSize.height * 0.75));
    this->addChild(scoreLabel,2);

    playAgainLabel = Label::createWithTTF("play again", "fonts/space age.ttf", 24);
    mainMenuLabel = Label::createWithTTF("main menu", "fonts/space age.ttf", 24);

    auto menuItem1 = MenuItemLabel::create(playAgainLabel, CC_CALLBACK_1(GameOverScene::goToGameScene, this));
    menuItem1->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    menuItem1->setColor(Color3B(200,200,255));
    menuItem1->setDisabledColor( Color3B(32,32,64));

    auto menuItem2 = MenuItemLabel::create(mainMenuLabel, CC_CALLBACK_1(GameOverScene::goToMainMenuScene, this));
    menuItem2->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - menuItem1->getContentSize().height));
    menuItem2->setColor(Color3B(200,200,255));
    menuItem2->setDisabledColor( Color3B(32,32,64));

    auto menu = Menu::create(menuItem1, menuItem2, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

}

void GameOverScene::goToGameScene (Ref* pSender)
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto gameScene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInT::create(TRANSITION_TIME, gameScene));
}

void GameOverScene::goToMainMenuScene (Ref* pSender)
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto mainMenuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInT::create(TRANSITION_TIME, mainMenuScene));
}

GameOverScene::GameOverScene()
{

}

GameOverScene::~GameOverScene()
{

}
