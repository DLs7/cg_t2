#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

class Botao{
  // Armazena a altura e a largura do botão, assim como booleanos que identificam a cor do mesmo.
  // Também armazena o seu label.
  float altura, largura, x, y, r, g, b, textPos;
  char label[20];

public:
  // Cria um botão dado uma largura, uma altura, um label e booleanos r, g, b que definem a cor.
  // A posição do botão não é dada, uma vez que a posição dele constantemente atualiza.
  // Essa posição é passada no método Render.
  Botao(float x, float y, float larg, float alt, float r, float g, float b, char *label, float textPos)
  {
     this->x = x;
     this->y = y;
     this->altura  = alt;
     this->largura = larg;
     this->textPos = textPos;
     strcpy(this->label, label);

     this->r = r;
     this->g = g;
     this->b = b;
  }

  // Renderiza o botão.
  void Render()
  {
      color(r, g, b);
      rectFill(x, y, x + largura, y + altura);

      color(0, 0, 0);
      text(x + largura/textPos, y + altura/2.5, (const char*)label);
  }

  // Recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse está em cima do botão.
  bool Colidiu(int mx, int my)
  {
      if( mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura) )
      {
          return true;
      }
      return false;
  }

  // Getter de botão.
  int GetButtonX() { return x; }

  // Setter de botão.
  void SetButtonX(int mx) { x = mx; }
};

#endif
