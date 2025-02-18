#define RAYGUI_IMPLEMENTATION

#include <stdint.h>
#include <stdlib.h>
#include <raylib.h>
#include "raygui.h"

typedef enum GameScreen {LOGO, MAINMENU, GAMEPLAY, ENDING} GameScreen;
typedef enum buildingType {EMPTY, SOLARPANEL, WINDGENERATOR} buildingType;

#define COLS 16
#define ROWS 16

typedef struct SolarPanel
{
    int cost;
    int upkeep;
    int level;
} SolarPanel;

typedef struct WindGenerator
{
    int cost;
    int upkeep;
    int level;
} WindGenerator;

typedef struct Cell
{
    int i;
    int j;
    int CellId;
    bool isBuilt;
    buildingType building;

    bool isInMenu;
    bool isInBuyMenu;
    bool isBuying;
} Cell;

Cell grid[COLS][ROWS];

const int screenWidth = 800;
const int screenHeight = 800;
int cellWidth = screenHeight / COLS;
int cellHeight = screenWidth / ROWS;

void CellDraw(Cell*);
void CellPaint(Cell*);
bool IsIndexValid(int, int);
void CellPanel(int, int);
void BoxMenu(Cell*);

bool inMenu = false;

int main(){

    //Init Window
    
    InitWindow(screenWidth, screenHeight, "Debug");
    //Setting FPS
    int ID = 0;

    for(int i = 0; i < COLS; i++)
    {
        for(int j = 0; j < ROWS; j++)
        {
            grid[i][j] = (Cell) 
            {
                .i = i,
                .j = j,
                .CellId = ID++,
                .isInMenu = false,
                .isInBuyMenu = false,
                .isBuying = false,
                .isBuilt = false,
                .building = EMPTY
            };
        }
    }
    //Main while loop
    while(!WindowShouldClose()) 
    {

        Vector2 mousePosition = GetMousePosition();

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int indexI = mousePosition.x / cellWidth;
            int indexJ = mousePosition.y / cellHeight;
            if(IsIndexValid(indexI, indexJ) && inMenu == false)
            {
                CellPanel(indexI, indexJ);
            }
        } 

        //loading up the different game states
        
            BeginDrawing();
            for(int i = 0; i < COLS; i++)
            {
                for(int j = 0; j < ROWS; j++)
                {
                    CellPaint(&grid[i][j]);
                    CellDraw(&grid[i][j]);
                }
            }
            
            for(int i = 0; i < COLS; i++)
            {
                for(int j = 0; j < ROWS; j++)
                {
                    if(grid[i][j].isInMenu)
                    {
                        BoxMenu(&grid[i][j]);
                    }
                }
            }

            DrawText(TextFormat("X: %f\nY: %f", mousePosition.x, mousePosition.y), 500, 500, 50, BLACK);
            ClearBackground(RAYWHITE);
            EndDrawing();
        
    }
    //closing up the game

    CloseWindow();
    return 0;

}

void CellDraw(Cell *cell)
{ 
    DrawRectangleLines(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, BLACK);
}

void CellPaint(Cell *cell)
{
    if(cell->building == SOLARPANEL)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, GREEN);
}


void CellPanel(int i, int j)
{
    grid[i][j].isInMenu = true;
    inMenu = true;
}

bool IsIndexValid(int i, int j)
{
   return i >= 0 && i < COLS && j >= 0 && j < ROWS;
}

void BoxMenu(Cell *cell)
{
    int infoBox;
    int buyBox;
    int checkBuyBoxSolarPanel;

    Rectangle msgBoxRect = {0, 0, 200, 100};

    if(cell->i > 4 && cell->i < 12 && cell->j > 4 && cell->j < 12)
    {
        msgBoxRect.x = 300;
        msgBoxRect.y = 50;
    }

    else
    {
        msgBoxRect.x = 300;
        msgBoxRect.y = 350;
    }

    if(!cell->isInBuyMenu)
    {   
        infoBox = GuiMessageBox(msgBoxRect,
            "Info", TextFormat("CellID: %d\nBuilding: %d\n", cell->CellId, cell->building), "Build");

        if(infoBox == 0)
        {
            cell->isInMenu = false;
            inMenu = false;
        }
        else if (infoBox == 1)
            cell->isInBuyMenu = true;
    
    }
    else
    {
        if(!cell->isBuying)
        {
            buyBox = GuiMessageBox(msgBoxRect,
            "Build Panel", "What would you like to build?", "Solar Panel;Wind Generator");
            if(buyBox == 0)
            {
                cell->isInMenu = false;
                cell->isInBuyMenu = false;
                inMenu = false;
            }
            else if(buyBox == 1)
            {
                cell->isBuying = true;
            }
        }
        else
        {
            checkBuyBoxSolarPanel = GuiMessageBox(msgBoxRect,
            "Build Panel", "Are you sure you'd like to buy\nthe solar panel for 3000€", "Yes;Cancel");
            if(checkBuyBoxSolarPanel == 1)
            {
                cell->building = SOLARPANEL;
                cell->isBuilt = true;

                cell->isInMenu = false;
                cell->isInBuyMenu = false;
                cell->isBuying = false;
                inMenu = false;
            }
            else if(checkBuyBoxSolarPanel == 0 || checkBuyBoxSolarPanel == 2)
            {
                cell->isInMenu = false;
                cell->isInBuyMenu = false;
                cell->isBuying = false;
                inMenu = false;
            }
        }
    }
}

