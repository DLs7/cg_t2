/***************************************************************************************
// TRABALHO B - AUGUSTO GAI DAL'ASTA (MATR. 201810291)
//
// Este trabalho conta com as seguintes funcionalidades essenciais:
// > Inserção e exclusão de figura;
// > Edição da cor da figura;
// > Edição do preenchimento da figura;
// > Edição do tamanho da figura;
// > Enviar a figura para frente e para trás;
// > Salvar cena em arquivo.
//
// Este trabalho também conta com as seguintes funcionalidades adicionais:
// > Possibilidade de criação de diversas formas geométricas (renderiza-se um polígono a
// partir do número de vértices definido.
// > Possibilidade de edição de uma figura já criada;
// > Sliders para gerenciamento da cor da figura.
//
// INSTRUÇÕES:
// > PARA CRIAR UMA FIGURA:
//   >> Botão esquerdo do mouse (click) -> Cria uma figura dados os parâmetros contidos
//   na HUD na posição do curso.
//   >> Botão esquerdo do mouse (hold) -> Arrasta a figura que colidir com a posição
//   do cursor.
//   >> Botão direito do mouse -> Deleta a figura que o cursor estiver colidindo;
//
// > PARA EDITAR CORES/ROTAÇÃO/TAMANHO/CENA
//   >> Sliders R, G, B -> Mudam o valor RGB da figura selecionada. (0 - 1)
//   >> Slider @ -> Muda o ângulo de rotação da figura selecionada. (0 - 360)
//   >> Slider W -> Muda o tamanho da figura selecionada. (10 - 210)
//   >> Select V -> Muda a quantidade de vértices da figura selecionada. (3 - 360)
//   >> Botão MOVE UP -> Move a figura selecionada uma posição para cima.
//   >> Botão MOVE DOWN -> Move a figura selecionada uma posição para baixo.
//   >> Botão FILL/UNFILL -> Edita o preenchimento da figura selecionada.
//   >> Botão CLEAR SCENE -> Remove todas as figuras da cena.
//   >> Botão EXIT -> Sai do programa.
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

// Variáveis que indicam:
int slider_drag = -1; // Qual slider foi selecionado.
bool shape_drag = false; // Se o usuário está arrastando a forma selecionada.

// HUD do programa
Hud *hud = NULL;

// Lista de formas que são apresentadas na cena.
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

// Função chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

// Função chamada toda vez que uma tecla for liberada.
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

// Cria uma forma na posição do cursor dado os parâmetros da HUD.
// Desseleciona todas as formas da lista e seleciona a criada.
void createShape(int x, int y) {
    for(auto shape : shapeList) shape->Selected(false);

    shapeList.push_back(new Shape(x, y, hud->GetVertices(),
                         hud->GetWidth(), hud->GetAngle(),
                         hud->GetR(), hud->GetG(), hud->GetB(),
                         hud->GetFill()));
}


// Deleta uma forma. A lista percorre em ordem reversa para que o usuário
// não possa deletar uma forma que está embaixo de outra.
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

// Função para tratamento de mouse: cliques, movimentos e arrastos.
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    // Caso o botão esquerdo for clicado.
    if(button == 0) {
        // Se sim, checa se colidiu com uma figura já existente.
        if(state == 0) {
            bool c = checkShapeCollision(x, y);
            // Se não colidiu e o x do mouse encontra-se na cena, cria uma figura na
            // cena.
            if (!c && x < hud->GetHudSize()) {
                createShape(x, y);
            // Se não, checa se o usuário clicou na HUD.
            } else if (!c && x > hud->GetHudSize()) {
                checkHudUpdates(x, y);
            }
        // Se o botão 0 for solto, desabilita o arrasto de figuras e sliders.
        } else if(state == 1) {
            disableDrag();
        }
    // Caso o botão direito for clicado.
    } else if (button == 2) {
        // Verifica se colidiu em uma forma pra deletar ela.
        if(state == 0) {
            deleteShape(x, y);
        }
    // Se o usuário estiver segurando o botão direito.
    } else if(button == -2) {
        dragStuff(x, y);
    }
}

// Main.
int main(void)
{
    // Cria a HUD.
    hud = new Hud(screenWidth - 320, screenWidth, screenHeight);

    // Lê o arquivo "figures.gr" na pasta "resources". Abre ele toda vez que o
    // programa inicia (caso ele exista).
    shapeList = hud->ReadFile();

    // Inicia a Canvas2D.
    initCanvas(&screenWidth, &screenHeight, "TB - Augusto Gai Dal'Asta");
    runCanvas();
}
