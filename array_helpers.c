#include <stdio.h>
#include <stdlib.h>
#include "city.h"
#include "tour.h"
#include "population.h"

void init_array_tour(Tour* tour, int value) {
    tour->path= malloc(sizeof(int) * value);
    
}
void init_array_population(Population* population, int value) {
    population->tours = malloc(sizeof(Tour) * value);
    
}
void put_array(int* array, int value) {
    array = realloc(array, sizeof(int) * value);
}