/* 
 * File:   NMEAParser.h
 * Author: Ing. Juan Camilo GÃ³mez Cadavid MSc.
 * 
 * Created on 7 de abril de 2011, 04:39 PM
 */

#ifndef NMEAPARSER_H
#define	NMEAPARSER_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <string.h>
/*
GPRMC INFO:
*/

    
    
#define TRMC    "$GPRMC,"
#define TGGA    "$GPGGA,"    
    
typedef enum  {RMC, GGA}NMEA_Frame_t;    
    
#define     IDX_GPRMC_TIMEOFFIX             0   // Time of fix hhmmss.ss UTC
#define     IDX_GPRMC_STATUS                1   // Status A=active or V=Void
#define     IDX_GPRMC_LATITUDE              2   // Latitude  
#define     IDX_GPRMC_DIR_LATITUDE          3   // Latitude Direction
#define     IDX_GPRMC_LONGITUDE             4   // Longitude
#define     IDX_GPRMC_DIR_LONGITUDE         5   // Latitude Direction
#define     IDX_GPRMC_SPEED                 6   // Speed over the ground in knots 
#define     IDX_GPRMC_TRACK_ANGLE           7   // Track angle in degrees True
#define     IDX_GPRMC_DATE                  8   // Date ddmmyy
#define     IDX_GPRMC_MAGVAR                9   // Magnetic Variation
#define     IDX_GPRMC_DIRMAGVAR             10  // Magnetic Variation Direction
#define     IDX_GPRMC_CHECKSUM              11  // CheckSum    
/*
LAT/LONG Format: when a numeric latitude or longitude is given, the two digits immediately 
to the left  of the decimal point are whole minutes, to the right are decimals of minutes, and the 
remaining digits to the left of the whole minutes are whole degrees.
eg. 4533.35 is 45 degrees and 33.35 minutes. ".35" of a minute is exactly 21 seconds.
eg. 16708.033 is 167 degrees and 8.033 minutes. ".033" of a minute is about 2 seconds.
*/    
    
#define _GPS_NMEA_GPRMC_AMOUNT_OF_DATA_     11    
#define _GPS_NMEA_GPGGA_AMOUNT_OF_DATA_     13   



typedef struct{
    char *Data[_GPS_NMEA_GPGGA_AMOUNT_OF_DATA_+1];
    size_t Size[_GPS_NMEA_GPGGA_AMOUNT_OF_DATA_+1];
}GPGGAInfoIndex_t;  
int ParseNMEAFrameGPGGA(GPGGAInfoIndex_t *obj,  const char *buffer);    
typedef struct{
    char *Data[_GPS_NMEA_GPRMC_AMOUNT_OF_DATA_+1];
    size_t Size[_GPS_NMEA_GPRMC_AMOUNT_OF_DATA_+1];
}GPRMCInfoIndex_t;  
int ParseNMEAFrameGPRMC(GPRMCInfoIndex_t *obj,  const char *buffer);

typedef struct{
    unsigned char LatitudeDir;
    unsigned char LongitudeDir;
    short LatitudeDegress;
    double LatitudeMinutes;
    short LongitudeDegress;
    double LongitudeMinutes;
    double LatitudeDD;
    double LongitudeDD;
}GPSPositionData;

typedef struct{
    float SpeedKnots;
    float SpeedKmh;
}GPRMCSpeedData;

typedef struct{
    unsigned char Day;
    unsigned char Month;
    unsigned char Year;
}GPRMCDateData;

typedef struct{
    unsigned char Hour;
    unsigned char Minute;
    float Seconds;
}GPRMCTimeOfFix;

typedef struct{
    float TrackAngle;
    float MagneticVariation;
    unsigned char MVDir;
}GPRMCOtherInfo;

typedef struct{
    unsigned char FixQuality;
    unsigned char NoOfSatellitesBeingTracked;
    float HorizontalDilution;
    float Altitude;
    float HeightOfGeoid;
}GPGGAFixData;

int GPRMCGetPosition(GPSPositionData *datobj, GPRMCInfoIndex_t *infoobj);
int GPGGAGetPosition(GPSPositionData *datobj, GPGGAInfoIndex_t *infoobj);
int GPGGAGetFixData(GPGGAFixData *dataobj, GPGGAInfoIndex_t *infoobj);

unsigned char GPRMCGetStatus(GPRMCInfoIndex_t *infoobj);
unsigned char GPRMCGetCheckSum(GPRMCInfoIndex_t *infoobj);
int GPRMCGetSpeed(GPRMCSpeedData *datobj, GPRMCInfoIndex_t *infoobj);

int GPRMCGetTimeOfFix(GPRMCTimeOfFix *dataobj, GPRMCInfoIndex_t *infoobj);
int GPRMCGetDate(GPRMCDateData *dataobj, GPRMCInfoIndex_t *infoobj);
int GPRMCGetOtherInfo(GPRMCOtherInfo *dataobj, GPRMCInfoIndex_t *infoobj);

char* GPRMCFrameIsolate(char *buffer, char *type);

#define GPS_INTERNAL_BUFFER_SIZE    10

//#define     NMEAPARSEGPRMC_MAXOPTIM

//extern unsigned char __gps_buff_data[GPS_INTERNAL_BUFFER_SIZE];


/*
$GPRMC

Recommended minimum specific GPS/Transit data

eg1. $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
eg2. $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68


           225446       Time of fix 22:54:46 UTC
           A            Navigation receiver warning A = OK, V = warning
           4916.45,N    Latitude 49 deg. 16.45 min North
           12311.12,W   Longitude 123 deg. 11.12 min West
           000.5        Speed over ground, Knots
           054.7        Course Made Good, True
           191194       Date of fix  19 November 1994
           020.3,E      Magnetic variation 20.3 deg East
           *68          mandatory checksum


eg3. $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
              1    2    3    4    5     6    7    8      9     10  11 12


      1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum


eg4. $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
1    = UTC of position fix
2    = Data status (V=navigation receiver warning)
3    = Latitude of fix
4    = N or S
5    = Longitude of fix
6    = E or W
7    = Speed over ground in knots
8    = Track made good in degrees True
9    = UT date
10   = Magnetic variation degrees (Easterly var. subtracts from true course)
11   = E or W
12   = Checksum
 
*/


#ifdef	__cplusplus
}
#endif

#endif	/* NMEAPARSER_H */

