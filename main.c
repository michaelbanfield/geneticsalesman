/* 
 * File:   main.c
 * Author: michaelb
 *
 * Created on 29 July 2013, 8:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "city.h"
#include "tour.h"

//global variables

City cities[10];
Tour tour;
Tour * ptrTour = &tour;


/*
 * 
 */

int main(int argc, char** argv) {
    FILE* mapFile;
    
    int x = 0, y = 0, i = 0;
    
    if (argc > 1) {
        mapFile = fopen(argv[1], "r");
        if (mapFile == NULL) {
            return 2;
        }
    }
    
    while(fscanf(mapFile, "%d %d\n", &cities[i].x, &cities[i].y) == 2) {
        printf("%d %d\n", cities[i].x, cities[i].y);
        i++;
    }
    createPath(ptrTour, 20);
    printf("%f", getDistance(cities, tour, 20));
    
    
    
    
    
    return (EXIT_SUCCESS);
}

