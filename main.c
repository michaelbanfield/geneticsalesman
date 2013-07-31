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
#include "population.h"

//global variables

City cities[10];
Tour tour;
Tour * ptrTour = &tour;
Population population;
Population * ptrPopulation = &population;

/*
 * 
 */






int getFittest(int size) {
    int count = 4;
    population.fittest = 0;
    for (count = 0; count < size; count++) {
    
    printArray(population.tours[count].path, 10);
        
        population.tours[count].distance = getDistance(cities, &population.tours[count], 10);
        printf("This is the float: %f\n", population.tours[count].distance);
        if( population.tours[count].distance < population.tours[population.fittest].distance) {
            population.fittest = count;
            
        }
    }
    return population.fittest;
}


int main(int argc, char** argv) {
    FILE* mapFile;

    int x = 0, y = 0, i = 0;

    if (argc > 1) {
        mapFile = fopen(argv[1], "r");
        if (mapFile == NULL) {
            return 2;
        }
    }

    while (fscanf(mapFile, "%d %d\n", &cities[i].x, &cities[i].y) == 2) {
        printf("%d %d\n", cities[i].x, cities[i].y);
        i++;
    }
    initPopulation(ptrPopulation, 10, 10);
    
    int fittest = getFittest(10);
    
    
    printf("The best is: %d with a distance of %f", fittest, population.tours[fittest].distance);





    return (EXIT_SUCCESS);
}

