#ifndef SELECT_H
#define SELECT_H


class Select
{
    // Variáveis de select.
    int x, y, width, height;
    int min, current, max; // Armazena o valor mínimo, atual e máximo desse slider.
    float r, g, b;

    Botao *button_minus, *button_plus; // Botões que aumentam ou diminuem o valor atual

public:
    // Constructor do Select.
    Select(int x, int y, int width, int height, float r, float g, float b, int min, int max)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        this->r = r;
        this->g = g;
        this->b = b;

        this->min = min;
        current = 4;
        this->max = max;

        // Temos 2 botões, um para aumentar e outro para diminuir o valor atual.
        button_minus = new Botao(x, y, width, height, r, g, b, (char*)"-", 2.5);
        button_plus = new Botao(x + 7 * width, y, width, height, r, g, b, (char*)"+", 2.5);
    }

    // Renderiza o Select.
    void Render()
    {
        button_minus->Render();
        button_plus->Render();

        color(0, 0, 0);
        rectFill(x + 2 * width, y, x + 6 * width, y + height);

        RenderText();
    }

    // Renderiza o texto mostrando o valor atual do Select.
    void RenderText()
    {
        char b[7];

        if(current < 10) sprintf(b, "V = 00%d", current);
        else if(current < 100) sprintf(b, "V = 0%d", current);
        else if(current < 1000) sprintf(b, "V = %d", current);

        color(1, 1, 1);
        text(x + 3 * width, y + height/2.5, b);
    }

    // Retorna o valor + 1 ou valor - 1 dependendo do botão clicado.
    int Colidiu(int mx, int my)
    {
        if(button_minus->Colidiu(mx, my) && current > min) {
            current--;
            return current;
        }
        if(button_plus->Colidiu(mx, my) && current < max) {
            current++;
            return current;
        }
        return current;
    }

    // Setter de valor do Select.
    void SetCurrent(int current) { this->current = current; }
};

#endif // SELECT_H
