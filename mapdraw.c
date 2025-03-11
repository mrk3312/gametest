#include "game.h"

void CellDraw(int i, int j, int cellWidth, int cellHeight)
{ 
    Cell *cell = &grid[i][j];
    if(!cell->isInactive && cell->isUnlocked)
    {
        Rectangle rect = {cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight};
        DrawRectangleLinesEx(rect, 1.f, BLACK);
    }
}

void CellPaint(int i, int j, int cellWidth, int cellHeight)
{
    Cell *cell = &grid[i][j];
    if(cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, ORANGE);
    else if(cell->building == SOLARPANEL && !cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, BLUE);
    else if(cell->building == WINDGENERATOR && !cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, YELLOW);
    else if((cell->building == CONSTRUCTING_SOLARPANEL || cell->building == CONSTRUCTING_WINDGENERATOR) && !cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, GRAY);
    else if((cell->building == UPGRADING_SOLARPANEL || cell->building == UPGRADING_WINDGENERATOR) && !cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, DARKPURPLE);
    else if(!cell->isUnlocked && !cell->isInactive  && !IsBorderingLockedCellsOnly(cell->i, cell->j))
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, DARKGREEN);   
}
