#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NMEAParseGPRMC.h"
#include "PosCoordTransform.h"

const char *buff = 
"$GPGGA,174233.551,4003.8861,N,10512.5838,W,1,04,5.8,1593.0,M,-20.7,M,,0000*51"
"$GPGSA,A,3,30,22,31,12,,,,,,,,,10.6,5.8,8.9*0B"
"$GPGSV,2,1,08,31,68,170,31,22,30,119,36,30,22,079,36,12,11,046,32*7D"
"$GPGSV,2,2,08,14,51,047,20,32,38,309,,11,28,285,,20,11,306,*73"
"$GNRMC,174233.551,A,4003.8861,N,10512.5838,W,0.07,84.92,200510,,,A*4A"
"$GPVTG,84.92,T,,M,0.07,N,0.1,K,A*3C"
"$GPGGA,174234.551,4003.8370,N,10512.6372,W,1,04,2.8,1285.8,M,-20.7,M,,0000*54"
"$GPGSA,A,3,30,22,31,12,,,,,,,,,4.1,2.8,3.0*3C"
"$GNRMC,174234.551,A,4003.8370,N,10512.6372,W,0.62,352.44,200510,,,A*70"
        ;

GPRMCInfoIndex_t GPRMCInfo;
GPRMCPositionData GPRMCPosition;
GPRMCSpeedData GPRMCSpeed;
GPRMCDateData GPRMCDate;
GPRMCTimeOfFix GPRMCTime;
GPRMCOtherInfo GPRMCOthers;
Position_UTM_t UTMcord;

int main(void) {
    printf("\r\n %s \r\n ",buff);
    if (ParseNMEAFrameGPRMC(&GPRMCInfo,buff)==-1){
        perror("\r\n ERROR\r\n");
        return (EXIT_FAILURE);
    }
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
    return (EXIT_SUCCESS);
}
