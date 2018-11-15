#pragma once

#pragma pack (push, 1)
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);

	bool operator ==(Vector3 const &_compareTo) const;
	Vector3 operator +(Vector3 const &_right) const;
	Vector3 operator -(Vector3 const &_right) const;
	void operator +=(Vector3 const &_right);
	void operator -=(Vector3 const &_right);

	Vector3 operator *(float const &_right) const;
};
#pragma pack (pop)