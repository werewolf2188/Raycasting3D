//
//  Vector2D.h
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#ifndef __SDLExample__Vector2D__
#define __SDLExample__Vector2D__

#include <stdio.h>

struct Vector2DD
{
    double x;
    double y;
};

struct Vector2DI
{
    int x;
    int y;
};

Vector2DD createVector2DD(double x, double y);
Vector2DD createVector2DD();
double length(const Vector2DD& v);
Vector2DD normalize(const Vector2DD& v);
double distance(const Vector2DD& v, const Vector2DD& w);
double dot(const Vector2DD& v, const Vector2DD& w);
void rotate(Vector2DD &v, double angle);

Vector2DI createVector2DI(int x, int y);
Vector2DI createVector2DI();
double length(const Vector2DI& v);
Vector2DI normalize(const Vector2DI& v);
double distance(const Vector2DI& v, const Vector2DI& w);
double dot(const Vector2DI& v, const Vector2DI& w);
void rotate(Vector2DI &v, double angle);

#endif /* defined(__SDLExample__Vector2D__) */
