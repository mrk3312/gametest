#define RAYGUI_IMPLEMENTATION
#include "game.h"

Cell grid[COLS][ROWS];

bool inMenu = false;
bool justExitedMenu = true;

bool turnShow = true;
unsigned int turn = 0;

unsigned short int numberWindGenerator = 0;
unsigned short int numberSolarPanel = 0;

float money = 3000;
float priceCell = 1000;


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
                .isUnlocked = (i < 2 && j < 4),
                .building = EMPTY,
                .calculatedBuilding = false,
                .isInMenu01 = false,
                .isInMenu02 = false,
                .isInMenu03 = false
            };
        }
    }

    //Main while loop
    while(!WindowShouldClose()) 
    {   
        CalculateEntities();

        Vector2 mousePosition = GetMousePosition();

        
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

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int indexI = mousePosition.x / cellWidth;
            int indexJ = mousePosition.y / cellHeight;
            if(IsIndexValid(indexI, indexJ) && inMenu == false && grid[indexI][indexJ].isInactive == false)
            {
                    CellPanel(indexI, indexJ);
                    justExitedMenu = false;
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
                        DrawText(TextFormat("Turn: %d", turn), 0, 0, 40, GRAY);
                    
                    DrawText(TextFormat("$%.2f", money), 400, 0, 40, GREEN);

                    for(int i = 0; i < COLS; i++)
                    {
                        for(int j = 0; j < ROWS; j++)
                        {
                            CellPaint(&grid[i][j], cellWidth, cellHeight);
                            CellDraw(&grid[i][j], cellWidth, cellHeight);
                        }
                    }
                    
                    for(int i = 0; i < COLS; i++)
                    {
                        for(int j = 0; j < ROWS; j++)
                        {
                            if(grid[i][j].isInMenu01)
                            {
                                BoxMenu(&grid[i][j]);
                            }
                        }
                    }
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


