#include "game.h"

int GenerateNumber(int maxNumber)
{
    srand(time(NULL) ^ GetCurrentProcessId());
    int randomNumber = rand() % maxNumber;
    return randomNumber;
}

void EndTurn(void)
{
    float buildingProducedEnergy = 0;
    float totalProducedEnergy = 0;
    float sunlightHours = 5;
    float averageWind = 10;

    int randomWeather = GenerateNumber(6);
    int panelPower;
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
                    buildingProducedEnergy = (panelPower * 2 * 0.15) * sunlightHours;
                }
                else if(grid[i][j].building == WINDGENERATOR)
                {
                    buildingProducedEnergy = ((1.225 * 78.54 * pow(averageWind, 3) * 0.15) / 2);
                }
                totalProducedEnergy+=buildingProducedEnergy;
            }
        }
    }
    turn++;

    float income = (totalProducedEnergy/1000) * 0.32;
    money+=income;
}

void CalculateEntities(void)
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(grid[i][j].isUnlocked && grid[i][j].building != EMPTY && !grid[i][j].calculatedBuilding)
            {
                if(grid[i][j].building == SOLARPANEL)
                {
                    numberSolarPanel++;
                }
                else if(grid[i][j].building == WINDGENERATOR)
                {
                    numberWindGenerator++;
                }
                grid[i][j].calculatedBuilding = true;
            }
        }
    }
}