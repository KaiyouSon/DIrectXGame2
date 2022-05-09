#include "MathUtil.h"

const float PI = 3.141592654f;

/// <summary>
/// 弧度法から度数法に変換する関数
/// </summary>
float Angle(float radian)
{
	return radian * 360 / (2 * PI);
}

/// <summary>
/// 度数法から弧度法に変換する関数
/// </summary>
float Radian(float angle)
{
	return angle * (2 * PI) / 360;
}