#include "PlayerBullet.h"
#include "DebugText.h"
#include "Util.h"
#include "ViewProjection.h"

PlayerBullet::PlayerBullet() : pos(0, 0, 0), speed(2), isShot(false)
{
}

PlayerBullet::~PlayerBullet()
{
	delete model;
}

void PlayerBullet::Initialize()
{
	textureHandle = TextureManager::Load("bullet.png");
	model = Model::Create();

	trans.Initialize();
	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.WorldTransformationMatrix();
	isShot = false;
}

void PlayerBullet::Generate(const Vector3& pos)
{
	this->pos = pos;
	isShot = true;
}

void PlayerBullet::Update()
{
	if (isShot == false) return;
	pos.z += speed;

	if (pos.z >= 200) isShot = false;

	trans.translation_ = pos;
	trans.WorldTransformationMatrix();

	DebugText* debug = DebugText::GetInstance();

	debug->SetPos(0, 0);
	debug->Printf("%f,%f,%f", pos.x, pos.y, pos.z);

}

void PlayerBullet::Draw()
{
	if (isShot == false) return;
	model->Draw(trans, view, textureHandle);
}
