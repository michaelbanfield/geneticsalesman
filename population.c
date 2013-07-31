#include <stdio.h>
#include <stdlib.h>
#include "tour.h"
#include "city.h"
#include "population.h"

int getFittest(City* cities, Population* population, int size) {
    int count = 4;
    population->fittest = 0;
    for (count = 0; count < size; count++) {

        //printArray(population->tours[count].path, 10);

        population->tours[count].distance = getDistance(cities, &population->tours[count], 10);
        printf("This is the float: %f\n", population->tours[count].distance);
        if (population->tours[count].distance < population->tours[population->fittest].distance) {
            population->fittest = count;

        }
    }
    return population->fittest;
}

void initPopulation(Population* population, int size, int max) {
    int count = 0;
    Tour * ptrTour;
    //population = malloc(sizeof (Population) + size * sizeof (Tour));
    
    
    for (count = 0; count < size; count++) {
        ptrTour = &(population->tours[count]);
        createPath(ptrTour, max);
        
    }
}

Tour tournament(int size, int max, City* cities) {
    Population tournament;
    
    initPopulation(&tournament, size, max);
    
    return tournament.tours[getFittest(cities, &tournament, size)];
    
    
}

void evolvePopulation(Population* population, int size, int max, Tour elite, City* cities, int generation) {
    int count = 0, index = 0, mutator = 0;
    Tour parent1, parent2;
    printf("hello evolve\n");
     
    initPopulation(population, size, max);
    for(count = 1; count < size; count++) {

                
        //while(generation > 0) {
            shuffle2(population->tours[count].path, max, generation);
            //generation--;
       // }
       
    }
    population->tours[0] = elite;
    
/*
    for(count = 1; count < size; count++) {
        
        parent1 = tournament(size, max, cities);
        parent2 = tournament(size, max, cities);
        for(index = 0; index < size/2; index ++) {
            parent1.path[index] = parent2.path[index];
        }
        
        population->tours[count] = parent1;
       
        
    }
*/
    
    printf("goodbye evolve\n");
    
    
}






