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
#include <omp.h>


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
 *      4 - Need at least 2 nodes
 */

int main(int argc, char** argv) {

    //Local variables
    FILE* mapFile;
    Tour eliteTour;
    int generation = 0, maxGeneration = 0,
            numOfCities = 0, block = 0, numOfPopulation = 0, i = 0,
            fittest = 0, rank = 0, size = 0, node = 1;
    City* cities;
    Population population;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "Need at least 2 nodes\n");
        return 4;
    }

    MPI_Status status;

    //Seed random number generator based on current time multiplied by rank.
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



    if (!rank) { /*master node */

        //generate initial population
        
        int path[numOfCities], request = 1, zeroRequest = 0;
        double newLength = 0, oldLength = 0;
        

        initPopulation(&population, numOfPopulation, numOfCities);
        fittest = getFittest(cities, &population, numOfPopulation,
                numOfCities); /* find fittest */
        
        for(i = 0; i < numOfCities; i++) {
            path[i] = population.tours[fittest].path[i];
            
        }
        oldLength = population.tours[fittest].distance;


        for (generation = 0; generation < maxGeneration; generation++) {
            MPI_Recv(&node, 1, MPI_INT, MPI_ANY_SOURCE, 220, MPI_COMM_WORLD,
                    &status);
            
            MPI_Send(&request, 1, MPI_INT, node, 150, MPI_COMM_WORLD);
            
           
            MPI_Recv(&newLength, 1, MPI_DOUBLE, node, 230, MPI_COMM_WORLD,
                    &status);
            if(newLength < oldLength ) {
                 
                MPI_Send(&request, 1, MPI_INT, node, 150, MPI_COMM_WORLD);
                MPI_Recv(&path, numOfCities, MPI_INT, node, 240, MPI_COMM_WORLD,
                    &status);
                oldLength = newLength;
                
            } else {
                MPI_Send(&zeroRequest, 1, MPI_INT, node, 150, MPI_COMM_WORLD);
            }

            if (!rank) {
                printf("Fittest from generation %d has distance of %f\n",
                        generation,
                        oldLength); /* print result */
            }
        }

        for (node = 1; node < size; node++) { /*shutdown message */
            MPI_Send(&block, 1, MPI_INT, node, 150, MPI_COMM_WORLD);
        }




    } else { /*slave node */

        Tour paths[numOfPopulation - 1], parent1, parent2;
        init_array_tour(&parent1, numOfCities);
        init_array_tour(&parent2, numOfCities);
        
        init_array_population(&population, numOfPopulation);

        for (i = 0; i < numOfPopulation; i++) {
            init_array_tour(&population.tours[i], numOfCities);
        }

        while (1) {

#pragma omp parallel for private(i)
            for (i = 0; i < numOfPopulation; i++) {
                parent1 = tournament(numOfPopulation, numOfCities, cities);
                parent2 = tournament(numOfPopulation, numOfCities, cities);
                population.tours[i] = crossover(&parent1, &parent2, numOfCities);
            }
            mutatePopulation(&population, numOfPopulation, numOfCities, fittest);
            fittest = getFittest(cities, &population, numOfPopulation,
                    numOfCities);

            MPI_Send(&rank, 1, MPI_INT, 0, 220, MPI_COMM_WORLD);

            MPI_Recv(&block, 1, MPI_INT, 0, 150, MPI_COMM_WORLD, &status);
            if (!block) {
                    return (EXIT_SUCCESS);
                }
           
            MPI_Send(&population.tours[fittest].distance, 1, MPI_DOUBLE, 0, 230, MPI_COMM_WORLD);
            MPI_Recv(&block, 1, MPI_INT, 0, 150, MPI_COMM_WORLD, &status);
            if(block) {
                MPI_Send(population.tours[fittest].path, numOfCities, MPI_INT, 0, 240,
                        MPI_COMM_WORLD);
            }
        }

    }


    MPI_Finalize();

    return (EXIT_SUCCESS);
}

