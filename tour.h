/* 
 * File:   tour.h
 * Author: michaelb
 *
 * Created on 29 July 2013, 9:23 PM
 */

#ifndef TOUR_H
#define	TOUR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "city.h"
    
    typedef struct {
        
        double distance;
        int path[10];
    }Tour;
    void printArray (int[], int);
    void shuffle(int *, size_t);
    void shuffle2(int *, size_t, int);
    double getDistance(City*, Tour*, int);
    void createPath(Tour*, int);




#ifdef	__cplusplus
}
#endif

#endif	/* TOUR_H */

