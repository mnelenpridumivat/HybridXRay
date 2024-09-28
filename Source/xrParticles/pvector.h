#pragma once

namespace PAPI
{
    class pVector: public Fvector
    {
    public:
        IC pVector(float ax, float ay, float az)
        {
            set(ax, ay, az);
        }
        IC       pVector() {}
        IC float length() const
        {
            return _sqrt(x * x + y * y + z * z);
        }
        IC float length2() const
        {
            return (x * x + y * y + z * z);
        }
        IC float operator*(const pVector& a) const
        {
            return x * a.x + y * a.y + z * a.z;
        }
        IC pVector operator*(const float s) const
        {
            return pVector(x * s, y * s, z * s);
        }
        IC pVector operator/(const float s) const
        {
            float invs = 1.0f / s;
            return pVector(x * invs, y * invs, z * invs);
        }
        IC pVector operator+(const pVector& a) const
        {
            return pVector(x + a.x, y + a.y, z + a.z);
        }
        IC pVector operator-(const pVector& a) const
        {
            return pVector(x - a.x, y - a.y, z - a.z);
        }
        IC pVector operator-()
        {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }
        IC pVector& operator+=(const pVector& a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }
        IC pVector& operator-=(const pVector& a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }
        IC pVector& operator*=(const float a)
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }
        IC pVector& operator/=(const float a)
        {
            float b = 1.0f / a;
            x *= b;
            y *= b;
            z *= b;
            return *this;
        }
        IC pVector& operator=(const pVector& a)
        {
            x = a.x;
            y = a.y;
            z = a.z;
            return *this;
        }
        IC pVector operator^(const pVector& b) const
        {
            return pVector(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
        }
    };
}   // namespace PAPI