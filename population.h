/* 
 * File:   population.h
 * Author: michaelb
 *
 * Created on 31 July 2013, 1:05 PM
 */

#ifndef POPULATION_H
#define	POPULATION_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "tour.h"
    
    typedef struct{
        Tour* tours;
        int fittest;
    }Population;
    
    int getFittest(City*, Population*, int, int);
    void mutatePopulation(Population*, int, int);
    void evolvePopulation(Population*, int, int, Tour, City*);
    void initPopulation(Population*, int, int);




#ifdef	__cplusplus
}
#endif

#endif	/* POPULATION_H */

