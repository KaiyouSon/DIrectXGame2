#pragma once
class BaseEnemyState
{
protected:
	bool isEnd;
public:
	virtual ~BaseEnemyState() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual BaseEnemyState* NextState() = 0;

	bool GetisEnd() { return isEnd; }
};

