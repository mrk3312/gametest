#include "game.h"

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

void CellPanel(int i, int j)
{       
        grid[i][j].isSelected = true;
        inMenu = true;
        justExitedMenu = false;
}

bool HasEnoughCC(int usedConstructionCapabilities, int requiredConstructionCapabilities)
{
    return usedConstructionCapabilities>=requiredConstructionCapabilities;
}

void BoxMenu(int i, int j)
{
    Cell *cell = &grid[i][j];
    static MenuState menuState = NONE;
    static PressedButton stateButtonMenu01 = WAITING;
    static PressedButton stateButtonMenu02 = WAITING;
    static PressedButton stateButtonMenu03 = WAITING;

    Rectangle msgBoxRect = {300, 0, 200, 100};
    (cell->i > 4 && cell->i < 12 && cell->j > 4 && cell->j < 12) ? (msgBoxRect.y = 50) : (msgBoxRect.y = 350);

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
                    inMenu = false;
                    menuState = NONE;
                    cell->isSelected = false;
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
                    inMenu = false;
                    cell->isSelected = false;
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
                        menuState = NONE;
                        inMenu = false;
                        cell->isSelected = false;
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
                            inMenu = false;
                            menuState = NONE;
                            cell->isSelected = false;
                        }
                        else
                            menuState = MENU04;
                    }
                    else if(stateButtonMenu03 == EXIT || stateButtonMenu03 == BUTTON02)
                    {
                        inMenu = false;
                        menuState = NONE;
                        cell->isSelected = false;
                    }               
                }
                break;
            }
            case MENU04:
            {
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Build Panel", "Not enough\nConstruction Capabilities", "OK");
                if(stateButtonMenu01 == EXIT || stateButtonMenu01 == BUTTON01)
                {
                    inMenu = false;
                    menuState = NONE;
                    cell->isSelected = false;
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

                        inMenu = false; 
                        justExitedMenu = true;
                        cell->isUnlocked = true;
                        menuState = NONE;
                        cell->isSelected = false;
                    }            
                    else
                    {
                        menuState = MENU02;
                    }
                }
                else if(stateButtonMenu01 == EXIT)
                {
                    inMenu = false;  
                    justExitedMenu = true;  
                    menuState = NONE;
                    cell->isSelected = false;       
                }
                break;
            }
            case MENU02:
            {
                stateButtonMenu02 = GuiMessageBox(msgBoxRect, "Info", "Insufficient Funds!", "OK");
                
                if(stateButtonMenu02 == EXIT || stateButtonMenu02 == BUTTON01)
                {
                    inMenu = false;  
                    justExitedMenu = true;  
                    menuState = NONE;
                    cell->isSelected = false;  
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
                stateButtonMenu01 = GuiMessageBox(msgBoxRect, "Info", TextFormat("Building: %s\n", BuildingToString(cell)), "Destroy");

                if(stateButtonMenu01 == EXIT)
                {
                    inMenu = false;
                    cell->isSelected = false;
                    menuState = NONE;
                }
                else if (stateButtonMenu01 == BUTTON01)
                    menuState = MENU02;
                break;
            }

            case MENU02:
            {
                stateButtonMenu03 = GuiMessageBox(msgBoxRect, "Destroy Panel", TextFormat("Are you sure you want\nto destroy the %s?", BuildingToString(cell)), "Yes;Cancel");

                if(stateButtonMenu03 == EXIT || stateButtonMenu03 == BUTTON02)
                {
                    inMenu = false;
                    cell->isSelected = false;
                    menuState = NONE;
                }
                else if(stateButtonMenu03 == BUTTON01)
                {
                    RemoveBuilding(cell);
                    inMenu = false;
                    justExitedMenu = true;
                    cell->building = EMPTY;
                    cell->isSelected = false;
                    menuState = NONE;
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
    else if(cell->building == CONSTRUCTING_SOLARPANEL || cell->building == CONSTRUCTING_WINDGENERATOR)
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

            }
        }
    }
}




