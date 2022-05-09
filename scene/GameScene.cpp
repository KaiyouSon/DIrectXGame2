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

	// 乱数シート生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0, 360);
	// 乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	// ワールドトランスフォームの初期化
	for (size_t i = 0; i < _countof(worldTransforms); i++)
	{
		worldTransforms[i].Initialize();

		// モデルのスケーリング
		worldTransforms[i].scale_ = { 1,1,1 };
		// モデルの回転
		worldTransforms[i].rotation_ = {
			Radian(rotDist(engine)),Radian(rotDist(engine)),Radian(rotDist(engine)) };
		// モデルの平行移動
		worldTransforms[i].translation_ = {
			posDist(engine),posDist(engine),posDist(engine) };

		worldTransforms[i].WorldTransformationMatrix();
	}


	// カメラの視点指定
	viewProjection.eye = { 0,0,-50 };
	viewProjection.target = { 10,0,0 };
	viewProjection.up = { cos(Radian(45)), sin(Radian(45)), 0 };

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

	// 視点の移動ベクトル
	Vector3 eyeMove = { 0,0,0 };
	// 視点の移動速さ
	const float viewEyeSpd = 0.2f;

	// 移動処理
	if (input_->PushKey(DIK_W)) eyeMove.z += viewEyeSpd;
	if (input_->PushKey(DIK_S)) eyeMove.z -= viewEyeSpd;

	// 視点移動
	viewProjection.eye += eyeMove;

	// 注視点の移動ベクトル
	Vector3 targetMove = { 0,0,0 };
	// 注視点の移動速さ
	const float viewTargetSpd = 0.2f;

	// 移動処理
	if (input_->PushKey(DIK_LEFT)) targetMove.x -= viewTargetSpd;
	if (input_->PushKey(DIK_RIGHT)) targetMove.x += viewTargetSpd;

	// 注視点移動
	viewProjection.target += targetMove;

	const float viewUpSpd = 1;

	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += viewUpSpd;
		viewAngle = fmodf(viewAngle, 360);
	}
	viewProjection.up = { cos(Radian(viewAngle)), sin(Radian(viewAngle)), 0 };

	// 行列の再計算
	viewProjection.UpdateMatrix();

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
		"target:(%f,%f,%f)",
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
	for (size_t i = 0; i < _countof(worldTransforms); i++)
		model->Draw(worldTransforms[i], viewProjection, textureHandle);
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