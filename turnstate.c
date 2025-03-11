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

bool IsCellConstructing(Cell *cell, const int ConstructionTurns, const BuildingType building)
{
    return !cell->isInactive && cell->isUnlocked && cell->turnsConstructing < ConstructionTurns && cell->building == building;
}

bool IsCellUpgrading(Cell *cell, const int upgradeTurns, const UpgradeStatus upgradeStatus)
{
    return !cell->isInactive && cell->isUnlocked && cell->turnsConstructing < upgradeTurns && cell->constructionUpgradingStatus == upgradeStatus;
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

void UpgradeConstruction(int i, int j, const int requiredConstructionCapabilities, const int upgradeTurns, const BuildingType building, const UpgradeStatus typeOfUpgrade)
{
    Cell *cell = &grid[i][j];
    if(IsCellUpgrading(cell, upgradeTurns, typeOfUpgrade))
    {
        if(cell->turnsConstructing == upgradeTurns - 1)
        {
            cell->building = building;
            cell->turnsConstructing = 0;
            usedConstructionCapabilities+=requiredConstructionCapabilities;
            switch(typeOfUpgrade)
            {
                case NOTUPGRADING:
                    break;
                case UPGRADING_EFFICIENCY:
                {
                    cell->efficiencyLevel++;
                    cell->constructionUpgradingStatus = NOTUPGRADING;
                    break;
                }
                case UPGRADING_RELIABILITY:
                {
                    cell->reliabilityLevel++;
                    cell->constructionUpgradingStatus = NOTUPGRADING;
                    break;
                }
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


// Calculate how much Construction capability will the upgrade cost
const int CalculateConstructionCapabilityCost(int i, int j, UpgradeStatus typeOfUpgrade)
{
    Cell *cell = &grid[i][j];
    
    if(cell->building == SOLARPANEL)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 300;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->efficiencyLevel + 1) * 350;
            }
            case NOTUPGRADING:
            break;
        }
    }
    else if(cell->building == WINDGENERATOR)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 500;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->efficiencyLevel + 1) * 550;
            }
            case NOTUPGRADING:
                break;
        }
    }
}

const int CalculateTurns(int i, int j, UpgradeStatus typeOfUpgrade)
{
    Cell *cell = &grid[i][j];

    if(cell->building == SOLARPANEL)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 3;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->efficiencyLevel + 1) * 4;
            }
            case NOTUPGRADING:
            break;
        }
    }
    else if(cell->building == WINDGENERATOR)
    {
        switch(typeOfUpgrade)
        {
            case UPGRADING_EFFICIENCY:
            {
                return (cell->efficiencyLevel + 1) * 5;
            }
            case UPGRADING_RELIABILITY:
            {
                return (cell->efficiencyLevel + 1) * 6;
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
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_EFFICIENCY), CalculateTurns(i, j, UPGRADING_EFFICIENCY), SOLARPANEL, UPGRADING_EFFICIENCY);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_RELIABILITY), CalculateTurns(i, j, UPGRADING_RELIABILITY), SOLARPANEL, UPGRADING_RELIABILITY);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_EFFICIENCY), CalculateTurns(i, j, UPGRADING_EFFICIENCY), WINDGENERATOR, UPGRADING_EFFICIENCY);
           UpgradeConstruction(i, j, CalculateConstructionCapabilityCost(i, j, UPGRADING_RELIABILITY), CalculateTurns(i, j, UPGRADING_RELIABILITY), WINDGENERATOR, UPGRADING_RELIABILITY);
        }
    }

    turn++;

    float income = (totalProducedEnergy/1000) * 0.32;
    money+=income;

    randomWind = GenerateNumber(22);
    randomWeather = GenerateNumber(6);
}