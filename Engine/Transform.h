#pragma once
class Transform
{
protected:
	Math::Matrix m_localMatrix;
	Math::Matrix m_worldMatrix;

	Transform* m_pParent = nullptr;

	Transform();
	~Transform() = default;

	Math::Vector3 GetWorldPosition() const;
	void SetParent(Transform* pParent) { m_pParent = pParent; }

	virtual void Update(float deltaTime);
};

