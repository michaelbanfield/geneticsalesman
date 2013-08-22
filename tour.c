#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "city.h" 
#include "tour.h"
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
// A utility function to print an array

void printArray(int arr[], int n) {
    //int i = 0;
    //for (i = 0; i < n; i++)
        //printf("%d ", arr[i]);
    //printf("\n");
}

void shuffle(int *array, size_t n)
 {
   
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



void shuffle2(int *array, size_t n, int multiplier)
 {
    
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + multiplier / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

double getDistance(City* cities, Tour* tour, int numOfCities) {
    int count = 0, section = 0;
    double distance = 0;
    City start, end;
    //printf("the path is: ");
    //printArray(tour->path, numOfCities);

    for (count = 0; count < numOfCities - 1; count++) {
        start = cities[tour->path[count]];
        //printf("city value: %d %d\n", max, tour->path[count + 1]);
        end = cities[tour->path[count + 1]];
        section = (end.x - start.x) + (end.y - start.y);
        distance += sqrt(abs(section));
    }

    return distance;
}

void createPath(Tour* tour, int numOfCities) {
    int count = 0;
    
    init_array_tour(tour, numOfCities);
    
    
    
    //tour = malloc(sizeof (Tour) + max * sizeof (int));
    //tour->path[0] = 22;
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
    //for (i = numOfCities - 1; i > 0; i--) {
        
        j = rand() % (i + 1);
        swap(&tour->path[i], &tour->path[j]);
        j = rand() % (i + 2);
        swap(&tour->path[i+1], &tour->path[j]);
        j = rand() % (i + 3);
        swap(&tour->path[i+2], &tour->path[j]);
        j = rand() % (i + 4);
        swap(&tour->path[i+3], &tour->path[j]);
        j = rand() % (i + 5);
        swap(&tour->path[i+4], &tour->path[j]);
        
    }
    //tour->path[numOfCities - 1] = tour->path[0];

    //printArray(tour->path, numOfCities);


}
