//
//  Player.h
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#ifndef __SDLExample__Player__
#define __SDLExample__Player__

#include <stdio.h>
#include "Vector2D.h"
#include "Constants.h"

struct Player
{
    Vector2DD position;
    Vector2DD direction;
    Vector2DD cameraPlane;
    
    double cameraX;
    Vector2DD rayPosition;
    Vector2DD rayDirection;
    
    Vector2DI map;
    
    Vector2DD sideDistance;
    Vector2DD deltaDistance;
    double perpWallDist;
    
    Vector2DI step;
    
    int hit; //was there a wall hit?
    int side;
};

Player createPlayer(Vector2DD position, Vector2DD direction, Vector2DD cameraPlane);
void initialCalculations(Player &player, int x, int width);
void calculateInitialSideDistanceAndStep(Player &player);
void DDA(Player &player, int worldMap[mapWidth][mapHeight]);
void fishEye(Player &player);
#endif /* defined(__SDLExample__Player__) */
