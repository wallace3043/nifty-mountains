/// draws the greyscale of the mountain range

#include "niftyMountains.h"

using namespace std;

void drawColumnLowest(apmatrix<short> elevations, int column, ALLEGRO_COLOR colour) {
    short lowest = elevations[0][column];
    short lowestI = 0;
    for (int i = 0; i < elevations.numrows(); i++) {
        if (lowest > elevations[i][column]) {
            lowest = elevations[i][column];
            lowestI = i;
        }
    }
    al_draw_pixel(column, lowestI, colour);
    al_flip_display();
}

void drawAllColumnLowests(apmatrix<short> elevations, ALLEGRO_COLOR colour) {
    for (int i = 0; i < elevations.numcols(); i++) {
        drawColumnLowest(elevations, i, colour);
    }
    al_flip_display();
}

void drawRange(apmatrix<short> elevations) {
    int highest = elevations[0][0];
    int lowest = elevations[0][0];

    // determine highest, lowest elevations
    for (int i = 0; i < elevations.numrows(); i++) {
        for (int j = 0; j < elevations.numcols(); j++) {
            if (highest < elevations[i][j])
                highest = elevations[i][j];
            if (lowest > elevations[i][j])
                lowest = elevations[i][j];
        }
    }

    // draw to display
    for (int i = 0; i < elevations.numrows(); i++) {
        for (int j = 0; j < elevations.numcols(); j++) {
            //al_draw_pixel(j, i, greyscaleInterpolation(lowest, highest, elevations[i][j])); //draws pixels
            al_draw_pixel(j, i, colourInterpolation(lowest, highest, elevations[i][j])); //draws pixels
        }
    }

    al_flip_display();
}

ALLEGRO_COLOR greyscaleInterpolation (int lowest, int highest, int elevation) {
    int greyness = 255.0 * (elevation - lowest) / (highest - lowest);
    return al_map_rgb(greyness, greyness, greyness);
}

// interpolates the colour
ALLEGRO_COLOR colourInterpolation (int lowest, int highest, int elevation) {
    ALLEGRO_COLOR colour;
    int treeline = TREELINE_WEIGHT * highest + (1 - TREELINE_WEIGHT) * lowest;
    if (elevation < treeline) { // if it's below the treeline, it's a gradient from forest green to olive green
        colour.r = FOREST.r + (OLIVE.r - FOREST.r) * (float(elevation - lowest) / treeline);
        colour.g = FOREST.g + (OLIVE.g - FOREST.g) * (float(elevation - lowest) / treeline);
        colour.b = FOREST.b + (OLIVE.b - FOREST.b) * (float(elevation - lowest) / treeline);
    } else { // else, it's a gradient from olive green to dirt brown
        colour.r = BROWN.r + (OLIVE.r - BROWN.r) * (float(highest - elevation) / (highest - treeline));
        colour.g = BROWN.g + (OLIVE.g - BROWN.g) * (float(highest - elevation) / (highest - treeline));
        colour.b = BROWN.b + (OLIVE.b - BROWN.b) * (float(highest - elevation) / (highest - treeline));
    }

    return colour;
}
