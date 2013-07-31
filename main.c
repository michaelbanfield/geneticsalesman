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
Population population;
Population * ptrPopulation = &population;

int main(int argc, char** argv) {
    FILE* mapFile;
    int generation = 0;
    int maxGeneration = 0;
    
    srand ( time(NULL) );
    

    int x = 0, y = 0, i = 0;

    if (argc > 1) {
        mapFile = fopen(argv[1], "r");
        if (mapFile == NULL) {
            return 2;
        }
        maxGeneration = atoi(argv[2]);
    }

    while (fscanf(mapFile, "%d %d\n", &cities[i].x, &cities[i].y) == 2) {
        printf("%d %d\n", cities[i].x, cities[i].y);
        i++;
    }
    initPopulation(ptrPopulation, 10, 10);

    int fittest = getFittest(cities, ptrPopulation, 10);
    printf("The best is: %d with a distance of %f\n", fittest, population.tours[fittest].distance);
    Tour eliteTour = population.tours[fittest];
    for (generation = 0; generation < maxGeneration; generation++) {
        srand (generation * 20);
        evolvePopulation(ptrPopulation, 10, 10, eliteTour, cities, generation);
        fittest = getFittest(cities, ptrPopulation, 10);
        eliteTour = population.tours[fittest];
        printf("The best is: %d with a distance of %f\n", fittest, population.tours[fittest].distance);

    }










    return (EXIT_SUCCESS);
}

