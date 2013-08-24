
/* 
 * File:   tour.c
 * Author: michaelb
 * Contents: Contains functions related to the tour struct
 * arrays
 * Created on 29 July 2013, 8:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "city.h" 
#include "tour.h"
#include <time.h>

/*
 * Function:  swap
 * --------------------
 * utility function that swaps 2 integers
 *
 *  a: first integer
 *  b: second integer
 *  value: the element being searched for
 */

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * Function:  find_index
 * --------------------
 * testing function that prints an array arr of size n
 *
 *  arr: integer array
 *  n: number of elements in array
 *
 */

void printArray(int arr[], int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

/*
 * Function:  shuffle
 * --------------------
 * shuffles integer array
 *
 *  array: integer array
 *  n: size of array
 *  value: the element being searched for
 */

void shuffle(int *array, size_t n) {

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

/*
 * Function:  getDistance
 * --------------------
 * calculates the total distance of a tour path
 *
 *  cities: array of City structs
 *  tour: tour to be calculated
 *  numOfCities: size of cities array
 *
 *  returns: double distance of Tour path
 */


double getDistance(City* cities, Tour* tour, int numOfCities) {
    int count = 0, section = 0;
    double distance = 0;
    City start, end;

    for (count = 0; count < numOfCities - 1; count++) {
        start = cities[tour->path[count]];
        end = cities[tour->path[count + 1]];
        section = (end.x - start.x) + (end.y - start.y);
        distance += sqrt(abs(section));
    }

    return distance;
}

/*
 * Function:  createPath
 * --------------------
 * dynamically allocates a Tour and creates a shuffled path through cities
 *
 *  tour: Tour struct to be created
 *  numOfCities: length of tour to be created
 */


void createPath(Tour* tour, int numOfCities) {
    int count = 0;

    init_array_tour(tour, numOfCities);
    for (count = 0; count < numOfCities; count++) {
        tour->path[count] = count;
    }

    int i = 0, m = 0, j = 0;

    m = numOfCities % 5;

    for (i = 0; i < m; i++) {
        int j = rand() % (i + 1);
        swap(&tour->path[i], &tour->path[j]);
    }

    for (i = m; i < numOfCities; i = i + 5) {

        j = rand() % (i + 1);
        swap(&tour->path[i], &tour->path[j]);
        j = rand() % (i + 2);
        swap(&tour->path[i + 1], &tour->path[j]);
        j = rand() % (i + 3);
        swap(&tour->path[i + 2], &tour->path[j]);
        j = rand() % (i + 4);
        swap(&tour->path[i + 3], &tour->path[j]);
        j = rand() % (i + 5);
        swap(&tour->path[i + 4], &tour->path[j]);

    }

}
