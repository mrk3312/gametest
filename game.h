#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include <time.h>
#include "raygui.h"
#include "cJSON.h"


#define COLS 16
#define ROWS 16

typedef enum GameScreen {MAINMENU, GAMEPLAY} GameScreen;
typedef enum BuildingType {EMPTY, SOLARPANEL, WINDGENERATOR} BuildingType;
typedef enum WeatherConditions {SUNNY, CLOUDY, STORMY, FOGGY, RAINY, DRIZZLY} WeatherConditions;

typedef struct Cell
{
    int i;
    int j;
    bool isSelected;
    bool isInactive;
    bool isUnlocked;
    BuildingType building;
    bool calculatedBuilding;
    bool isInMenu01;
    bool isInMenu02;
    bool isInMenu03;
} Cell;

extern Cell grid[COLS][ROWS];
extern WeatherConditions randomWeather;
extern unsigned short int randomWind;
extern unsigned short int numberWindGenerator;
extern unsigned short int numberSolarPanel;
extern bool inMenu;
extern bool justExitedMenu;
extern unsigned int turn;
extern const int screenWidth;
extern const int screenHeight;
extern bool turnShow;
extern float money;
extern float priceCell;

// Related to Front End
void CellDraw(Cell*, int, int);
void CellPaint(Cell*, int, int);
bool IsIndexValid(int, int);
bool IsBorderingLockedCellsOnly(int, int);
void CellPanel(int, int);
void BoxMenu(Cell*);
void ButtonMenu(void);

// Related to Back End
void EndTurn(void);
void CalculateEntities(void);
WeatherConditions GenerateNumber(int);
const char* WeatherToString(WeatherConditions);

#endif