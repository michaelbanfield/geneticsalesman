/* 
 * File:   array_helpers.c
 * Author: michaelb
 * Contents: Contains helper functions of dynamically allocating and free-ing
 * arrays
 * Created on 29 July 2013, 8:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "city.h"
#include "tour.h"
#include "population.h"

/*
 * Function:  init_array_tour
 * --------------------
 * dynamically allocates an array of integers in a Tour struct
 *      tour - the tour struct which needs an array allocated
 *      value - an integer value for how large the path array needs to be
 *  
 */

void init_array_tour(Tour* tour, int value) {
    tour->path = malloc(sizeof (int) * value);

}

/*
 * Function:  init_array_population
 * --------------------
 * dynamically allocates an array of Tour structs in a Population struct
 *      population - the population struct which needs an array allocated
 *      value - an integer value for how large the tour array needs to be
 *  
 */

void init_array_population(Population* population, int value) {
    population->tours = malloc(sizeof (Tour) * value);

}

/*
 * Function:  free_tour
 * --------------------
 * frees a tour
 *      tour - the tour struct to be free'd
 *     
 *  
 */

void free_tour(Tour* tour) {
    free(tour->path);
}

/*
 * Function:  free_population
 * --------------------
 * free's all the integer arrays in a given population except where the tour
 *      is the fittest and needs to be used later
 * population - the population struct which needs tour's free'd
 * numOfPopulation - the size of the tour
 * fittest - the fittest tour, which won't be freed
 *  
 */

void free_population(Population* population, int numOfPopulation, int fittest) {
    int i = 0;
    for (i = 0; i < (numOfPopulation); i++) {
        if (i == fittest) {
            continue;
        }
        free(population->tours[i].path);

    }
}