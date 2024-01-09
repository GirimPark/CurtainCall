#pragma once

#include "GameObject.h"

#include <vector>
#include <unordered_map>
#include <cassert>
#include <memory>
#include <memory>

/// <summary>
/// World�� ���� Ŭ����
/// GameApp�� World ���� ������ ���带 �˻�, ��ȯ�Ѵ�.
/// World�� GameObject ����Ʈ�� ��� �ִ�.
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
	// GetGameObjects�� ���� ���� ���͸� ������� ������ ����Ͽ� ��ü ���� ȥ���� �ָ� �ȵȴ�.
	// ��� �뵵 ���� ���� �Ŀ� ����
	// std::vector<std::weak_ptr<GameObject>>& GetGameObjects(EObjectType type) const;
	std::shared_ptr<GameObject> GetGameObject(std::string_view name);

public:
	virtual void Initialize();
	virtual void Update(float deltaTime);
	// ������ ��� ���� ��ȯ �̺�Ʈ ó���� delete ���� Finalize���� �����ؾ� �ϹǷ� Finalize�� �ʿ��ϴ�.
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

