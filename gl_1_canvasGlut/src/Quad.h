#ifndef QUAD_H_INCLUDED
#define QUAD_H_INCLUDED

#include "gl_canvas2d.h"

class Quad {
    int x0, y0, xf, yf;

public:
    Quad(int x0, int y0, int xf, int yf)
    {
        this->x0 = x0;
        this->y0 = y0;
        this->xf = xf;
        this->yf = yf;
    }

    void Render()
    {
        color(1, 1, 1);
        rectFill(x0, y0, xf, yf);
    }
};

#endif // QUAD_H_INCLUDED
