#pragma once
#include <string>

class BaseEnemyState
{
protected:
	bool isEnd;
	std::string tag;
public:
	virtual ~BaseEnemyState() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual BaseEnemyState* NextState() = 0;

	bool GetisEnd() { return isEnd; }
	std::string GetTag() { return tag; }
};

