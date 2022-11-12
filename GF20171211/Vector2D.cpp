#include "Vector2D.h"

Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;

	return v1;
}

Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;

	return v1;
}