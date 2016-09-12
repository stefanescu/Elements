#include "AppDelegate.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Elements", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Elements");
#endif
        director->setOpenGLView(glview);
    }
    //director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();

    UserDefault::getInstance()->setIntegerForKey("high_score", 0);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player1_anim.plist");

    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("for nines psg.mp3");
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("ancientbgm.mp3");

    SimpleAudioEngine::getInstance()->preloadEffect("SFX_Jump_09.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("jumpland44100.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("Explosion_03.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("laser8");
    SimpleAudioEngine::getInstance()->preloadEffect("laser2.mp3");

    auto scene = MainMenuScene::createScene();

    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
