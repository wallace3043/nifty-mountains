/// draws the path given

#include "niftyMountains.h"

using namespace std;

void drawPath(PATH p, ALLEGRO_COLOR colour) {
    int cRow = p.sRow; // current row
    for (int i = 0; i < p.steps.length() - 1; i++) {
        al_draw_pixel(i, cRow, colour);
        cRow += p.steps[i];
        //al_flip_display(); // draws in real time, kinda slow
    }
    al_draw_pixel(p.steps.length(), cRow, colour);
    al_flip_display();
}

int drawLowestElevPath(apmatrix<short> elevations, int startingRow, ALLEGRO_COLOR colour) {
    PATH p;
    p.sRow = startingRow;
    p.steps.resize(elevations.numcols() - 1);
    p.totalElevC = determineGreedyPath(elevations, p);
    drawPath(p, colour);
    al_flip_display();
    return p.totalElevC;
}
