#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include <list>

#include "Botao.h"
#include "Select.h"
#include "Slider.h"

#include "gl_canvas2d.h"

// Classe destinada à HUD do programa.
// Funciona como uma espécie de ButtonManager. Serve pra comunicar a
// main com a figura.
class Hud {

    // Posições e tamanho default para bloco.
    int x0, y0, xf, yf;
    int block;

    // Variáveis de figura.
    float r, g, b;
    float angle;
    float vertices;
    float width;
    bool fill;

    // Nome do arquivo a ser aberto/escrito.
    char* filename;

    // Lista de botões.
    std::list<Botao*> bt_list;
        Botao *button_up;
        Botao *button_down;
        Botao *button_load;
        Botao *button_clear;
        Botao *button_fill;
        Botao *button_save;
        Botao *button_exit;

    // Lista de sliders.
    std::list<Slider*> sl_list;
        Slider *slider_r;
        Slider *slider_g;
        Slider *slider_b;
        Slider *slider_angle;
        Slider *slider_width;

    // Lista de selects.
    std::list<Select*> se_list;
        Select *select_vertices;

public:
    // Constructor da classe HUD. Recebe poucos parâmetros e seta os parâ-
    // metros de figura default quando criada.
    Hud(int x0, int screenWidth, int screenHeight)
    {
        this->x0 = x0;
        this->y0 = 0;
        this->xf = screenWidth;
        this->yf = screenHeight;

        block = 32;

        r = g = b = angle = 0;
        vertices = 4;
        width = 10;
        fill = true;

        CreateButtons();
        CreateSliders();
        CreateSelects();

        filename = (char*)".\\agasta\\resources\\shapes.gr";
    }

    // Cria os botões do programa e adiciona eles numa lista.
    void CreateButtons()
    {
        bt_list.push_back(button_up =
                         new Botao(x0 + 5.5 * block, yf - 14 * block, 3.5 * block, block, 1, 1, 1, (char*)"MOVE UP", 5));
        bt_list.push_back(button_down =
                         new Botao(x0 + block, yf - 14 * block, 3.5 * block, block, 1, 1, 1, (char*)"MOVE DOWN", 10));
        bt_list.push_back(button_clear =
                         new Botao(x0 + block, yf - 18 * block, 8 * block, block, 1, 1, 1, (char*)"CLEAR SCENE", 3.5));
        bt_list.push_back(button_fill =
                         new Botao(x0 + block, yf - 16 * block, 8 * block, block, 1, 1, 1, (char*)"FILL/UNFILL", 3.5));
        bt_list.push_back(button_load =
                         new Botao(x0 + block, yf - 20 * block, 3.5 * block, block, 1, 1, 1, (char*)"LOAD SCENE", 15));
        bt_list.push_back(button_save =
                         new Botao(x0 + 5.5 * block, yf - 20 * block, 3.5 * block, block, 1, 1, 1, (char*)"SAVE SCENE", 15));
        bt_list.push_back(button_exit =
                         new Botao(x0 + block, yf - 22 * block, 8 * block, block, 1, 0, 0, (char*)"EXIT", 2.3));
    }

    // Cria os sliders do programa e adiciona eles numa lista.
    void CreateSliders()
    {
        sl_list.push_back(slider_r =
                         new Slider(x0 + block, yf - 2 * block, block, block, 1, 0, 0, (char*)"R", 0, 1));
        sl_list.push_back(slider_g =
                         new Slider(x0 + block, yf - 4 * block, block, block, 0, 1, 0, (char*)"G", 0, 1));
        sl_list.push_back(slider_b =
                         new Slider(x0 + block, yf - 6 * block, block, block, 0, 0, 1, (char*)"B", 0, 1));
        sl_list.push_back(slider_angle =
                         new Slider(x0 + block, yf - 8 * block, block, block, 1, 1, 1, (char*)"@", 0, 360));
        sl_list.push_back(slider_width =
                         new Slider(x0 + block, yf - 10 * block, block, block, 1, 1, 1, (char*)"W", 10, 210));
    }

    // Cria os selects do programa e adiciona eles numa lista.
    void CreateSelects()
    {
        se_list.push_back(select_vertices =
                          new Select(x0 + block, yf - 12 * block, block, block, 1, 1, 1, 3, 360));
    }

    // Renderiza a HUD.
    void Render()
    {
        color(0.5, 0.5, 0.5);
        rectFill(x0, y0, xf, yf);

        for(auto bt : bt_list) bt->Render();
        for(auto sl : sl_list) sl->Render();
        for(auto se : se_list) se->Render();
    }

    // Se o mouse colidiu com um dos botões, executa a ação definida. Retorna uma lista
    // pois muitos desses botões trabalham com a lista de Shapes.
    std::list<Shape*> CollidedButtons(int mx, int my, std::list<Shape*> shapeList)
    {
        if(button_up->Colidiu(mx, my))    shapeList = MoveShape(shapeList, true);
        if(button_down->Colidiu(mx, my))  shapeList = MoveShape(shapeList, false);
        if(button_fill->Colidiu(mx, my))  fill = !fill;
        if(button_clear->Colidiu(mx, my)) shapeList.clear();
        if(button_load->Colidiu(mx, my))  shapeList = ReadFile();
        if(button_save->Colidiu(mx, my))  WriteFile(shapeList);
        if(button_exit->Colidiu(mx, my))  exit(0);

        return shapeList;
    }

    // Move a figura um nó para cima OU para baixo na lista. Caso essa figura esteja no início,
    // e tentem empurrar ela para cima mais uma vez, ela vai para o fim, e vice-versa.
    std::list<Shape*> MoveShape(std::list<Shape*> shapeList, bool up)
    {
        auto it = shapeList.begin();
        Shape *next = NULL;

        // Percorre toda a lista para encontrar a figura selecionada.
        for(auto shape : shapeList){
            if(shape->IsSelected()){
                if(up) {
                    next = *(std::next(it, 1));
                    std::advance(it, 2);
                } else {
                    next = *(std::next(it, -2));
                    std::advance(it, -1);
                }

                // Copia a figura selecionada para a nova posição.
                shapeList.remove(shape);
                shapeList.insert(it, shape);

                break;
            }
            std::advance(it, 1);
        }
        return shapeList;
    }

    // Lê o arquivo e retorna a lista resultante.
    std::list<Shape*> ReadFile()
    {
        std::list<Shape*> list;
        std::fstream fs(filename, std::ios::in | std::ios::binary);

        // Se der erro, faz nada.
        if(!fs.is_open()) {
            printf("\nErro na abertura do arquivo.\n");
        // Se não...
        } else {
            // Recebe o tamanho da lista (primeiro item carregado)
            unsigned int listSize;
            fs.read(reinterpret_cast<char*>(&listSize), sizeof(listSize));

            // Pega Shapes até o iterador acabar e adiciona elas em uma lista.
            for(unsigned int i = 0; i < listSize; i++) {
                Shape *shape = new Shape();
                shape->Serialize(false, fs);
                list.push_back(shape);
            }

            fs.close();
        }
        return list;
    }

    // Escreve no arquivo.
    void WriteFile(std::list<Shape*> shapeList)
    {
        std::fstream fs(filename, std::ios::out | std::ios::binary);

        // Salva o tamanho da lista.
        int listSize = shapeList.size();
        fs.write(reinterpret_cast<char*>(&listSize), sizeof(listSize));

        // Salva todas as Shapes da lista.
        for(auto shape : shapeList) shape->Serialize(true, fs);

        fs.close();
    }

    // Retorna qual slider o cursor colidiu.
    int CollidedSliders(int mx, int my)
    {
        if(slider_r->Colidiu(mx, my)) return 0;
        else if(slider_g->Colidiu(mx, my)) return 1;
        else if(slider_b->Colidiu(mx, my)) return 2;
        else if(slider_angle->Colidiu(mx, my)) return 3;
        else if(slider_width->Colidiu(mx, my)) return 4;
        else return -1;
    }

    // Retorna o novo valor de select caso o cursor tenha colidido com seus botões.
    void CollidedSelects(int mx, int my)
    {
        vertices = select_vertices->Colidiu(mx, my);
    }

    // Atualiza as informações da HUD de acordo com a figura selecionada.
    void UpdateHud(Shape *shape, std::list<Shape*> shapeList)
    {
        GetShape(shape);

        for(auto s : shapeList) s->Selected(false);
        shape->Selected(true);

        UpdateSelects();
        UpdateSliders();
    }

    // Atualiza os selects da HUD de acordo com a figura selecionada.
    void UpdateSelects()
    {
        select_vertices->SetCurrent(vertices);
    }

    // Atualiza os sliders da HUD de acordo com a figura selecionada.
    void UpdateSliders()
    {
        slider_r->Update(r);
        slider_g->Update(g);
        slider_b->Update(b);
        slider_angle->Update(angle);
        slider_width->Update(width);
    }

    // Pega as informações da Shape selecionada.
    void GetShape(Shape *shape)
    {
        r = shape->GetR();
        g = shape->GetG();
        b = shape->GetB();
        angle = shape->GetAngle();
        vertices = shape->GetVertices();
        width = shape->GetWidth();
        fill = shape->GetFill();
    }

    // Atualiza a shape selecionada com os parâmetros da HUD atuais.
    void UpdateShape(Shape *shape)
    {
        shape->SetR(r);
        shape->SetG(g);
        shape->SetB(b);
        shape->SetAngle(angle);
        shape->SetWidth(width);
        shape->SetVertices(vertices);
        shape->SetFill(fill);

        shape->Build();
    }

    // Bota o "slide" em slider.
    void MouseSliders(int mx, int slider)
    {
        if(slider == 0) r = slider_r->SetButtonX(mx);
        else if(slider == 1) g = slider_g->SetButtonX(mx);
        else if(slider == 2) b = slider_b->SetButtonX(mx);
        else if(slider == 3) angle = slider_angle->SetButtonX(mx);
        else if(slider == 4) width = slider_width->SetButtonX(mx);
    }

    // Getters da HUD.
    float GetR() { return r; }
    float GetG() { return g; }
    float GetB() { return b; }
    float GetAngle() { return angle; }
    float GetWidth() { return width; }
    int GetVertices() { return vertices; }
    bool GetFill() { return fill; }

    int GetHudSize() { return x0; }
};

#endif // HUD_H_INCLUDED
