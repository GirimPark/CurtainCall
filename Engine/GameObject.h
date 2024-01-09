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
/// GameObject들의 원형
/// 게임오브젝트 자체로는 아무것도 가지지 않는다.
/// 컴포넌트 중 대표 컴포넌트를 지정하여 해당 컴포넌트의 위치값을 대표위치값으로 가진다.
/// </summary>
class GameObject
	: public Object
{
protected:
	std::string m_name;
	EObjectType m_objectType = EObjectType::END;

	World* m_pOwnerWorld = nullptr;
	GameObject* m_pOwnerObj = nullptr;

	std::unordered_map<std::string, int> m_componentMap; // GetComponent 속도 향상을 위한 Query용 자료구조
	std::vector<Component*> m_ownComponents;
	SceneComponent* m_pRootComponent = nullptr;	// m_pRootComponent는 m_OwnedComponent 중 하나로 설정, 대표 위치값

public:
	virtual ~GameObject();

public:
	void SetName(std::string_view name) { m_name = name; }
	void SetObjectType(EObjectType type) { m_objectType = type; }
	void SetOwnerWorld(World* pWorld) { m_pOwnerWorld = pWorld; }
	void SetRootComponent(SceneComponent* pRootcomponent) { m_pRootComponent = pRootcomponent; }

	SceneComponent* GetRootComponent() const { return m_pRootComponent; }

	Component* GetComponent(std::string_view name) const;

public:
	virtual void Initialize();
	virtual void Update(float deltaTime);

public:
	template <typename T>
	T* CreateComponent(std::string_view name)
	{
		// TODO
		// temp에 대한 누수?
		// T* 타입 리턴인데 World쪽은 흠
		// assert warning
		T* temp = new T;
		Component* component = dynamic_cast<Component*>(temp);
		assert(component, L"GameObject.h, CreateComponent() : dynamic_cast assertion");

		int keyIdx = m_ownComponents.size();
		m_componentMap.emplace(name, keyIdx);

		//component->SetName(name);
		//component->SetOwner(this);


		m_ownComponents.emplace_back(component);

		return component;
	}
};