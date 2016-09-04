#include "mainmenuscene.h"
#include "GameScene.h"

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    layer->retain();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
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

    titleLabel = Label::createWithTTF("START", "fonts/space age.ttf", 32);
    startLabel = Label::createWithTTF("START", "fonts/space age.ttf", 32);

    auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MainMenuScene::goToGameScene, this));
    //auto startLabel = LabelAtlas::create("START", "fonts/arial.ttf", 16, 24, '.');
    //auto startItem = MenuItemFont::create("START", CC_CALLBACK_1(MainMenuScene::goToGameScene, this));
            //MenuItemLabel::create(startLabel, CC_CALLBACK_1(MainMenuScene::goToGameScene, this));
    startItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    startItem->setDisabledColor( Color3B(32,32,64) );
    startItem->setColor( Color3B(200,200,255) );

    auto menu = Menu::create(startItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void MainMenuScene::goToGameScene(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto gameScene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInT::create(TRANSITION_TIME, gameScene));

}



MainMenuScene::MainMenuScene()
{
}

MainMenuScene:: ~MainMenuScene()
{

}
