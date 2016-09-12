#include "GameScene.h"

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
        return false;


    collidesX = false;
    constantTouch = false;
    moveTouchLocation = lastTouch = shootTouchLocation = Point(0,0);
    tapCount = 0, shotCount = 0, score = 0, shotInterval = 1.0;
    powerAvailable = false, invulnerable = false;


    srand(time(NULL));

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
    level->loadMap("finalMap.tmx");
    level->retain();

    auto director = Director::getInstance();
    level->getMap()->setScale(SCALE_FACTOR);

    SimpleAudioEngine::getInstance()->playBackgroundMusic("ancientbgm.mp3", true);


    this->addChild(level->getMap());

    player = Player::create();
    player->retain();



    Point point = Point(startingX, startingY);

    player->setPosition(level->tileCoordinateToPosition(point));

    ship = Sprite::create("shipGreen.png");
    ship->setPosition(level->tileCoordinateToPosition(shipLocation));
    ship->setScale(0.4f);
    ship->retain();
    this->addChild(ship);

    Size wsize = Director::getInstance()->getVisibleSize();
    Point *center = new Point(wsize.width/2 + origin.x, wsize.height/2 + origin.y);

    cameraTarget = Sprite::create();
    cameraTarget->setPositionX(player->getPositionX());
    cameraTarget->setPositionY(wsize.height/2 + origin.y + 10);

    cameraTarget->retain();

    this->addChild(player);

    this->schedule(schedule_selector(GameScene::resetShotCount), shotInterval);
    this->schedule(schedule_selector(GameScene::updateScene));
    this->schedule(schedule_selector(GameScene::gameLogic), 1.0);

    this->addChild(cameraTarget);
    camera = Follow::create(cameraTarget, Rect::ZERO);
    camera->retain();

    this->runAction(camera);

    //this->addEnemies();
    //this->addEnemies();
    //this->addEnemies();

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
}

void GameScene::powerCheck()
{
    if (player->getKillCount() == 10)
    {
        powerNotification = Sprite::create("exclamation_1.png");
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
    auto shipProjectile1 = Sprite::create("shipProjectile.png");
    shipProjectile1->retain();
    shipProjectile1->setPosition(level->tileCoordinateToPosition(Point(shipLocation.x - 2, shipLocation.y)));
    shipProjectile1->setScale(0.4f);
    shipProjectile1->setFlippedX(true);

    auto shipProjectile2 = Sprite::create("shipProjectile.png");
    shipProjectile2->retain();
    shipProjectile2->setPosition(level->tileCoordinateToPosition(Point(shipLocation.x + 2, shipLocation.y)));
    shipProjectile2->setScale(0.4f);
    shipProjectile2->setFlippedX(false);

    SimpleAudioEngine::getInstance()->playEffect("laser8.wav");
    auto travel1 = MoveBy::create(2, Vec2(-700, 0));
    auto travel2 = MoveBy::create(2, Vec2(700, 0));

    projectileList.push_back(shipProjectile1);
    projectileList.push_back(shipProjectile2);

    shipProjectile1->runAction(travel1);
    shipProjectile2->runAction(travel2);
    this->addChild(shipProjectile1,3);
    this->addChild(shipProjectile2,3);
    if (shotInterval >=0.2f)
        shotInterval-= 0.2f;
    //Enemy::maxHP++;

}

void GameScene::updateScene(float delta)
{
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
            player->jump();
            SimpleAudioEngine::getInstance()->playEffect("SFX_Jump_09.mp3", false, 1.0, 1.0, 1.0);
        }
    }
    }


    if (shootTouchLocation != Point(0,0) && shotCount < 2)
    {
       // auto projectile = player->shootLaser();
        auto projectile = Sprite::create("laserprojectile.png");
        projectileList.push_back(projectile);
        projectile->setPosition(player->getPositionX() + player->getDirection(), player->getPositionY()+ 22);
        projectile->setScale(0.2f);
        auto travel = MoveBy::create(2, Vec2(700 * player->getDirection(),0));
        this->addChild(projectile);
        //SimpleAudioEngine::getInstance()->playEffect("laser8.wav");
        SimpleAudioEngine::getInstance()->playEffect("laser2.mp3");
        projectile->runAction(travel);
        ++shotCount;
    }

    player->applyGravity();

    for(Enemy* enemy : enemyList)
        enemy->applyGravity();

    Rect player_rect;


    std::vector<Rect> tiles;
    std::vector<Rect> deathTiles;

    tiles.clear();
    deathTiles.clear();

    Point tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->getPlayerSize().width * 0.5f ,
            player->getPositionY() + player->getPlayerSize().height * 0.5f));



    tiles = level->getCollisionTilesX(tmp, dirX);

    player_rect.setRect(
                    player->getBoundingBox().getMinX() + player->getVelocityX(),
                    player->getBoundingBox().getMinY() + 1.0f,
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
    tiles.clear();
    Rect enemy_rect;
    for (Enemy *enemy : enemyList)
    {
        auto enemyCent = level->positionToTileCoordinate(Point(enemy->getPositionX() + enemy->getPlayerSize().width * 0.5f ,
                                                               enemy->getPositionY() + enemy->getPlayerSize().height * 0.5f));
        enemy_rect.setRect (
                    enemy->getBoundingBox().getMinX() + enemy->getVelocityX(),// + 1.0f,
                    enemy->getBoundingBox().getMinY() + 1.0f,
                    enemy->getPlayerSize().width,
                    enemy->getPlayerSize().height
                    );
        tiles = level->getCollisionTilesX(enemyCent, enemy->getDirection());
        for (Rect tile : tiles)
        {
            if (enemy_rect.intersectsRect(tile))
            {
                enemy->reverseDirection();
                break;
            }
        }
    }

    for (Sprite* projectile : projectileList)
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

    }

    for (Sprite* projectile : projectileList)
    {
        auto projectilePos = projectile->getPosition();//player->convertToWorldSpace(projectile->getPosition());
        auto projectileRect = Rect (projectilePos.x, projectilePos.y,
                                    projectile->getContentSize().width, projectile->getContentSize().height);
        for (Enemy* enemy : enemyList)
        {
            enemy_rect.setRect (
                        enemy->getBoundingBox().getMinX(),
                        enemy->getBoundingBox().getMinY() + 1.0f,
                        enemy->getPlayerSize().width,
                        enemy->getPlayerSize().height
                        );
            if (projectileRect.intersectsRect(enemy_rect))
            {
                //auto aux = projectile;
                projectile->setVisible(false);
                projectile->removeFromParent();
                projectile->release();
                projectileList.erase(remove(projectileList.begin(), projectileList.end(), projectile), projectileList.end());
                //aux->setVisible(false);
                //aux->removeFromParent();
                //aux->release();

                enemy->takeDmg();
                if(!enemy->getHP())
                {
                    player->increaseKillCount();
                    ++score;
                    auto aux2 = enemy;

                    auto rotateAction = RotateBy::create (1.0f, 40.0f);
                    auto fadeAction = FadeOut::create (1.0f);
                    auto spawn = Spawn::createWithTwoActions(rotateAction, fadeAction);
                    auto deleteEnemy = CallFunc::create([aux2](){
                        aux2->removeFromParent();
                        aux2->release();
                    });

                    auto sequence = Sequence::create(spawn, deleteEnemy, nullptr);
                    enemy->runAction(sequence);
                    //enemy->runAction(spawn);

                    enemyList.erase(remove(enemyList.begin(), enemyList.end(), enemy),enemyList.end()); //  ???

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
                player->setPositionY( player->getPositionY() - player->getVelocityY());

            else
            {
                player->setPositionY(tile.getMaxY());

                if (!player->isGrounded())
                    SimpleAudioEngine::getInstance()->playEffect("jumpland44100.mp3", false, 1.0, 1.0, 1.0);

                player->setGrounded(true);

            }
            player->setVelocityY(0);
            break;
        }
    }
    tiles.clear();

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
                break;
            }


        }
        enemy->setGrounded(false);
    }

      deathTiles.clear();
      deathTiles = level->getCollisionDeathTiles(tmp, dirX);
        for (Rect tile : deathTiles)
        {
            if (tile.intersectsRect(player_rect) ) //death
            {
                SimpleAudioEngine::getInstance()->playEffect("Explosion_03.mp3", false, 1.0, 1.0, 1.0);
                Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
                player->setPosition(p);
            }
        }




    for (Enemy *enemy : enemyList)
    {
        if (enemy->getBoundingBox().intersectsRect(player_rect) &&  (!invulnerable))
        {
            player->takeDmg();
            activateInvulnerability();
            if (player->getHP())
            {
                Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
                player->setPosition(p);
            }
            else
            {
             UserDefault::getInstance()->setIntegerForKey("last_score", score);
             if (score > UserDefault::getInstance()->getIntegerForKey("high_score"))
                 UserDefault::getInstance()->setIntegerForKey("high_score", score);

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

    player->setVelocityX(0);
    cameraTarget->setPositionX (player->getPositionX());
    //cameraTarget->setPositionY (player->getPositionY() + (player->getPositionY() < 5 ? 0 : 20)); // ???
    if (player->getPositionY() <= 10)
        cameraTarget->setPositionY(0);
    else
        cameraTarget->setPositionY(player->getPositionY());
    moveTouchLocation = Point(0,0);
}

void GameScene::activateInvulnerability()
{
    invulnerable = true;
    this->schedule(schedule_selector(GameScene::disableInvulnerability), 2.0f);
    player->setOpacity(200.0f);
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
        for (Rect tile : deathTiles)
        {
            if (tile.intersectsRect(player_rect) ) //death
            {
                SimpleAudioEngine::getInstance()->playEffect("Explosion_03.mp3", false, 1.0, 1.0, 1.0);
                Point p = level->tileCoordinateToPosition( Point(startingX, startingY));
                player->setPosition(p);
            }
        }


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

void GameScene::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{

}

void GameScene::touch(const Point& location)
{
    if ((location.y >= Director::getInstance()->getVisibleSize().height *0.9) && powerAvailable)
        //&& (location.x == Director::getInstance()->getVisibleSize().width / 2))

        usePower();
    else if ((location.y <= Director::getInstance()->getVisibleSize().height/4)
        || (location.y > Director::getInstance()->getVisibleSize().height/2) &&(location.y < Director::getInstance()->getVisibleSize().height * 0.9))
        this->moveTouchLocation = location;

    else if((location.y > Director::getInstance()->getVisibleSize().height/4)
            && (location.y <= Director::getInstance()->getVisibleSize().height/2))
    {
        ++tapCount;

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

void GameScene::resetShotCount(float t)
{
    shotCount = 0;
}

void GameScene::disableInvulnerability(float t)
{
    invulnerable = false;
    player->setOpacity(300.0f);
}

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}
