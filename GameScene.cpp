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
    tapCount = 0, shotCount = 0, shotInterval = 1.0;
    powerAvailable = false;

    srand(time(NULL));
    //MainMenuScene::debugTest = "MERGE";

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
    //level->loadMap("map1-test2.tmx");
    level->loadMap("finalMap.tmx");
    level->retain();

    auto director = Director::getInstance();
    level->getMap()->setScale(SCALE_FACTOR);



    //SimpleAudioEngine::getInstance()->playBackgroundMusic("ancientbgm.mp3", true);


    this->addChild(level->getMap());

    player = Player::create();
    player->retain();

    this->addEnemies();

    Point point = Point(startingX, startingY);

    player->setPosition(level->tileCoordinateToPosition(point));

    ship = Sprite::create("shipGreen.png");
    ship->setPosition(level->tileCoordinateToPosition(shipLocation));
    ship->setScale(0.3f);
    ship->retain();
    this->addChild(ship);

    Size wsize = Director::getInstance()->getVisibleSize();  //default screen size (or design resolution size, if you are using design resolution)
    Point *center = new Point(wsize.width/2 + origin.x, wsize.height/2 + origin.y);

    cameraTarget = Sprite::create();
    cameraTarget->setPositionX(player->getPositionX()); // set to players x
    cameraTarget->setPositionY(wsize.height/2 + origin.y + 10); // center of height

    cameraTarget->retain();

    this->addChild(player);

    this->schedule(schedule_selector(GameScene::resetShotCount), shotInterval);
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
     //playerScoreText = Label::createWithTTF("Points:", "fonts/space age.ttf", 32);
     //playerScore = Label::createWithTTF("000 MERGE", "fonts/space age.ttf", 32);
    //playerScore = Label::createWithSystemFont("TEST", "arial", 24);
    // playerScore->setAnchorPoint(Vec2(0.5, 0.5));
    //playerScoreText->retain();
   // playerScore->retain();
     //playerScore->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
     //this->addChild(playerScore, 2);
     //this->addChild(playerScore);
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
    this->addEnemies();
}

void GameScene::addEnemies()
{
    int r = rand() % 2;

    Enemy *enemy = Enemy::create();
    enemy->setPosition(level->tileCoordinateToPosition(r == 0 ? enemySpawnLeft : enemySpawnRight));
    r = rand() % 2;
    enemy->move(r == 0 ? -1 : 1);
    this->addChild(enemy);
    enemyList.push_back(enemy);

    /*Sprite *enemy = Sprite::create("94.png");

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
*/

}

/*void GameScene::spriteMoveFinished(Node* sender)
{
  Sprite *sprite = (Sprite *)sender;
  std::vector<Node*>::iterator position = std::find(enemyList.begin(), enemyList.end(), sprite);
  if (position != enemyList.end())
      enemyList.erase(position);
  this->removeChild(sprite, true);
}*/


/*void GameScene::loadEnemies()
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
*/

void GameScene::powerCheck()
{
    if (player->getKillCount() == 10)
    {
        powerNotification = Sprite::create("explamation_1.png");
        powerNotification->setPosition(level->tileCoordinateToPosition(Point(24,5)));
        this->addChild(powerNotification);
        powerAvailable = true;
        player->resetKillCount();
    }
}

void GameScene::usePower()
{
    powerAvailable = false;
    powerNotification->removeFromParent();
}

void GameScene::updateScene(float delta)
{
    //player->updateState(0.1f);


    //player->velocity_y-= GRAVITY;
    auto winSize = Director::getInstance()->getWinSize();
    auto dirX =1, dirY = 1;
    auto enemyDirX = 1, enemyDirY = -1;
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


    if (shootTouchLocation != Point(0,0) && shotCount < 2)
    {
        auto projectile = player->shootLaser();
        projectileList.push_back(projectile);
        projectile->setPosition(player->getPositionX() + player->getDirection(), player->getPositionY()+ 22);
        projectile->setScale(0.2f);
        auto travel = MoveBy::create(2, Vec2(700 * player->getDirection(),0));
        /*auto action  = Sequence::create(
                MoveBy::create(2, Vec2(700 * player->getDirection(),0)),
                CallFuncN::create( CC_CALLBACK_1(GameScene::doRemoveFromParentAndCleanup, this, true)),
                NULL);*/

        ++shotCount;
        this->addChild(projectile);
        SimpleAudioEngine::getInstance()->playEffect("laser8.wav");
        projectile->runAction(travel);
    }
    player->applyGravity();

    for(Enemy* enemy : enemyList)
        enemy->applyGravity();
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
    Rect enemy_rect;
    for (Enemy *enemy : enemyList)
    {
        auto enemyCent = level->positionToTileCoordinate(Point(enemy->getPositionX() + enemy->getPlayerSize().width * 0.5f ,
                                                               enemy->getPositionY() + enemy->getPlayerSize().height * 0.5f));
        enemy_rect = enemy->getBoundingBox();
        enemy_rect.setRect (
                    enemy->getBoundingBox().getMinX() + enemy->getVelocityX(),
                    enemy->getBoundingBox().getMinY() + 1.0f,
                    enemy->getPlayerSize().width,
                    enemy->getPlayerSize().height
                    );
        tiles = level->getCollisionTilesX(enemyCent, enemy->getDirection());
        for (Rect tile : tiles)
        {
            if (enemy_rect.intersectsRect(tile))
                enemy->reverseDirection();
        }
    }

    /*for (Sprite* projectile : projectileList)
    {
        auto projectilePos = projectile->getPosition();//player->convertToWorldSpace(projectile->getPosition());
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

    }*/
    for (Sprite* projectile : projectileList)
    {
        auto projectilePos = projectile->getPosition();//player->convertToWorldSpace(projectile->getPosition());
        auto projectileRect = Rect (projectilePos.x, projectilePos.y,
                                    projectile->getContentSize().width, projectile->getContentSize().height);
        for (Enemy* enemy : enemyList)
        {
            enemy_rect = enemy->getBoundingBox();
            enemy_rect.setRect (
                        enemy->getBoundingBox().getMinX() + enemy->getVelocityX(),
                        enemy->getBoundingBox().getMinY() + 1.0f,
                        enemy->getPlayerSize().width,
                        enemy->getPlayerSize().height
                        );
            if (projectileRect.intersectsRect(enemy_rect))
            {
                player->increaseKillCount();
                auto aux = projectile;
                projectileList.erase(remove(projectileList.begin(), projectileList.end(), projectile), projectileList.end());
                aux->removeFromParent();
                aux->release();

                enemy->takeDmg();
                if(!enemy->getHP())
                {
                    aux = enemy;
                    enemyList.erase(remove(enemyList.begin(), enemyList.end(), enemy),enemyList.end());
                    aux->removeFromParent();
                    aux->release();

                }
            }
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
            //player->setGrounded(false);
    }

    for (Enemy* enemy : enemyList)
    {
        auto enemyCent = level->positionToTileCoordinate(Point(enemy->getPositionX() + enemy->getPlayerSize().width * 0.5f ,
                                                               enemy->getPositionY() + enemy->getPlayerSize().height * 0.5f));
        enemy_rect.setRect(
                    enemy->getBoundingBox().getMinX(),
                    enemy->getBoundingBox().getMinY(),
                    enemy->getPlayerSize().width,
                    enemy->getPlayerSize().height
        );
        tiles = level->getCollisionTilesY(enemyCent, enemyDirY);
        for (Rect tile : tiles)
        {
            if (enemy_rect.intersectsRect(tile))
            {
                enemy->setPositionY(tile.getMaxY());

                enemy->setGrounded(true);
                enemy->setVelocityY(0);
            }


        }
    }

      /*deathTiles.clear();
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
        }*/




    // check for enemy collisions
    for (Node *tile : enemyList)
    {
        if (tile->getBoundingBox().intersectsRect(player_rect) )
        {
            player->takeDmg();
            if (player->getHP())
            {
                Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
                player->setPosition(p);
            }
            else
            {
             SimpleAudioEngine::getInstance()->stopBackgroundMusic();
             auto gameScene = GameOverScene::createScene();
             Director::getInstance()->replaceScene(TransitionSlideInT::create(TRANSITION_TIME, gameScene));
            }
        }
    }
    if (player->getVelocityX() == 0 && player->getVelocityY() == 0)
        player->idle();

    powerCheck();
    player->updateState(delta);
    for (Enemy* enemy : enemyList)
        enemy->updateState(delta);
\
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
    //Director::getInstance()->pushScene(MainMenuScene::createScene());

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
    else if ((location.y < Director::getInstance()->getVisibleSize().height /2)
        && (location.x == Director::getInstance()->getVisibleSize().width / 2))
        usePower();
}

void GameScene::resetTapCount(float t)
{
    tapCount = 0;
}

void GameScene::resetShotCount(float t)
{
    shotCount = 0;
}

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}
