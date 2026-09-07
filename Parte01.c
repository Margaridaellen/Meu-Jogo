#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define TAM_CELULA     40   // tamanho de cada célula da grade (matriz)

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float   raio;
    Color   cor;
} Bola;

int **criarMatriz(int linhas, int colunas) {
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    if (matriz == NULL) return NULL;

    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        if (matriz[i] == NULL) {
            // liberar o que já foi alocado
            for (int k = 0; k < i; k++) free(matriz[k]);
            free(matriz);
            return NULL;
        }
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = GetRandomValue(0, 1);
        }
    }
    return matriz;
}

void liberarMatriz(int **matriz, int linhas) {
    if (!matriz) return;
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void desenharMatriz(int **matriz, int linhas, int colunas) {
    if (!matriz) return;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Color cor = (matriz[i][j] == 1) ? (Color){20, 40, 70, 255}
                                            : (Color){15, 30, 55, 255};
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA,
                          TAM_CELULA - 2, TAM_CELULA - 2, cor);
        }
    }
}

Bola *criarBolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    if (bolas == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Bola *b = (bolas + i);
        b->pos = (Vector2){ (float)GetRandomValue(50, LARGURA_JANELA - 50),
                            (float)GetRandomValue(50, ALTURA_JANELA - 50) };
        b->vel = (Vector2){ (float)GetRandomValue(-4, 4),
                            (float)GetRandomValue(-4, 4) };
        b->raio = (float)GetRandomValue(10, 25);
        b->cor  = (Color){ (unsigned char)GetRandomValue(100,255), 
                           (unsigned char)GetRandomValue(100,255),
                           (unsigned char)GetRandomValue(100,255), 255 };
    }
    return bolas;
}

void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Ponteiros e Alocacao Dinamica - raylib");
    SetTargetFPS(60);

    int linhas   = ALTURA_JANELA / TAM_CELULA;
    int colunas  = LARGURA_JANELA / TAM_CELULA;
    int **grade  = criarMatriz(linhas, colunas);
    if (grade == NULL) {
        CloseWindow();
        return -1;
    }

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);
    if (bolas == NULL) {
        liberarMatriz(grade, linhas);
        CloseWindow();
        return -1;
    }

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            int novaQtd = quantidadeBolas + 1;
            Bola *tmp = (Bola *)realloc(bolas, novaQtd * sizeof(Bola));
            if (tmp != NULL) {
                bolas = tmp;
                quantidadeBolas = novaQtd;
                bolas[quantidadeBolas - 1] = (Bola){
                    { (float)GetRandomValue(50, LARGURA_JANELA - 50), (float)GetRandomValue(50, ALTURA_JANELA - 50) },
                    { (float)GetRandomValue(-3, 3), (float)GetRandomValue(-3, 3) },
                    10.0f,
                    BLUE
                };
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && quantidadeBolas > 0) {
            quantidadeBolas--;
            if (quantidadeBolas == 0) {
                free(bolas);
                bolas = NULL;
            } else {
                Bola *tmp = (Bola *)realloc(bolas, quantidadeBolas * sizeof(Bola));
                if (tmp != NULL) {
                    bolas = tmp;
                }
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharMatriz(grade, linhas, colunas);

            for (int i = 0; i < quantidadeBolas; i++) {
                atualizarBola(&bolas[i]);
                DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
            }

            DrawText("Matriz (int**) e vetor de structs (Bola*) alocados com malloc",
                     10, 10, 18, WHITE);
            DrawText("Pressione ESC para sair", 10, ALTURA_JANELA - 25, 16, WHITE);
        EndDrawing();
    }

    free(bolas);
    liberarMatriz(grade, linhas);

    CloseWindow();
    return 0;
}