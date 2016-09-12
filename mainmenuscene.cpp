#include "mainmenuscene.h"

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

    titleLabel = Label::createWithTTF("HELP IS COMING", "fonts/space age.ttf", 32);
    titleLabel->setPosition(Point(visibleSize.width/2, visibleSize.height * 0.75));
    this->addChild(titleLabel,2);
    note = Label::createWithTTF("...right?", "fonts/space age.ttf", 18);
    note->setPosition(Point(visibleSize.width/2 + titleLabel->getContentSize().width * 0.3f, visibleSize.height * 0.75 - titleLabel->getContentSize().height));
    this->addChild(note, 2);

    startLabel = Label::createWithTTF("START", "fonts/space age.ttf", 20);

    auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MainMenuScene::goToGameScene, this));

    startItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height * 0.25f + origin.y));
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
