#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#define PI 3.14159265358979323846

#include "gl_canvas2d.h"
#include <fstream>

// Classe respons�vel pela elabora��o de todas as figuras da cena.
class Shape {

    // Par�metros da Shape.
    float x, y, width, angle;
    float *vx, *vy; // Esses vetores ser�o usados para o polygon.
    float r, g, b;
    int vertices;

    bool selected, fill;

public:
    // Constructor vazio usado somente para o carregamento de arquivo.
    Shape(){
        selected = false;
    }

    // Full-fledged constructor.
    Shape(float x, float y, int vertices, int width, float angle, float r, float g, float b, bool fill)
    {
        // A posi��o � dada de acordo com a posi��o do cursor.
        this->x = x;
        this->y = y;

        // Todos os outros par�metros s�o dados de acordo com o que se tem na HUD.
        this->width = width;
        this->angle = angle;

        this->vertices = vertices;
        // Aloca o vx e o vy de acordo com a quantidade de v�rtices que a figura tem.
        vx = new float[vertices];
        vy = new float[vertices];

        this->r = r;
        this->g = g;
        this->b = b;

        // A figura � selecionada por default.
        selected = true;
        this->fill = fill;

        // "Constr�i" o vx e o vy.
        Build();
    }

    void Build()
    {
        // Calcula as posi��es de vx e vy para bolar a figura.
        for(int i = 0; i < vertices; i++) {
            vx[i] = width * cos(2 * PI * i/vertices) + x;
            vy[i] = width * sin(2 * PI * i/vertices) + y;
        }

        // Altera cada �ndice desse vetor (menos a origem) dependendo da rota��o.
        Rotate();
    }

    // Renderiza a figura
    void Render()
    {
        color(r, g, b);
        if(fill) polygonFill(vx, vy, vertices); // Caso o usu�rio queira ela preenchida
        else polygon(vx, vy, vertices); // Caso n�o
    }

    // Rotaciona a figura
    void Rotate()
    {
        float rad = angle * PI/180;

        // Percorre todo o vetor novamente e aplica a devida rota��o por matriz de rota��o.
        for(int i = 0; i < vertices; i++) {
            float aux_x = vx[i];
            vx[i] = (x - vx[i])*cos(rad) - (y - vy[i])*sin(rad) + x;
            vy[i] = (x - aux_x)*sin(rad) + (y - vy[i])*cos(rad) + y;
        }
    }

    // Verifica se o cursor colidiu com a figura.
    bool Colidiu(int mx, int my)
    {
        int i, j;
        bool c = false;

        for (i = 0, j = vertices - 1; i < vertices; j = i++) {
            if (((vy[i] > my) != (vy[j] > my)) &&
                (mx < (vx[j] - vx[i]) * (my - vy[i])/(vy[j] - vy[i]) + vx[i]))
            c = !c;
        }

        return c;
    }

    // Usado para mudar a posi��o da figura caso o usu�rio esteja arrastando ela.
    void UpdatePos(int mx, int my) {
        this->x = mx;
        this->y = my;

        Build();
    }

    // L�/Escreve no arquivo.
    void Serialize(bool bWrite, std::fstream &fs)
    {
        // Escrita:
        if (bWrite) {
            fs.write(reinterpret_cast<char*>(&x), sizeof(x));
            fs.write(reinterpret_cast<char*>(&y), sizeof(y));
            fs.write(reinterpret_cast<char*>(&vertices), sizeof(vertices));
            fs.write(reinterpret_cast<char*>(&width), sizeof(width));
            fs.write(reinterpret_cast<char*>(&angle), sizeof(angle));
            fs.write(reinterpret_cast<char*>(&vx[0]), sizeof(int)*vertices);
            fs.write(reinterpret_cast<char*>(&vy[0]), sizeof(int)*vertices);
            fs.write(reinterpret_cast<char*>(&r), sizeof(r));
            fs.write(reinterpret_cast<char*>(&g), sizeof(g));
            fs.write(reinterpret_cast<char*>(&b), sizeof(b));
            fs.write(reinterpret_cast<char*>(&fill), sizeof(fill));
        // Leitura:
        } else {
            fs.read(reinterpret_cast<char*>(&x), sizeof(x));
            fs.read(reinterpret_cast<char*>(&y), sizeof(y));
            fs.read(reinterpret_cast<char*>(&vertices), sizeof(vertices));
            fs.read(reinterpret_cast<char*>(&width), sizeof(width));
            fs.read(reinterpret_cast<char*>(&angle), sizeof(angle));

            // Aloca vx e vy dado o n�mero de v�rtices.
            vx = (float*)malloc(sizeof(float) * vertices);
            vy = (float*)malloc(sizeof(float) * vertices);

            fs.read(reinterpret_cast<char*>(&vx[0]), sizeof(int)*vertices);
            fs.read(reinterpret_cast<char*>(&vy[0]), sizeof(int)*vertices);
            fs.read(reinterpret_cast<char*>(&r), sizeof(r));
            fs.read(reinterpret_cast<char*>(&g), sizeof(g));
            fs.read(reinterpret_cast<char*>(&b), sizeof(b));
            fs.read(reinterpret_cast<char*>(&fill), sizeof(fill));
        }
    }

    // Getters de Shape
    float GetR() { return r; }
    float GetG() { return g; }
    float GetB() { return b; }
    float GetAngle() { return angle; }
    float GetWidth() { return width; }
    int GetVertices() { return vertices; }
    bool GetFill() { return fill; }

    // Setters de Shape
    void SetR(float r) { this->r = r; }
    void SetG(float g) { this->g = g; }
    void SetB(float b) { this->b = b; }
    void SetAngle(float angle) { this->angle = angle; }
    void SetWidth(float width) { this->width = width; }
    void SetVertices(int vertices) {
        // Caso aumente o n�mero de v�rtices, realoca o vetor vx e vy e reconstr�i eles.
        this->vertices = vertices;
        free(vx);
        free(vy);

        vx = new float[vertices];
        vy = new float[vertices];
    }
    void SetFill(bool fill) { this->fill = fill; }

    // Seta se a forma � selecionada.
    void Selected(bool selected) { this->selected = selected; }

    // Retorna true caso seja a selecionada.
    bool IsSelected() { return this->selected; }
};

#endif // SHAPE_H_INCLUDED
