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
            numOfCities = 0, numOfPopulation = 0, i = 0, fittest = 0, rank = 0;
    City* cities;
    Population population;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;


    //Seed random number generator based on current time.
    srand(time(NULL));
    timer = time(NULL);



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

    if (rank == 0) {



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

            evolvePopulation(&population, numOfPopulation, numOfCities, eliteTour,
                    cities, rank, generation + 1);
            //mutate population
            mutatePopulation(&population, numOfPopulation, numOfCities);
            fittest = getFittest(cities, &population, numOfPopulation, numOfCities);
            eliteTour = population.tours[fittest];

            if (rank == 0) {
                printf("The best is: %d with a distance of %f\n", fittest,
                        eliteTour.distance); /* print result */
            }





        }
        if (rank == 0) {
            printf("Time is %d", time(NULL) - timer);
            int myN = 6;
            for (generation = 1; generation < maxGeneration; generation++) {
            MPI_Send(&myN, 1, MPI_INT, 1, 150, MPI_COMM_WORLD);
            //MPI_Send(&myN, 1, MPI_INT, 2, 150, MPI_COMM_WORLD);
            //MPI_Send(&myN, 1, MPI_INT, 3, 150, MPI_COMM_WORLD);
            //MPI_Send(&myN, 1, MPI_INT, 4, 150, MPI_COMM_WORLD);
            //MPI_Send(&myN, 1, MPI_INT, 5, 150, MPI_COMM_WORLD);
            }

        }


    }



    if (rank != 0) {
        Tour parents[numOfPopulation];
        int block, i;
        while (1) {
            #pragma omp parallel for private(i)
            for (i = 0; i < numOfPopulation; i++) {
                parents[i] = tournament(numOfPopulation, numOfCities, cities);
            }
            for (i = 0; i < numOfPopulation; i++) {

                MPI_Recv(&block, 1, MPI_INT, 0, 150, MPI_COMM_WORLD, &status);
                if (block > 5) {
                    printf("process end\n");
                    return(EXIT_SUCCESS);
                }
                MPI_Send(parents[i].path, numOfCities, MPI_INT, 0, 100, MPI_COMM_WORLD);

            }



        }

    }


    MPI_Finalize();

    return (EXIT_SUCCESS);
}

