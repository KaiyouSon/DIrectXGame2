#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MathUtil.h"
#include <cassert>
#include <math.h>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model;
	delete debugCamera;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle = TextureManager::Load("mario.jpg");
	model = Model::Create();

	worldTransform.Initialize();
	viewProjection.Initialize();

	worldTransform.scale_ = { 5,5,5 };							// モデルのスケーリング
	worldTransform.rotation_ = { Radian(45),Radian(45),0 };		// モデルの回転
	worldTransform.translation_ = { 10,10,10 };					// モデルの平行移動
	
	TransformInit2();

	debugCamera = new DebugCamera(WIN_WIDTH, WIN_HEIGHT);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera->GetViewProjection());

	// ライン描画が参照参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera->GetViewProjection());
}

void GameScene::Update() { debugCamera->Update(); }

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	model->Draw(worldTransform, debugCamera->GetViewProjection(), textureHandle);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, 0), Vector3(1, 1, 0), Vector4(255, 255, 255, 255));

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::TransformInit2()
{
	// スケーリング行列を宣言
	Matrix4 matScale = MathUtility::Matrix4Scaling(
		worldTransform.scale_.x,
		worldTransform.scale_.y,
		worldTransform.scale_.z);

	// 回転行列を宣言
	Matrix4 matRotZ = MathUtility::Matrix4RotationZ(worldTransform.rotation_.z);
	Matrix4 matRotX = MathUtility::Matrix4RotationX(worldTransform.rotation_.x);
	Matrix4 matRotY = MathUtility::Matrix4RotationY(worldTransform.rotation_.y);
	Matrix4 matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Translation(
		worldTransform.translation_.x,
		worldTransform.translation_.y,
		worldTransform.translation_.z);

	// 単位行列
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform.matWorld_ *= matScale * matRot * matTrans;

	worldTransform.TransferMatrix();
}