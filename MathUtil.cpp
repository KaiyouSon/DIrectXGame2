#include "MathUtil.h"

const float PI = 3.141592654f;

/// <summary>
/// �ʓx�@����x���@�ɕϊ�����֐�
/// </summary>
float Angle(float radian)
{
	return radian * 360 / (2 * PI);
}

/// <summary>
/// �x���@����ʓx�@�ɕϊ�����֐�
/// </summary>
float Radian(float angle)
{
	return angle * (2 * PI) / 360;
}