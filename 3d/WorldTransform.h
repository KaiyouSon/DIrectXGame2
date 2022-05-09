#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void WorldTransformationMatrix()
	{
		// スケーリング行列を宣言
		Matrix4 matScale = MathUtility::Matrix4Scaling(scale_.x, scale_.y, scale_.z);

		// 回転行列を宣言
		Matrix4 matRotZ = MathUtility::Matrix4RotationZ(rotation_.z);
		Matrix4 matRotX = MathUtility::Matrix4RotationX(rotation_.x);
		Matrix4 matRotY = MathUtility::Matrix4RotationY(rotation_.y);
		Matrix4 matRot = matRotZ * matRotX * matRotY;

		// 平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Translation(
			translation_.x,
			translation_.y,
			translation_.z);

		// 単位行列
		matWorld_ = MathUtility::Matrix4Identity();
		matWorld_ *= matScale * matRot * matTrans;

		TransferMatrix();
	}
};
