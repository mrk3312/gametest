#include "game.h"

void CellDraw(Cell *cell, int cellWidth, int cellHeight)
{ 
    if(!cell->isInactive && cell->isUnlocked)
    {
        Rectangle rect = {cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight};
        DrawRectangleLinesEx(rect, 1.f, BLACK);
    }
}

void CellPaint(Cell *cell, int cellWidth, int cellHeight)
{
    if(cell->building == SOLARPANEL && cell->isSelected == false)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, BLUE);
    else if(cell->building == WINDGENERATOR && cell->isSelected == false)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, YELLOW);
    else if(cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, ORANGE);
    else if(!cell->isUnlocked && !cell->isInactive  && !IsBorderingLockedCellsOnly(cell->i, cell->j))
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, DARKGREEN);
}
