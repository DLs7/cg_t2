/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         03/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <stdio.h>
#include <list>

#include "gl_canvas2d.h"

#include "Circle.h"
#include "Quad.h"
#include "Botao.h"

int screenWidth = 500, screenHeight = 500; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

int option = 0;

std::list<Quad*> quadList;
std::list<Circle*> circleList;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    clear(0, 0, 0);

    for(auto quad : quadList) {
        quad->Render();
    }

    for(auto circle : circleList) {
        circle->Render();
    }
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    option = key;
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if(state == 0) {
        if(option == 49) {
          quadList.push_back(new Quad(x, y, x + 10, y + 10));
          printf("\nCriou");
        }
        if(option == 50) {
          circleList.push_back(new Circle(x, y, 5));
          printf("\nCriou");
        }
   }
}

int main(void)
{
   initCanvas(&screenWidth, &screenHeight, "TB - Augusto Gai Dal'Asta");

   quadList.push_back(new Quad(100, 100, 110, 110));
   circleList.push_back(new Circle(150, 150, 5));

   runCanvas();
}
