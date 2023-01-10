#include "Settings.h"
#include "Common.h"
#include "Input.h"
#include "Assets.h"
#include "Game.h"

// IMPORTANT :
// Les librairies SDL_TTF et SDL_Mixer sont configurées dans le projet mais ne
// sont pas initialisées.
// Vous devez pour cela modifier les fonctions App_Init() et App_Quit() dans
// le fichier Common.c

#define GAP_WIDTH 137.8f
#define GAP_HEIGHT 120

int main(int argc, char *argv[])
{
    // Initialisation de la SDL
    App_Init(SDL_INIT_VIDEO, IMG_INIT_PNG);

    // Crée la fenêtre et le moteur de rendu
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif

    SDL_Window *window = SDL_CreateWindow(
        u8"Basecode SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le temps global du jeu
    g_time = Timer_New();
    AssertNew(g_time);

    // Crée le gestionnaire des entrées utilisateur
    Input *input = Input_New();

    // Chargement des assets
    Assets *assets = Assets_New(renderer);

    //Lancement de la partie
    Game* game = Game_New();


    // Boucle de jeu
    /*while (Game_GetState(game) == GAME_IN_PROGRESS)
    {
        printf("----------------------------------------\n");
        printf("Tour du joueur %d\n\n", Game_GetPlayerID(game));

        Game_Print(game);
        printf("\n");

        // Choix de la colonne
        int column = 0;
        do
        {
            printf("Choisissez votre colonne (entre 0 et 6) : ");
            int res = scanf("%d", &column);
        } while (!Game_CanPlayAt(game, column));

        // Action du joueur
        Game_PlayTurn(game, column);
        printf("\n");
    }
    Game_Print(game);
    Game_Delete(game);*/

    //--------------------------------------------------------------------------
    // Boucle de rendu


    // Position et taille du plateau
    float boardx = 150.0f;
    float boardy = 0.0f;
    float boardw = LOGICAL_WIDTH - 300.0f;
    float boardh = LOGICAL_HEIGHT;
    //Point d'origine du plateau (en bas à gauche)
    float originx = 174.0f;
    float originy = 609.0f;
    //Position et taille des icônes de joueurs
    float p1x = 40.0f;
    float p1y = 10.0f;
    float p1w = 70.0f;
    float p1h = 70.0f;

    float p2x = 1170.0f;
    float p2y = 10.0f;
    float p2w = 70.0f;
    float p2h = 70.0f;
    //Initialisation des cases
    CellState cells[GRID_W][GRID_H] = { CELL_EMPTY };
    SDL_FRect slots[GRID_W][GRID_H];
    for (int i = 0; i < GRID_W; i++)
    {
        for (int j = 0; j < GRID_H; j++)
        {
            slots[i][j].x = originx + GAP_WIDTH * i;
            slots[i][j].y = originy - GAP_HEIGHT * j;
            slots[i][j].w = 105.0f;
            slots[i][j].h = 105.0f;
        }
    }

    while (true)
    {
        // Met à jour le temps
        Timer_Update(g_time);

        // Met à jour les entrées
        Input_Update(input);

        if (input->quitPressed)
            break;

        // Efface le rendu précédent
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        // Dessin du plateau
        SDL_FRect dstBoard;
        dstBoard.x = boardx;
        dstBoard.y = boardy;
        dstBoard.w = boardw;
        dstBoard.h = boardh;
        SDL_RenderCopyF(renderer, assets->textureBoard, NULL, &dstBoard);

        //Dessin des icônes player
        SDL_FRect dstP1;
        dstP1.x = p1x;
        dstP1.y = p1y;
        dstP1.w = p1w;
        dstP1.h = p1h;
        SDL_RenderCopyF(renderer, assets->textureP1, NULL, &dstP1);
        
        SDL_FRect dstP2;
        dstP2.x = p2x;
        dstP2.y = p2y;
        dstP2.w = p2w;
        dstP2.h = p2h;
        SDL_RenderCopyF(renderer, assets->textureP2, NULL, &dstP2);

        //Pointeur de la souris
        SDL_Point mouse;
        mouse.x = input->mouseXPosition;
        mouse.y = input->mouseYPosition;
        //Affichage de chaque pion
        for (int i = 0; i < GRID_W; i++)
        {
            for (int j = 0; j < GRID_H; j++)
            {
                SDL_FRect* display = &(slots[i][j]);
                //Pion fantome qui suit la souris
                if (SDL_PointInFRect(&mouse, display) && cells[i][j] == CELL_EMPTY)
                {
                    if (input->leftClick && cells[i][j] == CELL_EMPTY && Game_CanPlayAt(game, i))
                    {
                        if(game->playerID == 1)
                            cells[i][j] = CELL_P1;
                        else
                            cells[i][j] = CELL_P2;
                        Game_PlayTurn(game, i);
                        printf("Placed in : %d, %d\n",i , j);
                    }
                    if (game->playerID == 1)
                        SDL_RenderCopyF(renderer, assets->textureYellowPiece, NULL, display);
                    else if (game->playerID == 2)
                        SDL_RenderCopyF(renderer, assets->textureRedPiece, NULL, display);
                }

                //Pions posés par les joueurs
                if(cells[i][j] == CELL_P1)
                    SDL_RenderCopyF(renderer, assets->textureYellowPiece, NULL, display);
                else if(cells[i][j] == CELL_P2)
                    SDL_RenderCopyF(renderer, assets->textureRedPiece, NULL, display);
            }
        }
 
        // Affiche le nouveau rendu
        SDL_RenderPresent(renderer);
    }

    //--------------------------------------------------------------------------
    // Libération de la mémoire

    Assets_Delete(assets);
    assets = NULL;

    Input_Delete(input);
    input = NULL;

    Timer_Delete(g_time);
    g_time = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    App_Quit();

    return EXIT_SUCCESS;
}