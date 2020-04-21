#ifndef LABEL_H
#define LABEL_H


class Label
{
    int x, y, width, height;
    float r, g, b;
    char labelTag[1];

public:
    Label(int x, int y ,int width, int height, float r, float g, float b, char *labelTag)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        this->r = r;
        this->g = g;
        this->b = b;

        strcpy(this->labelTag, labelTag);
    }

    void Render()
    {
        color(r, g, b);
        rectFill(x, y, x + width, y + height);

        color(0, 0, 0);
        text(x + width/2.5, y + height/2.5, (const char*)labelTag);
    }
};

#endif // LABEL_H
