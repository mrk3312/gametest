#include "game.h"

void CellDraw(Cell *cell, int cellWidth, int cellHeight)
{ 
    if(!cell->isInactive)
    {
        Rectangle rect = {cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight};
        DrawRectangleLinesEx(rect, 1.f, BLACK);
    }
}

void CellPaint(Cell *cell, int cellWidth, int cellHeight)
{
    if(cell->building == SOLARPANEL && cell->isSelected == false)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, GREEN);
    else if(cell->building == WINDGENERATOR && cell->isSelected == false)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, YELLOW);
    else if(cell->isSelected)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, ORANGE);
    else if(!cell->isUnlocked && !cell->isInactive)
        DrawRectangle(cell->i * cellWidth, cell->j * cellHeight, cellWidth, cellHeight, RED);

}
