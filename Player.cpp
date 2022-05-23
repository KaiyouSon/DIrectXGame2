#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "Util.h"
#include "ViewProjection.h"

Player::Player() : pos(0, 0, 0)
{
}

Player::~Player()
{
	delete model;
}

void Player::Initialize()
{
	textureHandle = TextureManager::Load("mario.jpg");
	model = Model::Create();


	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	DebugText* debug = DebugText::GetInstance();

	if (input->PushKey(DIK_LEFT))
		pos.x--;
	if (input->PushKey(DIK_RIGHT))
		pos.x++;
	if (input->PushKey(DIK_UP))
		pos.y++;
	if (input->PushKey(DIK_DOWN))
		pos.y--;

	if (pos.x >= 30)  pos.x = 30;
	if (pos.x <= -30) pos.x = -30;
	if (pos.y >= 15)  pos.y = 15;
	if (pos.y <= -15) pos.y = -15;

	trans.translation_ = pos;
	trans.WorldTransformationMatrix();


	debug->SetPos(0, 0);
	debug->Printf("%f,%f", pos.x, pos.y);
}

void Player::Draw()
{
	//model->Draw(worldTransforms, debugCamera->GetViewProjection(), textureHandle);
	model->Draw(trans, view, textureHandle);
}
