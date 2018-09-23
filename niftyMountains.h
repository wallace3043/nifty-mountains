#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <apvector.h>
#include <apmatrix.h>
#include <cmath>
#include <random>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#define COLUMNS 844
#define ROWS 480
#define UP -1
#define SAME 0
#define DOWN 1
#define TREELINE_WEIGHT 0.432 // percent up the mountain, 1 means it's all trees

#define RED al_map_rgb(255, 0, 0)
#define GREEN al_map_rgb(0, 255, 0)
#define BLUE al_map_rgb(0, 0, 255)
#define PURPLE al_map_rgb(128, 0, 128)
#define YELLOW al_map_rgb(255, 255, 0)
#define HOT_PINK al_map_rgb(248, 24, 148)
#define CYAN al_map_rgb(0, 255, 255)
#define BLACK al_map_rgb(255, 255, 255)
#define WHITE al_map_rgb(0, 0, 0)
#define BROWN al_map_rgb(102, 27, 4)
#define OLIVE al_map_rgb(94, 89, 25)
#define FOREST al_map_rgb(163, 216, 4)

#define WRONG RED
#define RIGHT BLUE
#define LOWEST PURPLE
#define BEST CYAN

const std::string FILENAME = "Colorado_844x480.dat";

class PATH {
    public:
        apvector<short> steps;
        int sRow; // starting row
        int totalElevC;
        bool gotten; // data has been evaluated, for saving time when determining best path

        void cpy(PATH p) {
            sRow = p.sRow;
            totalElevC = p.totalElevC;
            steps.resize(p.steps.length());
            steps = p.steps;
        }
};

// niftyMountains.cpp
int initializeAllegro(int width, int height, const char title[]);

// determinePath.cpp
int determineGreedyPath(apmatrix<short> elevations, PATH &p);
int randInt(int low, int high);
short determineGreedyDirection(short upC, short sameC, short downC);
PATH determineBestPath(apmatrix<short> elevations, int row, int col);

// drawRange.cpp
void drawColumnLowest(apmatrix<short> elevations, int column, ALLEGRO_COLOR colour);
void drawAllColumnLowests(apmatrix<short> elevations, ALLEGRO_COLOR colour);
void drawRange(apmatrix<short> elevations);
ALLEGRO_COLOR colourInterpolation (int lowest, int highest, int elevation);
ALLEGRO_COLOR greyscaleInterpolation (int lowest, int highest, int elevation);

// drawPath.cpp
void drawPath(PATH p, ALLEGRO_COLOR colour);
int drawLowestElevPath(apmatrix<short> elevations, int startingRow, ALLEGRO_COLOR colour);
