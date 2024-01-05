#pragma once

#include "Object.h"

class GameObject;

class Component
	: public Object
{
protected:
	GameObject* m_pOwner;
	std::string m_name;

public:
	virtual ~Component() = default;

public:
	void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	GameObject* GetOwner() const { return m_pOwner; }

public:
	virtual void Update(float deltaTime) abstract;
};

