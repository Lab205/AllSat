/* 
 * File:   Utility.h
 * Author: saffie
 *
 * Created on May 23, 2015, 3:52 PM
 */

#ifndef UTILITY_H
#define	UTILITY_H

#include <sys/time.h>      

long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


#endif	/* UTILITY_H */

