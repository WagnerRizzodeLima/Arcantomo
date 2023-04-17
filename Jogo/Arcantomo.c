/*******************************************************************************************
*   Jogo desenvolvido para a aula de Programação Avançada do curso de Jogos Digitais da 
*   FATEC de Americana, no ano de 2023.
*   Gabriel Fogaça: Programador.
*   Nayumi Akutagwa: Artista.
*   Valentina Belchior: Artista.
*   Wagner Rizzo: Programador.
*   O jogo é desenvolvido utilizando linguagem C e a biblioteca Raylib, o intuito desse jogo
*   é aprender a aplicar a estrtutura de dados do tipo fila em jogos digitais.
********************************************************************************************/

#include <raylib.h>
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define SCREEN_WIDTH 1360
#define SCREEN_HEIGHT 768
#define PLAYER_SPEED 9
#define jogador_SPEED 9
#define NPC_COUNT 2
#define MAX_FRAME_DELAY     20
#define MIN_FRAME_DELAY      1

typedef struct no{
    int valor;
    struct no *proximo;

}No;

typedef struct player{
    int vivo;
    int golpe, vida;
}Player;

typedef struct {
    Texture2D texture;  // Textura do personagem
    Vector2 position;   // Posição do personagem na tela
} Character;

// Função auxiliar para desenhar um personagem na tela
void drawCharacter(Character character) {
    DrawTexture(character.texture, character.position.x, character.position.y, WHITE);
}

void inserir_na_fila(No **fila, int num){
    No *aux, *novo = malloc(sizeof(No));
    if(novo){
        novo->valor = num;
        novo->proximo = NULL;
        if(*fila == NULL){
            *fila = novo;
        }else{
            aux = *fila;
            while(aux->proximo)
                aux = aux->proximo;
        }
    }else
    printf("\nErro ao alocar memoria");
}

No* remover_da_fila(No **fila){
    No *remover = NULL;
    if(*fila){
        remover = *fila;
        *fila = remover->proximo;
    }else{
        printf("/tFila vazia\n");
    }
    return remover;
}

const int screenWidth = 1360;
const int screenHeight = 768;
const Rectangle ATK1_BUTTON_RECTANGLE = {5, 600, 200, 80};
const Rectangle ATK2_BUTTON_RECTANGLE = {275, 600, 200, 80};
const Rectangle ATK3_BUTTON_RECTANGLE = {545, 600, 200, 75};
const Color BUTTON_COLOR_NORMAL = GRAY;
const Color BUTTON_COLOR_HOVER = LIGHTGRAY;
const Color BUTTON_COLOR_CLICK = BLACK;
bool jogando = false, showPopup = false,mapa;

// Enumerador para indicar qual botão foi clicado
typedef enum { NO_BUTTON, ATK1_BUTTON, ATK2_BUTTON, ATK3_BUTTON } ButtonType;

int main(void)
{
    // Inicializa a janela
    const int screenWidth = 1360;
    const int screenHeight = 768;
    
    InitWindow(screenWidth, screenHeight, "Menu Principal");
    InitAudioDevice();
    Image icon = LoadImage("Imagens/magko.png");
    SetWindowIcon(icon);
    
    int animFrames = 0;

    Image imScarfyAnim = LoadImageAnim("Animations/Animacao.gif", &animFrames);
    Image imScarfyAnim1 = LoadImageAnim("Animations/Animacao2.gif", &animFrames);
    Texture2D texScarfyAnim = LoadTextureFromImage(imScarfyAnim);
    Texture2D texScarfyAnim1 = LoadTextureFromImage(imScarfyAnim1);

    unsigned int nextFrameDataOffset = 0;  // Current byte offset to next frame in image.data

    int currentAnimFrame = 0;       // Current animation frame to load and draw
    int frameDelay = 8;             // Frame delay to switch between animation frames
    int frameCounter = 0;           // General frames counter
    int contadorVitorias;
    bool gifAtivoMonstro = true;
    bool gifAtivoProfessor = true;
    bool imagemAtivaMonstro = true;
    bool imagemAtivaProfessor = true;
    // Carrega as texturas dos personagens e do mapa de background
    Character jogador = { LoadTexture("Animations/frente.png"), (Vector2){ 400, 1100} };
    Character npcs[NPC_COUNT] = {
        { LoadTexture("Sprites/Professor.png"), (Vector2){ 700, 220 } },
        { LoadTexture("Sprites/Monstro1.png"), (Vector2){ 1900, 930 } },
    };
    Texture2D background = LoadTexture("Sprites/Mapa.png");
    Texture2D Arvore1 = LoadTexture("Sprites/Arvore1.png");
    Texture2D Arvore2 = LoadTexture("Sprites/Arvore2.png");
    Texture2D Arvore3= LoadTexture("Sprites/Arvores3.png");
    // Define a área do limitador baseada no tamanho do mapa de background
    Rectangle limitador = { 320, 320, 2000, 1000 };
    
    Rectangle frameRec = { 0.0f, 0.0f, (float)jogador.texture.width/2, (float)jogador.texture.height };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 2; 
    
    // Inicializa a câmera
    Camera2D camera = { 0 };
    camera.target = jogador.position;
    camera.offset = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    SetTargetFPS(60);   
    
    // Carrega a imagem de background
    Texture2D backgroundMenu = LoadTexture("Imagens/Fundo Tela Inicial (1).png");
    // Carrega as imagens
    Texture2D startButtonBackground = LoadTexture("Imagens/Botao.png");
    Texture2D aboutButtonBackground = LoadTexture("Imagens/Botao.png");
    Texture2D exitButtonBackground = LoadTexture("Imagens/Botao.png");
    Texture2D character = LoadTexture("C:/raylib/raylib/examples/textures/resources/scarfy_run.gif");
    Texture2D pauseButton = LoadTexture("Imagens/ConfButton.png");
    Texture2D pauseButtonBG = LoadTexture("Sprites/About.png");
    Rectangle pauseButtonRec = { screenWidth - pauseButton.width - 10, 10, pauseButton.width, pauseButton.height };
    
    Image image = LoadImage("Imagens/Fundo de batalha.png");
    Texture2D texturaFundo = LoadTextureFromImage(image);
    UnloadImage(image);
    
    Image playerImage = LoadImage("Imagens/foxy.png");
    Texture2D texturaPlayer = LoadTextureFromImage(playerImage);
    UnloadImage(playerImage);
    
    Image playerInimigo = LoadImage("Imagens/magko.png");
    Texture2D texturaInimigo = LoadTextureFromImage(playerInimigo);
    UnloadImage(playerInimigo);
    
    Image playerInimigoMostro = LoadImage("Sprites/Boneben.png");
    Texture2D texturaInimigoMosntro = LoadTextureFromImage(playerInimigoMostro );
    UnloadImage(playerInimigoMostro );
    
    Image acoes = LoadImage("Imagens/Borda Painel de ataque.png");
    Texture2D texturaLayoutAcoes = LoadTextureFromImage(acoes);
    UnloadImage(acoes);

    Image atk1= LoadImage("Imagens/Botao2.png");
    Texture2D texturaAtk1 = LoadTextureFromImage(atk1);
    UnloadImage(atk1);
    
    Image atk2= LoadImage("Imagens/Botao1.png");
    Texture2D texturaAtk2 = LoadTextureFromImage(atk2);
    UnloadImage(atk2);
    
    Image atk3= LoadImage("Imagens/BotaoAr.png");
    Texture2D texturaAtk3 = LoadTextureFromImage(atk3);
    UnloadImage(atk3);
    
    Image painelAcoes = LoadImage("Imagens/Tela.png");
    Texture2D texturaTelaInfo = LoadTextureFromImage(painelAcoes);
    UnloadImage(painelAcoes);
    
    Image painel = LoadImage("Imagens/PainelAtaques.png");
    Texture2D texturaPainelAtaques = LoadTextureFromImage(painel);
    UnloadImage(painel);
    
    Image imgInimigo1 = LoadImage("Imagens/VidaVaziaHpInimigo.png");
    Texture2D texturaInimigo1 = LoadTextureFromImage(imgInimigo1);
    UnloadImage(imgInimigo1);
    
    Image imgInimigo2 = LoadImage("Imagens/fundoHpInimigo.png");
    Texture2D texturaInimigo2 = LoadTextureFromImage(imgInimigo2);
    UnloadImage(imgInimigo2);
    
    Image imgInimigo3 = LoadImage("Imagens/VidaHpInimigo.png");
    Texture2D texturaInimigo3 = LoadTextureFromImage(imgInimigo3);
    UnloadImage(imgInimigo3);
    
    Image imgPlayer1 = LoadImage("Imagens/VidaVaziaPlayer.png");
    Texture2D texturaPlayer1 = LoadTextureFromImage(imgPlayer1);
    UnloadImage(imgPlayer1);
    
    Image imgPlayer2 = LoadImage("Imagens/fundoHpPlayer.png");
    Texture2D texturaPlayer2 = LoadTextureFromImage(imgPlayer2);
    UnloadImage(imgPlayer2);
    
    Image imgPlayer3 = LoadImage("Imagens/FundoPlayer.png");
    Texture2D texturaPlayer3 = LoadTextureFromImage(imgPlayer3);
    UnloadImage(imgPlayer3);
    
    // Carrega a imagem da seta
    Texture2D arrow = LoadTexture("Imagens/Seta.png");

    // Define as cores utilizadas
    Color buttonColor1 = BUTTON_COLOR_NORMAL;
    Color buttonColor2 = BUTTON_COLOR_NORMAL;
    Color buttonColor3 = BUTTON_COLOR_NORMAL;
    Color pauseButtonColor = WHITE;
    Color white = (Color){ 255, 255, 255, 255 };
    
    Sound MusiGame = LoadSound("Sound/MusicaBatalha.wav");
    Sound MusiMenu = LoadSound("Sound/MusicaMenu.wav");
    Sound MusiMapa = LoadSound("Sound/MusicaMapa.wav");
    Sound attackAir = LoadSound("Sound/Air.wav");
    Sound attackFire = LoadSound("Sound/Fire.wav");
    Sound attackGnd = LoadSound("Sound/Ground.wav");
    Sound attackWater = LoadSound("Sound/Water.wav");
    Sound attackBlock = LoadSound("Sound/Block.wav");
    Sound attackLava = LoadSound("Sound/lava.wav");
    Sound attackIce = LoadSound("Sound/ice.wav");
    Sound vitoriaSound = LoadSound("Sound/vitoria.wav");
    Sound derrotaSound = LoadSound("Sound/Perda.wav");

    Texture2D Vitoria = LoadTexture("Sprites/vitoria.png");
    Texture2D Derrota = LoadTexture("Sprites/derrota.png");
    Texture2D TutorialTextura = LoadTexture("Sprites/Bordas.png");

    // Define as dimensões e posição dos botões
    const int buttonWidth = 330;
    const int buttonHeight = 125;
    const int buttonPadding = 5;
    const int buttonX = 60;
    const int buttonY = 480;
    bool jogando = true, showPopup = false, playSoundMenu = true, Menu = true, pauseMenuExit = false;
 
    // Renderiza o Pop-Up de pausa
    Rectangle pauseMenuRec = { screenWidth / 2 - 200, screenHeight / 2 - 100, 400, 200 };
    
    
    // Definindo o estado do jogo
    enum GameState { MENU, LOADING, PLAYING, ABOUT, EXIT};
    int gameState = MENU;
    
    // Define a posição inicial do personagem com uma margem de 50 pixels à esquerda
    float characterPositionX = -character.width / 2 + 50;
    
    // Define a velocidade do personagem em pixels por segundo
    float characterSpeed = 400;
    
    // Definindo as propriedades da barra de loading
    Rectangle loadingBar = { screenWidth/2 - 700, screenHeight/2 + 200, 1370, 20 };
    float loadingProgress = 0;
    
    Player player;
    player.vivo = 1;
    player.vida = 40;
    Vector2 posicaoPlayer = {120, 180};
    
    Player inimigo;
    inimigo.vivo = 1;
    inimigo.vida = 40;
    Vector2 posicaoInimigo = {800, 250}; 
    
    No *r;
    No *fila = NULL;

    int dano = 0;
    
    Color corTexto = WHITE;
    
    char *status = "Escolha um golpe";
    char *vida;
    // Adiciona valores à fila
    
    // Inicializa a variável que irá indicar qual botão foi clicado
    ButtonType clickedButton = NO_BUTTON;

    // TODO: Load resources / Initialize variables at this point
    int golpeInimigo[20],i;
    SetTargetFPS(60);
    fflush(stdin);
    
    if(playSoundMenu == true){
        PlaySound(MusiMenu);
    }
    
    // Loop principal
    while (!WindowShouldClose())
    {
        StopSound(MusiMapa);
        StopSound(MusiGame);
        
        // Atualizando o estado do jogo
        switch (gameState)
        {
            case PLAYING:{
                // Código para iniciar o jogo
                mapa = true;
                jogando = false;
                player.vivo = 1;
                player.vida = 40;
                Vector2 posicaoPlayer = {120, 180};
                loadingProgress = 0;
                inimigo.vivo = 1;
                inimigo.vida = 40;
                Vector2 posicaoInimigo = {800, 30}; 
                
                No *r;
                No *fila = NULL;

                int dano = 0;
                
                Color corTexto = WHITE;
                
                //int golpeInimigo[20];
                char *status = "Escolha um golpe";
                char *vida;
                PlaySound(MusiMapa);
                StopSound(MusiMenu);
                StopSound(MusiGame);
                while (mapa)
                {
                    framesCounter++;

                    if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;

                        if (currentFrame > 5) currentFrame = 0;

                        frameRec.x = (float)currentFrame*(float)jogador.texture.width/2;
                    }
                    
                    // Atualiza a posição do jogador com base na entrada do teclado
                    if (IsKeyDown(KEY_RIGHT) && CheckCollisionRecs((Rectangle){ jogador.position.x + jogador_SPEED, jogador.position.y, jogador.texture.width, jogador.texture.height }, limitador))
                        jogador.position.x += jogador_SPEED;
                    if (IsKeyDown(KEY_LEFT) && CheckCollisionRecs((Rectangle){ jogador.position.x - jogador_SPEED, jogador.position.y, jogador.texture.width, jogador.texture.height }, limitador))
                        jogador.position.x -= jogador_SPEED;
                    if (IsKeyDown(KEY_UP) && CheckCollisionRecs((Rectangle){ jogador.position.x, jogador.position.y - jogador_SPEED, jogador.texture.width, jogador.texture.height }, limitador))
                        jogador.position.y -= jogador_SPEED;
                    if (IsKeyDown(KEY_DOWN) && CheckCollisionRecs((Rectangle){ jogador.position.x, jogador.position.y + jogador_SPEED, jogador.texture.width, jogador.texture.height }, limitador))
                        jogador.position.y += jogador_SPEED;

                    // Atualiza a posição da câmera para seguir o jogador
                    camera.target = jogador.position;

                    // Desenha os personagens e o mapa de background na tela, usando a câmera
                    BeginDrawing();

                    ClearBackground(BLACK);

                    BeginMode2D(camera);

                    DrawTexture(background, 0, 0, WHITE);
                    DrawTexture(Arvore1, 0, 0, WHITE);
                    DrawTexture(Arvore2, 0, 0, WHITE);
                    
                    // Verifica se o jogador está colidindo com algum NPC
                    for (int i = 0; i < NPC_COUNT; i++) {
                        if (CheckCollisionRecs((Rectangle){ jogador.position.x, jogador.position.y, jogador.texture.width, jogador.texture.height },(Rectangle){ npcs[i].position.x, npcs[i].position.y, npcs[i].texture.width, npcs[i].texture.height })) {
                            // Se o jogador colidir com o NPC i, exibe uma mensagem na tela
                            if (i == 0) {
                                //DrawText("Encostou no NPC 1", 100, 100, 20, BLACK);
                                while (gifAtivoProfessor)    // Detect window close button or ESC key
                                {
                                    // Update
                                    //----------------------------------------------------------------------------------
                                    frameCounter++;
                                    if (frameCounter >= frameDelay)
                                    {
                                        // Move to next frame
                                        // NOTE: If final frame is reached we return to first frame
                                        currentAnimFrame++;
                                         if(currentAnimFrame == 11){
                                            gifAtivoProfessor = false;
                                            imagemAtivaProfessor = !imagemAtivaProfessor;
                                            mapa = false;
                                            jogando = true;
                                        
                                        }
                                        if (currentAnimFrame >= animFrames) currentAnimFrame = 0;

                                        // Get memory offset position for next frame data in image.data
                                        nextFrameDataOffset = imScarfyAnim.width*imScarfyAnim.height*4*currentAnimFrame;

                                        // Update GPU texture data with next frame image data
                                        // WARNING: Data size (frame size) and pixel format must match already created texture
                                        UpdateTexture(texScarfyAnim, ((unsigned char *)imScarfyAnim.data) + nextFrameDataOffset);

                                        frameCounter = 0;
                                    }

                                    // Control frames delay
                                    if (IsKeyPressed(KEY_RIGHT)) frameDelay++;
                                    else if (IsKeyPressed(KEY_LEFT)) frameDelay--;

                                    if (frameDelay > MAX_FRAME_DELAY) frameDelay = MAX_FRAME_DELAY;
                                    else if (frameDelay < MIN_FRAME_DELAY) frameDelay = MIN_FRAME_DELAY;
                                   
                                    //----------------------------------------------------------------------------------
                                    // Draw
                                    //----------------------------------------------------------------------------------
                                    BeginDrawing();

                                        ClearBackground(RAYWHITE);

                                        DrawTexture(texScarfyAnim, 0,0, WHITE);

                                    EndDrawing();
                                    //----------------------------------------------------------------------------------
                                }
                            } else if (i == 1) {
                                //DrawText("Encostou no NPC 2", 100, 100, 20, BLACK);
                                while (gifAtivoMonstro)    // Detect window close button or ESC key
                                {
                                    // Update
                                    //----------------------------------------------------------------------------------
                                    frameCounter++;
                                    if (frameCounter >= frameDelay)
                                    {
                                        // Move to next frame
                                        // NOTE: If final frame is reached we return to first frame
                                        currentAnimFrame++;
                                         if(currentAnimFrame == 11){
                                            gifAtivoMonstro = false;
                                            imagemAtivaMonstro = !imagemAtivaMonstro;
                                            mapa = false;
                                            jogando = true;
                                            
                                        }
                                        if (currentAnimFrame >= animFrames) currentAnimFrame = 0;

                                        // Get memory offset position for next frame data in image.data
                                        nextFrameDataOffset = imScarfyAnim1.width*imScarfyAnim1.height*4*currentAnimFrame;

                                        // Update GPU texture data with next frame image data
                                        // WARNING: Data size (frame size) and pixel format must match already created texture
                                        UpdateTexture(texScarfyAnim, ((unsigned char *)imScarfyAnim1.data) + nextFrameDataOffset);

                                        frameCounter = 0;
                                        
                                        
                                    }

                                    // Control frames delay
                                    if (IsKeyPressed(KEY_RIGHT)) frameDelay++;
                                    else if (IsKeyPressed(KEY_LEFT)) frameDelay--;

                                    if (frameDelay > MAX_FRAME_DELAY) frameDelay = MAX_FRAME_DELAY;
                                    else if (frameDelay < MIN_FRAME_DELAY) frameDelay = MIN_FRAME_DELAY;
                                    
                                   
                                    //----------------------------------------------------------------------------------

                                    // Draw
                                    //----------------------------------------------------------------------------------
                                    BeginDrawing();

                                        ClearBackground(RAYWHITE);

                                        DrawTexture(texScarfyAnim, 0,0, WHITE);

                                    EndDrawing();
                                    //----------------------------------------------------------------------------------
                                }
                            } 
                        }
                    }

                    // Desenha os personagens
                    DrawTextureRec(jogador.texture, frameRec, jogador.position, WHITE);
                    DrawTexture(Arvore3, 0, 0, WHITE);
                    if(imagemAtivaMonstro){
                       
                        drawCharacter(npcs[1]);
                    }
                    
                    if(imagemAtivaProfessor){
                       
                        drawCharacter(npcs[0]);
                    }
                      // Desenha o mapa de background
                    
                    EndMode2D();
                    EndDrawing();
                }
                    // Limpa a tela
                ClearBackground(RAYWHITE);
                PlaySound(MusiGame);
                StopSound(MusiMapa);
                 // Loop do jogo7
                 while (jogando)
                 {
                     pauseMenuExit = false;
                     // Verifica se o botão de pausa foi clicado
                     if (CheckCollisionPointRec(GetMousePosition(), pauseButtonRec))
                     {
                         pauseButtonColor = GRAY;
               
                         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                         {
                             while (!pauseMenuExit)
                             {
                                 // Verifica se o botão de retorno foi clicado
                                 if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){pauseMenuRec.x + 50, pauseMenuRec.y + 100, 100, 50}))
                                 {
                                     pauseButtonColor = WHITE;
                                    
               
                                     if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                                     {
                                        pauseMenuExit = true;
                                     }
                                 }
                                 else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){pauseMenuRec.x + 250, pauseMenuRec.y + 100, 100, 50}))
                                 {
                                     pauseButtonColor = WHITE;
                                     
               
                                     if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                                     {
                                        pauseMenuExit = false;
                                        jogando = false;
                                        imagemAtivaProfessor = true;
                                        playSoundMenu = true;
                                        gameState = MENU;
                                        jogador.position.x = 400;
                                        jogador.position.y = 1100;
                                        if(playSoundMenu == true){
                                            PlaySound(MusiMenu);
                                        }
                                     }
                                }
                                else
                                {
                                    pauseButtonColor = WHITE;
                                }
                                // Renderiza o Pop-Up de pausa
                                BeginDrawing();
                                DrawTexture(pauseButton, pauseButtonRec.x, pauseButtonRec.y, pauseButtonColor);
                                DrawTexture(TutorialTextura, 390, 170, RAYWHITE);
                                DrawText("Pause", pauseMenuRec.x + 135, pauseMenuRec.y + 15, 40, RAYWHITE);
                                DrawText("Retornar", pauseMenuRec.x + 50, pauseMenuRec.y + 110, 30, PURPLE);
                                DrawText("Sair", pauseMenuRec.x + 280, pauseMenuRec.y + 110, 30, RED);
                                EndDrawing();
                            }
                            pauseButtonColor = WHITE;
                         }
                     }
                     else
                     {
                        pauseButtonColor = WHITE;
                     }
                     
                     SetMousePosition(GetMouseX(), GetMouseY());
                        Vector2 mousePosition = GetMousePosition();
                        for(int i = 0; i <= 20; i++){
                            
                            golpeInimigo [i]= rand()%3;
                            inserir_na_fila(&fila, golpeInimigo[i]);
                        }
                        // Verifica se o mouse está sobre um dos botões e muda a cor do botão para indicar que ele está selecionado
                        if (CheckCollisionPointRec(mousePosition, ATK1_BUTTON_RECTANGLE))
                        {
                            buttonColor1 = BUTTON_COLOR_HOVER;
                            // Verifica se o botão foi clicado
                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                clickedButton = ATK1_BUTTON;
                                player.golpe = 0;
                                r = remover_da_fila(&fila);
                                PlaySound(attackIce);
                                if(player.golpe == r->valor){
                                    WaitTime(0.7);
                                    status = "Voces usaram golpes que se\n anularam!";
                                    corTexto = GRAY;
                                    PlaySound(attackBlock);
                                }
                                if(player.golpe == 0 && r->valor == 2){
                                    dano = 8;
                                    inimigo.vida -= dano;
                                    WaitTime(0.7);
                                    status = "Voce usou Gelo! \nAdversario sofreu 8 de dano!";
                                    corTexto = GREEN;
                                }
                                if(player.golpe == 0 && r->valor == 1){
                                    dano = 8;
                                    player.vida -= dano;
                                    WaitTime(0.7);
                                    status = "Adversario usou Lava! \nVoce sofreu 8 de dano!";
                                    corTexto = RED;
                                    PlaySound(attackLava);
                                }
                                clickedButton = NO_BUTTON;
                            }
                        }
                        else
                        {
                            buttonColor1 = BUTTON_COLOR_NORMAL;
                        }
                        
                        if (CheckCollisionPointRec(mousePosition, ATK2_BUTTON_RECTANGLE))
                        {
                            buttonColor2 = BUTTON_COLOR_HOVER;
                            // Verifica se o botão foi clicado
                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                clickedButton = ATK2_BUTTON;
                                player.golpe = 1;
                                r = remover_da_fila(&fila);
                                PlaySound(attackWater);
                                if(player.golpe == r->valor){
                                    WaitTime(0.7);
                                    status = "Voces usaram golpes que se\n anularam!";
                                    corTexto = GRAY;
                                    PlaySound(attackBlock);
                                }
                                if(player.golpe == 1 && r->valor == 0){
                                    dano = 8;
                                    inimigo.vida -= dano;
                                    WaitTime(0.7);
                                    status = "Voce usou Agua!\nAdversario sofreu 8 de dano!";
                                    corTexto = GREEN;
                                }
                                if(player.golpe == 1 && r->valor == 2){
                                    dano = 8;
                                    player.vida -= dano;
                                    WaitTime(0.7);
                                    status = "Adversario usou Fogo!\nVoce sofreu 8 de dano!";
                                    corTexto = RED;
                                    PlaySound(attackFire);
                                    
                                }
                                clickedButton = NO_BUTTON;
                            }
                        }
                        else
                        {
                            buttonColor2 = BUTTON_COLOR_NORMAL;
                            
                        }
                        
                        if (CheckCollisionPointRec(mousePosition, ATK3_BUTTON_RECTANGLE))
                        {
                            buttonColor3 = BUTTON_COLOR_HOVER;
                            // Verifica se o botão foi clicado
                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                clickedButton = ATK3_BUTTON;
                                player.golpe = 2;
                                r = remover_da_fila(&fila);
                                PlaySound(attackAir);
                                if(player.golpe == r->valor){
                                    WaitTime(0.7);
                                    status = "Voces usaram golpes que se\n anularam!";
                                    corTexto = GRAY;
                                    PlaySound(attackBlock);
                                }
                                if(player.golpe == 2 && r->valor == 1){
                                    dano = 8;
                                    inimigo.vida -= dano;
                                    WaitTime(0.7);
                                    status = "Voce usou vento!\nAdversario sofreu 8 de dano!";
                                    corTexto = GREEN;
                                }
                                if(player.golpe == 2 && r->valor == 0){
                                    dano = 8;
                                    player.vida -= dano;
                                    WaitTime(0.7);
                                    status = "Adversario usou Terra!\nVoce sofreu 8 de dano!";
                                    corTexto = RED;
                                    PlaySound(attackGnd);
                                }
                                clickedButton = NO_BUTTON;
                            }
                        }
                        else
                        {
                            buttonColor3 = BUTTON_COLOR_NORMAL;
                        }
                        
                        if(inimigo.vida <= 0){
                        inimigo.vivo = 0;
                        status = "Adversario Morreu!\nVoce venceu a batalha!";
                        }
                        if(player.vida <= 0){
                        player.vivo = 0;
                        status = "Voce Morreu!";
                        } 
                        //----------------------------------------------------------------------------------
                        // Draw
                    
                        //--------------------------------------------------------------------------------
                        if(inimigo.vivo == 0){
                            WaitTime(2);
                             StopSound(attackAir);
                             StopSound(attackFire);
                             StopSound(attackIce);
                             StopSound(attackWater);
                             StopSound(attackGnd);
                             StopSound(attackLava);
                             StopSound(MusiGame);
                             PlaySound(vitoriaSound);
                             BeginDrawing();
                             DrawTexture(Vitoria,0,0,RAYWHITE);
                             contadorVitorias++;
                             EndDrawing();
                             WaitTime(3);
                            pauseMenuExit = false;
                            jogando = false;
                            mapa = true;
                            imagemAtivaProfessor = true;
                        }
                        if(player.vivo == 0){
                            WaitTime(2);
                            StopSound(attackAir);
                            StopSound(attackFire);
                            StopSound(attackIce);
                            StopSound(attackWater);
                            StopSound(attackGnd);
                            StopSound(attackLava);
                            StopSound(MusiGame);
                            PlaySound(derrotaSound);
                             BeginDrawing();
                             DrawTexture(Derrota,0,0,RAYWHITE);
                             EndDrawing();
                             WaitTime(3);
                             StopSound(derrotaSound);
                            pauseMenuExit = false;
                            jogando = false;
                            imagemAtivaProfessor = true;
                            gameState = MENU;
                            playSoundMenu = true;
                            jogador.position.x = 400;
                            jogador.position.y = 1100;
                            if(playSoundMenu == true){
                                PlaySound(MusiMenu);
                            }
                        }
                        
                        if(contadorVitorias == 2){
                            StopSound(attackAir);
                            StopSound(attackFire);
                            StopSound(attackIce);
                            StopSound(attackWater);
                            StopSound(attackGnd);
                            StopSound(attackLava);
                            StopSound(MusiGame);
                            PlaySound(derrotaSound);
                            WaitTime(3);
                            StopSound(derrotaSound);
                            pauseMenuExit = false;
                            jogando = false;
                            imagemAtivaProfessor = true;
                            gameState = MENU;
                            playSoundMenu = true;
                            jogador.position.x = 400;
                            jogador.position.y = 1100;
                            if(playSoundMenu == true){
                                PlaySound(MusiMenu);
                            }
                        }
                        // Renderiza a tela do jogo
                        BeginDrawing();
                        DrawTexture(texturaFundo, 0, 0, WHITE);
                        if(imagemAtivaProfessor == false){
                            DrawTexture(texturaInimigoMosntro, 800, 130, WHITE);
                        }else{
                            DrawTexture(texturaInimigo,posicaoInimigo.x, posicaoInimigo.y, WHITE);
                        }
                        
                        DrawTexture(texturaPlayer,posicaoPlayer.x, posicaoPlayer.y, WHITE);
                        DrawTexture(texturaPainelAtaques, 0, 558, WHITE );
                        DrawTexture(texturaLayoutAcoes, 0, 520, WHITE );
                        DrawTexture(texturaAtk1, ATK1_BUTTON_RECTANGLE.x, ATK1_BUTTON_RECTANGLE.y, WHITE);
                        DrawTexture(texturaAtk2, ATK2_BUTTON_RECTANGLE.x, ATK2_BUTTON_RECTANGLE.y, WHITE);
                        DrawTexture(texturaAtk3, ATK3_BUTTON_RECTANGLE.x, ATK3_BUTTON_RECTANGLE.y, WHITE);
                        DrawTexture(texturaTelaInfo, 850, 570, WHITE);
                        DrawTexture(texturaInimigo1, 65, 45, WHITE);
                        DrawRectangle(188, 55, inimigo.vida*10, 27,RED);
                        DrawTexture(texturaInimigo2, 65, 40, WHITE);
                        DrawTexture(texturaPlayer1, 760, 402, WHITE);
                        DrawRectangle(846, 415, player.vida*10, 23,GREEN);
                        DrawTexture(texturaPlayer2, 790, 411, WHITE);     
                        DrawTexture(texturaPlayer3, 762, 402, WHITE);   
                        DrawTexture(pauseButton, pauseButtonRec.x, pauseButtonRec.y, pauseButtonColor);
                        DrawText(status,880, 600, 28, corTexto);
                        DrawText("Gelo", ATK1_BUTTON_RECTANGLE.x+120, ATK1_BUTTON_RECTANGLE.y+50, 30, RAYWHITE);
                        DrawText("Agua", ATK2_BUTTON_RECTANGLE.x+120, ATK2_BUTTON_RECTANGLE.y+50, 30, RAYWHITE);
                        DrawText("Ar", ATK3_BUTTON_RECTANGLE.x+120, ATK3_BUTTON_RECTANGLE.y+50, 30, RAYWHITE);
                        DrawText(TextFormat("%i/40", player.vida),843, 455, 28, MAGENTA);
                        EndDrawing();
                } 
            }break;
            
            case MENU:{
                    playSoundMenu = true;
                    contadorVitorias = 0;
                   // Desenha a imagem de background
                DrawTexture(backgroundMenu, 0, 0, WHITE);
                
                // Verifica se o mouse está sobre os botões
                Vector2 mousePosition = GetMousePosition();
                
                if (CheckCollisionPointRec(mousePosition, (Rectangle){buttonX, buttonY - (buttonHeight + buttonPadding), buttonWidth, buttonHeight})){
                    DrawTexture(arrow, buttonX - arrow.width, buttonY - (buttonHeight + buttonPadding) + (buttonHeight / 2) - (arrow.height / 2), WHITE);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        jogando = true;
                        Menu = false;
                        playSoundMenu = false;
                        gameState = LOADING;
                    }
                    
                }
                if (CheckCollisionPointRec(mousePosition, (Rectangle){buttonX, buttonY, buttonWidth, buttonHeight})){
                    DrawTexture(arrow, buttonX - arrow.width, buttonY + (buttonHeight / 2) - (arrow.height / 2), WHITE);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                       // gameState = ABOUT;
                        showPopup = true;
                    }
                }
                if (CheckCollisionPointRec(mousePosition, (Rectangle){buttonX, buttonY + (buttonHeight + buttonPadding), buttonWidth, buttonHeight})){
                    DrawTexture(arrow, buttonX - arrow.width, buttonY + (buttonHeight + buttonPadding) + (buttonHeight / 2) - (arrow.height / 2), WHITE);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        gameState = EXIT;
                    }
                }
                
                // Verifica se o Pop-Up deve ser exibido e o exibe
                if (showPopup) {
                    DrawTexture(pauseButtonBG, 390, 170, WHITE); // Desenha um fundo 
                    DrawText("Voltar", GetScreenWidth()/3 ,GetScreenHeight()/3, 20, PURPLE); // Desenha o texto do botão "CLOSE"
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // Verifica se o botão esquerdo do mouse foi pressionado
                        Vector2 mousePos = GetMousePosition();
                        Rectangle closeButton = (Rectangle){GetScreenWidth()/3 ,GetScreenHeight()/3, 100, 40};
                        if (CheckCollisionPointRec(mousePos, closeButton)) { // Verifica se o mouse está sobre o botão "CLOSE"
                            showPopup = false; // Define o estado do Pop-Up como "não mostrar"
                        }
                    }
                }
                
                BeginDrawing();
                ClearBackground(RAYWHITE);
        
                // Desenha os botões
                DrawTextureRec(startButtonBackground, (Rectangle){0, 0, startButtonBackground.width, -startButtonBackground.height}, (Vector2){buttonX, buttonY - (buttonHeight + buttonPadding)}, WHITE);
                DrawText("Start", buttonX + (buttonWidth / 2) - (MeasureText("Start", 30) / 2), buttonY - (buttonHeight + buttonPadding) + (buttonHeight / 2) - 15, 30, white);
                DrawTextureRec(aboutButtonBackground, (Rectangle){0, 0, aboutButtonBackground.width, -aboutButtonBackground.height}, (Vector2){buttonX, buttonY}, WHITE);
                DrawText("Equipe", buttonX + (buttonWidth / 2) - (MeasureText("Equipe", 30) / 2), buttonY + (buttonHeight / 2) - 15, 30, white);
                DrawTextureRec(exitButtonBackground, (Rectangle){0, 0, exitButtonBackground.width, -exitButtonBackground.height}, (Vector2){buttonX, buttonY + (buttonHeight + buttonPadding)}, WHITE);
                DrawText("Close", buttonX + (buttonWidth / 2) - (MeasureText("Close", 30) / 2), buttonY + (buttonHeight + buttonPadding) + (buttonHeight / 2) - 15, 30, white);
                
                // Atualiza a janela
                EndDrawing(); 
 
            }break;

            case LOADING:{
                loadingProgress += 0.5f;
                if (loadingProgress >= 100)
                {
                    gameState = PLAYING;
                    gifAtivoMonstro = true;
                    gifAtivoProfessor = true;
                    imagemAtivaMonstro = true;
                    imagemAtivaProfessor = true;
                }
                
                // Começa o processo de renderização
                BeginDrawing();
                
                // Limpa a tela com a cor branca
                DrawTexture(backgroundMenu, 0, 0, WHITE);
                
                // Desenha a tela de instruções
                DrawTexture(TutorialTextura, 390, 170, RAYWHITE);
                DrawText("Use as SETAS para se mover\npelo cenario.\nDurante a batalha use o\nBOTAO ESQUERDO DO MOUSE \npara selecionar o golpe",440, 270,32, MAGENTA);
                // Desenha a barra de loading
                DrawRectangleRec(loadingBar, GRAY);
                DrawRectangle(loadingBar.x, loadingBar.y, loadingBar.width * (loadingProgress / 100), loadingBar.height, SKYBLUE);
                DrawText("Carregando...", screenWidth/2 - 150, screenHeight/2 + 250, 50, WHITE);
                
                // Finaliza o processo de renderização
                EndDrawing();
                
                
            }break;
            
            case EXIT:{
                // Descarrega a imagem de background e fecha a janela
                UnloadTexture(background);
                UnloadTexture(startButtonBackground);
                UnloadTexture(aboutButtonBackground);
                UnloadTexture(exitButtonBackground);
                UnloadTexture(pauseButton);
                UnloadTexture(character);
                UnloadTexture(arrow);
                
                UnloadTexture(texturaAtk1);
                UnloadTexture(texturaAtk2);
                UnloadTexture(texturaAtk3);
                UnloadTexture(texturaFundo);
                UnloadTexture(texturaInimigo);
                UnloadTexture(texturaInimigo1);
                UnloadTexture(texturaInimigo2);
                UnloadTexture(texturaInimigo3);
                UnloadTexture(texturaLayoutAcoes);
                UnloadTexture(texturaPainelAtaques);
                UnloadTexture(texturaPlayer);
                UnloadTexture(texturaPlayer1);
                UnloadTexture(texturaPlayer2);
                UnloadTexture(texturaPlayer3);
                UnloadTexture(texturaTelaInfo);
                UnloadSound(attackAir);
                UnloadSound(attackFire);
                UnloadSound(attackIce);
                UnloadSound(attackWater);
                UnloadSound(attackGnd);
                UnloadSound(attackLava);
                UnloadSound(MusiGame);
                
                // Fecha a janela
                CloseWindow();
            }break;
               
        }
       
    }
    // Fecha a janela
    CloseAudioDevice();   
    CloseWindow();
    return 0;
}