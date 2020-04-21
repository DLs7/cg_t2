#ifndef SLIDER_H
#define SLIDER_H

#include "gl_canvas2d.h"
#include "Label.h"

class Slider
{
    // Variáveis de slider.
    int x, y, width, height;
    float r, g, b;
    char labelTag[1];
    float min, current, max; // Armazena o valor mínimo, atual e máximo desse slider.

    Botao *slide_button;

    int offset_x, offset_y;
    int min_graph, max_graph, graph_width;

public:
    // Constructor do slider.
    Slider(int x, int y, int width, int height, float r, float g, float b, char *labelTag, float min, float max)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        offset_x = 1.5 * width;
        offset_y = 0.5 * height;

        graph_width = 200;
        min_graph = x + offset_x - width/4;
        max_graph = min_graph + graph_width;

        this->slide_button = new Botao(min_graph, y, width/2, height, 0, 0, 0, (char*)"", 1);

        this->r = r;
        this->g = g;
        this->b = b;

        strcpy(this->labelTag, labelTag);

        this->min = min;
        current = 0;
        this->max = max;
    }

    // Renderiza o slider.
    void Render()
    {
        color(r, g, b);
        rectFill(x, y, x + width, y + height);

        color(0, 0, 0);
        text(x + width/2.5, y + height/2.5, (const char*)labelTag);

        RenderLine();

        slide_button->Render();
    }

    // Renderiza a linha do slider e os marcadores dela.
    void RenderLine()
    {
        color(0, 0, 0);
        rectFill(min_graph + width/4, y + offset_y - 1, max_graph + width/4, y + offset_y + 1);

        for(int i = 0; i <= 8; i++) {
            int size = (max_graph - min_graph)/8;
            rectFill(min_graph + width/4 + (i * size) - 1, y, min_graph + width/4 + (i * size) + 1, y + 16);
        }
    }

    // Verifica se o cursor colidiu com o slider.
    bool Colidiu(int mx, int my)
    {
        if(slide_button->Colidiu(mx, my))
            return true;
        return false;
    }

    // Altera a posição do slider caso o mouse esteja arrastando ele.
    float SetButtonX(int mx)
    {
        if(mx < min_graph) slide_button->SetButtonX(min_graph);
        else if(mx > max_graph) slide_button->SetButtonX(max_graph);
        else slide_button->SetButtonX(mx);

        return AdjustVariable();
    }

    // Arruma a posição do slider para novos parâmetros de HUD de uma nova forma selecionada.
    void Update(float current)
    {
        this->current = current;
        float graph100 = max_graph - min_graph;

        slide_button->SetButtonX(min_graph + ((current - min) * graph100/max));
    }

    // Arruma a variável que estamos alterando para diferentes posições do botão no slider.
    float AdjustVariable()
    {
        float graph100 = max_graph - min_graph;
        float current_x = slide_button->GetButtonX() - min_graph;

        current = min + (max * current_x/graph100);
        return current;
    }
};

#endif // SLIDER_H
