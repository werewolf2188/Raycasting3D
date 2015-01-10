//
//  Player.cpp
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#include "Player.h"
#include <cmath>

Player createPlayer(Vector2DD position, Vector2DD direction, Vector2DD cameraPlane)
{
    Player player;
    player.position.x = position.x;
    player.position.y = position.y;
    player.direction.x = direction.x;
    player.direction.y = direction.y;
    player.cameraPlane.x = cameraPlane.x;
    player.cameraPlane.y = cameraPlane.y;
    return player;
}
void initialCalculations(Player &player, int x, int width)
{
    //calculate ray position and direction
    player.cameraX = 2 * x / double(width) - 1; //x-coordinate in camera space
    player.rayPosition.x = player.position.x;
    player.rayPosition.y = player.position.y;
    player.rayDirection.x = player.direction.x + player.cameraPlane.x * player.cameraX;
    player.rayDirection.y = player.direction.y + player.cameraPlane.y * player.cameraX;
    
    //which box of the map we're in
    player.map.x = int(player.rayPosition.x);
    player.map.y = int(player.rayPosition.y);
    
    //length of ray from current position to next x or y-side
    player.sideDistance = createVector2DD();
    
    
    //length of ray from one x or y-side to next x or y-side
    player.deltaDistance.x = sqrt(1 + (player.rayDirection.y * player.rayDirection.y) / (player.rayDirection.x * player.rayDirection.x));
    player.deltaDistance.y = sqrt(1 + (player.rayDirection.x * player.rayDirection.x) / (player.rayDirection.y * player.rayDirection.y));
    player.perpWallDist = 0;
    
    //what direction to step in x or y-direction (either +1 or -1)
    player.step = createVector2DI();
    
    
    player.hit = 0; //was there a wall hit?
    player.side = 0; //was a NS or a EW wall hit?
}

void calculateInitialSideDistanceAndStep(Player &player)
{
    if (player.rayDirection.x < 0)
    {
        player.step.x = -1;
        player.sideDistance.x = (player.rayPosition.x - player.map.x) * player.deltaDistance.x;
    }
    else
    {
        player.step.x = 1;
        player.sideDistance.x = (player.map.x + 1.0 - player.rayPosition.x) * player.deltaDistance.x;
    }
    if (player.rayDirection.y < 0)
    {
        player.step.y = -1;
        player.sideDistance.y = (player.rayPosition.y - player.map.y) * player.deltaDistance.y;
    }
    else
    {
        player.step.y = 1;
        player.sideDistance.y = (player.map.y + 1.0 - player.rayPosition.y) * player.deltaDistance.y;
    }
    
}

void DDA(Player &player, int worldMap[mapWidth][mapHeight])
{
    while (player.hit == 0)
    {
        //jump to next map square, OR in x-direction, OR in y-direction
        if (player.sideDistance.x < player.sideDistance.y)
        {
            player.sideDistance.x += player.deltaDistance.x;
            player.map.x += player.step.x;
            player.side = 0;
        }
        else
        {
            player.sideDistance.y += player.deltaDistance.y;
            player.map.y += player.step.y;
            player.side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[player.map.x][player.map.y] > 0) player.hit = 1;
    }
    
}

void fishEye(Player &player)
{
    if (player.side == 0)
        player.perpWallDist = fabs((player.map.x - player.rayPosition.x + (1 - player.step.x) / 2) / player.rayDirection.x);
    else
        player.perpWallDist = fabs((player.map.y - player.rayPosition.y + (1 - player.step.y) / 2) / player.rayDirection.y);
}