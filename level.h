#pragma once

#include "cocos2d.h"
#include <vector>
#include "globals.h"

using namespace std;
using namespace cocos2d;

class Level : public Object
{
private:
    TMXTiledMap *map;

public:
    void loadMap(const char* name);
    TMXTiledMap * getMap();

    Point tileCoordinateToPosition(Point point);
    Point positionToTileCoordinate(Point point);
    vector<Rect> getCollisionTilesY(Point point, int direction);
    vector<Rect> getCollisionTilesX(Point point, int direction);
    vector<Rect> getCollisionDeathTiles(Point point, int direction);

    Level(void);
    virtual ~Level(void);
};
