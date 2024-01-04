#pragma once
class Transform
{
protected:
	Math::Matrix m_localMatrix;
	Math::Matrix m_worldMatrix;

	Transform* m_pParent = nullptr;

	//void SetParent(Transform* )
};

