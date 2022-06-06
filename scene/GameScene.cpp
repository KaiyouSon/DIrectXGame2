#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MathUtil.h"
#include <cassert>
#include <math.h>
#include <random>

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

	//// 乱数シート生成器
	//std::random_device seed_gen;
	//// メルセンヌ・ツイスターの乱数エンジン
	//std::mt19937_64 engine(seed_gen());
	//// 乱数範囲（回転角用）
	//std::uniform_real_distribution<float> rotDist(0, 360);
	//// 乱数範囲（座標用）
	//std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	CharcterInit();
	// ワールドトランスフォームの初期化
	//worldTransforms[0].Initialize();

	//// モデルのスケーリング
	//worldTransforms[0].scale_ = { 1,1,1 };
	//// モデルの回転
	//worldTransforms[0].rotation_ = { 0,0,0 };
	//// モデルの平行移動
	//worldTransforms[0].translation_ = { 0,0,0 };

	//worldTransforms[0].WorldTransformationMatrix();

	//// ワールドトランスフォームの初期化
	//worldTransforms[1].Initialize();

	//// モデルのスケーリング
	//worldTransforms[1].scale_ = { 1,1,1 };
	//// モデルの回転
	//worldTransforms[1].rotation_ = { 0,0,0 };
	//// モデルの平行移動
	//worldTransforms[1].translation_ = { 5,0,0 };

	//worldTransforms[1].WorldTransformationMatrix();

	//worldTransforms[1].parent_ = &worldTransforms[0];

	// カメラの視点指定
	viewProjection.eye = { 0,0,-50 };
	viewProjection.target = { 0,0,0 };
	viewProjection.up = { 0, 1, 0 };

	viewProjection.Initialize();

	debugCamera = new DebugCamera(WIN_WIDTH, WIN_HEIGHT);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);

	// ライン描画が参照参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera->GetViewProjection());
}

void GameScene::Update() {

	debugCamera->Update();

	CharcterUpdate();


	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"eye:(%f,%f,%f)",
		viewProjection.eye.x,
		viewProjection.eye.y,
		viewProjection.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"target:(%f,%f,%f)",
		viewProjection.target.x,
		viewProjection.target.y,
		viewProjection.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"up:(%f,%f,%f)",
		viewProjection.up.x,
		viewProjection.up.y,
		viewProjection.up.z);
}

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
	CharcterDraw();
	//model->Draw(worldTransforms[i], debugCamera->GetViewProjection(), textureHandle);

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

void GameScene::CharcterInit()
{
	// 大元の初期化
	worldTransforms[PartID::Root].translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Root].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Root].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::Root].Initialize();
	worldTransforms[PartID::Root].WorldTransformationMatrix();

	// 上半身の初期化
	worldTransforms[PartID::UpperBody].parent_ = &worldTransforms[PartID::Root];
	worldTransforms[PartID::UpperBody].translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::UpperBody].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::UpperBody].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::UpperBody].Initialize();
	worldTransforms[PartID::UpperBody].WorldTransformationMatrix();

	// 頭の初期化
	worldTransforms[PartID::Head].parent_ = &worldTransforms[PartID::UpperBody];
	worldTransforms[PartID::Head].translation_ = { 0.0f, 2.5f, 0.0f };
	worldTransforms[PartID::Head].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Head].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::Head].Initialize();
	worldTransforms[PartID::Head].WorldTransformationMatrix();

	// 胸の初期化
	worldTransforms[PartID::Chest].parent_ = &worldTransforms[PartID::UpperBody];
	worldTransforms[PartID::Chest].translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Chest].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Chest].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::Chest].Initialize();
	worldTransforms[PartID::Chest].WorldTransformationMatrix();

	// 左腕の初期化
	worldTransforms[PartID::ArmL].parent_ = &worldTransforms[PartID::UpperBody];
	worldTransforms[PartID::ArmL].translation_ = { 2.5f, 0.0f, 0.0f };
	worldTransforms[PartID::ArmL].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::ArmL].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::ArmL].Initialize();
	worldTransforms[PartID::ArmL].WorldTransformationMatrix();

	// 右腕の初期化
	worldTransforms[PartID::ArmR].parent_ = &worldTransforms[PartID::UpperBody];
	worldTransforms[PartID::ArmR].translation_ = { -2.5f, 0.0f, 0.0f };
	worldTransforms[PartID::ArmR].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::ArmR].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::ArmR].Initialize();
	worldTransforms[PartID::ArmR].WorldTransformationMatrix();

	// 下半身の初期化
	worldTransforms[PartID::LowerBody].parent_ = &worldTransforms[PartID::Root];
	worldTransforms[PartID::LowerBody].translation_ = { 0.0f, -2.5f, 0.0f };
	worldTransforms[PartID::LowerBody].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::LowerBody].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::LowerBody].Initialize();
	worldTransforms[PartID::LowerBody].WorldTransformationMatrix();

	// 尻の初期化
	worldTransforms[PartID::Hip].parent_ = &worldTransforms[PartID::LowerBody];
	worldTransforms[PartID::Hip].translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Hip].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::Hip].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::Hip].Initialize();
	worldTransforms[PartID::Hip].WorldTransformationMatrix();

	// 左足の初期化
	worldTransforms[PartID::LegL].parent_ = &worldTransforms[PartID::LowerBody];
	worldTransforms[PartID::LegL].translation_ = { 2.5f, -2.5f, 0.0f };
	worldTransforms[PartID::LegL].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::LegL].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::LegL].Initialize();
	worldTransforms[PartID::LegL].WorldTransformationMatrix();

	// 右足の初期化
	worldTransforms[PartID::LegR].parent_ = &worldTransforms[PartID::LowerBody];
	worldTransforms[PartID::LegR].translation_ = { -2.5f, -2.5f, 0.0f };
	worldTransforms[PartID::LegR].rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransforms[PartID::LegR].scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransforms[PartID::LegR].Initialize();
	worldTransforms[PartID::LegR].WorldTransformationMatrix();
}

void GameScene::CharcterUpdate()
{
	if (input_->PushKey(DIK_RIGHT))
		worldTransforms[PartID::Root].translation_.x++;
	if (input_->PushKey(DIK_LEFT))
		worldTransforms[PartID::Root].translation_.x--;

	const float rotAngle = 5;

	if (input_->PushKey(DIK_U))
		worldTransforms[PartID::UpperBody].rotation_.y += Radian(rotAngle);
	if (input_->PushKey(DIK_I))
		worldTransforms[PartID::UpperBody].rotation_.y -= Radian(rotAngle);

	if (input_->PushKey(DIK_J))
		worldTransforms[PartID::LowerBody].rotation_.y += Radian(rotAngle);
	if (input_->PushKey(DIK_K))
		worldTransforms[PartID::LowerBody].rotation_.y -= Radian(rotAngle);

	worldTransforms[PartID::Root].WorldTransformationMatrix();

	worldTransforms[PartID::UpperBody].WorldTransformationMatrix();
	worldTransforms[PartID::UpperBody].matWorld_ *= worldTransforms[PartID::Root].matWorld_;
	worldTransforms[PartID::UpperBody].TransferMatrix();

	worldTransforms[PartID::Head].WorldTransformationMatrix();
	worldTransforms[PartID::Head].matWorld_ *= worldTransforms[PartID::UpperBody].matWorld_;
	worldTransforms[PartID::Head].TransferMatrix();

	worldTransforms[PartID::Chest].WorldTransformationMatrix();
	worldTransforms[PartID::Chest].matWorld_ *= worldTransforms[PartID::UpperBody].matWorld_;
	worldTransforms[PartID::Chest].TransferMatrix();

	worldTransforms[PartID::ArmL].WorldTransformationMatrix();
	worldTransforms[PartID::ArmL].matWorld_ *= worldTransforms[PartID::UpperBody].matWorld_;
	worldTransforms[PartID::ArmL].TransferMatrix();

	worldTransforms[PartID::ArmR].WorldTransformationMatrix();
	worldTransforms[PartID::ArmR].matWorld_ *= worldTransforms[PartID::UpperBody].matWorld_;
	worldTransforms[PartID::ArmR].TransferMatrix();

	worldTransforms[PartID::LowerBody].WorldTransformationMatrix();
	worldTransforms[PartID::LowerBody].matWorld_ *= worldTransforms[PartID::Root].matWorld_;
	worldTransforms[PartID::LowerBody].TransferMatrix();

	worldTransforms[PartID::Hip].WorldTransformationMatrix();
	worldTransforms[PartID::Hip].matWorld_ *= worldTransforms[PartID::LowerBody].matWorld_;
	worldTransforms[PartID::Hip].TransferMatrix();

	worldTransforms[PartID::LegL].WorldTransformationMatrix();
	worldTransforms[PartID::LegL].matWorld_ *= worldTransforms[PartID::LowerBody].matWorld_;
	worldTransforms[PartID::LegL].TransferMatrix();

	worldTransforms[PartID::LegR].WorldTransformationMatrix();
	worldTransforms[PartID::LegR].matWorld_ *= worldTransforms[PartID::LowerBody].matWorld_;
	worldTransforms[PartID::LegR].TransferMatrix();
}

void GameScene::CharcterDraw()
{
	model->Draw(worldTransforms[PartID::Head], viewProjection, textureHandle);
	model->Draw(worldTransforms[PartID::Chest], viewProjection, textureHandle);
	model->Draw(worldTransforms[PartID::ArmL], viewProjection, textureHandle);
	model->Draw(worldTransforms[PartID::ArmR], viewProjection, textureHandle);
	model->Draw(worldTransforms[PartID::Hip], viewProjection, textureHandle);
	model->Draw(worldTransforms[PartID::LegL], viewProjection, textureHandle);
	model->Draw(worldTransforms[PartID::LegR], viewProjection, textureHandle);
}
