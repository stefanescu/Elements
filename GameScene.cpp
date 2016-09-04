#include "GameScene.h"

#include <fstream>
#include <sstream>


Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    //setKeyboardEnabled(true);
    collidesX = false;
    constantTouch = false;
    moveTouchLocation = lastTouch = shootTouchLocation = Point(0,0);
    tapCount = 0;



    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto listener = EventListenerTouchAllAtOnce::create();    
    listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(self::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(self::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(self::onTouchesCancelled, this);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(self::onKeyReleased, this);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

    level = new Level();
    //level->loadMap("level1.tmx");
    level->loadMap("map1-test2.tmx");
    level->retain();

    auto director = Director::getInstance();
    level->getMap()->setScale(SCALE_FACTOR);



    //SimpleAudioEngine::getInstance()->playBackgroundMusic("ancientbgm.mp3", true);


    this->addChild(level->getMap());

    player = Player::create();
    player->retain();

    Point point = Point(startingX, startingY);

    player->setPosition(level->tileCoordinateToPosition(point));


    Size wsize = Director::getInstance()->getVisibleSize();  //default screen size (or design resolution size, if you are using design resolution)
    Point *center = new Point(wsize.width/2 + origin.x, wsize.height/2 + origin.y);

    cameraTarget = Sprite::create();
    cameraTarget->setPositionX(player->getPositionX()); // set to players x
    cameraTarget->setPositionY(wsize.height/2 + origin.y + 10); // center of height

    cameraTarget->retain();

    this->addChild(player);


    this->schedule(schedule_selector(GameScene::updateScene));
    this->schedule(schedule_selector(GameScene::gameLogic), 2.0);

    this->addChild(cameraTarget);

    rectWithBorder = DrawNode::create();
    Vec2 vertices[] =
    {
        Vec2(0, player->getPlayerSize().height),
        Vec2(player->getPlayerSize().width, player->getPlayerSize().height),
                Vec2(player->getPlayerSize().width, 0),
        Vec2(0,0)
    };

    rectWithBorder->drawPolygon(vertices, 4, Color4F(0.0f,0.3f,0.3f,1), 0, Color4F(0.0f,0.2f,0.0f,1));

    addChild(rectWithBorder);

    camera = Follow::create(cameraTarget, Rect::ZERO);
    camera->retain();

    //auto labelNode = Node::create();
    //labelNode->retain();
     playerScoreText = Label::createWithTTF("Points:", "fonts/space age.ttf", 32);
     playerScore = Label::createWithTTF("000 MERGE", "fonts/space age.ttf", 32);
     playerScore->setAnchorPoint(Vec2(0.5, 0.5));
    //playerScoreText->retain();
   // playerScore->retain();
     playerScore->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
     //this->addChild(playerScore, 5);
     this->addChild(playerScore);
     /*
     playerScoreText->setPosition(playerScoreText->getContentSize().width / 2,
                                       wsize.height - playerScoreText->getContentSize().height / 2);

     playerScore->setPosition(Vec2(0 + playerScoreText->getPositionX() + playerScoreText->getContentSize().width,
                                   wsize.height - playerScoreText->getContentSize().height / 2 ));


     this->addChild(playerScoreText);

    labelNode->addChild(playerScoreText, 1);
    labelNode->addChild(playerScore, 2);
    playerScoreText->setPosition(10,10);




*/
    //labelNode->addChild(playerScore);







//TODO pushScene(), popScene() for pauseMenu
    //loadEnemies();

    this->runAction(camera);
    return true;
}

void GameScene::gameLogic(float d)
{
    //this->addEnemies();
}

void GameScene::addEnemies()
{
    Sprite *enemy = Sprite::create("94.png");

        // Determine where to spawn the enemy along the Y axis
        Size winSize = Director::getInstance()->getWinSize();
        int minX = enemy->getContentSize().width/2;
        int maxX = winSize.width -  minX;
        int rangeX = maxX - minX;

        // srand( TimGetTicks() );
        int actualX = ( rand() % rangeX ) + minX;

        // Create the enemy slightly off-screen along the right edge,
        // and along a random position along the Y axis as calculated
        //enemy->setPosition(ccp(winSize.height + (enemy->getContentSize().height/2), actualX) );
        enemy->setPosition(actualX, winSize.height + enemy->getContentSize().height/2);
        this->addChild(enemy);

        // Determine speed of the enemy
        int minDuration = (int)2.0;
        int maxDuration = (int)4.0;
        int rangeDuration = maxDuration - minDuration;
        // srand( TimGetTicks() );
        int actualDuration = ( rand() % rangeDuration )  + minDuration;

        enemyList.push_back(enemy);
        // Create the actions
        //CCFiniteTimeAction* actionMove = CCMoveTo::create( (float)actualDuration, ccp(0 - enemy->getContentSize().height/2, actualX) );
        auto actionMove = MoveTo::create( (float)actualDuration, Vec2(35, 10) );
        auto actionMoveDone = CCCallFuncN::create( this,  callfuncN_selector(GameScene::spriteMoveFinished));

        enemy->runAction( CCSequence::create(actionMove,  actionMoveDone, NULL) );


}

void GameScene::spriteMoveFinished(Node* sender)
{
  Sprite *sprite = (Sprite *)sender;
  std::vector<Node*>::iterator position = std::find(enemyList.begin(), enemyList.end(), sprite);
  if (position != enemyList.end())
      enemyList.erase(position);
  this->removeChild(sprite, true);
}


void GameScene::loadEnemies()
{
    Sprite *enemy1 = Sprite::create("94.png");
    enemy1->setPosition(level->tileCoordinateToPosition( Point(33, 2)));
    enemy1->setAnchorPoint(Point::ZERO);
    enemy1->setScale(ENEMY_SCALE_FACTOR);
    enemy1->setFlippedX(true);
    enemy1->retain();

    enemyList.push_back(enemy1);
    this->addChild(enemy1);

    Sprite *enemy2 = Sprite::create("94.png");
    enemy2->setPosition(level->tileCoordinateToPosition( Point(44, 2)));
    enemy2->setAnchorPoint(Point::ZERO);
    enemy2->setScale(ENEMY_SCALE_FACTOR);
    enemy2->setFlippedX(true);
    enemy2->retain();

    enemyList.push_back(enemy2);
    this->addChild(enemy2);

    Sprite *enemy3 = Sprite::create("94.png");
    enemy3->setPosition(level->tileCoordinateToPosition( Point(55, 2)));
    enemy3->setAnchorPoint(Point::ZERO);
    enemy3->setScale(ENEMY_SCALE_FACTOR);
    enemy3->setFlippedX(true);
    enemy3->retain();

    enemyList.push_back(enemy3);
    this->addChild(enemy3);

    Sprite *enemy4 = Sprite::create("94.png");
    enemy4->setPosition(level->tileCoordinateToPosition( Point(100, 2)));
    enemy4->setAnchorPoint(Point::ZERO);
    enemy4->setScale(ENEMY_SCALE_FACTOR);
    enemy4->setFlippedX(true);
    enemy4->retain();

    enemyList.push_back(enemy4);
    this->addChild(enemy4);

    Sprite *enemy5 = Sprite::create("94.png");
    enemy5->setPosition(level->tileCoordinateToPosition( Point(100, 6)));
    enemy5->setAnchorPoint(Point::ZERO);
    enemy5->setScale(ENEMY_SCALE_FACTOR);
    enemy5->setFlippedX(true);
    enemy5->retain();

    enemyList.push_back(enemy5);
    this->addChild(enemy5);
}


void GameScene::updateScene(float delta)
{
    //player->updateState(0.1f);


    //player->velocity_y-= GRAVITY;
    auto winSize = Director::getInstance()->getWinSize();
    auto dirX =1, dirY = 1;
    if (moveTouchLocation != Point(0,0))
    {
    if (moveTouchLocation.y < (winSize.height * 0.5f))
    {
        if (moveTouchLocation.x < (winSize.width * 0.5f))
            dirX = -1;
        player->move(dirX);
    }

    // jump
    else
    {
        if(player->isGrounded() && player->getVelocityY() <= 0)
        {
            //player->velocity_y = PLAYER_JUMP_VELOCITY;
            //player->grounded = false;
            player->jump();
            SimpleAudioEngine::getInstance()->playEffect("Explosion_03", false, 1.0, 1.0, 1.0);

        }
    }
    }


    if (shootTouchLocation != Point(0,0))
    {
        auto projectile = player->shootLaser();
        projectileList.push_back(projectile);
        projectile->setPosition(player->getPositionX() + player->getDirection(), player->getPositionY()+ 22);
        projectile->setScale(0.2f);
        auto travel = MoveBy::create(2, Vec2(700 * player->getDirection(),0));


        this->addChild(projectile);
        SimpleAudioEngine::getInstance()->playEffect("laser8.wav");
        projectile->runAction(travel);
    }
    player->applyGravity();
    Rect player_rect = player->getBoundingBox();

    Point tmp;
    std::vector<Rect> tiles;
    std::vector<Rect> deathTiles;

    tiles.clear();
    deathTiles.clear();

    // center of player's sprite
    tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->getPlayerSize().width * 0.5f ,
            player->getPositionY() + player->getPlayerSize().height * 0.5f));



    tiles = level->getCollisionTilesX(tmp, dirX);

    player_rect.setRect(
                    player->getBoundingBox().getMinX() + player->getVelocityX(),
                    player->getBoundingBox().getMinY() + 1.0f, // dont let the rectangle touch the ground otherwise, will count as collision
                    player->getPlayerSize().width,
                    player->getPlayerSize().height
    );

    for (Rect tile : tiles)
    {
        if (player_rect.intersectsRect(tile))
        {
            player->idle();
            break;
        }
    }

    for (Sprite* projectile : projectileList)
    {
        auto projectilePos = player->convertToWorldSpace(projectile->getPosition());
        auto projectileRect = Rect (projectilePos.x, projectilePos.y,
                                    projectile->getContentSize().width, projectile->getContentSize().height);
        for (Rect tile : tiles)
            if (projectileRect.intersectsRect(tile))
            {
                auto aux = projectile;
                projectileList.erase(remove(projectileList.begin(), projectileList.end(), projectile), projectileList.end());
                aux->removeFromParent();
                aux->release();

            }
    }
    shootTouchLocation = Point(0,0);
    tiles.clear();

    dirY = player->getVelocityY() > 0 ? 1 : -1;
    tiles = level->getCollisionTilesY(tmp, dirY);

    player_rect.setRect(
                player->getBoundingBox().getMinX(),
                player->getBoundingBox().getMinY(),
                player->getPlayerSize().width,
                player->getPlayerSize().height
    );

    for (Rect tile : tiles)
    {
        if (tile.intersectsRect(player_rect))
        {
            if (player->getVelocityY() > 0)
            {
                player->setPositionY( player->getPositionY() - player->getVelocityY());
                //player->fall();
            }
            else
            {
                player->setPositionY(tile.getMaxY());

                // if we hit the ground, mark us as grounded so we can jump
                if (!player->isGrounded())
                    SimpleAudioEngine::getInstance()->playEffect("jumpland44100.mp3", false, 1.0, 1.0, 1.0);

                player->setGrounded(true);

            }
            player->setVelocityY(0);
            break;
        }
            player->setGrounded(false);
    }

      deathTiles.clear();
      deathTiles = level->getCollisionDeathTiles(tmp, dirX);
      // check for hazard collisions
        for (Rect tile : deathTiles)
        {
            if (tile.intersectsRect(player_rect) ) //death
            {
                SimpleAudioEngine::getInstance()->playEffect("Explosion_03.mp3", false, 1.0, 1.0, 1.0);
                Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
                player->setPosition(p);
            }
        }



    // check for enemy collisions
    for (Node *tile : enemyList)
    {
        if (tile->getBoundingBox().intersectsRect(player_rect) )
        {
            Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
            player->setPosition(p);
        }
    }
    if (player->getVelocityX() == 0 && player->getVelocityY() == 0)
        player->idle();

    player->updateState(delta);
    //player->velocity_x = 0;
    //player->setVelocityY(0);
    player->setVelocityX(0);
    cameraTarget->setPositionX (player->getPositionX());

    //cameraTarget->setPositionY (player->getPositionY() + 100);
    cameraTarget->setPositionY (player->getPositionY() + (player->getPositionY() < 5 ? 0 : 100));
    moveTouchLocation = Point(0,0);
    //this->updatePlayer(delta);
}

void GameScene::updatePlayer(float delta)
{
    auto dirX =0, dirY = 0;
    if (std::find(heldKeys.begin(), heldKeys.end(), SPACEBAR) != heldKeys.end())
    {
        if(player->isGrounded() && player->getVelocityY() <= 0)
        {
            player->jump();
            SimpleAudioEngine::getInstance()->playEffect("Explosion_03", false, 1.0, 1.0, 1.0);
        }
    }

    if(std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end())
        dirX = 1;
    if(std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end())
        dirX = -1;



    //player->applyGravity();

    Rect player_rect = player->getBoundingBox();

    Point tmp;
    std::vector<Rect> tiles;
    std::vector<Rect> deathTiles;

    tiles.clear();
    deathTiles.clear();

    // center of player's sprite
    tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->getPlayerSize().width * 0.5f ,
            player->getPositionY() + player->getPlayerSize().height * 0.5f));



    tiles = level->getCollisionTilesX(tmp, dirX);

    player_rect.setRect(
                    player->getBoundingBox().getMinX() + player->getVelocityX(),
                    player->getBoundingBox().getMinY() + 1.0f, // dont let the rectangle touch the ground otherwise, will count as collision
                    player->getPlayerSize().width,
                    player->getPlayerSize().height
    );

    for (Rect tile : tiles)
    {
        if (player_rect.intersectsRect(tile))
        {
            //player->velocity_x = 0;
            //player->idle();
            break;
        }
    }

    tiles.clear();


    tiles = level->getCollisionTilesY(tmp, dirY);

    player_rect.setRect(
                player->getBoundingBox().getMinX(),
                player->getBoundingBox().getMinY(),
                player->getPlayerSize().width,
                player->getPlayerSize().height
    );

    for (Rect tile : tiles)
    {
        if (tile.intersectsRect(player_rect))
        {
            if (player->getVelocityY() > 0)
            {
                player->setPositionY( player->getPositionY() - player->getVelocityY());
            }
            else
            {
                player->setPositionY(tile.getMaxY());

                // if we hit the ground, mark us as grounded so we can jump
                if (!player->isGrounded())
                    SimpleAudioEngine::getInstance()->playEffect("jumpland44100.mp3", false, 1.0, 1.0, 1.0);

                player->setGrounded(true);

            }
            player->setVelocityY(0);
            break;
        }
            player->setGrounded(false);
    }

      deathTiles.clear();
      deathTiles = level->getCollisionDeathTiles(tmp, dirX);
      // check for hazard collisions
        for (Rect tile : deathTiles)
        {
            if (tile.intersectsRect(player_rect) ) //death
            {
                SimpleAudioEngine::getInstance()->playEffect("Explosion_03.mp3", false, 1.0, 1.0, 1.0);
                Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
                player->setPosition(p);
            }
        }


    // check for enemy collisions
    for (Node *tile : enemyList)
    {
        if (tile->getBoundingBox().intersectsRect(player_rect) )
        {
            Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
            player->setPosition(p);
        }
    }

    player->updateState(delta);
    //player->velocity_x = 0;
    player->setVelocityX(0);

}


void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end())
            heldKeys.push_back(keyCode);


}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
    if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
       Director::getInstance()->end();
}

/*int GameScene::signum(float x)
{
  if (x > 0.0L)
    return 1.0L;
  else if (x < 0.0L)
    return -1.0L;
  else
    return 0.0L;
}*/

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

inline Point locationInGLFromTouch(Touch& touch)
{
  auto director = Director::getInstance();
  return director->convertToGL(touch.getLocationInView());
}

void GameScene::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    //constantTouch = true;
    //while(constantTouch)
        //for (auto& touch : touches)
            //this->touch(locationInGLFromTouch(*touch));
    //for (auto& touch : touches)
        //lastTouch = locationInGLFromTouch(*touch);
    //for (auto& touch : touches)
        //this->touch(locationInGLFromTouch(*touch));
    for (auto& touch : touches)
        this->touch(locationInGLFromTouch(*touch));

}

void GameScene::onTouchesMoved(const vector<Touch*>& touches, Event* event)
{
    for (auto& touch : touches)
        this->touch(locationInGLFromTouch(*touch));
}

void GameScene::onTouchesCancelled(const vector<Touch *> &touches, Event *event)
{

}

/*void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {

    if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
       Director::getInstance()->end();

}*/


void GameScene::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
    //constantTouch = false;

}

void GameScene::touch(const Point& location)
{
    if ((location.y <= Director::getInstance()->getVisibleSize().height/4)
        || (location.y > Director::getInstance()->getVisibleSize().height/2))
        this->moveTouchLocation = location;
    else if((location.y > Director::getInstance()->getVisibleSize().height/4)
            && (location.y <= Director::getInstance()->getVisibleSize().height/2))
    {
        ++tapCount;
        //lastTouch = location;

        if(tapCount == 1)
            this->schedule(schedule_selector(GameScene::resetTapCount), 0.25);
        else
        {
            this->unschedule(schedule_selector(GameScene::resetTapCount));
            shootTouchLocation = location;

        }
    }
}

void GameScene::resetTapCount(float t)
{
    tapCount = 0;
}

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}
