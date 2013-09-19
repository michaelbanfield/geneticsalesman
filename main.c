/* 
 * File:   main.c
 * Author: michaelb
 * Contents: The main function for the program
 * Created on 29 July 2013, 8:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "city.h"
#include "tour.h"
#include "population.h"
#include "array_helpers.h"
#include <mpi.h>


//global variables

/*
 * Function:  main
 * --------------------
 * the main algorithm control function.
 * Usage: filename generations cities population 
 * 
 *  returns: 
 *      0 - program exits correctly
 *      2 - Mapfile doesn't exist
 *      3 - Wrong number of arguments
 */

int main(int argc, char** argv) {

    //Local variables
    time_t timer;
    FILE* mapFile;
    Tour eliteTour;
    int generation = 0, maxGeneration = 0,
            numOfCities = 0, block = 6, numOfPopulation = 0, i = 0, 
            fittest = 0, rank = 0, size = 0, node = 1, segment = 0;
    City* cities;
    Population population;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    


    //Seed random number generator based on current time.
    srand(time(NULL) * (rank + 1));
    



    //Read in arguments, return error code's if conditions not met


    if (argc > 4) {
        mapFile = fopen(argv[1], "r");
        if (mapFile == NULL) {
            fprintf(stderr, "Mapfile doesn't exist\n");
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

    //read in *.map file



    while (fscanf(mapFile, "%d %d\n", &cities[i].x, &cities[i].y) == 2) {
        i++;
        if (i == numOfCities) {
            break;
        }
    }
    
    

    if (!rank) {



        //generate initial population

        initPopulation(&population, numOfPopulation, numOfCities);
        fittest = getFittest(cities, &population, numOfPopulation,
                numOfCities); /* find fittest */
        eliteTour = population.tours[fittest];
        //print out result
        printf("The best is: %d with a distance of %f\n", fittest,
                eliteTour.distance);
        //iterate through algorithm for number of generations
        
        for (generation = 0; generation < maxGeneration; generation++) {
            MPI_Recv(&node, 1, MPI_INT, MPI_ANY_SOURCE, 220, MPI_COMM_WORLD, &status);

            evolvePopulation(&population, numOfPopulation, numOfCities,
                    fittest, cities, rank, node);
            int tempint;
            //mutate population
            mutatePopulation(&population, numOfPopulation, numOfCities, fittest);
            fittest = getFittest(cities, &population, numOfPopulation,
                    numOfCities);

            if (!rank) {
                printf("The best is: %d with a distance of %f\n", fittest,
                       population.tours[fittest].distance); /* print result */
            }





        }

        
        block = 6;
        for (node = 1; node < size; node++) {
            MPI_Send(&block, 1, MPI_INT, node, 150, MPI_COMM_WORLD);
        }




    } else {
        Tour parents[numOfPopulation - 1];
        Tour child, parent1, parent2;
        init_array_tour(&child, numOfCities);
        init_array_tour(&parent1, numOfCities);
        init_array_tour(&parent2, numOfCities);
        int block, i;
        for (i = 0; i < numOfPopulation - 1; i++) {
            init_array_tour(&parents[i], numOfCities);
        }
        while (1) {
#pragma omp parallel for private(i)
            for (i = 0; i < numOfPopulation - 1; i++) {
                parent1 = tournament(numOfPopulation, numOfCities, cities);
                parent2 = tournament(numOfPopulation, numOfCities, cities);
                parents[i] = crossover(&parent1, &parent2, numOfCities);
            }
            MPI_Send(&rank, 1, MPI_INT, 0, 220, MPI_COMM_WORLD);
            for (i = 0; i < numOfPopulation - 1; i++) {
                MPI_Recv(&block, 1, MPI_INT, 0, 150, MPI_COMM_WORLD, &status);
                if (block > 5) {
                    return (EXIT_SUCCESS);
                }

                MPI_Send(parents[i].path, numOfCities, MPI_INT, 0, 100,
                        MPI_COMM_WORLD);

            }
            
            
            



        }

    }


    MPI_Finalize();

    return (EXIT_SUCCESS);
}

