/* 
 * File:   PostitionCoordinatesTransformatios.c
 * Author: Ing. Juan Camilo Gomez Cadavid MSc.
 *
 * Created on 7 de mayo de 2011, 12:08 PM
 */

#include "PosCoordTransform.h"

static double EquatorialRadius[]={6377563, 6378160, 6377397, 6377484, 6378206, 6378249, 6377276, 6378166, 6378150, 6378160,
                               6378137, 6378200, 6378270, 6378388, 6378245, 6377340, 6377304, 6378155, 6378160, 6378165,
                               6378145, 6378135, 6378137};
static double SquareOfEccentricity[]={.00667054,.006694542,.006674372,.006674372,.006768658,.006803511,.006637847,.006693422,.006693422,.006694605,
                                   .00669438,.006693422,.00672267,.00672267,.006693422,.00667054,.006637847,.006693422,.006694542,.006693422,
                                   .006694542,.006694318,0.006694380004260827};
static unsigned char UTMHemisphere[]={'N','S'};
void Geodesic2UTM(Position_UTM_t *obj, unsigned char ReferenceEllipsoid, double Lat, double Long){
    double a=EquatorialRadius[ReferenceEllipsoid];
    double eccSquared = SquareOfEccentricity[ReferenceEllipsoid];
    double k0 = 0.9996; //scale factor along the central meridian
    double LongOriginRad, eccPrimeSquared, N, T, C, AA, M;
    double LongTemp = (Long+180)-floor((Long+180)/360)*360-180; // -180.00 .. 179.9;
    double LatRad = Lat*__DEG2RAD;
    double LongRad = LongTemp*__DEG2RAD;
    double AA2,AA3,AA4,AA5,AA6,eccSquared2,eccSquared3;
    obj->UTMZone = floor((LongTemp + 180)/6) + 1;
    if( Lat>=56.0 && Lat<64.0 && LongTemp>=3.0 && LongTemp< 12.0) obj->UTMZone = 32;
    
    if( Lat >= 72.0 && Lat < 84.0 ){  // Special zones for Svalbard
      if     (LongTemp>=0.0  && LongTemp<9.0 )  obj->UTMZone = 31;
      else if(LongTemp>=9.0  && LongTemp<21.0)  obj->UTMZone = 33;
      else if(LongTemp>=21.0 && LongTemp<33.0)  obj->UTMZone = 35;
      else if(LongTemp>=33.0 && LongTemp<42.0)  obj->UTMZone = 37;
    }
    LongOriginRad =( (obj->UTMZone - 1)*6 - 180 + 3 )*__DEG2RAD;  //+3 puts origin in middle of zone
    // transformation to utm
    eccPrimeSquared = (eccSquared)/(1-eccSquared);
    N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
    T = tan(LatRad)*tan(LatRad);
    C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
    AA = cos(LatRad)*(LongRad-LongOriginRad);
    
    AA2=AA*AA;
    AA3=AA2*AA;
    AA4=AA3*AA;
    AA5=AA4*AA;
    AA6=AA5*AA;
    eccSquared2=eccSquared*eccSquared;
    eccSquared3=eccSquared2*eccSquared;
    
    M  = (1 - eccSquared/4 - 3*eccSquared2/64 - 5*eccSquared3/256)*LatRad;
    M -= (3*eccSquared/8 + 3*eccSquared2/32 + 45*eccSquared3/1024)*sin(2*LatRad);
    M += (15*eccSquared2/256 + 45*eccSquared3/1024)*sin(4*LatRad);
    M -= (35*eccSquared3/3072)*sin(6*LatRad);
    M *= a;
    
    obj->UTMEasting = (k0*N*(AA+(1-T+C)*AA3/6+ (5-18*T+T*T+72*C-58*eccPrimeSquared)*AA5/120)+ 500000.0);
    obj->UTMNorthing  = (k0*(M+N*tan(LatRad)*(AA2/2+(5-T+9*C+4*C*C)*AA4/24+ (61-58*T+T*T+600*C-330*eccPrimeSquared)*AA6/720)));
    obj->UTMHemisphere = UTMHemisphere[(Lat < 0)];
    if(obj->UTMHemisphere=='S') obj->UTMNorthing+= 10000000.0; //10000000 meter offset for southern hemisphere

}

