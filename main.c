#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NMEAParseGPRMCGGA.h"
#include "PosCoordTransform.h"

char buff[] = 
//"$GPRMC,155310.000,A,0611.6743,N,07535.3953,W,0.42,297.38,310517,,,A*77\r\n"
"$GPGGA,174233.551,4003.8861,N,10512.5838,W,1,04,5.8,1593.0,M,-20.7,M,,0000*51\r\n"
"$GPGSA,A,3,30,22,31,12,,,,,,,,,10.6,5.8,8.9*0B\r\n"
"$GPGSV,2,1,08,31,68,170,31,22,30,119,36,30,22,079,36,12,11,046,32*7D\r\n"
"$GPGSV,2,2,08,14,51,047,20,32,38,309,,11,28,285,,20,11,306,*73\r\n"
"$GPRMC,155312.000,A,0611.6744,N,07535.3955,W,0.37,279.20,310517,,,A*7F\r\n"
"$GPVTG,84.92,T,,M,0.07,N,0.1,K,A*3C\r\n"
"$GPGGA,174234.551,4003.8370,N,10512.6372,W,1,04,2.8,1285.8,M,-20.7,M,,0000*54\r\n"
"$GPGSA,A,3,30,22,31,12,,,,,,,,,4.1,2.8,3.0*3C\r\n"
"$GNRMC,174234.551,A,4003.8370,N,10512.6372,W,0.62,352.44,200510,,,A*70\r\n"
        ;

GPRMCInfoIndex_t GPRMCInfo;
GPGGAInfoIndex_t GPGGAInfo;

GPSPositionData GPSPosition;
GPRMCSpeedData GPRMCSpeed;
GPRMCDateData GPRMCDate;
GPRMCTimeOfFix GPRMCTime;
GPRMCOtherInfo GPRMCOthers;
Position_UTM_t UTMcord;

GPGGAFixData GPGGAFix;

int main(void) {
    printf("\r\n%s",buff);
    if (ParseNMEAFrameGPRMC(&GPRMCInfo,buff)==-1){
        perror("\r\n ERROR\r\n");
        return (EXIT_FAILURE);
    }
    /*
    if (ParseNMEAFrameGPGGA(&GPGGAInfo,buff)==-1){
        perror("\r\n ERROR\r\n");
        return (EXIT_FAILURE);
    }*/
   
    GPRMCGetPosition(&GPSPosition, &GPRMCInfo);
    //GPGGAGetPosition(&GPSPosition, &GPGGAInfo);
    //GPGGAGetFixData(&GPGGAFix, &GPGGAInfo);
    
    printf("\r\n Frame: %s ",GPRMCFrameIsolate(buff, TRMC));
    printf("\r\n checkum: 0x%02X   %d", GPRMCGetCheckSum(&GPRMCInfo), ValidateGPRMCCheckSUM(&GPRMCInfo, buff));
    printf("\r\n INIT:%s   \r\nEND:%s", buff+GPRMCInfo.indexinit, buff+GPRMCInfo.indexend);
    //return EXIT_SUCCESS;
    printf("\r\n - Lat: %dº %0.5f\"  | Long : %dº %0.5f\"",GPSPosition.LatitudeDegress,GPSPosition.LatitudeMinutes,GPSPosition.LongitudeDegress,GPSPosition.LongitudeMinutes);
    printf("\r\n - Lat: %0.7f  | Long : %0.7f",GPSPosition.LatitudeDD,GPSPosition.LongitudeDD);
    printf("\r\n - FixQuality:%d #Satelites:%d HorDil: %f   Altitude:%f   HeigElip: %f\r\n", GPGGAFix.FixQuality, GPGGAFix.NoOfSatellitesBeingTracked, GPGGAFix.HorizontalDilution, GPGGAFix.Altitude, GPGGAFix.HeightOfGeoid);
    /*
    GPRMCGetPosition(&GPRMCPosition, &GPRMCInfo);
    GPRMCGetSpeed(&GPRMCSpeed, &GPRMCInfo);
    GPRMCGetDate(&GPRMCDate, &GPRMCInfo);
    GPRMCGetTimeOfFix(&GPRMCTime, &GPRMCInfo);
    GPRMCGetOtherInfo(&GPRMCOthers, &GPRMCInfo);
    printf("\r\n - Status: %c  CheckSum: 0x%2.2X",GPRMCGetStatus(&GPRMCInfo), GPRMCGetCheckSum(&GPRMCInfo));
    printf("\r\n - Lat: %dº %0.5f\"  | Long : %dº %0.5f\"",GPRMCPosition.LatitudeDegress,GPRMCPosition.LatitudeMinutes,GPRMCPosition.LongitudeDegress,GPRMCPosition.LongitudeMinutes);
    printf("\r\n - Lat: %0.7f  | Long : %0.7f",GPRMCPosition.LatitudeDD,GPRMCPosition.LongitudeDD);
    printf("\r\n - Speed : %f knots - %f km/h",GPRMCSpeed.SpeedKnots, GPRMCSpeed.SpeedKmh);
    printf("\r\n - Date %d/%d/%d",GPRMCDate.Day, GPRMCDate.Month, GPRMCDate.Year);
    printf("\r\n - Time of Fix: %d : %d : %f",GPRMCTime.Hour, GPRMCTime.Minute, GPRMCTime.Seconds);
    printf("\r\n - Track angle: %f  Magnetic Variation : %f %c",GPRMCOthers.TrackAngle, GPRMCOthers.MagneticVariation, GPRMCOthers.MVDir);
    puts("");
    Geodesic2UTM(&UTMcord,EN_WGS_84, GPRMCPosition.LatitudeDD, GPRMCPosition.LongitudeDD);
    printf("\r\n UTM(lineal/meters) : x=%5.5f  y=%5.5f   %d  %c \r\n",UTMcord.UTMEasting,UTMcord.UTMNorthing,UTMcord.UTMZone, UTMcord.UTMHemisphere);
    
     */
    return (EXIT_SUCCESS);
}
