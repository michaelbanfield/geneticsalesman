#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "city.h" 
#include "tour.h"

double getDistance(City* cities, Tour tour, int max) {
    int count = 0, section = 0;
    double distance = 0;
    City start, end;
    
    for(count = 0; count < max; count++) {
        start = cities[count];
        end = cities[count + 1];
        section = (end.x - start.x) + (end.y - start.y);
        distance += sqrt(abs(section));
    }
    
    return distance;
}

void createPath(Tour *tour, int max) {
    int count = 0;
    tour = malloc(sizeof(Tour) + max * sizeof(int));
    for (count = 0; count < max; count++) {
        tour->path[count] = count;
    }
}
