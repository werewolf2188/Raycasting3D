/*
 Copyright (c) 2004-2007, Lode Vandevenne
 
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "main.h"


////////////////////////////////////SOLO MODIFICO CODIGO AQUI

//Flat map
//#include "FlatMap.h"
//Texture map
//#include "TextureMap.h"
//Texture map with floor casting
#include "TextureSpriteMap.h"
//Load PNG textures
#define USEPNG 1
//Sprites and stuff
#include "SpritesConstants.h"

/////////////////////////////////////

Uint32 buffer[screenWidth][screenHeight];
std::vector<Uint32> texture[8];

/*
 g++ *.cpp -lSDL -O3 -W -Wall -ansi -pedantic
 g++ *.cpp -lSDL
 */

int main(int /*argc*/, char */*argv*/[])
{
    Player player = createPlayer(createVector2DD(22, 12), createVector2DD(-1, 0), createVector2DD(0, 0.66));
    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame
    
    
#ifdef SDLExample_FlatMapp_h
    screen(512, 384, 0, "Raycaster");
#endif
    
#ifdef USETEXTURE
    for(int i = 0; i < 8; i++) texture[i].resize(texWidth * texHeight);
#ifdef SPRITES
    for(int i = 0; i < 3; i++) texture2[i].resize(texWidth * texHeight);
#endif
    screen(screenWidth,screenHeight, 0, "Raycaster");
#endif
    generateTextures();
    while(!done())
    {
        for(int x = 0; x < w; x++)
        {            
            //calculate ray position and direction
            initialCalculations(player, x, w);
            //calculate step and initial sideDist
            calculateInitialSideDistanceAndStep(player);
            //perform DDA
            DDA(player, worldMap);
            //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
            fishEye(player);
            //Calculate height of line to draw on screen
            drawColorLine(player, x);
        }
        //timing for input and FPS counter
#ifdef USETEXTURE
        drawBuffer(buffer[0]);
        for(int x = 0; x < w; x++) for(int y = 0; y < h; y++) buffer[x][y] = 0; //clear the buffer instead of cls()
#endif
        oldTime = time;
        time = getTicks();
        double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
        print(1.0 / frameTime); //FPS counter
        redraw();
#ifdef SDLExample_FlatMapp_h
        cls();
#endif
        
        readKeys();
        //move forward if no wall in front of you
        writeKeysData(player, frameTime);
        
    }
}

void drawColorLine(Player player, int x)
{
    int lineHeight = abs(int(h / player.perpWallDist));
    
    //calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + h / 2;
    if(drawStart < 0)drawStart = 0;
    int drawEnd = lineHeight / 2 + h / 2;
    if(drawEnd >= h)drawEnd = h - 1;
    
    
    
    //draw the pixels of the stripe as a vertical line
#ifdef SDLExample_FlatMapp_h
    
    //choose wall color
    ColorRGB color = getColorRGBFromMapBlock(worldMap[player.map.x][player.map.y]);
    //give x and y sides different brightness
    if (player.side == 1) {color = color / 2;}
    
    verLine(x, drawStart, drawEnd, color);
#endif
#ifdef USETEXTURE
    drawTextures(player, drawStart, drawEnd, lineHeight, x);
#endif
}

ColorRGB getColorRGBFromMapBlock(int block)
{
    ColorRGB color;
    switch(block)
    {
        case 1:  color = RGB_Red;  break; //red
        case 2:  color = RGB_Green;  break; //green
        case 3:  color = RGB_Blue;   break; //blue
        case 4:  color = RGB_White;  break; //white
        default: color = RGB_Yellow; break; //yellow
    }
    return color;
}

void writeKeysData(Player &player, double frameTime)
{
    //speed modifiers
    double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 3.0; //the constant value is in radians/second

    if (keyDown(SDL_SCANCODE_UP))
    {
        if(worldMap[int(player.position.x + player.direction.x * moveSpeed)][int(player.position.y)] == false) player.position.x += player.direction.x * moveSpeed;
        if(worldMap[int(player.position.x)][int(player.position.y + player.direction.y * moveSpeed)] == false) player.position.y += player.direction.y * moveSpeed;
    }
    //move backwards if no wall behind you
    if (keyDown(SDL_SCANCODE_DOWN))
    {
        if(worldMap[int(player.position.x + player.direction.x * moveSpeed)][int(player.position.y)] == false) player.position.x -= player.direction.x * moveSpeed;
        if(worldMap[int(player.position.x)][int(player.position.y + player.direction.y * moveSpeed)] == false) player.position.y -= player.direction.y * moveSpeed;
    }
    //rotate to the right
    if (keyDown(SDL_SCANCODE_RIGHT))
    {
        //both camera direction and camera plane must be rotated
        rotate(player.direction, -rotSpeed);
        rotate(player.cameraPlane, -rotSpeed);
    }
    //rotate to the left
    if (keyDown(SDL_SCANCODE_LEFT))
    {
        rotate(player.direction, rotSpeed);
        rotate(player.cameraPlane, rotSpeed);
        //both camera direction and camera plane must be rotated
    }
}

void generateTextures()
{
#ifdef USETEXTURE
    
    if (!USEPNG) {
        //generate some textures
        for(int x = 0; x < texWidth; x++)
            for(int y = 0; y < texHeight; y++)
            {
                int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
                //int xcolor = x * 256 / texWidth;
                int ycolor = y * 256 / texHeight;
                int xycolor = y * 128 / texHeight + x * 128 / texWidth;
                texture[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
                texture[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
                texture[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
                texture[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
                texture[4][texWidth * y + x] = 256 * xorcolor; //xor green
                texture[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
                texture[6][texWidth * y + x] = 65536 * ycolor; //red gradient
                texture[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
            }

    }
    else {
        unsigned long tw, th, error = 0;
        error |= loadImage(texture[0], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/eagle.png");
        error |= loadImage(texture[1], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/redbrick.png");
        error |= loadImage(texture[2], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/purplestone.png");
        error |= loadImage(texture[3], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/greystone.png");
        error |= loadImage(texture[4], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/bluestone.png");
        error |= loadImage(texture[5], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/mossy.png");
        error |= loadImage(texture[6], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/wood.png");
        error |= loadImage(texture[7], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/colorstone.png");
        
#ifdef SPRITES
        error |= loadImage(texture2[0], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/barrel.png");
        error |= loadImage(texture2[1], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/pillar.png");
        error |= loadImage(texture2[2], tw, th, "/Users/werewolf2188/Proyectos/Mobile/SDLExample/SDLExample/pics/greenlight.png");
#endif
        
        if(error) { std::cout << "error loading images" << std::endl; exit(1); }
    }
    
    
#endif
}

void drawTextures(Player &player, int drawStart, int drawEnd, int lineHeight, int x)
{
    int texNum = worldMap[player.map.x][player.map.y] - 1; //1 subtracted from it so that texture 0 can be used!
    
    //calculate value of wallX
    double wallX; //where exactly the wall was hit
    if (player.side == 1) wallX = player.rayPosition.x + ((player.map.y - player.rayPosition.y + (1 - player.step.y) / 2) / player.rayDirection.y) * player.rayDirection.x;
    else       wallX = player.rayPosition.y + ((player.map.x - player.rayPosition.x + (1 - player.step.x) / 2) / player.rayDirection.x) * player.rayDirection.y;
    wallX -= floor((wallX));
    
    //x coordinate on the texture
    int texX = int(wallX * double(texWidth));
    if(player.side == 0 && player.rayDirection.x > 0) texX = texWidth - texX - 1;
    if(player.side == 1 && player.rayDirection.y < 0) texX = texWidth - texX - 1;
    
    for(int y = drawStart; y<drawEnd; y++)
    {
        int d = y * 256 - h * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
        int texY = ((d * texHeight) / lineHeight) / 256;
        Uint32 color = texture[texNum][texHeight * texY + texX];
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if(player.side == 1) color = (color >> 1) & 8355711;
        buffer[x][y] = color;
    }
    
#ifdef USEFLOORCASTING
    floorCasting(player, drawStart, drawEnd, lineHeight, x, wallX);
#ifdef SPRITES
    spriteCasting(player, buffer[0], screenHeight);
#endif
    
#endif
}

void floorCasting(Player &player, int drawStart, int &drawEnd, int lineHeight, int x, double wallX)
{
    
#ifdef SPRITES
    ZBuffer[x] = player.perpWallDist; //perpendicular distance is used
#endif
    //FLOOR CASTING
    double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall
    
    //4 different wall directions possible
    if(player.side == 0 && player.rayDirection.x > 0)
    {
        floorXWall = player.map.x;
        floorYWall = player.map.y + wallX;
    }
    else if(player.side == 0 && player.rayDirection.x < 0)
    {
        floorXWall = player.map.x + 1.0;
        floorYWall = player.map.y + wallX;
    }
    else if(player.side == 1 && player.rayDirection.y > 0)
    {
        floorXWall = player.map.x + wallX;
        floorYWall = player.map.y;
    }
    else
    {
        floorXWall = player.map.x + wallX;
        floorYWall = player.map.y + 1.0;
    }
    
    double distWall, distPlayer, currentDist;
    
    distWall = player.perpWallDist;
    distPlayer = 0.0;
    if (drawEnd < 0) drawEnd = h; //becomes < 0 when the integer overflows
    //draw the floor from drawEnd to the bottom of the screen
    for(int y = drawEnd + 1; y < h; y++)
    {
        currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead
        
        double weight = (currentDist - distPlayer) / (distWall - distPlayer);
        
        double currentFloorX = weight * floorXWall + (1.0 - weight) * player.position.x;
        double currentFloorY = weight * floorYWall + (1.0 - weight) * player.position.y;
        
        int floorTexX, floorTexY;
        floorTexX = int(currentFloorX * texWidth) % texWidth;
        floorTexY = int(currentFloorY * texHeight) % texHeight;
        
        int floorTexture = 3;
        
//        int checkerBoardPattern = (int(currentFloorX) + int(currentFloorY)) % 2;
//        int checkerBoardPattern = (int(currentFloorX + currentFloorY)) % 2;
//        if(checkerBoardPattern == 0) floorTexture = 3;
//        else floorTexture = 4;
        
        //floor
        buffer[x][y] = (texture[floorTexture][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
        //ceiling (symmetrical!)
        buffer[x][h - y] = texture[6][texWidth * floorTexY + floorTexX];
    }
}


