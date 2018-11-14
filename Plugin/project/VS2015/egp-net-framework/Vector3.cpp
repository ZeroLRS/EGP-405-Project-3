#include "Vector3.h"

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

bool Vector3::operator ==(Vector3 const &_compareTo) const
{
	return (_compareTo.x == x && _compareTo.y == y && _compareTo.z == z);
}

Vector3 Vector3::operator +(Vector3 const &_right) const
{
	return Vector3(x + _right.x, y + _right.y, z + _right.z);
}

Vector3 Vector3::operator -(Vector3 const &_right) const
{
	return Vector3(x - _right.x, y - _right.y, z - _right.z);
}

void Vector3::operator +=(Vector3 const &_right)
{
	x += _right.x;
	y += _right.y;
	z += _right.z;
}

void Vector3::operator -=(Vector3 const &_right)
{
	x -= _right.x;
	y -= _right.y;
	z -= _right.z;
}

Vector3 Vector3::operator*(float const & _right) const
{
	return Vector3(x * _right, y * _right, z * _right);
}
