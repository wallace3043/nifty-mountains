/// determineGreedyPath determines the greediest path from a specified starting point
/// determineBestPath determines the best path starting anywhere on the left going to anywhere on the right

#include "niftyMountains.h"

using namespace std;

int randInt(int low, int high) {
    return rand() % (high - low + 1) + low;
}

int determineGreedyPath(apmatrix<short> elevations, PATH &p) {
    static int nRows = elevations.numrows();
    static int nCols = elevations.numcols();
    p.steps.resize(nCols); // first int is the starting row

    int totalElevC = 0;
    int cRow = p.sRow;
    // cRow is used to track where you are
    for (int i = 0; i < nCols - 1; i++) {
        short cElev = elevations[cRow][i]; // current elevation
        short upC, sameC, downC; // C stands for change

        sameC = abs(cElev - elevations[cRow][i + 1]);
        if (cRow != 0) // can go up
            upC = abs(cElev - elevations[cRow - 1][i + 1]);
        else
            upC = sameC + 1; // guarantees to be larger
        if (cRow != nRows - 1) // can go down
            downC = abs(cElev - elevations[cRow + 1][i + 1]);
        else
            downC = sameC + 1;

        p.steps[i] = determineGreedyDirection(upC, sameC, downC);
        cRow += p.steps[i];

        // add change to running total
        if (p.steps[i] == UP) {
            totalElevC += upC;
        } else if (p.steps[i] == SAME) {
            totalElevC += sameC;
        } else {
            totalElevC += downC;
        }
    }

    return totalElevC;
}

short determineGreedyDirection(short upC, short sameC, short downC) {
    //if tied, go forwards if it's an option, otherwise a 50/50 b/w up/down
    // one smallest
    if (upC < sameC && upC < downC)
        return UP;
    if (sameC < upC && sameC < downC)
        return SAME;
    if (downC < upC && downC < sameC)
        return DOWN;

    // all equal
    if (upC == sameC && sameC == downC)
        return SAME;

    if (upC == downC && upC < sameC) {
        if (randInt(0, 1) == 0)
            return UP;
        else
            return DOWN;
    }

    return SAME;
}

// should work, never tested, but should also take a heck of a lot of time (which is why i haven't tested to completion)
PATH determineBestPath(apmatrix<short> elevations, int row, int col) {
    static int nRows = elevations.numrows();
    static int nCols = elevations.numcols();
    static apmatrix<PATH> allPaths(nRows, nCols);
     // printing probably slows the process down by quite a bit, better to just comment out, has to do like 480*844 evaluations

    //system("pause");
    if (!allPaths[row][col].gotten) { // avoid reevaluation
        //cout << "determining best path of " << row << ", " << col << endl;
        if (col == 0) {
            // "inductive step" (if that's what it's called)
            allPaths[row][col].steps.resize(col);
            allPaths[row][col].sRow = row;
            allPaths[row][col].totalElevC = 0;
            allPaths[row][col].gotten = true;
        } else {
            // determine min path to get to a coordinate based on the 3 that could get to it
            short cElev = elevations[row][col];
            short upBC, sameBC, downBC; // up backwards change, i.e. down if walking forwards
            int upBTC, sameBTC, downBTC; // up backwards total change, i.e. elevation change for 1 step + all accumulated to get there, not including the potential change in the line above

            sameBC = abs(elevations[row][col - 1] - cElev);
            if (row != 0)
                downBC = abs(elevations[row - 1][col - 1] - cElev);
            else
                downBC = sameBC + 1; // automatically higher than going straight
            if (row != nRows - 1)
                upBC = abs(elevations[row + 1][col - 1] - cElev);
            else
                upBC = sameBC + 1; // same deal

            sameBTC = determineBestPath(elevations, row, col - 1).totalElevC;
            if (row != 0)
                downBTC = determineBestPath(elevations, row - 1, col - 1).totalElevC;
            else
                downBTC = sameBTC + 1; // same deal
            if (row != nRows - 1)
                upBTC = determineBestPath(elevations, row + 1, col - 1).totalElevC;
            else
                upBTC = sameBTC + 1; // same deal

            // adding the next step to the steps vector according to what would be the smallest accumulated change
            short direction = determineGreedyDirection(downBC + downBTC, sameBC + sameBTC, upBC + upBTC);
            switch(direction) {
                case UP:
                    allPaths[row][col].cpy(allPaths[row - 1][col - 1]);
                    allPaths[row][col].steps.resize(col);
                    allPaths[row][col].steps[col - 1] = UP;
                    allPaths[row][col].totalElevC += downBC;
                    break;
                case SAME:
                default:
                    allPaths[row][col].cpy(allPaths[row][col - 1]);
                    allPaths[row][col].steps.resize(col);
                    allPaths[row][col].steps[col - 1] = SAME;
                    allPaths[row][col].totalElevC += sameBC;
                    break;
                case DOWN:
                    allPaths[row][col].cpy(allPaths[row + 1][col - 1]);
                    allPaths[row][col].steps.resize(col);
                    allPaths[row][col].steps[col - 1] = DOWN;
                    allPaths[row][col].totalElevC += upBC;
                    break;
            }
            allPaths[row][col].gotten = true;
        }
    }

    return allPaths[row][col];
}
