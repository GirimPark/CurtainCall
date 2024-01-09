#pragma once

#include "Object.h"

#include <unordered_map>

class World;
class Component;
class SceneComponent;

enum class EObjectType
{
	TEST,
	PLAYER,

	END
};

/// <summary>
/// GameObject���� ����
/// ���ӿ�����Ʈ ��ü�δ� �ƹ��͵� ������ �ʴ´�.
/// ������Ʈ �� ��ǥ ������Ʈ�� �����Ͽ� �ش� ������Ʈ�� ��ġ���� ��ǥ��ġ������ ������.
/// </summary>
class GameObject
	: public Object
{
protected:
	std::string m_name;
	EObjectType m_objectType = EObjectType::END;

	World* m_pOwnerWorld = nullptr;
	GameObject* m_pOwnerObj = nullptr;

	std::unordered_map<std::string, int> m_componentMap; // GetComponent �ӵ� ����� ���� Query�� �ڷᱸ��
	std::vector<Component*> m_ownComponents;
	SceneComponent* m_pRootComponent = nullptr;	// m_pRootComponent�� m_OwnedComponent �� �ϳ��� ����, ��ǥ ��ġ��

public:
	virtual ~GameObject();

public:
	void SetName(std::string_view name) { m_name = name; }
	void SetObjectType(EObjectType type) { m_objectType = type; }
	void SetOwnerWorld(World* pWorld) { m_pOwnerWorld = pWorld; }
	void SetRootComponent(SceneComponent* pRootcomponent) { m_pRootComponent = pRootcomponent; }

	SceneComponent* GetRootComponent() const { return m_pRootComponent; }

	Component* GetComponent(std::string_view name) const;
	
};