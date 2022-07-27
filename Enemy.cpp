#include "Enemy.h"
#include "Util.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "DebugText.h"
#include "PrimitiveDrawer.h"
using namespace std;

Enemy::Enemy() :
	generatePos(15, 0, 30), pos(generatePos),
	speed(0.25), currentState(new EnemyStateApproach),
	generateTimer(0), maxGenerateTimer(30)
{
}
Enemy::~Enemy()
{
	delete model;
	delete currentState;
}

void Enemy::Initialize()
{
	textureHandle = TextureManager::Load("kuribo.png");
	model = Model::Create();

	currentState->Initialize();
	//enemyBullet->Initialize();

	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}
void Enemy::Update()
{
	DebugText* debug = DebugText::GetInstance();

	// �e�̏���
	ShotUpdate();

	// ��ԑJ��
	StateChangeUpdate();

	trans.translation_ = pos;
	trans.WorldTransformationMatrix();

	//debug->SetPos(0, 0);
	//debug->Printf("generateTimer = %f", generateTimer);
}

void Enemy::Draw()
{
	model->Draw(trans, view, textureHandle);
	for (std::unique_ptr <EnemyBullet>& enemyBullet : enemyBullets)
	{
		enemyBullet->Draw();
	}

}

void Enemy::SetPos(Vector3 pos)
{
	this->pos = pos;
}

Vector3 Enemy::GetGeneratePos()
{
	return generatePos;
}
Vector3 Enemy::GetPos()
{
	return pos;
}

void Enemy::Fire()
{
	unique_ptr <EnemyBullet> newBullet = make_unique<EnemyBullet>();
	newBullet->Initialize();
	if (newBullet->GetisActive() == false)
		newBullet->Generate(pos);
	enemyBullets.push_back(move(newBullet));
}

void Enemy::ShotUpdate()
{
	if (currentState->GetTag() == "Approach")
	{
		generateTimer++;
		if (generateTimer >= maxGenerateTimer)
		{
			// �����o�[�֐��ƌĂяo������bind����std::function�ɑ��
			function<void(void)> callback = bind(&Enemy::Fire, this);

			// ���������C�x���g�𐶐�
			unique_ptr<TimedCall> timedCall = make_unique<TimedCall>(callback, maxGenerateTimer);

			// ���������C�x���g�����������C�x���g���X�g�ɒǉ�
			timedCalls.push_back(move(timedCall));

			generateTimer = 0;
		}

		timedCalls.remove_if([](unique_ptr<TimedCall>& tempTimedCall) {	return tempTimedCall->GetisEnd(); });
		// �^�C�}�[�̏���
		for (unique_ptr<TimedCall>& timedCall : timedCalls)
		{
			timedCall->Update();
		}
	}
	else
	{
		generateTimer = 0;
		timedCalls.clear();
	}

	// �e�̈ړ�����
	for (unique_ptr <EnemyBullet>& enemyBullet : enemyBullets)
	{
		enemyBullet->Update();
	}
}
void Enemy::StateChangeUpdate()
{
	currentState->Update();
	if (currentState->GetisEnd() == true)
	{
		BaseEnemyState* tmp = currentState->NextState();
		tmp->Initialize();
		delete currentState;
		currentState = tmp;
	}
}
