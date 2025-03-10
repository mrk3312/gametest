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


typedef enum MenuState {NONE, MENU01, MENU02, MENU03, MENU04} MenuState;
typedef enum PressedButton {WAITING = -1, EXIT, BUTTON01, BUTTON02} PressedButton;
typedef enum GameScreen {MAINMENU, GAMEPLAY} GameScreen;
typedef enum BuildingType {EMPTY, CONSTRUCTING_SOLARPANEL, CONSTRUCTING_WINDGENERATOR, SOLARPANEL, WINDGENERATOR} BuildingType;
typedef enum BuildingLevel {LEVEL00, LEVEL01, LEVEL02, LEVEL03, LEVEL04, LEVEL05} BuildingLevel;
typedef enum WeatherConditions {SUNNY, CLOUDY, STORMY, FOGGY, RAINY, DRIZZLY} WeatherConditions;

typedef struct Cell
{
    int i;
    int j;
    bool isInactive;
    bool isUnlocked;
    bool isSelected;
    BuildingType building;
    BuildingLevel efficiencyLevel;
    BuildingLevel reliabilityLevel;
    short int turnsConstructing;
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
extern int constructionCapabilities;
extern int usedConstructionCapabilities;

//gui.c
void EnterCellMenu(int, int);
void CellMenu(int, int);
void ExitCellMenu(Cell*, PressedButton*, MenuState*);
const char* BuildingToString(Cell*);
int CalculateMousePosition(int, int);
bool IsIndexValid(int, int);
bool IsBorderingLockedCellsOnly(int, int);
bool IsAffordable(float);
bool HasEnoughCC(int, int);
void ButtonMenu(void);
void RemoveBuilding(Cell*);

//turnstate.c
WeatherConditions GenerateNumber(int);
const char* WeatherToString(WeatherConditions);
void EndTurn(void);
void CalculateEntities(void);
void UpdateConstruction(int, int, const int, const int, const BuildingType, const BuildingType);
bool IsCellConstructing(int, int, const int, const BuildingType);
bool IsCellContainingBuilding(Cell*);

//mapdraw.c
void CellDraw(int, int, int, int);
void CellPaint(int, int, int, int);


#endif