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
#include "array_helpers.h"

//global variables

City* cities;
Population population;
int numOfCities = 0, numOfPopulation = 0, i = 0;

int main(int argc, char** argv) {
    FILE* mapFile;
    int generation = 0, maxGeneration = 0;

    srand(time(NULL));

    if (argc > 4) {
        mapFile = fopen(argv[1], "r");
        if (mapFile == NULL) {
            return 2;
        }
        maxGeneration = atoi(argv[2]);
        cities = malloc(sizeof (City) * atoi(argv[3]));
        numOfCities = atoi(argv[3]);
        numOfPopulation = atoi(argv[4]);
    } else {
        fprintf(stderr, "Wrong number of arguments\n");
        return 3;
    }

    while (fscanf(mapFile, "%d %d\n", &cities[i].x, &cities[i].y) == 2) {
        i++;
    }
    initPopulation(&population, 10, 10);

    int fittest = getFittest(cities, &population, 10);
    Tour eliteTour = population.tours[fittest];
    printf("The best is: %d with a distance of %f\n", fittest, eliteTour.distance);

    for (generation = 0; generation < maxGeneration; generation++) {
        evolvePopulation(&population, numOfPopulation, numOfCities, eliteTour, cities, generation);
        fittest = getFittest(cities, &population, 10);
        eliteTour = population.tours[fittest];
        printf("The best is: %d with a distance of %f\n", fittest, population.tours[fittest].distance);
    }
    return (EXIT_SUCCESS);
}

