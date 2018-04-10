/**
  ******************************************************************************
  * @file    /MotoParmTrans.c 
  * @author  MMY
  * @version V0.0.1
  * @date    3-10-2016
  * @brief   a package of moto parameter transfer
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	
	#include "MotoParmTrans.h"
	
	
	/*四舍五入*/
f64 Round(f64 num) {
	return (s32)(num > 0 ? num + 0.5 : num - 0.5);
}
	/*通过半径获得圆心，起点sx，sy；终点ex，ey*/
_PT CircleCentreFromRadius(f64 sx, f64 sy, f64 ex, f64 ey
	, f64 r, u8 cw) {
	_VT vse = { 0, 0 };//起点至终点的向量
	_VT t = { 0, 0 };//vse的垂直向量之一
	_PT p = { -1,-1 };//返回point
	f64 cj,z2,z;
	vse.x = ex - sx;
	vse.y = ey - sy;//起点至终点的向量
	t.x = vse.y;
  t.y =	-vse.x;//vse的垂直向量之一
	cj = vse.x*t.y - vse.y*t.x;//叉积判断方向
//	if (0 == cj) {
//		return p;
//	}
	if (0 == cj) {//终点和起点为同一点
		if(cw==0)
		{
			 p.x = ex-r;
			 p.y = ey;
		}	
		else
		{
			 p.x = r-ex;
			 p.y = ey;
		}	
		return p;
	}
	else if(cw) {//顺时针

	}
//	else if(cj > 0 && cw) {//顺时针

//	}
	else//逆时针，改用垂直向量之二
	{
		t.x = -vse.y;
		t.y = vse.x;
	}

	//计算垂直向量系数
	z2 = (4 * r*r) / (vse.x * vse.x + vse.y * vse.y) - 1;

	z = sqrt(z2);
	//得到圆心
	p.x = Round(sx + (vse.x + t.x * z) / 2);
	p.y = Round(sy + (vse.y + t.y * z) / 2);

	return p;

}
/*通过第三点获得圆心，起点sx，sy；终点ex，ey；第三点tx，ty*/
_PT CircleCentreFromThirdPoint(f64 sx, f64 sy, f64 ex, f64 ey, f64 tx, f64 ty) {
	f64 u1, u2, d11, d12, d21, d22;
	_PT p;
	u1 = (ex*ex - sx*sx + ey*ey - sy*sy) / 2;
	u2 = (tx*tx - sx*sx + ty*ty - sy*sy) / 2;
	d11 = ex - sx;
	d12 = ey - sy;
	d21 = tx - sx;
	d22 = ty - sy;

	p.x = Round((u1*d22 - u2*d12) / (d11*d22 - d21*d12));
	p.y = Round((u2*d11 - u1*d21) / (d11*d22 - d21*d12));

	return p;
}
#define PI 3.1415926
/*通过切线角获得圆心，起点sx，sy；终点ex，ey；*/
_PT CircleCentreFromTangent(f64 sx, f64 sy, f64 ex, f64 ey, f64 d, u8 cw) {
	_VT vse = {0, 0 };//起点至终点的向量
	f64 r;
	_PT p = { -1, -1 };//返回point
  vse.x = ex - sx;
  vse.y = ey - sy;//起点至终点的向量
	if (d <= 0 || d >= 180) {
		return p;
	}
//	double sinV = sin(d*PI/180);
	r = sqrt(vse.x*vse.x + vse.y*vse.y) / (2 * sin(d*PI / 180));

	p = CircleCentreFromRadius(sx, sy, ex, ey, r, cw);

	return p;
}


