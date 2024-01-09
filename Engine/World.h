#pragma once

#include "GameObject.h"

#include <vector>
#include <unordered_map>
#include <cassert>
#include <memory>
#include <memory>

/// <summary>
/// World의 원형 클래스
/// GameApp은 World 맵을 가지고 월드를 검색, 전환한다.
/// World는 GameObject 리스트를 들고 있다.
/// </summary>
class World
{
protected:
	std::string m_name;

	std::vector<std::shared_ptr<GameObject>> m_gameObjects[static_cast<int>(EObjectType::END)];
	std::vector<std::weak_ptr<GameObject>> m_renderGameObjects;
	std::unordered_map<std::string, std::pair<EObjectType, int>> m_objectMap;

public:
	World();
	virtual ~World();

public:
	// GetGameObjects를 통해 얻은 벡터를 멤버변수 등으로 사용하여 객체 생명에 혼동을 주면 안된다.
	// 사용 용도 등을 보고 후에 구현
	// std::vector<std::weak_ptr<GameObject>>& GetGameObjects(EObjectType type) const;
	std::shared_ptr<GameObject> GetGameObject(std::string_view name);

public:
	virtual void Initialize();
	virtual void Update(float deltaTime);
	// 월드의 경우 월드 전환 이벤트 처리시 delete 없이 Finalize만을 수행해야 하므로 Finalize가 필요하다.
	virtual void Finalize();

	virtual void OnEnter() abstract;
	virtual void OnExit() abstract;

public:
	template <typename T>
	std::shared_ptr<T> CreateGameObject(std::string_view objName, EObjectType objectType)
	{
		T* obj = new T;
		GameObject* gameObj = dynamic_cast<GameObject*>(obj);
		assert(gameObj, L"World.h, CreateGameObject() : dynamic_cast assertion");

		int keyIdx = m_gameObjects[static_cast<int>(objectType)].size();
		m_objectMap.emplace(objName, std::make_pair(objectType, keyIdx));

		gameObj->SetName(objName);
		gameObj->SetObjectType(objectType);
		gameObj->SetOwnerWorld(this);

		std::shared_ptr<GameObject> sharedObj = std::make_shared<GameObject>(*gameObj);
		m_gameObjects[static_cast<int>(objectType)].emplace_back(sharedObj);

		delete obj;
		delete gameObj;

		return sharedObj;
	}
};

