#ifndef QUAT_HPP
#define QUAT_HPP

#define _USE_MATH_DEFINES
#include <cmath>

template< typename T >
struct matrix_t
{
	T data[16];
};

template< typename T >
struct vector3_t
{
	T x, y, z;
};

template< typename T >
class Quat
{
  public:
	Quat() : m_value{} {}
	Quat(T a, T b, T c, T d) : m_value{ b, c, d, a } {}
	Quat(T an, bool rad, vector3_t< T > v)
	{
		T len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		T angle = an / 2;
		if (!rad)
		{
			angle = angle * (T)(M_PI / 180);
		}
		if (len != 0)
		{
			m_value[0] = sin(angle) * v.x / len;
			m_value[1] = sin(angle) * v.y / len;
			m_value[2] = sin(angle) * v.z / len;
		}
		else
		{
			m_value[0] = 0;
			m_value[1] = 0;
			m_value[2] = 0;
		}
		m_value[3] = cos(angle);
	}

	const T *data() const { return m_value; }

	T angle(bool rad = true) const
	{
		const T len = T(*this);
		T angle;
		if (len != 0)
		{
			angle = 2 * acos(m_value[3] / T(*this));
		}
		else
		{
			angle = M_PI;
		}
		if (rad)
		{
			return angle;
		}
		else
		{
			return angle / (M_PI / 180);
		}
	}

	Quat operator+(const Quat< T > &right) const
	{
		Quat result = *this;
		result += right;
		return result;
	}

	Quat operator-(const Quat< T > &right) const
	{
		Quat result = *this;
		result -= right;
		return result;
	}

	Quat &operator+=(const Quat< T > &right)
	{
		for (int i = 0; i < 4; i++)
		{
			m_value[i] += right.m_value[i];
		}
		return *this;
	}

	Quat &operator-=(const Quat< T > &right)
	{
		for (int i = 0; i < 4; i++)
		{
			m_value[i] -= right.m_value[i];
		}
		return *this;
	}

	Quat operator~() const { return Quat(m_value[3], -m_value[0], -m_value[1], -m_value[2]); }

	bool operator==(const Quat< T > right) const
	{
		const T *r_value = right.m_value;
		bool res = true;
		for (int i = 0; i < 4; i++)
		{
			if (m_value[i] != r_value[i])
			{
				res = false;
				break;
			}
		}
		return res;
	}

	bool operator!=(const Quat< T > right) const { return !((*this) == right); }

	explicit operator T() const
	{
		T res = 0;
		for (T i : m_value)
		{
			res += (i * i);
		}
		res = sqrt(res);
		return res;
	}

	Quat operator*(const Quat< T > &right) const
	{
		const T ra = right.m_value[3], rb = right.m_value[0], rc = right.m_value[1], rd = right.m_value[2];
		const T a = m_value[3], b = m_value[0], c = m_value[1], d = m_value[2];
		return Quat(
			a * ra - b * rb - c * rc - d * rd,
			a * rb + b * ra + c * rd - d * rc,
			a * rc + c * ra + d * rb - b * rd,
			a * rd + d * ra + b * rc - c * rb);
	}

	Quat operator*(const T c) const { return (*this) * Quat(c, 0, 0, 0); }

	Quat operator*(const vector3_t< T > v) const { return (*this) * Quat(0, v.x, v.y, v.z); }

	matrix_t< T > rotation_matrix() const
	{
		const T k = T(*this);
		T a, b, c, d;
		if (k != 0)
		{
			b = m_value[0] / k;
			c = m_value[1] / k;
			d = m_value[2] / k;
			a = m_value[3] / k;
		}
		else
		{
			b = 0;
			c = 0;
			d = 0;
			a = 0;
		}
		matrix_t< T > mat = {
			1 - 2 * c * c - 2 * d * d,
			2 * b * c + 2 * d * a,
			2 * b * d - 2 * c * a,
			0,
			2 * b * c - 2 * d * a,
			1 - 2 * b * b - 2 * d * d,
			2 * c * d + 2 * b * a,
			0,
			2 * b * d + 2 * c * a,
			2 * c * d - 2 * b * a,
			1 - 2 * b * b - 2 * c * c,
			0,
			0,
			0,
			0,
			1
		};
		return mat;
	}

	matrix_t< T > matrix() const
	{
		T a = m_value[3], b = m_value[0], c = m_value[1], d = m_value[2];
		matrix_t< T > mat = { a, -b, -c, -d, b, a, -d, c, c, d, a, -b, d, -c, b, a };
		return mat;
	}

	vector3_t< T > apply(const vector3_t< T > vec) const
	{
		const T len = T(*this);
		Quat temp;
		if (len != 0)
		{
			temp = Quat(m_value[3] / len, m_value[0] / len, m_value[1] / len, m_value[2] / len);
		}
		else
		{
			temp = Quat();
		}
		temp = temp * vec * (~temp);
		vector3_t< T > v = { temp.m_value[0], temp.m_value[1], temp.m_value[2] };
		return v;
	}

  private:
	T m_value[4];
};

#endif /* QUAT_HPP */
