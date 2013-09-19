
/* 
 * File:   population.c
 * Author: michaelb
 * Contents: Contains functions related to the population struct
 * arrays
 * Created on 29 July 2013, 8:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "tour.h"
#include "city.h"
#include "population.h"
#include "array_helpers.h"
#include <omp.h>
#include <mpi.h>

#define TRUE 1;
#define FALSE 0;

/*
 * Function:  getFittest
 * --------------------
 * returns the index of the shortest (fittest) tour in a given population
 *
 *  cities: an array City structs
 *  population: the population to be tested
 *  numOfPopulation: the size of the population
 *  numOfCities: the number of cities
 *  
 *
 *  returns: the integer index for the fittest Tour struct in the population
 */

int getFittest(City* cities, Population* population, int numOfPopulation, int numOfCities) {

    int count = 0;

    population->fittest = 0;

    for (count = 0; count < numOfPopulation; count++) {
        population->tours[count].distance = getDistance(cities,
                &population->tours[count], numOfCities);
        if (population->tours[count].distance <
                population->tours[population->fittest].distance) {
            population->fittest = count;

        }
    }

    return population->fittest;
}

/*
 * Function:  initPopulation
 * --------------------
 * initialize the population by allocated array space and creating random tours
 *
 *  population: the population to be initialized
 *  numOfPopulation: the size of the population
 */


void initPopulation(Population* population, int numOfPopulation,
        int numOfCities) {

    int count;
    Tour * ptrTour;
    init_array_population(population, numOfPopulation);
    for (count = 0; count < numOfPopulation; count++) {
        ptrTour = &(population->tours[count]);
        createPath(ptrTour, numOfCities);

    }




}

/*
 * Function:  tournament
 * --------------------
 * creates a population and finds the shortest tour
 *
 *  numOfPopulation: size of population
 *  numOfCities: number of cities
 *  cities: array of City structs
 *
 *  returns:fittest tour
 */


Tour tournament(int numOfPopulation, int numOfCities, City* cities) {
    Tour tour;
    int fittest;
    init_array_tour(&tour, numOfCities);
    Population tournament;

    initPopulation(&tournament, numOfPopulation, numOfCities);
    fittest = getFittest(cities, &tournament,
            numOfPopulation, numOfCities);
    free_population(&tournament, numOfPopulation, fittest);

    return tournament.tours[fittest];
}

/*
 * Function:  find_index
 * --------------------
 * finds if an element is in a integer array
 *
 *  a: array of ints to be searched
 *  num_elements: the length of the array
 *  value: the element being searched for
 *
 *  returns:true if the element is in the list, false otherwise
 */

int find_index(int a[], int num_elements, int value) {

    int i = 0, m = 0;
    m = num_elements % 5;
    for (i = 0; i < m; i++) {
        if (a[i] == value) {
            return TRUE; /* it was found */
        }
    }
    for (i = m; i < num_elements; i = i + 5) {
        if (a[i] == value) {
            return TRUE;
        }
        if (a[i + 1] == value) {
            return TRUE;
        }
        if (a[i + 2] == value) {
            return TRUE;
        }
        if (a[i + 3] == value) {
            return TRUE;
        }
        if (a[i + 4] == value) {
            return TRUE;
        }
    }
    return FALSE;
}

/*
 * Function:  crossover
 * --------------------
 * combines two Tour structs together by taking a subset from one and adding 
 * the unique elements from the other
 *
 *  parent1: tour struct to be combined
 *  parent2 second tour struct to be combined
 *  numOfCities: number of elements in a tour
 *  
 *
 *  returns:combined Tour
 */


Tour crossover(Tour* parent1, Tour* parent2, int numOfCities) {

    Tour tour;

    init_array_tour(&tour, numOfCities);
    int count = 0, count2 = 0, start = rand() % (numOfCities / 2),
            end = (rand() % (numOfCities / 2)) + 5;
    

    for (count = 0; count < numOfCities; count++) {
        tour.path[count] = -1;
    }
    

    for (count = start; count < end; count++) {
        tour.path[count] = parent1->path[count];
    }
    

    for (count = 0; count < start; count++) {
        count2 = 0;

        while (find_index(tour.path, numOfCities, parent2->path[count2])) {
            count2++;

        }

        tour.path[count] = parent2->path[count2];
    }
    

    for (count = end; count < numOfCities; count++) {
        count2 = 0;
        while (find_index(tour.path, numOfCities, parent2->path[count2]) == 1) {
            count2++;
        }

        tour.path[count] = parent2->path[count2];
    }


    return tour;


}

/*
 * Function:  mutatePopulation
 * --------------------
 * swaps 2 random elements in each tour in a population, except for the first
 *      tour which is the "elite" tour
 *
 *  population: the population to be mutated
 *  numOfPopulation: number of tours in population
 *  numofCities: number of elements in a tour
 *
 */


void mutatePopulation(Population* population, int numOfPopulation,
        int numOfCities, int fittest) {

    int count = 0, a = 0, b = 0, temp = 0;

    for (count = 0; count < numOfPopulation; count++) {
        if(count == fittest) {
            continue;
        }
        a = rand() % numOfCities;
        b = rand() % numOfCities;
        temp = (int) population->tours[count].path[a];
        population->tours[count].path[a] = 
                (int) population->tours[count].path[b];
        population->tours[count].path[b] = a;
    }


}

/*
 * Function:  evolvePopulation
 * --------------------
 * takes an initial population and uses tournaments to evolve it
 * 
 * *Parralel Update*
 * 
 * Now retrieves paths from node specified in argument node
 * 
 *  population: population to be evolved
 *  numOfPopulation: number of tours in population
 *  numOfCities: number of cities in tour
 *  elite: Tour struct that is shortest of population
 *  node: rank to get paths from
 *
 */


void evolvePopulation(Population* population, int numOfPopulation,
        int numOfCities, int elite, int node) {
    
    MPI_Status status;
    int count = 0, request= 1;
    
    for (count = 0; count < numOfPopulation; count++) {
        
        if(count == elite) {
            continue;
        }

        MPI_Send(&request, 1, MPI_INT, node, 150, MPI_COMM_WORLD);
        MPI_Recv(population->tours[count].path, numOfCities, MPI_INT, 
                node, 100, MPI_COMM_WORLD, &status);
    }


}