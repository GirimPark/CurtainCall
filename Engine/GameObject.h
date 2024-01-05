#pragma once

#include "Object.h"

class SceneComponent;

class GameObject
	: public Object
{
	SceneComponent* m_pRootComponent;

};