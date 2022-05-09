#pragma once
/// <summary>
/// 行列
/// </summary>
class Matrix4 {
public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	// 単項演算子オーバーロード
	Matrix4 operator*(const Matrix4& other) const
	{
		return {
			//	一行目
			m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] +
			m[0][2] * other.m[2][0] + m[0][3] * other.m[3][0],

			m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] +
			m[0][2] * other.m[2][1] + m[0][3] * other.m[3][1],

			m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] +
			m[0][2] * other.m[2][2] + m[0][3] * other.m[3][2],

			m[0][0] * other.m[0][3] + m[0][1] * other.m[1][3] +
			m[0][2] * other.m[2][3] + m[0][3] * other.m[3][3],

			// 二行目
			m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] +
			m[1][2] * other.m[2][0] + m[1][3] * other.m[3][0],

			m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] +
			m[1][2] * other.m[2][1] + m[1][3] * other.m[3][1],

			m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] +
			m[1][2] * other.m[2][2] + m[1][3] * other.m[3][2],

			m[1][0] * other.m[0][3] + m[1][1] * other.m[1][3] +
			m[1][2] * other.m[2][3] + m[1][3] * other.m[3][3],

			// 三行目
			m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] +
			m[2][2] * other.m[2][0] + m[2][3] * other.m[3][0],

			m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] +
			m[2][2] * other.m[2][1] + m[2][3] * other.m[3][1],

			m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] +
			m[2][2] * other.m[2][2] + m[2][3] * other.m[3][2],

			m[2][0] * other.m[0][3] + m[2][1] * other.m[1][3] +
			m[2][2] * other.m[2][3] + m[2][3] * other.m[3][3],

			// 四行目
			m[3][0] * other.m[0][0] + m[3][1] * other.m[1][0] +
			m[3][2] * other.m[2][0] + m[3][3] * other.m[3][0],

			m[3][0] * other.m[0][1] + m[3][1] * other.m[1][1] +
			m[3][2] * other.m[2][1] + m[3][3] * other.m[3][1],

			m[3][0] * other.m[0][2] + m[3][1] * other.m[1][2] +
			m[3][2] * other.m[2][2] + m[3][3] * other.m[3][2],

			m[3][0] * other.m[0][3] + m[3][1] * other.m[1][3] +
			m[3][2] * other.m[2][3] + m[3][3] * other.m[3][3],
		};
	}
};
