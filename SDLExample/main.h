//
//  main.h
//  SDLExample
//
//  Created by Enrique on 15/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#ifndef __SDLExample__main__
#define __SDLExample__main__

#include <stdio.h>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "Vector2D.h"
#include "Player.h"
#include "Sprite.h"
#include "TextureConstants.h"

#include "quickcg.h"
using namespace QuickCG;

void drawColorLine(Player player, int x);
ColorRGB getColorRGBFromMapBlock(int block);
void writeKeysData(Player &player, double frameTime);
void generateTextures();
void drawTextures(Player &player, int drawStart, int drawEnd, int lineHeight, int x);
void floorCasting(Player &player, int drawStart, int &drawEnd, int lineHeight, int x, double wallX);

#endif /* defined(__SDLExample__main__) */
