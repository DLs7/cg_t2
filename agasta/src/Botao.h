#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

class Botao{
  // Armazena a altura e a largura do bot�o, assim como booleanos que identificam a cor do mesmo.
  // Tamb�m armazena o seu label.
  float altura, largura, x, y, r, g, b, textPos;
  char label[20];

public:
  // Cria um bot�o dado uma largura, uma altura, um label e booleanos r, g, b que definem a cor.
  // A posi��o do bot�o n�o � dada, uma vez que a posi��o dele constantemente atualiza.
  // Essa posi��o � passada no m�todo Render.
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

  // Renderiza o bot�o.
  void Render()
  {
      color(r, g, b);
      rectFill(x, y, x + largura, y + altura);

      color(0, 0, 0);
      text(x + largura/textPos, y + altura/2.5, (const char*)label);
  }

  // Recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse est� em cima do bot�o.
  bool Colidiu(int mx, int my)
  {
      if( mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura) )
      {
          return true;
      }
      return false;
  }

  // Getter de bot�o.
  int GetButtonX() { return x; }

  // Setter de bot�o.
  void SetButtonX(int mx) { x = mx; }
};

#endif
