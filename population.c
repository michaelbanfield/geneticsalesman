#include <stdio.h>
#include <stdlib.h>
#include "tour.h"
#include "population.h"

void initPopulation(Population* population, int size, int max) {
    int count = 0;
    Tour * ptrTour;
    //population = malloc(sizeof (Population) + size * sizeof (Tour));
    srand ( time(NULL) );
    for (count = 0; count < size; count++) {
        ptrTour = &(population->tours[count]);
        //ptrTour = malloc(sizeof (Tour) + max * sizeof (int));
        createPath(ptrTour, max);
        printf("this is before the print array ");
        printArray(ptrTour->path, 10);
        
    }
}


