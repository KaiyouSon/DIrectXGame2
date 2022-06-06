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

	// X, Y, Z 軸周りの平行移動を設定
	worldTransform.translation_ = { 0.0f, 0.0f, 0.0f };
	// X, Y, Z 軸周りの回転角を設定
	worldTransform.rotation_ = { 0.0f, 0.0f, 0.0f };
	// X, Y, Z 方向のスケーリングを設定
	worldTransform.scale_ = { 1.0f, 1.0f, 1.0f };
	// ワールドトランスフォームの初期化
	worldTransform.Initialize();

	//// ワールドトランスフォームの初期化
	//for (size_t i = 0; i < _countof(worldTransforms); i++)
	//{
	//	worldTransforms[i].Initialize();

	//	// モデルのスケーリング
	//	worldTransforms[i].scale_ = { 1,1,1 };
	//	// モデルの回転
	//	worldTransforms[i].rotation_ = {
	//		Radian(rotDist(engine)),Radian(rotDist(engine)),Radian(rotDist(engine)) };
	//	// モデルの平行移動
	//	worldTransforms[i].translation_ = {
	//		posDist(engine),posDist(engine),posDist(engine) };

	//	worldTransforms[i].WorldTransformationMatrix();
	//}

	for (size_t i = 0; i < _countof(worldTransform2); i++) {

		// ワールドトランスフォームの初期化
		worldTransform2[i].Initialize();
		// X, Y, Z 軸周りの平行移動を設定
		worldTransform2[i].translation_ = { posDist(engine), posDist(engine), posDist(engine) };
		// X, Y, Z 軸周りの回転角を設定
		worldTransform2[i].rotation_ = {
			Radian(rotDist(engine)), Radian(rotDist(engine)), Radian(rotDist(engine)) };
		// X, Y, Z 方向のスケーリングを設定
		worldTransform2[i].scale_ = { 1.0f, 1.0f, 1.0f };

		worldTransform2[i].WorldTransformationMatrix();
	}

	// カメラの視点指定
	viewProjection.eye = { 0,5,-10 };
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

	objPos = { 0.0f, 0.0f, 0.0f };
	objFrontVec = { 0.0f, 0.0f, 1.0f };
	objRightVec = { 1.0f, 0.0f, 0.0f };
	objSpeed = { 0.1f, 0.0f, 0.1f };
	angle = 0.0f;

	walkType = false;
}

void GameScene::Update() {

	debugCamera->Update();

	if (input_->TriggerKey(DIK_Q))
	{
		if (walkType == false) walkType = true;
		else                   walkType = false;
	}

	if (walkType == false)
	{
		// バイオ歩き
		BioWalk();
	}
	if (walkType == true)
	{
		// カメラ視点の前後左右歩き
		DefaultWalk();
	}

	debugText_->SetPos(0, 0);
	if (walkType == false)	debugText_->Printf("BioWalk");
	if (walkType == true)	debugText_->Printf("Move of the camera viewport");

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
	model->Draw(worldTransform, debugCamera->GetViewProjection(), textureHandle);

	for (size_t i = 0; i < _countof(worldTransform2); i++)
		model->Draw(worldTransform2[i], debugCamera->GetViewProjection(), textureHandle);


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

void GameScene::BioWalk()
{
	if (input_->PushKey(DIK_W)) {

		objPos += objFrontVec * objSpeed;
	}
	if (input_->PushKey(DIK_S)) {

		objPos -= objFrontVec * objSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		angle -= 2;
		if (angle < 0)
			angle = 360;
	}
	if (input_->PushKey(DIK_D)) {
		angle += 2;
		if (angle > 360)
			angle = 0;
	}

	objFrontVec = { sin(Radian(angle)), 0.0f, cos(Radian(angle)) };
	worldTransform.translation_ = { objPos.x, objPos.y, objPos.z };
	worldTransform.rotation_ = { 0.0f, Radian(angle), 0.0f };

	worldTransform.WorldTransformationMatrix();
}
void GameScene::DefaultWalk()
{
	objFrontVec = debugCamera->GetViewProjection().target - debugCamera->GetViewProjection().eye;
	objRightVec = Vector3::Cross(Vector3(0, 1, 0), objFrontVec);
	objFrontVec = objFrontVec.Normalized();
	objRightVec = objRightVec.Normalized();

	float speed = 0.1;
	if (input_->PushKey(DIK_W))
	{
		objPos += objFrontVec * objSpeed;
	}
	if (input_->PushKey(DIK_S))
	{
		objPos -= objFrontVec * objSpeed;
	}
	if (input_->PushKey(DIK_D))
	{
		objPos += objRightVec * objSpeed;
	}
	if (input_->PushKey(DIK_A))
	{
		objPos -= objRightVec * objSpeed;
	}

	worldTransform.translation_ = { objPos.x, objPos.y, objPos.z };
	worldTransform.WorldTransformationMatrix();
}