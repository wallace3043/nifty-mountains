/***
* Wallace Long
* 17/09/18
* Nifty mountains
* path of least resistance, aka least elevation change
* greedy: done
* lowest of all columns: done
* best: done
***/

#include "niftyMountains.h"

using namespace std;

ALLEGRO_DISPLAY *display;
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *event_queue;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    initializeAllegro(COLUMNS, ROWS, "Nifty Mountains");

    ifstream fin(FILENAME);
    if (!fin) {
        cout << "could not open " << FILENAME << endl;
        return 404;
    }

    // create the elevation map, read the data in
    apmatrix<short> elevations(ROWS, COLUMNS, 0);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            fin >> elevations[i][j];
        }
    }
    drawRange(elevations);

    int bestGreedyStartI = 0; // index of the best greedy path starting row
    int bestGreedyElevC = drawLowestElevPath(elevations, bestGreedyStartI, WRONG);
    for (int i = 1; i < elevations.numrows(); i++) {
        int totalElevC = drawLowestElevPath(elevations, i, WRONG);
        if (totalElevC < bestGreedyElevC) {
            bestGreedyElevC = totalElevC;
            bestGreedyStartI = i;
        }
    }

    cout << "Starting on row " << bestGreedyStartI + 1 << ": total elevation change of " <<
        drawLowestElevPath(elevations, bestGreedyStartI, RIGHT) << endl;

    // this highlights the spots of the lowest elevation in each column
    cout << "Drawing the lowest elevations of each column in the colour " << "\"purple\"" << endl;
    drawAllColumnLowests(elevations, LOWEST);

    ///* should work in theory but it might take like 20 minutes, don't know how to cut down on time any further
    cout << "Determining the best possible path (this may take a while)" << endl;
    int bestEndI = 0; // row index of the row on which the best route ends
    PATH bestPath = determineBestPath(elevations, bestEndI, elevations.numcols() - 1);
    for (int i = 1; i < elevations.numrows(); i++) {
        if (determineBestPath(elevations, i, elevations.numcols() - 1).totalElevC < bestPath.totalElevC) {
            bestEndI = i;
            bestPath = determineBestPath(elevations, bestEndI, elevations.numcols() - 1);
        }
    }

    drawPath(bestPath, BEST);
    cout << "Starting on row " << bestPath.sRow + 1 << " and ending on row " << bestEndI + 1 << ": total elevation change of " <<
        bestPath.totalElevC << endl;
    //*/

    /* test
    drawPath(determineBestPath(elevations, 6,6), BEST);
    cout << determineBestPath(elevations, 6,6).totalElevC<<endl;
    */

    system("pause");

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    return 0;
}

int initializeAllegro(int width, int height, const char title[]) {

    const float FPS = 32;			// set frame rate

    // Initialize Allegro
	al_init();

	// initialize display
	display = al_create_display(width, height);
	if (!display) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
       	return -1;
	}
	al_set_window_title(display, title);

   	// Initialize keyboard routines
	if (!al_install_keyboard()) {
	    al_show_native_message_box(display, "Error", "Error", "failed to initialize the keyboard!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
     	return -1;
   	}

	// need to add image processor
 	if (!al_init_image_addon()) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize image addon!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}
	// 2. setup timer
	timer = al_create_timer(1.0 / FPS);
   	if (!timer) {
   		al_show_native_message_box(display, "Error", "Error", "Failed to create timer!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Initialize primative add on
 	if (!al_init_primitives_addon()) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize primatives addon!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}

	// set up event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
 	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_flip_display();
	return 0;
}
