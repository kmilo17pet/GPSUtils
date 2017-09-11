/* 
 * File:   NMEAParser.c
 * Author: Ing. Juan Camilo GÃƒÂ³mez Cadavid MSc.
 * 
 * Created on 7 de abril de 2011, 04:39 PM
 */

#include "NMEAParseGPRMCGGA.h"

volatile GPSISRBufferDataHandler_t GPSFrameISRData={{0}, 0, ISR_GPSISR_IDLE, 
#ifdef GPS_BUFFER_ISOLATION
{0}
#endif
};

volatile GPSISRBufferDataHandler_t *GPSISRptr = &GPSFrameISRData;


/*============================================================================*/
unsigned char GPRMCGetStatus(GPRMCInfoIndex_t *infoobj){
    return infoobj->Data[1][0];
}
#ifdef NMEA_EXTENDEDINFO
/*============================================================================*/
int GPRMCGetOtherInfo(GPRMCOtherInfo *dataobj, GPRMCInfoIndex_t *infoobj){
    dataobj->TrackAngle = (float)satof(infoobj->Data[7]);
    dataobj->MagneticVariation = (float)satof(infoobj->Data[9]);
    //dataobj->MVDir = infoobj->Data[10][0];
    return 0;
}
/*============================================================================*/
unsigned char GPRMCGetCheckSum(GPRMCInfoIndex_t *infoobj){
    unsigned char nible[2]={0x00};    
    nible[1] = (unsigned char)(infoobj->Data[11][0]-0x30);
    nible[1] = (nible[1]>=17)? nible[1]-7 : nible[1];
    nible[0] = (unsigned char)(infoobj->Data[11][1]-0x30);
    nible[0] = (nible[0]>=17)? nible[0]-7 : nible[0];
    return (((nible[1]<<4)&0xF0) | (nible[0]&0x0F));
}
/*============================================================================*/
int GPRMCGetTimeOfFix(GPRMCTimeOfFix *dataobj, GPRMCInfoIndex_t *infoobj){
    dataobj->Hour = (unsigned char)((infoobj->Data[0][0]-0x30)*10 +  (infoobj->Data[0][1]-0x30));
    dataobj->Minute = (unsigned char)((infoobj->Data[0][2]-0x30)*10 +  (infoobj->Data[0][3]-0x30));
    //dataobj->Seconds = (float)satof(infoobj->Data[0]+4);
    dataobj->Seconds = (unsigned char)((infoobj->Data[0][4]-0x30)*10 +  (infoobj->Data[0][5]-0x30));
    if (dataobj->Hour>24 || dataobj->Minute>60 || dataobj->Seconds>60.0) return -1;
    else return 0;
}
/*============================================================================*/
int GPRMCGetDate(GPRMCDateData *dataobj, GPRMCInfoIndex_t *infoobj){
    dataobj->Day = (unsigned char)((infoobj->Data[8][0]-0x30)*10 +  (infoobj->Data[8][1]-0x30));
    dataobj->Month = (unsigned char)((infoobj->Data[8][2]-0x30)*10 +  (infoobj->Data[8][3]-0x30));
    dataobj->Year = (unsigned char)((infoobj->Data[8][4]-0x30)*10 +  (infoobj->Data[8][5]-0x30));
    if (dataobj->Day>31 || dataobj->Month>12) return -1;
    else return 0;
}
#endif
/*============================================================================*/
int GPRMCGetSpeed(GPRMCSpeedData *datobj, GPRMCInfoIndex_t *infoobj){
    if(GPRMCGetStatus(infoobj)=='A'){
       datobj->SpeedKnots = (float)satof(infoobj->Data[6]);
       datobj->SpeedKmh = (float)((1.851999999984)*(datobj->SpeedKnots));
       return 0;
    }
    else return -1;
}
/*============================================================================*/
int GPRMCGetPosition(GPSPositionData *datobj, GPRMCInfoIndex_t *infoobj){       
    char *ptr;
    if(GPRMCGetStatus(infoobj)=='A'){
        datobj->LatitudeDir = infoobj->Data[3][0];
        datobj->LongitudeDir = infoobj->Data[5][0];        
        ptr = strstr(infoobj->Data[2], "."); 
        datobj->LatitudeMinutes  = satof(ptr-2); 
        ptr = strstr(infoobj->Data[4], ".");
        datobj->LongitudeMinutes  = satof(ptr-2); 
        datobj->LatitudeDegress = (short)(satof(infoobj->Data[2])/1E2);        
        datobj->LongitudeDegress = (short)(satof(infoobj->Data[4])/1E2);        
       
        datobj->LatitudeDD = ((datobj->LatitudeDir=='N')?1.0:-1.0) * ((double)datobj->LatitudeDegress) + (datobj->LatitudeMinutes/60.0);
        datobj->LongitudeDD = ((datobj->LongitudeDir=='W')?-1.0:1.0) * (((double)datobj->LongitudeDegress) + (datobj->LongitudeMinutes/60.0));
        return 0;
    }
    else return -1;
}
/*============================================================================*/
#ifdef NMEA_USE_GGA
int GPGGAGetPosition(GPSPositionData *datobj, GPGGAInfoIndex_t *infoobj){
     char *ptr;
     datobj->LatitudeDir = infoobj->Data[2][0];
     datobj->LongitudeDir = infoobj->Data[4][0];
     ptr = strstr(infoobj->Data[1], "."); 
     datobj->LatitudeMinutes  = satof(ptr-2); 
     ptr = strstr(infoobj->Data[3], ".");
     datobj->LongitudeMinutes  = satof(ptr-2); 
     datobj->LatitudeDegress = (short)satof(infoobj->Data[1])/1E2;        
     datobj->LongitudeDegress = (short)satof(infoobj->Data[3])/1E2; 
     datobj->LatitudeDD = ((datobj->LatitudeDir=='N')?1.0:-1.0) * ((double)datobj->LatitudeDegress) + (datobj->LatitudeMinutes/60.0);
     datobj->LongitudeDD = ((datobj->LongitudeDir=='W')?-1.0:1.0) * (((double)datobj->LongitudeDegress) + (datobj->LongitudeMinutes/60.0));
     return 0;
}
/*============================================================================*/
int GPGGAGetFixData(GPGGAFixData *dataobj, GPGGAInfoIndex_t *infoobj){
    dataobj->FixQuality  = atoi(infoobj->Data[5]);
    dataobj->NoOfSatellitesBeingTracked = atoi(infoobj->Data[6]);
    dataobj->HorizontalDilution = satof(infoobj->Data[7]);
    dataobj->Altitude = satof(infoobj->Data[8]);
    dataobj->HeightOfGeoid = satof(infoobj->Data[10]);
    return 0;
}
#endif
/*============================================================================*/
char* GPRMCFrameIsolate(char *buffer, char *type){
    char pattern[8];
    strcpy(pattern, type);
    char *ptr = NULL;
    char *end = NULL;
    if ( (ptr= strstr(buffer,pattern)) == NULL ){ 
        pattern[2]='N';
        if ( (ptr= strstr(buffer,pattern)) == NULL ) return NULL;
    }
    if((end = strstr(ptr+2,"*"))==NULL ) return NULL;
    *(end+3)=0x00;
    return ptr;
}
/*============================================================================*/
unsigned char ValidateGPRMCCheckSUM(GPRMCInfoIndex_t *obj, const char *buffer){
    unsigned char chk = 0;
    int i;
    for(i=obj->indexinit; i<=obj->indexend;i++) chk^=buffer[i];
    return (unsigned char)(chk == GPRMCGetCheckSum(obj));
}
/*============================================================================*/
int ParseNMEAFrameGPRMC(GPRMCInfoIndex_t *obj,  const char *buffer){
    char indexes[_GPS_NMEA_GPRMC_AMOUNT_OF_DATA_]={0};
    char *ptrinit=NULL;
    char *ptrend=NULL;
    int FrameSize=0;
    ptrinit=strstr(buffer,"$GPRMC,");
    if (ptrinit==NULL){ 
        ptrinit=strstr(buffer,"$GNRMC,");
        if(ptrinit==NULL) return -1;
    }
    obj->indexinit = (unsigned char)(ptrinit-buffer+1);
    ptrend=strstr(ptrinit,"*");
    if (ptrend!=NULL){
    	obj->indexend = (unsigned char)(ptrend-buffer-1);
        int i;
        int mod=0;
        FrameSize = ptrend-ptrinit+2;
        for(i=0;i<(FrameSize);i++){
            if(ptrinit[i]==',' && mod<_GPS_NMEA_GPRMC_AMOUNT_OF_DATA_){
                indexes[mod]=(char)i;
                mod++;
            }
        }
        for(i=0;i<(_GPS_NMEA_GPRMC_AMOUNT_OF_DATA_-1);i++){
            obj->Data[i]=ptrinit+indexes[i]+1;
            obj->Size[i]=(size_t)(indexes[i+1]-indexes[i]-1);
        }
        obj->Data[11] = ptrend+1; //checksum
        obj->Size[11] = 2;
        return FrameSize;
    }
    else return -1;
}
/*============================================================================*/
#ifdef NMEA_USE_GGA
int ParseNMEAFrameGPGGA(GPGGAInfoIndex_t *obj,  const char *buffer){
    char indexes[_GPS_NMEA_GPGGA_AMOUNT_OF_DATA_]={0};
    char *ptrinit=NULL;
    char *ptrend=NULL;
    int FrameSize=0;
    ptrinit=strstr(buffer,"$GPGGA,");
    if (ptrinit==NULL){ 
        ptrinit=strstr(buffer,"$GNGGA,");
        if(ptrinit==NULL) return -1;
    }
    ptrend=strstr(ptrinit,"*");
    if (ptrend!=NULL){
        int i;
        int mod=0;
        FrameSize = ptrend-ptrinit+2;
        for(i=0;i<(FrameSize);i++){
            if(ptrinit[i]==',' && mod<_GPS_NMEA_GPGGA_AMOUNT_OF_DATA_){
                indexes[mod]=i;
                mod++;
            }
        }
        for(i=0;i<(_GPS_NMEA_GPGGA_AMOUNT_OF_DATA_-1);i++){
            obj->Data[i]=ptrinit+indexes[i]+1;
            obj->Size[i]=indexes[i+1]-indexes[i]-1;
        }
        obj->Data[11] = ptrend+1; //checksum
        obj->Size[11] = 2;
        return FrameSize;
    }
    else return -1;
}
#endif
/*============================================================================*/
unsigned char GPSISRHandler(char RxChar){
    if(GPSFrameISRData.State == ISR_GPSISR_READY) return 0;
    if(RxChar == '$'){
    	GPSFrameISRData.State  = ISR_GPSISR_PROCESSING;
    	GPSFrameISRData.Index = 0;
    }
    if(GPSFrameISRData.State == ISR_GPSISR_PROCESSING){
    	GPSFrameISRData.Buffer[GPSFrameISRData.Index] = RxChar;
    	GPSFrameISRData.Index++;
    	GPSFrameISRData.Buffer[GPSFrameISRData.Index] = 0x00;
    	if(RxChar == '\n'){
            GPSFrameISRData.State = ISR_GPSISR_READY;
            return 1;
        }
    }
    return 0;
}
/*============================================================================*/
#ifdef GPS_BUFFER_ISOLATION
char* GPSGetNMEAFrame(void){
    if (GPSFrameISRData.State == ISR_GPSISR_READY){
        memset((void*)GPSFrameISRData.IsolatedBuffer, 0x00, sizeof(GPSFrameISRData.IsolatedBuffer));
        strcpy((char*)GPSFrameISRData.IsolatedBuffer, (const char *)GPSFrameISRData.Buffer);
        memset((void*)GPSFrameISRData.Buffer, 0x00, sizeof(GPSFrameISRData.Buffer));
        GPSFrameISRData.State = ISR_GPSISR_IDLE;
        return (char*)GPSFrameISRData.IsolatedBuffer;
    }
    else return NULL;
}
#endif
/*============================================================================*/
#ifdef USE_SOFTWARE_ATOF
int ipow(int base, int exp){
    int result = 1;
    while (exp){
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

int numPlaces(int n){
    int r = 1;
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

double satof(const char *str){
    double dataout;
    int pe,pf,np;
    char *ptr = NULL;
    pe = atoi(str);
    if ((ptr = strstr(str,"."))==NULL) return (double)pe;
    pf = atoi(ptr+1); 
    np = numPlaces(pf);
    dataout = pe+(((str[0]=='-')? -1 : 1)*(((double)pf)/((double)ipow(10,np))));
    return dataout;
}
#endif
