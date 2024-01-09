#include "pch.h"
#include "World.h"

constexpr int resreveSize = 30;

World::World()
{
	for(auto& gameObjects : m_gameObjects)
	{
		gameObjects.reserve(resreveSize);
	}
	m_renderGameObjects.reserve(100);
	m_objectMap.reserve(resreveSize * static_cast<int>(EObjectType::END));
}

World::~World()
{
	World::Finalize();
}

std::shared_ptr<GameObject> World::GetGameObject(std::string_view name)
{
	std::pair<EObjectType, int> pair = m_objectMap.find(std::string(name))->second;

	return m_gameObjects[static_cast<int>(pair.first)][pair.second];
}

void World::Initialize()
{

}

void World::Update(float deltaTime)
{
}

void World::Finalize()
{
}
