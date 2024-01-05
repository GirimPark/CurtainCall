#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
	m_localMatrix = Math::Matrix::Identity;
	m_worldMatrix = Math::Matrix::Identity;
}

Math::Vector3 Transform::GetWorldPosition() const
{
	return m_worldMatrix.Translation();
}

void Transform::Update(float deltaTime)
{
	if(m_pParent)
	{
		m_worldMatrix = m_localMatrix * m_pParent->m_worldMatrix;
	}
	else
	{
		m_worldMatrix = m_localMatrix;
	}
}
