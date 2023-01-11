#include "Game.h"

Game* Game_New()
{
    Game* self = (Game*)calloc(1, sizeof(Game));
    AssertNew(self);

    self->state = GAME_IN_PROGRESS;
    self->playerID = PLAYER_1;

    return self;
}

void Game_Delete(Game* self)
{
    if (!self) return;
    free(self);
}

Game* Game_Copy(Game* self)
{
    Game* new = (Game*)calloc(1, sizeof(Game));
    int i, j;
    AssertNew(new);

    new->state = self->state;
    new->playerID = self->playerID;
    for (i = 0; i < GRID_H; i++)
    {
        for (j = 0; j < GRID_W; j++)
            new->grid[i][j] = self->grid[i][j];
    }

    return new;
}

/// @brief Met à jour l'état de la partie.
/// Cette méthode est appelée par Game_PlayTurn().
/// @param self la partie.
void Game_UpdateState(Game* self)
{
    int i, j, nbrcoloneplein = 0;
    for (i = 0; i < GRID_H; i++)
    {
        for (j = 0; j < GRID_W; j++)
        {
            if (((self->grid[i][j] == 1) && (self->grid[i + 1][j] == 1)) 
            && ((self->grid[i + 2][j] == 1) && (self->grid[i + 3][j] == 1)))
            {
                self->state = GAME_P1_WON;
                //return;
            }
            else if (((self->grid[i][j] == 2) && (self->grid[i + 1][j] == 2)) 
            && ((self->grid[i + 2][j] == 2) && (self->grid[i + 3][j] == 2)))
            {
                self->state = GAME_P2_WON;
                //return;
            }
            else if (((self->grid[i][j] == 1) && (self->grid[i][j + 1] == 1)) 
            && ((self->grid[i][j + 2] == 1) && (self->grid[i][j + 3] == 1)))
            {
                self->state = GAME_P1_WON;
                //return;
            }
            else if (((self->grid[i][j] == 2) && (self->grid[i][j + 1] == 2)) 
            && ((self->grid[i][j + 2] == 2) && (self->grid[i][j + 3] == 2)))
            {
                self->state = GAME_P2_WON;
                //return;
            }
            else if (((self->grid[i][j] == 1) && (self->grid[i + 1][j + 1] == 1)) 
            && ((self->grid[i + 2][j + 2] == 1) && (self->grid[i + 3][j + 3] == 1)))
            {
                self->state = GAME_P1_WON;
                //return;
            }
            else if (((self->grid[i][j] == 2) && (self->grid[i + 1][j + 1] == 2)) 
            && ((self->grid[i + 2][j + 2] == 2) && (self->grid[i + 3][j + 3] == 2)))
            {
                self->state = GAME_P2_WON;
                //return;
            }
            else if (((self->grid[i][j] == 1) && (self->grid[i + 1][j - 1] == 1)) 
            && ((self->grid[i + 2][j - 2] == 1) && (self->grid[i + 3][j - 3] == 1)))
            {
                self->state = GAME_P1_WON;
                //return;
            }
            else if (((self->grid[i][j] == 2) && (self->grid[i + 1][j - 1] == 2)) 
            && ((self->grid[i + 2][j - 2] == 2) && (self->grid[i + 3][j - 3] == 2)))
            {
                self->state = GAME_P2_WON;
                //return;
            }
        }
        if (!Game_CanPlayAt(self, i));
        nbrcoloneplein++;
    }
    if (nbrcoloneplein == GRID_W)
        self->state = GAME_IS_TIED;

    if(self->state != 1 && self->state != 2)
        self->state = GAME_IN_PROGRESS;
}

bool Game_CanPlayAt(Game* self, int column)
{
    assert(self);
    int gamestate = Game_GetState(self), i = 0;

    //Le joueur ne peut jouer si la partie est finie, ni si la colonne n'existe pas, ni si la colonne est pleine
    //Sinon il peut jouer
    if (gamestate != 0)
        return false;
    if (column > GRID_H)
        return false;

    for (i = 0; i < GRID_W; i++)
    {
        if (self->grid[i][column] == 0)
            return true;
    }
    return false;
}

void Game_PlayTurn(Game* self, int column)
{
    assert(self);
    assert(Game_CanPlayAt(self, column));

    //Revérifie si le joueur peut jouer sur cette colonne
    if (!Game_CanPlayAt(self, column))
    {
        return;
    }
    int i = 0, playerID;

    //Cherche la première ligne ou il peut poser son jeton
    for (i = 0; i < GRID_W; i++)
    {
        if (self->grid[i][column] == 0)
        {
            playerID = self->playerID;
            self->grid[i][column] = playerID;
            break;
        }
    }


    // Met à jour l'état de la partie
    Game_UpdateState(self);

    // Change le joueur courant
    self->playerID = PlayerID_GetOther(self->playerID);
}

void Game_Print(Game* self)
{
    //printf("%d", self->grid[0][0]);
    int i, j;
    for (i = GRID_H - 1; i >= 0; i--)
    {
        printf("| ");
        for (j = 0; j < GRID_W; j++)
        {
            if (self->grid[i][j] == 1)
                printf("1 ");
            else if (self->grid[i][j] == 2)
                printf("2 ");
            else
                printf("X ");
        }
        printf("|\n");
    }
    printf("----------------");
}