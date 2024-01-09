#pragma once

#include "Object.h"

class GameObject;

/// <summary>
/// ���� ������ Component
/// Component�� ���� ������ �θ� ������Ʈ, �̸��� ������.
/// �뷫���� Component ������ �Ʒ��� ����.
/// Component
///   -SceneComponent
///     -CameraComponent
///     -RenderComponent
///       -RenderableComponents...
///   -MoveComponent
///   -FSMComponent
/// </summary>
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

