#include "game.h"

void ResetButton(PressedButton *button)
{
    *button = WAITING;
}

const char* BuildingToString(Cell *cell)
{
    switch(cell->building)
    {
        case EMPTY:
        {
            return "Empty";
        }
        case SOLARPANEL:
        {
            return "Solar Panel";
        }
        case WINDGENERATOR:
        {
            return "Wind Generator";
        }
        case CONSTRUCTING_SOLARPANEL:
        {
            return "Constructing Solar Panel";
        }
        case CONSTRUCTING_WINDGENERATOR:
        {
            return "Constructing Wind Generator";
        }
        case UPGRADING_SOLARPANEL:
        {
            return "Upgrading Solar Panel";
        }
        case UPGRADING_WINDGENERATOR:
        {
            return "Upgrading Wind Generator";
        }
    }
    return "Building Unknown";
}


int CalculateMousePosition(int mPos, int cellDimension)
{
    int indexI = mPos / cellDimension;
    return indexI; 
}

void RemoveBuilding(Cell *cell)
{
    switch(cell->building)
    {
        case SOLARPANEL:
        {
            numberSolarPanel--;
            cell->building = EMPTY;
            break;
        }
        case WINDGENERATOR:
        {
            numberWindGenerator--;
            cell->building = EMPTY;
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

bool IsIndexValid(int i, int j)
{
   return i >= 0 && i < COLS && j >= 0 && j < ROWS;
}

bool IsBorderingLockedCellsOnly(int i, int j)
{   
    bool top = (i > 0)      ? !grid[i-1][j].isUnlocked : true;
    bool bottom = (i < 15)  ? !grid[i+1][j].isUnlocked : true;
    bool left = (j > 0)     ? !grid[i][j-1].isUnlocked : true;
    bool right = (j < 15)   ? !grid[i][j+1].isUnlocked : true; 
    return top && bottom && left && right;
}

bool isAffordable(float costs)
{
    return money>=costs;
}

void ButtonMenu(void)
{
    static bool panelActive = false;
    Rectangle endTurnButton = {350, 50, 50, 50};
    Rectangle infoButton = {400, 50, 50, 50};

    Rectangle panelBox = {0, 0, 800, 100};
    Rectangle panelBoxButton = {760, 0, 40, 23};

    int stateEndTurnButton = GuiButton(endTurnButton, "#131#");
    int stateInfoButton = GuiButton(infoButton, "#10#");
    int statePanelButton;

    if(stateInfoButton && !panelActive)
        panelActive = true;

    if(panelActive)
    {
        turnShow = false;

        GuiPanel(panelBox, "info");
        DrawText(TextFormat("Weather: %s\nWind Speed: %hu", WeatherToString(randomWeather), randomWind), 0, 25, 20, GRAY);
        DrawText(TextFormat("Number of Solar Panels: %d\nNumber of Wind Generators: %d\n", numberSolarPanel, numberWindGenerator), 450, 25, 20, GRAY);
        statePanelButton = GuiButton(panelBoxButton, "Close");
        if(statePanelButton)
        {
            turnShow = true;
            panelActive = false;
        }
    }

    if(stateEndTurnButton)
        EndTurn();

}

void EnterCellMenu(int i, int j)
{       
        grid[i][j].isSelected = true;
        inMenu = true;
        justExitedMenu = false;
}

bool HasEnoughCC(int usedConstructionCapabilities, int requiredConstructionCapabilities)
{
    return usedConstructionCapabilities>=requiredConstructionCapabilities;
}

void ExitCellMenu(Cell *cell, PressedButton *button, MenuState *menuState)
{
    *menuState = NONE;
    inMenu = false;
    justExitedMenu = true;
    cell->isSelected = false;
    ResetButton(button);
}

void CellMenu(int i, int j)
{
    Cell *cell = &grid[i][j];
    static MenuState menuState = NONE;
    static PressedButton stateButtonMenu01 = WAITING;
    static PressedButton stateButtonMenu02 = WAITING;
    static PressedButton stateButtonMenu03 = WAITING;

    Rectangle msgBoxRect = {250, 0, 300, 150};
    Rectangle upgradeRect01 = {460, 0, 75, 20};
    Rectangle upgradeRect02 = {460, 0, 75, 20};
    (cell->i > 4 && cell->i < 12 && cell->j > 4 && cell->j < 12) ? (msgBoxRect.y = 50, upgradeRect01.y = 110, upgradeRect02.y = 140) 
    : (msgBoxRect.y = 350, upgradeRect01.y = 410, upgradeRect02.y = 440);

    if(cell->isUnlocked && cell->building == EMPTY && !justExitedMenu)
    {
        switch(menuState)
        {
            case NONE:
            {
                menuState = MENU01;
                inMenu = true;
                break;
            }
            case MENU01:
            {   
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Info", "Building: Empty", "Build");
                if(stateButtonMenu01 == EXIT)
                {
                    ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                }
                else if (stateButtonMenu01 == BUTTON01)
                    menuState = MENU02;
                break;
            }
            case MENU02:
            {
                stateButtonMenu02 = GuiMessageBox(msgBoxRect, "Build Panel", "What would you like to build?", "Solar Panel;Wind Generator");
                if(stateButtonMenu02 == EXIT)
                {
                    ExitCellMenu(cell, &stateButtonMenu02, &menuState);
                }
                else if(stateButtonMenu02 == BUTTON01 || stateButtonMenu02 == BUTTON02)
                    menuState = MENU03;
                break;
            }
            case MENU03:
            {
                if(stateButtonMenu02 == BUTTON01)
                {   
                    stateButtonMenu03 = GuiMessageBox(msgBoxRect, "Build Panel", "Are you sure you want to buy\nthe solar panel for 3000", "Yes;Cancel");
                    if(stateButtonMenu03 == BUTTON01)
                    {
                        if(HasEnoughCC(usedConstructionCapabilities, 300))
                        {
                            usedConstructionCapabilities-=300;
                            cell->building = CONSTRUCTING_SOLARPANEL;
                            inMenu = false;
                            menuState = NONE;
                            cell->isSelected = false;
                        }
                        else
                            menuState = MENU04;
                    }
                    else if(stateButtonMenu03 == EXIT || stateButtonMenu03 == BUTTON02)
                    {
                        ExitCellMenu(cell, &stateButtonMenu03, &menuState);
                    }
                }
                else if(stateButtonMenu02 == BUTTON02)
                {
                    stateButtonMenu03 = GuiMessageBox(msgBoxRect, "Build Panel", "Are you sure you want to buy\nthe Wind Generator for 5000", "Yes;Cancel");
        
                    if(stateButtonMenu03 == BUTTON01)
                    {
                        if(HasEnoughCC(usedConstructionCapabilities, 500))
                        {
                            usedConstructionCapabilities-=500;
                            cell->building = CONSTRUCTING_WINDGENERATOR;
                            ExitCellMenu(cell, &stateButtonMenu03, &menuState);
                        }
                        else
                            menuState = MENU04;
                    }
                    else if(stateButtonMenu03 == EXIT || stateButtonMenu03 == BUTTON02)
                    {
                        ExitCellMenu(cell, &stateButtonMenu03, &menuState);
                    }               
                }
                break;
            }
            case MENU04:
            {
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Build Panel", "Not enough\nConstruction Capabilities", "OK");
                if(stateButtonMenu01 == EXIT || stateButtonMenu01 == BUTTON01)
                {
                    ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                }
                break;
            }
        }
    }

    if(!cell->isUnlocked)
    {
        switch(menuState)
        {
            case NONE:
            {
                menuState = MENU01;
                inMenu = true;
                break;
            }
            case MENU01:
            {
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Info", TextFormat("The cell is locked and can be\nbought for $%.0f", priceCell), "Buy");  
                if(stateButtonMenu01 == BUTTON01)
                {
                    if(isAffordable(priceCell))
                    {
                        money-=priceCell;
                        priceCell+=500;

                        cell->isUnlocked = true;
                        ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                    }            
                    else
                    {
                        menuState = MENU02;
                    }
                }
                else if(stateButtonMenu01 == EXIT)
                {
                    ExitCellMenu(cell, &stateButtonMenu01, &menuState);    
                }
                break;
            }
            case MENU02:
            {
                stateButtonMenu02 = GuiMessageBox(msgBoxRect, "Info", "Insufficient Funds!", "OK");
                
                if(stateButtonMenu02 == EXIT || stateButtonMenu02 == BUTTON01)
                {
                    ExitCellMenu(cell, &stateButtonMenu02, &menuState);
                }
                break;
            }
            case MENU03:
            case MENU04:
            {
                return;
                break;
            }
        }
    }
    else if(cell->building == SOLARPANEL || cell->building == WINDGENERATOR)
    {
        switch(menuState)
        {
            case NONE:
            {
                menuState = MENU01;
                inMenu = true;
                break;
            }

            case MENU01:
            {
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Info", TextFormat("Building: %s\n\nEfficiency: Level %d\n\nReliability: Level %d", BuildingToString(cell), cell->efficiencyLevel, cell->reliabilityLevel), "Destroy");
                if(cell->efficiencyLevel < 5)
                    stateButtonMenu02 = GuiButton(upgradeRect01, "Upgrade");
                if(cell->reliabilityLevel < 5)
                    stateButtonMenu03 = GuiButton(upgradeRect02, "Upgrade");

                if(stateButtonMenu01 == EXIT || stateButtonMenu01 == BUTTON02)
                {
                    ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                }
                else if (stateButtonMenu01 == BUTTON01)
                    menuState = MENU02;
                
                if(stateButtonMenu02 == BUTTON01 && cell->efficiencyLevel < 5)
                {
                    menuState = MENU03;
                }
                if(stateButtonMenu03 == BUTTON01 && cell->reliabilityLevel < 5)
                {
                    menuState = MENU04;
                }
                break;
            }

            case MENU02:
            {
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Destroy Panel", TextFormat("Are you sure you want\nto destroy the %s?", BuildingToString(cell)), "Yes;Cancel");

                if(stateButtonMenu01 == EXIT || stateButtonMenu01 == BUTTON02)
                {
                    ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                }
                else if(stateButtonMenu01 == BUTTON01)
                {
                    RemoveBuilding(cell);
                    ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                }
                
            break;
            }
            case MENU03:
            {
                stateButtonMenu02 = GuiMessageBox(msgBoxRect, "Upgrade Efficiency", TextFormat("Do you want to upgrade\nthe efficiency of this %s\nfrom Level %d to Level %d for $$$$", BuildingToString(cell), cell->efficiencyLevel, cell->efficiencyLevel + 1), "Yes;Cancel");
                if(stateButtonMenu02 == EXIT || stateButtonMenu02 == BUTTON02)
                {
                    ExitCellMenu(cell, &stateButtonMenu02, &menuState);
                }
                else if(stateButtonMenu02 == BUTTON01)
                {
                    const int calculateCC = CalculateConstructionCapabilityCost(i, j, UPGRADING_EFFICIENCY);
                    if(HasEnoughCC(usedConstructionCapabilities, calculateCC))
                    {
                        usedConstructionCapabilities-=calculateCC;
                        cell->constructionUpgradingStatus = UPGRADING_EFFICIENCY;
                        if(cell->building == SOLARPANEL)
                            cell->building = UPGRADING_SOLARPANEL;
                        else if(cell->building == WINDGENERATOR)
                            cell->building = UPGRADING_WINDGENERATOR;
                        ExitCellMenu(cell, &stateButtonMenu01, &menuState);
                    }
                }
                break;
            }
            case MENU04:
            {
                stateButtonMenu03 = GuiMessageBox(msgBoxRect, "Upgrade Reliability", TextFormat("Do you want to upgrade\nthe reliability of this %s\nfrom Level %d to Level %d for $$$$", BuildingToString(cell), cell->reliabilityLevel, cell->reliabilityLevel + 1), "Yes;Cancel");
                if(stateButtonMenu03 == EXIT || stateButtonMenu02 == BUTTON02)
                {
                    ExitCellMenu(cell, &stateButtonMenu03, &menuState);
                }
                else if(stateButtonMenu03 == BUTTON01)
                {
                    const int calculateCC = CalculateConstructionCapabilityCost(i, j, UPGRADING_RELIABILITY);
                    if(HasEnoughCC(usedConstructionCapabilities, calculateCC))
                    {
                        usedConstructionCapabilities-=calculateCC;
                        cell->constructionUpgradingStatus = UPGRADING_RELIABILITY;
                        if(cell->building == SOLARPANEL)
                            cell->building = UPGRADING_SOLARPANEL;
                        else if(cell->building == WINDGENERATOR)
                            cell->building = UPGRADING_WINDGENERATOR;
                        ExitCellMenu(cell, &stateButtonMenu03, &menuState);
                    }
                }
                break;
            }
        }
    }
}




