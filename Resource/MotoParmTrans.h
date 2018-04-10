
#ifndef  _MOTO_PARM_TRANS_
#define  _MOTO_PARM_TRANS_
typedef            float       f32;                        /* 32-bit floating point                                */
typedef            double      f64;                        /* 64-bit floating point                                */

#include "cdv_include.h"
#include <math.h>
typedef struct {
	f64 x;
	f64 y;
}_VT;

typedef struct {
	s32 x;
	s32 y;
}_PT;


_PT CircleCentreFromRadius(f64 sx, f64 sy, f64 ex, f64 ey, f64 r, u8 cw);
_PT CircleCentreFromThirdPoint(f64 sx, f64 sy, f64 ex, f64 ey, f64 tx, f64 ty);
_PT CircleCentreFromTangent(f64 sx, f64 sy, f64 ex, f64 ey, f64 d, u8 cw);


#endif

