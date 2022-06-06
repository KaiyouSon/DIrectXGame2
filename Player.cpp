#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "Util.h"
#include "ViewProjection.h"
#include <MathUtil.h>

Player::Player() :
	pos(0, 0, 0), frontVec(0, 0, 0), rotAngle(0),
	maxBullet(50), bullet(new PlayerBullet[maxBullet]),
	shotTimer(0), maxShotTimer(10)
{
}

Player::~Player()
{
	delete model;
	delete[] bullet;
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

	for (int i = 0; i < maxBullet; i++)
		bullet[i].Initialize();
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

	if (input->PushKey(DIK_Z))
	{
		rotAngle--;
		if (rotAngle < 0) rotAngle = 360;
	}

	if (input->PushKey(DIK_X))
	{
		rotAngle++;
		if (rotAngle > 360) rotAngle = 0;
	}

	frontVec = { sin(Radian(rotAngle)), 0.0f, cos(Radian(rotAngle)) };

	trans.translation_ = pos;
	trans.rotation_ = { 0,Radian(rotAngle),0 };
	trans.WorldTransformationMatrix();

	if (input->PushKey(DIK_SPACE))
	{
		shotTimer++;
		if (shotTimer >= maxShotTimer)
		{
			for (int i = 0; i < maxBullet; i++)
			{
				if (bullet[i].GetisShot() == false)
				{
					bullet[i].Generate(pos, frontVec);
					shotTimer = 0;
					break;
				}
			}
		}
	}

	for (int i = 0; i < maxBullet; i++)
		bullet[i].Update();



	//debug->SetPos(0, 0);
	//debug->Printf("%f,%f", pos.x, pos.y);
}

void Player::Draw()
{
	for (int i = 0; i < maxBullet; i++)
		bullet[i].Draw();

	model->Draw(trans, view, textureHandle);
}
