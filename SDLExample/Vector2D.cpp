//
//  Vector2D.cpp
//  SDLExample
//
//  Created by Enrique on 17/12/14.
//  Copyright (c) 2014 Enrique. All rights reserved.
//

#include "Vector2D.h"
#include <cmath>

Vector2DD createVector2DD(double x, double y)
{
    Vector2DD vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

Vector2DD createVector2DD()
{
    Vector2DD vector;
    vector.x = 0;
    vector.y = 0;
    return vector;
}

double length(const Vector2DD& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector2DD normalize(const Vector2DD& v)
{
    Vector2DD vector;
    double l = length(v);
    if(l != 0)
    {
        vector.x /= l;
        vector.y /= l;
        
    }
    return vector;
}

double distance(const Vector2DD& v, const Vector2DD& w)
{
    return sqrt((v.x - w.x) * (v.x - w.x) + (v.y - w.y) * (v.y - w.y));
}

double dot(const Vector2DD& v, const Vector2DD& w)
{
    return (w.x * v.x + w.y * v.y);
}

void rotate(Vector2DD &v, double angle)
{
    double oldDirX = v.x;
    v.x = v.x * cos(angle) - v.y * sin(angle);
    v.y = oldDirX * sin(angle) + v.y * cos(angle);
}

///
Vector2DI createVector2DI(int x, int y)
{
    Vector2DI vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

Vector2DI createVector2DI()
{
    Vector2DI vector;
    vector.x = 0;
    vector.y = 0;
    return vector;
}

double length(const Vector2DI& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector2DI normalize(const Vector2DI& v)
{
    Vector2DI vector;
    int l = length(v);
    if(l != 0)
    {
        vector.x /= l;
        vector.y /= l;
        
    }
    return vector;
}

double distance(const Vector2DI& v, const Vector2DI& w)
{
    return sqrt((v.x - w.x) * (v.x - w.x) + (v.y - w.y) * (v.y - w.y));
}

double dot(const Vector2DI& v, const Vector2DI& w)
{
    return (w.x * v.x + w.y * v.y);
}

void rotate(Vector2DI &v, double angle)
{
    double oldDirX = v.x;
    v.x = v.x * cos(angle) - v.y * sin(angle);
    v.y = oldDirX * sin(angle) + v.y * cos(angle);
}