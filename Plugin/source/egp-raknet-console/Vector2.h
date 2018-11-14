#pragma once

struct Vector2
{
	Vector2(float _x = 0.0f, float _y = 0.0f);

	float x;
	float y;

	bool operator ==(Vector2 const &_compareTo) const;
	Vector2 operator +(Vector2 const &_right) const;
	Vector2 operator -(Vector2 const &_right) const;
	void operator +=(Vector2 const &_right);
	void operator -=(Vector2 const &_right);

	Vector2 operator *(float const &_right) const;
};