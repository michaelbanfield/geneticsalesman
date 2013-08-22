/* 
 * File:   array_helpers.h
 * Author: michaelb
 *
 * Created on 31 July 2013, 7:48 PM
 */

#ifndef ARRAY_HELPERS_H
#define	ARRAY_HELPERS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "tour.h"
#include "city.h"
    
    void init_array_tour(Tour*, int);
    void init_array_population(Population*, int);
    void put_array(int*, int);
    void free_population(Population*, int);
    void free_tour(Tour*);
    
    




#ifdef	__cplusplus
}
#endif

#endif	/* ARRAY_HELPERS_H */

