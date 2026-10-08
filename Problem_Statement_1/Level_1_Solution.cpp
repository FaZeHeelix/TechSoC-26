#include <iostream>
#include <vector>
#include <string>

int liveneighbors(const std::vector<std::vector<char>>& grid, int r, int c, int rows, int columns) {
    
    int num = 0;
    if (r != 0 && grid[r-1][c] == '#') num += 1;
    if (r != 0 && c != columns-1 && grid[r-1][c+1] == '#') num += 1;
    if (r != 0 && c != 0 && grid[r-1][c-1] == '#') num += 1;
    if (c != 0 && grid[r][c-1] == '#') num += 1;
    if (c != columns-1 && grid[r][c+1] == '#') num += 1;
    if (r != rows-1 && c != 0 && grid[r+1][c-1] == '#') num += 1;
    if (r != rows-1 && grid[r+1][c] == '#') num += 1;
    if (r != rows-1 && c != columns-1 && grid[r+1][c+1] == '#') num += 1;
    return num;
}

int generate(std::vector<std::vector<char>>& grid, int rows, int columns) {
    int currentcount = 0;
    std::vector<std::vector<char>> newgrid(rows, std::vector<char>(columns));
    
    for (int r = 0; r<rows; r++) {
        for(int c = 0; c<columns; c++) {
            if (grid[r][c] == '.') newgrid[r][c] = '.';
            else if (grid[r][c] == '#') newgrid[r][c] = '#';
        }
    }//Making the new grid
    for (int r = 0; r<rows; r++) {
        for(int c = 0; c<columns; c++) {
            if (grid[r][c] == '#' && liveneighbors(grid,r,c,rows,columns) < 2) newgrid[r][c] = '.';
            else if (grid[r][c] == '#' && liveneighbors(grid,r,c,rows,columns) > 3) newgrid[r][c] = '.';
            else if (grid[r][c] == '.' && liveneighbors(grid,r,c,rows,columns) == 3) newgrid[r][c] = '#';
            }
                    }//Making the changes to new grid
    for (int r = 0; r<rows; r++) {
        for (int c = 0; c<columns; c++) {
            grid[r][c] = newgrid[r][c];
        }
    }//Converting the old grid into new one exactly
    for (int r = 0; r<rows; r++) {
        for (int c = 0; c<columns; c++) {
            if (grid[r][c] == '#') currentcount += 1;
        }
    }//Counting live in this grid
    return currentcount;
}
int main() {
    
    int rows, columns, gens;
    int oldcount = 0, newcount = 0;
    std::cin >> rows;
    std::cin >> columns;
    std::cin >> gens;

    std::vector<std::vector<char>> grid(rows, std::vector<char>(columns));
    
    for (int r = 0; r<rows; r++) {
        for (int c = 0; c<columns; c++) {
            std::cin >> grid[r][c];
        }
    }//Making the original grid
    for (int r = 0; r<rows; r++) {
        for (int c = 0; c<columns; c++) {
            if (grid[r][c] == '#') oldcount += 1;
        }
    }//Counting live in original grid
    
    std::cout << "Initiall Population: " << oldcount << std::endl;
   
    int peak = oldcount;
    for(int i = 0; i < gens; i++) {
        int count = generate(grid, rows, columns);
        if (count>peak) peak = count;
        
    }//Making the changes in loop and setting peak
    for (int r = 0; r<rows; r++) {
        for (int c = 0; c<columns; c++) {
            if (grid[r][c] == '#') newcount += 1;
        }
    }//Counting live in new grid
    
    std::cout << "Final Population: " << newcount << std::endl;
    std::cout << "Peak Population: " << peak << std::endl;
    std::cout << "Final Grid: " << std::endl;
    
    for (int r = 0; r<rows; r++) {
        for(int c = 0; c<columns; c++) {
            std::cout << grid[r][c] << "\t";
        }
        std::cout << std::endl;
    }//Printing the new grid
}