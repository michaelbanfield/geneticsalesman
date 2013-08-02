#include <stdio.h>
#include <stdlib.h>
#include "tour.h"
#include "city.h"
#include "population.h"
#include "array_helpers.h"

#define TRUE 1;
#define FALSE 0;

int getFittest(City* cities, Population* population, int numOfPopulation, int numOfCities) {
    
    int count = 4;
    population->fittest = 0;
    for (count = 0; count < numOfPopulation; count++) {
        population->tours[count].distance = getDistance(cities,
                &population->tours[count], numOfCities);
        //printf("Total length for %d: %f\n", count,
                //population->tours[count].distance);
        if (population->tours[count].distance <
                population->tours[population->fittest].distance) {
            population->fittest = count;

        }
    }
    return population->fittest;
}

void initPopulation(Population* population, int numOfPopulation,
        int numOfCities) {
    
    int count = 0;
    Tour * ptrTour;
    init_array_population(population, numOfPopulation);


    for (count = 0; count < numOfPopulation; count++) {
        ptrTour = &(population->tours[count]);
        createPath(ptrTour, numOfCities);

    }
}

Tour tournament(int numOfPopulation, int numOfCities, City* cities) {
    
    Population tournament;
    initPopulation(&tournament, numOfPopulation, numOfCities);
    return tournament.tours[getFittest(cities, &tournament, 
            numOfPopulation, numOfCities)];
}

int find_index(int a[], int num_elements, int value) {
    
    int i;
    for (i = 0; i < num_elements; i++) {
        if (a[i] == value) {
            return TRUE; /* it was found */
        }
    }
    return FALSE; /* if it was not found */
}

Tour crossover(Tour* parent1, Tour* parent2, int numOfCities) {
    
    Tour tour;
    
    init_array_tour(&tour, numOfCities);
    int count = 0, start = rand() % (numOfCities / 2),
            end = (rand() % (numOfCities / 2)) + 5;

    for (count = 0; count < numOfCities; count++) {
        tour.path[count] = -1;
    }

    for (count = start; count < end; count++) {
        tour.path[count] = parent1->path[count];
    }

    for (count = 0; count < start; count++) {
        int count2 = 0;
        
        while (find_index(tour.path, numOfCities, parent2->path[count2])) {
            count2++;

        }
        
        tour.path[count] = parent2->path[count2];
    }
    printArray(tour.path, numOfCities);
    
    for (count = end; count < numOfCities; count++) {
        int count2 = 0;
        while (find_index(tour.path, numOfCities, parent2->path[count2]) == 1) {
            count2++;
        }

        tour.path[count] = parent2->path[count2];
    }
    
    //printf("The final array is:");
    printArray(tour.path, numOfCities);
    
    return tour;


}

void mutatePopulation(Population* population, int numOfPopulation,
        int numOfCities) {
    
    int count = 0;
    
    for (count = 1; count < numOfPopulation; count++) {
        shuffle(population->tours[count].path, numOfCities);
    }

}

void evolvePopulation(Population* population, int numOfPopulation,
        int numOfCities, Tour elite, City* cities, int generation) {
    
    int count = 0, index = 0, mutator = 0;
    Tour parent1, parent2;
    
    population->tours[0] = elite;
    
    for (count = 1; count < numOfPopulation; count++) {
        parent1 = tournament(numOfPopulation, numOfCities, cities);
        parent2 = tournament(numOfPopulation, numOfCities, cities);
        population->tours[count] = crossover(&parent1, &parent2, numOfCities);

    }
}