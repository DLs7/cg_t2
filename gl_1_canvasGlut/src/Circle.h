#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include "gl_canvas2d.h"

class Circle {
    int x, y, r;

public:
    Circle(int x, int y, int r)
    {
        this->x = x;
        this->y = y;
        this->r = r;
    }

    void Render()
    {
        color(1, 1, 1);
        circleFill(x, y, r, 100);
    }
};

#endif // CIRCLE_H_INCLUDED
