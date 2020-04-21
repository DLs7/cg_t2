/***************************************************************************************
// TRABALHO B - AUGUSTO GAI DAL'ASTA (MATR. 201810291)
//
// Este trabalho conta com as seguintes funcionalidades essenciais:
// > Inser��o e exclus�o de figura;
// > Edi��o da cor da figura;
// > Edi��o do preenchimento da figura;
// > Edi��o do tamanho da figura;
// > Enviar a figura para frente e para tr�s;
// > Salvar cena em arquivo.
//
// Este trabalho tamb�m conta com as seguintes funcionalidades adicionais:
// > Possibilidade de cria��o de diversas formas geom�tricas (renderiza-se um pol�gono a
// partir do n�mero de v�rtices definido.
// > Possibilidade de edi��o de uma figura j� criada;
// > Sliders para gerenciamento da cor da figura.
//
// INSTRU��ES:
// > PARA CRIAR UMA FIGURA:
//   >> Bot�o esquerdo do mouse (click) -> Cria uma figura dados os par�metros contidos
//   na HUD na posi��o do curso.
//   >> Bot�o esquerdo do mouse (hold) -> Arrasta a figura que colidir com a posi��o
//   do cursor.
//   >> Bot�o direito do mouse -> Deleta a figura que o cursor estiver colidindo;
//
// > PARA EDITAR CORES/ROTA��O/TAMANHO/CENA
//   >> Sliders R, G, B -> Mudam o valor RGB da figura selecionada. (0 - 1)
//   >> Slider @ -> Muda o �ngulo de rota��o da figura selecionada. (0 - 360)
//   >> Slider W -> Muda o tamanho da figura selecionada. (10 - 210)
//   >> Select V -> Muda a quantidade de v�rtices da figura selecionada. (3 - 360)
//   >> Bot�o MOVE UP -> Move a figura selecionada uma posi��o para cima.
//   >> Bot�o MOVE DOWN -> Move a figura selecionada uma posi��o para baixo.
//   >> Bot�o FILL/UNFILL -> Edita o preenchimento da figura selecionada.
//   >> Bot�o CLEAR SCENE -> Remove todas as figuras da cena.
//   >> Bot�o EXIT -> Sai do programa.
//
****************************************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <stdio.h>
#include <list>

#include "gl_canvas2d.h"
#include "Shape.h"
#include "Hud.h"

// Largura e altura da tela.
int screenWidth = 1280, screenHeight = 720;

// Vari�veis que indicam:
int slider_drag = -1; // Qual slider foi selecionado.
bool shape_drag = false; // Se o usu�rio est� arrastando a forma selecionada.

// HUD do programa
Hud *hud = NULL;

// Lista de formas que s�o apresentadas na cena.
std::list<Shape*> shapeList;

// Render
void render()
{
    clear(1, 1, 1);

    // Renderiza a lista de shapes e atualiza a shape selecionada.
    for(auto shape : shapeList) {
        if(shape->IsSelected()) hud->UpdateShape(shape);
        shape->Render();
    }

    // Renderiza a HUD.
    hud->Render();
}

// Fun��o chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

// Fun��o chamada toda vez que uma tecla for liberada.
void keyboardUp(int key)
{
}

// Retorna true se o mouse colidir com uma shape. Isso impede que uma figura seja
// criada por cima de outra.
bool checkShapeCollision(int x, int y) {
    for(auto shape : shapeList) {
        if (shape->Colidiu(x, y)) {
            hud->UpdateHud(shape, shapeList);

            shape_drag = true;
            return true;
        }
    }
    return false;
}

// Checa se algo da HUD foi usado e faz o devido tratamento.
void checkHudUpdates(int x, int y) {
    shapeList = hud->CollidedButtons(x, y, shapeList);
    slider_drag = hud->CollidedSliders(x, y);
    hud->CollidedSelects(x, y);
}

// Cria uma forma na posi��o do cursor dado os par�metros da HUD.
// Desseleciona todas as formas da lista e seleciona a criada.
void createShape(int x, int y) {
    for(auto shape : shapeList) shape->Selected(false);

    shapeList.push_back(new Shape(x, y, hud->GetVertices(),
                         hud->GetWidth(), hud->GetAngle(),
                         hud->GetR(), hud->GetG(), hud->GetB(),
                         hud->GetFill()));
}


// Deleta uma forma. A lista percorre em ordem reversa para que o usu�rio
// n�o possa deletar uma forma que est� embaixo de outra.
void deleteShape(int x, int y) {
    for(std::list<Shape*>::reverse_iterator it = shapeList.rbegin();
    it != shapeList.rend(); it++) {
        if((*it)->Colidiu(x, y)) {
            shapeList.remove(*it);
            break;
        }
    }
}

// Desabilita o arrasto de figuras e sliders.
void disableDrag() {
    slider_drag = -1;
    shape_drag = false;
}

// Trata o arrasto de figuras e sliders.
void dragStuff(int x, int y) {
    hud->MouseSliders(x, slider_drag);

    if(shape_drag && x < hud->GetHudSize()) {
        for(auto shape : shapeList) {
            if(shape->IsSelected()) {
                shape->UpdatePos(x, y);
            }
        }
    }
}

// Fun��o para tratamento de mouse: cliques, movimentos e arrastos.
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    // Caso o bot�o esquerdo for clicado.
    if(button == 0) {
        // Se sim, checa se colidiu com uma figura j� existente.
        if(state == 0) {
            bool c = checkShapeCollision(x, y);
            // Se n�o colidiu e o x do mouse encontra-se na cena, cria uma figura na
            // cena.
            if (!c && x < hud->GetHudSize()) {
                createShape(x, y);
            // Se n�o, checa se o usu�rio clicou na HUD.
            } else if (!c && x > hud->GetHudSize()) {
                checkHudUpdates(x, y);
            }
        // Se o bot�o 0 for solto, desabilita o arrasto de figuras e sliders.
        } else if(state == 1) {
            disableDrag();
        }
    // Caso o bot�o direito for clicado.
    } else if (button == 2) {
        // Verifica se colidiu em uma forma pra deletar ela.
        if(state == 0) {
            deleteShape(x, y);
        }
    // Se o usu�rio estiver segurando o bot�o direito.
    } else if(button == -2) {
        dragStuff(x, y);
    }
}

// Main.
int main(void)
{
    // Cria a HUD.
    hud = new Hud(screenWidth - 320, screenWidth, screenHeight);

    // L� o arquivo "figures.gr" na pasta "resources". Abre ele toda vez que o
    // programa inicia (caso ele exista).
    shapeList = hud->ReadFile();

    // Inicia a Canvas2D.
    initCanvas(&screenWidth, &screenHeight, "TB - Augusto Gai Dal'Asta");
    runCanvas();
}
