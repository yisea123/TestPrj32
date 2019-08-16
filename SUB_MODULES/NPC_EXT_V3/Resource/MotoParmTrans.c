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
	
	
	/*��������*/
f64 Round(f64 num) {
	return (s32)(num > 0 ? num + 0.5 : num - 0.5);
}
	/*ͨ���뾶���Բ�ģ����sx��sy���յ�ex��ey*/
_PT CircleCentreFromRadius(f64 sx, f64 sy, f64 ex, f64 ey
	, f64 r, u8 cw) {
	_VT vse = { 0, 0 };//������յ������
	_VT t = { 0, 0 };//vse�Ĵ�ֱ����֮һ
	_PT p = { -1,-1 };//����point
	f64 cj,z2,z;
	vse.x = ex - sx;
	vse.y = ey - sy;//������յ������
	t.x = vse.y;
  t.y =	-vse.x;//vse�Ĵ�ֱ����֮һ
	cj = vse.x*t.y - vse.y*t.x;//����жϷ���
//	if (0 == cj) {
//		return p;
//	}
	if (0 == cj) {//�յ�����Ϊͬһ��
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
	else if(cw) {//˳ʱ��

	}
//	else if(cj > 0 && cw) {//˳ʱ��

//	}
	else//��ʱ�룬���ô�ֱ����֮��
	{
		t.x = -vse.y;
		t.y = vse.x;
	}

	//���㴹ֱ����ϵ��
	z2 = (4 * r*r) / (vse.x * vse.x + vse.y * vse.y) - 1;

	z = sqrt(z2);
	//�õ�Բ��
	p.x = Round(sx + (vse.x + t.x * z) / 2);
	p.y = Round(sy + (vse.y + t.y * z) / 2);

	return p;

}
/*ͨ����������Բ�ģ����sx��sy���յ�ex��ey��������tx��ty*/
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
/*ͨ�����߽ǻ��Բ�ģ����sx��sy���յ�ex��ey��*/
_PT CircleCentreFromTangent(f64 sx, f64 sy, f64 ex, f64 ey, f64 d, u8 cw) {
	_VT vse = {0, 0 };//������յ������
	f64 r;
	_PT p = { -1, -1 };//����point
  vse.x = ex - sx;
  vse.y = ey - sy;//������յ������
	if (d <= 0 || d >= 180) {
		return p;
	}
//	double sinV = sin(d*PI/180);
	r = sqrt(vse.x*vse.x + vse.y*vse.y) / (2 * sin(d*PI / 180));

	p = CircleCentreFromRadius(sx, sy, ex, ey, r, cw);

	return p;
}


