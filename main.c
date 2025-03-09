#define RAYGUI_IMPLEMENTATION
#include "game.h"

Cell grid[COLS][ROWS];

bool inMenu = false;
bool justExitedMenu = true;

bool turnShow = true;
unsigned int turn = 0;

unsigned short int numberWindGenerator = 0;
unsigned short int numberSolarPanel = 0;

float money = 3000000;
float priceCell = 1000;
WeatherConditions randomWeather = SUNNY;
unsigned short int randomWind = 15;

int constructionCapabilities = 1000;
int usedConstructionCapabilities = 1000;

int main(){

    GameScreen screen = GAMEPLAY;
    const int screenWidth = 800;
    const int screenHeight = 800;
    int cellWidth = screenHeight / COLS;
    int cellHeight = screenWidth / ROWS;
    
    InitWindow(screenWidth, screenHeight, "Debug");

    for(int i = 0; i < COLS; i++)
    {
        for(int j = 0; j < ROWS; j++)
        {
            grid[i][j] = (Cell) 
            {
                .i = i,
                .j = j,
                .isSelected = false,
                .isInactive = (j == 0 || j == 1),
                .isUnlocked = (i > 6 && i < 9 && j > 7 && j < 10),
                .building = EMPTY,
                .turnsConstructing = 0,
            };
        }
    }

    //Main while loop
    while(!WindowShouldClose()) 
    {   

        CalculateEntities();
        Vector2 mousePosition = GetMousePosition();
        static int indexI;
        static int indexJ;
        
        if(IsKeyPressed(KEY_ENTER))
        {
            switch(screen)
            {
                case(MAINMENU):
                {
                    screen = GAMEPLAY;
                    break;
                }

                case(GAMEPLAY):
                {
                    screen = MAINMENU;
                    break;
                }

            }
        }
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !inMenu)
        {
            indexI = CalculateMousePosition(mousePosition.x, cellWidth);
            indexJ = CalculateMousePosition(mousePosition.y, cellHeight);
            if(IsIndexValid(indexI, indexJ) && !inMenu && !grid[indexI][indexJ].isInactive && !IsBorderingLockedCellsOnly(indexI, indexJ))
            {
                    CellPanel(indexI, indexJ);
            }
        } 
        
            BeginDrawing();

            switch(screen)
            {

                case MAINMENU:
                {
                    DrawRectangle(0, 0, 800, 800, RED);
                    break;
                };

                case GAMEPLAY:
                {
                    ButtonMenu();
                    if(turnShow)
                    {
                        DrawText(TextFormat("Week: %d", turn), 325, 0, 40, GRAY);
                        DrawText(TextFormat("$%.2f", money), 0, 0, 40, GREEN);
                        DrawText(TextFormat("%d / %d", usedConstructionCapabilities, constructionCapabilities), 550, 0, 40, GRAY);
                    }

                    for(int i = 0; i < COLS; i++)
                    {
                        for(int j = 0; j < ROWS; j++)
                        {
                            CellPaint(i, j, cellWidth, cellHeight);
                            CellDraw(i, j, cellWidth, cellHeight);
                        }
                    }
                    if(grid[indexI][indexJ].isSelected)
                        BoxMenu(indexI, indexJ);
                }
                DrawText(TextFormat("X: %f\nY: %f", mousePosition.x, mousePosition.y), 500, 500, 50, BLACK);   
                break;
            }

            ClearBackground(RAYWHITE);
            EndDrawing();
        
    }
    //closing up the game

    CloseWindow();
    return 0;

}


