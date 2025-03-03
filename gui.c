#include "game.h"

bool IsIndexValid(int i, int j)
{
   return i >= 0 && i < COLS && j >= 0 && j < ROWS;
}

bool isAffordable(float costs)
{
    return money-costs > 0;
}

void ButtonMenu(void)
{
    static bool panelActive = false;
    Rectangle endTurnButton = {0, 50, 50, 50};
    Rectangle infoButton = {50, 50, 50, 50};

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
        DrawText(TextFormat("Weather: \nWind Speed: "), 0, 25, 20, GRAY);
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
        grid[i][j].isInMenu01 = true;
        grid[i][j].isSelected = true;
        inMenu = true;
}

void BoxMenu(Cell *cell)
{
    int stateButtonMenu01;
    static int stateButtonMenu02;
    int stateButtonMenu03;

    Rectangle msgBoxRect = {0, 0, 200, 100};

    if(cell->i > 4 && cell->i < 12 && cell->j > 4 && cell->j < 12)
    {
        msgBoxRect.x = 300;
        msgBoxRect.y = 50;
    }

    else
    {
        msgBoxRect.x = 300;
        msgBoxRect.y = 350;
    }

    if(cell->isUnlocked && cell->building == EMPTY && justExitedMenu == false)
    {
        if(!cell->isInMenu02)
        {   
            stateButtonMenu01 = GuiMessageBox(msgBoxRect,
                "Info", TextFormat("Building: %d\n", cell->building), "Build");
            if(stateButtonMenu01 == 0)
            {
                inMenu = false;
                cell->isInMenu01 = false;
                cell->isSelected = false;
            }
            else if (stateButtonMenu01 == 1)
                cell->isInMenu02 = true;
        }
        else
        {
            if(!cell->isInMenu03)
            {
                stateButtonMenu02 = GuiMessageBox(msgBoxRect,
                "Build Panel", "What would you like to build?", "Solar Panel;Wind Generator");
                if(stateButtonMenu02 == 0)
                {
                    inMenu = false;
                    cell->isInMenu01 = false;
                    cell->isInMenu02 = false;
                    cell->isSelected = false;
                }
                else if(stateButtonMenu02 == 1 || stateButtonMenu02 == 2)
                {
                    cell->isInMenu03 = true;
                }
            }
            else
            {
                if(stateButtonMenu02 == 1)
                {   
                    stateButtonMenu03 = GuiMessageBox(msgBoxRect,
                    "Build Panel", "Are you sure you want to buy\nthe solar panel for 3000", "Yes;Cancel");
                    if(stateButtonMenu03 == 1)
                    {
                        inMenu = false;
                        cell->building = SOLARPANEL;
                        cell->isInMenu01 = false;
                        cell->isInMenu02 = false;
                        cell->isInMenu03 = false;
                        cell->isSelected = false;
                    }

                    else if(stateButtonMenu03 == 0 || stateButtonMenu03 == 2)
                    {
                        inMenu = false;
                        cell->isSelected = false;
                        cell->isInMenu01 = false;
                        cell->isInMenu02 = false;
                        cell->isInMenu03 = false;
                    }
                }
                else if(stateButtonMenu02 == 2)
                {
                    stateButtonMenu03 = GuiMessageBox(msgBoxRect,
                    "Build Panel", "Are you sure you want to buy\nthe Wind Generator for 5000", "Yes;Cancel");
        
                    if(stateButtonMenu03 == 1)
                    {
                        cell->building = WINDGENERATOR;
                        
                        inMenu = false;
                        cell->isInMenu01 = false;
                        cell->isInMenu02 = false;
                        cell->isInMenu03 = false;
                        cell->isSelected = false;
                    }
                    else if(stateButtonMenu03 == 0 || stateButtonMenu03 == 2)
                    {
                        inMenu = false;
                        cell->isInMenu01 = false;
                        cell->isInMenu02 = false;
                        cell->isInMenu03 = false;
                        cell->isSelected = false;
                    }               
                }
            }
        }
    }
    else if(!cell->isUnlocked)
    {
        if(!cell->isInMenu02)
        {
            stateButtonMenu01 = GuiMessageBox(msgBoxRect,
                "Info", TextFormat("The cell is locked and can be\nbought for $%.0f", priceCell), "Buy");  
            if(stateButtonMenu01 == 1)
            {
                if(isAffordable(priceCell))
                {
                    money-=priceCell;
                    priceCell+=500;

                    inMenu = false; 
                    justExitedMenu = true;
                    cell->isUnlocked = true;
                    cell->isInMenu01 = false;
                    cell->isSelected = false;
                }            
                else
                {
                    cell->isInMenu02 = true;
                }
            }
            else if(stateButtonMenu01 == 0)
            {
                inMenu = false;  
                justExitedMenu = true;  
                cell->isInMenu01 = false;
                cell->isSelected = false;       
            }
        }
        else
        {
            stateButtonMenu02 = GuiMessageBox(msgBoxRect,
                "Info", "Insufficient Funds!", "OK");
            
            if(stateButtonMenu02 == 0 || stateButtonMenu02 == 1)
            {
                inMenu = false;  
                justExitedMenu = true;  
                cell->isInMenu01 = false;
                cell->isInMenu02 = false;
                cell->isSelected = false;  
            }
        }
    }
    else
    {
        if(!cell->isInMenu02)
        {   
            stateButtonMenu01 = GuiMessageBox(msgBoxRect,
                "Info", TextFormat("Building: %d\n", cell->building), "Destroy");

            if(stateButtonMenu01 == 0)
            {
                inMenu = false;
                cell->isInMenu01 = false;
                cell->isSelected = false;
            }
            else if (stateButtonMenu01 == 1)
                cell->isInMenu02 = true;
        }
        else
        {
            stateButtonMenu03 = GuiMessageBox(msgBoxRect,
                "Destroy Panel", "Are you sure you want to destroy the Building?", "Yes;Cancel");
            if(stateButtonMenu03 == 0 || stateButtonMenu03 == 2)
            {
                inMenu = false;
                cell->isInMenu01 = false;
                cell->isInMenu02 = false;
                cell->isSelected = false;
            }
            else if(stateButtonMenu03 == 1)
            {
                if(cell->building == SOLARPANEL)
                    numberSolarPanel--;
                else if(cell->building == WINDGENERATOR)
                    numberWindGenerator--;
                
                inMenu = false;
                justExitedMenu = true;
                cell->building = EMPTY;
                cell->calculatedBuilding = false;
                cell->isInMenu01 = false;
                cell->isInMenu02 = false;
                cell->isSelected = false;
            }
        }
    }
}
