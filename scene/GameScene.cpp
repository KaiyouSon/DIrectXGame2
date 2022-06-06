#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MathUtil.h"
#include <cassert>
#include <math.h>
#include <random>
#include "Util.h"
#include "Player.h"

Player* player = new Player;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera;
	delete player;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// カメラの視点指定
	view.eye = { 0,0,-50 };
	view.target = { 0,0,0 };
	view.up = { 0, 1, 0 };
	view.Initialize();

	debugCamera = new DebugCamera(WIN_WIDTH, WIN_HEIGHT);
	isDebug = false;

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(false);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&view);

	// ライン描画が参照参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera->GetViewProjection());

	player->Initialize();
}

void GameScene::Update() {

	player->Update();

	if (input_->TriggerKey(DIK_F5))
	{
		if (isDebug == false) isDebug = true;
		else if (isDebug == true)  isDebug = false;
	}
	if (isDebug == true)
	{
		debugCamera->Update();
		view.matView = debugCamera->GetViewProjection().matView;
		view.matProjection = debugCamera->GetViewProjection().matProjection;
		view.TransferMatrix();
	}

	view.TransferMatrix();

	//debugText_->SetPos(WIN_WIDTH - 100, 0);
	//debugText_->Printf("isDebug = %d", isDebug);
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

	player->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}