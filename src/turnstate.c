#include "game.h"


//srand(time(NULL) ^ GetCurrentProcessId());
int GenerateInt(int max)
{
    int randomNumber = rand() % max;
    return randomNumber;
}

WeatherCondition GenerateWeatherCondition(void)
{
    return GenerateInt(TOTAL_COUNT);
}

int GenerateWind(void)
{
    return GenerateInt(24);
}

const char* WeatherToString(WeatherCondition randomWeather)
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
        case TOTAL_COUNT:
            break;
    }
    return "Weather Unknown";
}

bool IsCellConstructing(Cell *cell, const int ConstructionTurns, const BuildingType building)
{
    return !cell->isInactive && cell->isUnlocked && cell->turnsConstructing < ConstructionTurns && cell->building == building;
}

void UpdateConstruction(int i, int j, const int requiredConstructionCapabilities, const int ConstructionTurns, const BuildingType constructBuilding, const BuildingType building)
{
    Cell *cell = &grid[i][j];
    if(IsCellConstructing(cell, ConstructionTurns, constructBuilding))
    {
        if(cell->turnsConstructing == ConstructionTurns - 1)
        {
            cell->building = building;
            cell->efficiencyLevel = LEVEL01;
            cell->reliabilityLevel = LEVEL01;
            cell->turnsConstructing = 0;
            usedConstructionCapabilities+=requiredConstructionCapabilities;
        }
        else
            cell->turnsConstructing++;
            
    }
}

bool IsCellUpgrading(Cell *cell, const int upgradeTurns, const UpgradeStatus upgradeStatus, const BuildingType building)
{
    return !cell->isInactive && cell->isUnlocked && cell->turnsConstructing < upgradeTurns && cell->constructionUpgradingStatus == upgradeStatus && cell->building == building;
}

void UpgradeConstruction(int i, int j, const int requiredConstructionCapabilities, const int upgradeTurns, const UpgradeStatus typeOfUpgrade, const BuildingType currentBuilding,  const BuildingType afterFinish)
{
    Cell *cell = &grid[i][j];
    if(IsCellUpgrading(cell, upgradeTurns, typeOfUpgrade, currentBuilding))
    {
        if(cell->turnsConstructing == upgradeTurns - 1)
        {
            cell->building = afterFinish;
            cell->turnsConstructing = 0;
            usedConstructionCapabilities+=requiredConstructionCapabilities;
            cell->constructionUpgradingStatus = NOTUPGRADING;
            switch(typeOfUpgrade)
            {
                case UPGRADING_EFFICIENCY:
                {
                    cell->efficiencyLevel++;
                    break;
                }
                case UPGRADING_RELIABILITY:
                {
                    cell->reliabilityLevel++;
                    break;
                }
                case NOTUPGRADING:
                    break;
            }
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
            case UPGRADING_SOLARPANEL:
            case UPGRADING_WINDGENERATOR:
                return;
        }
    }
}


// Calculate Construction
const int CalculateConstructionCapabilityCost(int i, int j, UpgradeStatus typeOfUpgrade)
{
    Cell *cell = &grid[i][j];
    
    if(cell->building == SOLARPANEL || cell->building == UPGRADING_SOLARPANEL)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 300;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->reliabilityLevel + 1) * 350;
            }
            case NOTUPGRADING:
            break;
        }
    }
    else if(cell->building == WINDGENERATOR || cell->building == UPGRADING_WINDGENERATOR)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 500;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->reliabilityLevel + 1) * 550;
            }
            case NOTUPGRADING:
                break;
        }
    }
}

const int CalculateTurns(int i, int j, UpgradeStatus typeOfUpgrade)
{
    Cell *cell = &grid[i][j];

    if(cell->building == SOLARPANEL || cell->building == UPGRADING_SOLARPANEL)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 3;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->reliabilityLevel + 1) * 4;
            }
            case NOTUPGRADING:
            break;
        }
    }
    else if(cell->building == WINDGENERATOR || cell->building == UPGRADING_WINDGENERATOR)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 5;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->reliabilityLevel + 1) * 6;
            }
            case NOTUPGRADING:
            break;
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
    float producedEnergy = 0;
    float sunlightHours = 5;
    float panelPower = 0;
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
        case TOTAL_COUNT:
            break;
    }

    float efficiencyBuilding = 0;

    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(!grid[i][j].isInactive && grid[i][j].isUnlocked && grid[i][j].building != EMPTY)
            {
                if(grid[i][j].building == SOLARPANEL)
                {
                    switch(grid[i][j].efficiencyLevel)
                    {
                        case LEVEL01:
                        {
                            efficiencyBuilding = 0.2f;
                            break;  
                        }
                        case LEVEL02:
                        {
                            efficiencyBuilding = 0.3f;
                            break;   
                        }
                        case LEVEL03:
                        {
                            efficiencyBuilding = 0.38f;
                            break;    
                        }
                        case LEVEL04:
                        {
                            efficiencyBuilding = 0.45f;
                            break;  
                        }
                        case LEVEL05:
                        {
                            efficiencyBuilding = 0.5f;
                            break;  
                        }
                        case LEVEL00:
                            return;
                    }
                    producedEnergy = 7.0f * ((panelPower * 2.0f * efficiencyBuilding) * sunlightHours);
                }
                else if(grid[i][j].building == WINDGENERATOR)
                {
                    switch(grid[i][j].efficiencyLevel)
                    {
                        case LEVEL01:
                        {
                            efficiencyBuilding = 0.15f;
                            break;  
                        }
                        case LEVEL02:
                        {
                            efficiencyBuilding = 0.25f;
                            break;   
                        }
                        case LEVEL03:
                        {
                            efficiencyBuilding = 0.35f;
                            break;    
                        }
                        case LEVEL04:
                        {
                            efficiencyBuilding = 0.4f;
                            break;  
                        }
                        case LEVEL05:
                        {
                            efficiencyBuilding = 0.45f;
                            break;  
                        }
                        case LEVEL00:
                            return;
                    }
                    producedEnergy = 7.0f * ((1.225f * 78.54f * ((float)pow(randomWind, 3)) * efficiencyBuilding) / 2.0f);
                }
                producedEnergy+=producedEnergy;
            }
        }
    }
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
           UpdateConstruction(i, j, 300, 3, CONSTRUCTING_SOLARPANEL, SOLARPANEL);
           UpdateConstruction(i, j, 500, 5, CONSTRUCTING_WINDGENERATOR, WINDGENERATOR);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_EFFICIENCY), CalculateTurns(i, j, UPGRADING_EFFICIENCY), UPGRADING_EFFICIENCY, UPGRADING_SOLARPANEL, SOLARPANEL);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_RELIABILITY), CalculateTurns(i, j, UPGRADING_RELIABILITY), UPGRADING_RELIABILITY, UPGRADING_SOLARPANEL, SOLARPANEL);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_EFFICIENCY), CalculateTurns(i, j, UPGRADING_EFFICIENCY), UPGRADING_EFFICIENCY, UPGRADING_WINDGENERATOR, WINDGENERATOR);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_RELIABILITY), CalculateTurns(i, j, UPGRADING_RELIABILITY), UPGRADING_RELIABILITY, UPGRADING_WINDGENERATOR, WINDGENERATOR);
        }
    }

    turn++;

    float income = (producedEnergy/1000.0f) * 0.32;
    money+=income;

    randomWeather = GenerateWeatherCondition();
    randomWind = GenerateWind();
}