/* 
 * File:   PostitionCoordinatesTransformatios.h
 * Author: Ing. Juan Camilo Gomez Cadavid MSc.
 * I write this fuc#?!%&! transformation for long-term 2D localization correction
 * Created on 7 de mayo de 2011, 12:08 PM
 */

#ifndef POSTITIONCOORDINATESTRANSFORMATIOS_H
#define	POSTITIONCOORDINATESTRANSFORMATIOS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <math.h>    

#define  __DEG2RAD     0.01745329;
#define  __RAD2DEG     57.29577951;

typedef struct{
	double UTMNorthing; /* X(metters)  --> (xr)*/
	double UTMEasting; /* Y(metters) -->(yr)*/
	unsigned char UTMZone; /* sigma */ 
        unsigned char UTMHemisphere; /* zeta */ 
}Position_UTM_t;

/*
Equations from USGS Bulletin 1532  http://pubs.usgs.gov/bul/1532/report.pdf
 * there is a precise method here : http://arxiv.org/pdf/1002.1417.pdf
East Longitudes are positive, West longitudes are negative. 
North latitudes are positive, South latitudes are negative
Lat and Long are in decimal degrees
*/

/*Ellipsoid Names (References)*/
#define EN_Airy                             0
#define EN_Australian_National              1
#define EN_Bessel_1841                      2
#define EN_Bessel_1841_Nambia_              3 
#define EN_Clarke_1866                      4
#define EN_Clarke_1880                      5
#define EN_Everest                          6
#define EN_Fischer_1960_Mercury             7
#define EN_Fischer_1968                     8
#define EN_GRS_1967                         9
#define EN_GRS_1980                         10
#define EN_Helmert_1906                     11
#define EN_Hough                            12
#define EN_International                    13
#define EN_Krassovsky                       14
#define EN_Modified_Airy                    15
#define EN_Modified_Everest                 16
#define EN_Modified_Fischer_1960            17
#define EN_South_American_1969              18
#define EN_WGS_60                           19
#define EN_WGS_66                           20
#define EN_WGS_72                           21           
#define EN_WGS_84                           22 //this is the one

#ifdef	__cplusplus
}
#endif

#endif	/* POSTITIONCOORDINATESTRANSFORMATIOS_H */

