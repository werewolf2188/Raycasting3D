//
//  Sprite.h
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#ifndef __SDLExample__Sprite__
#define __SDLExample__Sprite__

#include <stdio.h>
#include <vector>

struct Sprite
{
    double x;
    double y;
    int texture;
};

//function used to sort the sprites
void combSort(int* order, double* dist, int amount);

//1D Zbuffer


//arrays used to sort the sprites




#endif /* defined(__SDLExample__Sprite__) */
