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

int main(int argc, char* argv[])
{
    // Initialisation de la SDL
    App_Init(SDL_INIT_VIDEO, IMG_INIT_PNG);

    // Crée la fenêtre et le moteur de rendu
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif

    SDL_Window* window = SDL_CreateWindow(
        u8"Basecode SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
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
    Input* input = Input_New();

    // Chargement des assets
    Assets* assets = Assets_New(renderer);

    //Lancement de la partie
    Game* game = Game_New();

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

    //Position et taille des évènements victoire
    float victx = 400.0f;
    float victy = 150.0f;
    float victw = 400.0f;
    float victh = 400.0f;

    //Initialisation des cases
    SDL_FRect slots[GRID_H][GRID_W];
    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            slots[i][j].x = originx + GAP_WIDTH * j;
            slots[i][j].y = originy - GAP_HEIGHT * i;
            slots[i][j].w = 105.0f;
            slots[i][j].h = 105.0f;
        }
    }

    //Les questions importantes :
    // - comment qu'on fait pour rendre une image transparente ?
    // - pour les animations ça marche comme dans spw ? j'imagine mais on sait jamais
    // - 

    while (!input->quitPressed)
    {
        //Boucle de jeu P1 vs P2
        while (game->state == GAME_IN_PROGRESS)
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
            if (game->playerID == 1)
                SDL_RenderCopyF(renderer, assets->textureP1, NULL, &dstP1);

            SDL_FRect dstP2;
            dstP2.x = p2x;
            dstP2.y = p2y;
            dstP2.w = p2w;
            dstP2.h = p2h;
            if (game->playerID == 2)
                SDL_RenderCopyF(renderer, assets->textureP2, NULL, &dstP2);

            //Pointeur de la souris
            SDL_Point mouse;
            mouse.x = input->mouseXPosition;
            mouse.y = input->mouseYPosition;
            //Affichage de chaque pion
            for (int i = 0; i < GRID_H; i++)
            {
                for (int j = 0; j < GRID_W; j++)
                {
                    SDL_FRect* display = &(slots[i][j]);
                    //Pion fantome qui suit la souris
                    if (SDL_PointInFRect(&mouse, display) && game->grid[i][j] == 0)
                    {
                        if (input->leftClick && game->grid[i][j] == 0 && Game_CanPlayAt(game, j))
                        {
                            Game_PlayTurn(game, j);
                        }

                        if (game->playerID == 1)
                        {
                            SDL_RenderCopyF(renderer, assets->textureYellowPiece, NULL, display);
                        }
                        else if (game->playerID == 2)
                        {
                            SDL_RenderCopyF(renderer, assets->textureRedPiece, NULL, display);
                        }
                    }

                    //Pions posés par les joueurs
                    if (game->grid[i][j] == 1)
                        SDL_RenderCopyF(renderer, assets->textureYellowPiece, NULL, display);
                    else if (game->grid[i][j] == 2)
                        SDL_RenderCopyF(renderer, assets->textureRedPiece, NULL, display);
                }
            }

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        // Reset la partie
        for (int i = 0; i < GRID_H; i++)
        {
            for (int j = 0; j < GRID_W; j++)
            {
                game->grid[i][j] = CELL_EMPTY;
            }
        }
        game->playerID = 1;

        // Boucle d'affichage victoire du joueur 1
        while (game->state == 1)
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

            // Pointeur de la souris
            SDL_Point mouse;
            mouse.x = input->mouseXPosition;
            mouse.y = input->mouseYPosition;    

            // Dessin de l'évènement victoire du joueur 1
            SDL_FRect dstP1wins;
            dstP1wins.x = victx;
            dstP1wins.y = victy;
            dstP1wins.w = victw;
            dstP1wins.h = victh;
            SDL_RenderCopyF(renderer, assets->textureP1wins, NULL, &dstP1wins);

            // Dessin du bouton continuer
            SDL_FRect dstContinue;
            dstContinue.x = 450.0f;
            dstContinue.y = 500.0f;
            dstContinue.w = 300.0f;
            dstContinue.h = 100.0f;
            SDL_RenderCopyF(renderer, assets->textureContinue, NULL, &dstContinue);

            if (SDL_PointInFRect(&mouse, &dstContinue) && input->leftClick)
                game->state = GAME_MENU;

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        // Boucle d'affichage victoire du joueur 2
        while (game->state == 2)
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

            // Pointeur de la souris
            SDL_Point mouse;
            mouse.x = input->mouseXPosition;
            mouse.y = input->mouseYPosition;

            // Dessin de l'évènement victoire du joueur 2
            SDL_FRect dstP2wins;
            dstP2wins.x = victx;
            dstP2wins.y = victy;
            dstP2wins.w = victw;
            dstP2wins.h = victh;
            SDL_RenderCopyF(renderer, assets->textureP2wins, NULL, &dstP2wins);

            // Dessin du bouton continuer
            SDL_FRect dstContinue;
            dstContinue.x = 450.0f;
            dstContinue.y = 500.0f;
            dstContinue.w = 300.0f;
            dstContinue.h = 100.0f;
            SDL_RenderCopyF(renderer, assets->textureContinue, NULL, &dstContinue);

            if (SDL_PointInFRect(&mouse, &dstContinue) && input->leftClick)
                game->state = GAME_MENU;

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        game->state = GAME_MENU;

        // Boucle d'affichage du menu
        while (game->state == GAME_MENU)
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

            //Dessin des différents boutons
            SDL_FRect dstAIvsAI;
            dstAIvsAI.x = 425.0f;
            dstAIvsAI.y = 300.0f;
            dstAIvsAI.w = 400.0f;
            dstAIvsAI.h = 100.0f;
            SDL_RenderCopyF(renderer, assets->textureAIvsAI, NULL, &dstAIvsAI);

            SDL_FRect dstPvsAI;
            dstPvsAI.x = 425.0f;
            dstPvsAI.y = 400.0f;
            dstPvsAI.w = 400.0f;
            dstPvsAI.h = 145.0f;
            SDL_RenderCopyF(renderer, assets->texturePvsAI, NULL, &dstPvsAI);

            SDL_FRect dstP1vsP2;
            dstP1vsP2.x = 425.0f;
            dstP1vsP2.y = 550.0f;
            dstP1vsP2.w = 400.0f;
            dstP1vsP2.h = 100.0f;
            SDL_RenderCopyF(renderer, assets->textureP1vsP2, NULL, &dstP1vsP2);

            SDL_FRect dstExit;
            dstExit.x = 30.0f;
            dstExit.y = 30.0f;
            dstExit.w = 180.0f;
            dstExit.h = 90.0f;
            SDL_RenderCopyF(renderer, assets->textureExit, NULL, &dstExit);

            SDL_FRect dstOptions;
            dstOptions.x = 1020.0f;
            dstOptions.y = 30.0f;
            dstOptions.w = 220.0f;
            dstOptions.h = 90.0f;
            SDL_RenderCopyF(renderer, assets->textureOptions, NULL, &dstOptions);

            // Dessin du logo
            SDL_FRect dstConnect4;
            dstConnect4.x = 420.0f;
            dstConnect4.y = 50.0f;
            dstConnect4.w = 420.0f;
            dstConnect4.h = 200.0f;
            SDL_RenderCopyF(renderer, assets->textureConnect4, NULL, &dstConnect4);

            // Pointeur de la souris
            SDL_Point mouse;
            mouse.x = input->mouseXPosition;
            mouse.y = input->mouseYPosition;

            //PlayervsPlayer
            if (SDL_PointInFRect(&mouse, &dstP1vsP2) && input->leftClick)
                game->state = GAME_IN_PROGRESS;

            //Exit
            if (SDL_PointInFRect(&mouse, &dstExit) && input->leftClick)
                return;

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }
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