//
//  SpritesConstants.h
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#ifndef SDLExample_SpritesConstants_h
#define SDLExample_SpritesConstants_h

#include "Sprite.h"


#define SPRITES
#define numSprites 19
Sprite sprite[numSprites] =
{
    {20.5, 11.5, 2}, //green light in front of playerstart
    //green lights in every room
    {18.5,4.5, 2},
    {10.0,4.5, 2},
    {10.0,12.5,2},
    {3.5, 6.5, 2},
    {3.5, 20.5,2},
    {3.5, 14.5,2},
    {14.5,20.5,2},
    
    //row of pillars in front of wall: fisheye test
    {18.5, 10.5, 1},
    {18.5, 11.5, 1},
    {18.5, 12.5, 1},
    
    //some barrels around the map
    {21.5, 1.5, 0},
    {15.5, 1.5, 0},
    {16.0, 1.8, 0},
    {16.2, 1.2, 0},
    {3.5,  2.5, 0},
    {9.5, 15.5, 0},
    {10.0, 15.1,0},
    {10.5, 15.8,0},
};


int spriteOrder[numSprites];
double spriteDistance[numSprites];
double ZBuffer[screenWidth];
std::vector<Uint32> texture2[3];

inline void spriteCasting(Player &player, uint _buffer[], uint screenH)
{
    //SPRITE CASTING
    //sort sprites from far to close
    for(int i = 0; i < numSprites; i++)
    {
        spriteOrder[i] = i;
        spriteDistance[i] = ((player.position.x - sprite[i].x) * (player.position.x - sprite[i].x) + (player.position.y - sprite[i].y) * (player.position.y - sprite[i].y)); //sqrt not taken, unneeded
    }
    combSort(spriteOrder, spriteDistance, numSprites);
    
    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < numSprites; i++)
    {
        //translate sprite position to relative to camera
        double spriteX = sprite[spriteOrder[i]].x - player.position.x;
        double spriteY = sprite[spriteOrder[i]].y - player.position.y;
        
        //transform sprite with the inverse camera matrix
        // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        // [ planeY   dirY ]                                          [ -planeY  planeX ]
        
        double invDet = 1.0 / (player.cameraPlane.x * player.direction.y - player.direction.x * player.cameraPlane.y); //required for correct matrix multiplication
        
        double transformX = invDet * (player.direction.y * spriteX - player.direction.x * spriteY);
        double transformY = invDet * (-player.cameraPlane.y * spriteX + player.cameraPlane.x * spriteY); //this is actually the depth inside the screen, that what Z is in 3D
        
        int spriteScreenX = int((w / 2) * (1 + transformX / transformY));
        
        //parameters for scaling and moving the sprites
#define uDiv 1
#define vDiv 1
#define vMove 0.0
        int vMoveScreen = int(vMove / transformY);
        
        //calculate height of the sprite on screen
        int spriteHeight = abs(int(h / (transformY))) / vDiv; //using "transformY" instead of the real distance prevents fisheye
        //calculate lowest and highest pixel to fill in current stripe
        int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
        if(drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
        if(drawEndY >= h) drawEndY = h - 1;
        
        //calculate width of the sprite
        int spriteWidth = abs( int (h / (transformY))) / uDiv;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if(drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if(drawEndX >= w) drawEndX = w - 1;
        
        
        //loop through every vertical stripe of the sprite on screen
        for(int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
            //the conditions in the if are:
            //1) it's in front of camera plane so you don't see things behind you
            //2) it's on the screen (left)
            //3) it's on the screen (right)
            //4) ZBuffer, with perpendicular distance
            if(transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
                for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
                {
                    int d = (y-vMoveScreen) * 256 - h * 128 + spriteHeight * 128;  //256 and 128 factors to avoid floats
                    int texY = ((d * texHeight) / spriteHeight) / 256;
                    Uint32 color = texture2[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
                    
                    if((color & 0x00FFFFFF) != 0) *((int *)_buffer + stripe * screenH + y) = color; //_buffer[(stripe / screenW) * (y / screenH) ] = color; //paint pixel if it isn't black, black is the invisible color
                }
        }
    }
}


#endif
