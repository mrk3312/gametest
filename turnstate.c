#include "game.h"

WeatherConditions GenerateNumber(int maxNumber)
{
    //srand(time(NULL) ^ GetCurrentProcessId());
    int randomNumber = rand() % maxNumber;
    return randomNumber;
}

const char* WeatherToString(WeatherConditions randomWeather)
{
    switch(randomWeather) {
        case SUNNY:
        {
            return "SUNNY";
        }
        case CLOUDY:
        {
            return "CLOUDY";
        }
        case STORMY:
        {
            return "STORMY";
        }
        case RAINY:
        {
            return "RAINY";
        }
        case FOGGY:
        {
            return "FOGGY";
        }
        case DRIZZLY:
        {
            return "DRIZZLY";
        }
    }
    return "Weather Unknown";
}

bool IsCellConstructing(int i, int j, const int ConstructionTurns, const BuildingType building)
{
    Cell *cell = &grid[i][j];
    return !cell->isInactive && cell->isUnlocked && cell->turnsConstructing < ConstructionTurns && cell->building == building;
}

void UpdateConstruction(int i, int j, const int requiredConstructionCapabilities, const int ConstructionTurns, const BuildingType constructBuilding, const BuildingType building)
{
    Cell *cell = &grid[i][j];
    if(IsCellConstructing(i, j, ConstructionTurns, constructBuilding))
    {
        if(cell->turnsConstructing == ConstructionTurns - 1)
        {
            cell->building = building;
            cell->turnsConstructing = 0;
            usedConstructionCapabilities+=requiredConstructionCapabilities;
        }
        else
            cell->turnsConstructing++;
    }
}

bool IsCellContainingBuilding(Cell *cell)
{
    return cell->isUnlocked && cell->building != EMPTY;
}

void CountBuilding(Cell *cell)
{

    if(IsCellContainingBuilding(cell))
    {
        switch(cell->building)
        {
            case SOLARPANEL:
            {
                numberSolarPanel++;
                break;
            }
            case WINDGENERATOR:
            {
                numberWindGenerator++;
                break;
            }
            case EMPTY:
            case CONSTRUCTING_SOLARPANEL:
            case CONSTRUCTING_WINDGENERATOR:
                return;
        }
    }
}

void CalculateEntities(void)
{
    numberSolarPanel = 0;
    numberWindGenerator = 0;
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            CountBuilding(&grid[i][j]);
        }
    }
}

void EndTurn(void)
{
    float buildingProducedEnergy = 0;
    float totalProducedEnergy = 0;
    float sunlightHours = 5;
    int panelPower = 0;

    switch(randomWeather)
    {
        case SUNNY:
        {
            panelPower = 1000;
            break;
        }
        case CLOUDY:
        {
            panelPower = 600;
            break;
        }
        case STORMY:
        {
            panelPower = 100;
            break;
        }
        case FOGGY:
        {
            panelPower = 300;
            break;
        }
        case RAINY:
        {
            panelPower = 400;
            break;
        }
        case DRIZZLY:
        {
            panelPower = 600;
            break;
        }
    }

    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(grid[i][j].building != EMPTY && grid[i][j].isUnlocked)
            {
                if(grid[i][j].building == SOLARPANEL)
                {
                    buildingProducedEnergy = 7 * ((panelPower * 2 * 0.15) * sunlightHours);
                }
                else if(grid[i][j].building == WINDGENERATOR)
                {
                    buildingProducedEnergy = 7 * ((1.225 * 78.54 * ((float)pow(randomWind, 3)) * 0.15) / 2);
                }
                totalProducedEnergy+=buildingProducedEnergy;
            }
        }
    }
    
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
           UpdateConstruction(i, j, 300, 3, CONSTRUCTING_SOLARPANEL, SOLARPANEL);
           UpdateConstruction(i, j, 500, 5, CONSTRUCTING_WINDGENERATOR, WINDGENERATOR);
        }
    }

    turn++;

    float income = (totalProducedEnergy/1000) * 0.32;
    money+=income;

    randomWind = GenerateNumber(22);
    randomWeather = GenerateNumber(6);
}