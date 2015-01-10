//
//  Sprite.c
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#include <cmath>
#include <string>

#include <iostream>
#include "Sprite.h"

//sort algorithm
void combSort(int* order, double* dist, int amount)
{
    int gap = amount;
    bool swapped = false;
    while(gap > 1 || swapped)
    {
        //shrink factor 1.3
        gap = (gap * 10) / 13;
        if(gap == 9 || gap == 10) gap = 11;
        if (gap < 1) gap = 1;
        swapped = false;
        for (int i = 0; i < amount - gap; i++)
        {
            int j = i + gap;
            if (dist[i] < dist[j])
            {
                std::swap(dist[i], dist[j]);
                std::swap(order[i], order[j]);
                swapped = true;
            }
        }
    }
}