#ifndef vec3_h
#define vec3_h

#include <cmath>

class vec3
{
public:
	// Data
	float x, y, z;

	// Ctors
	vec3( float InX, float InY, float InZ ) : x( InX ), y( InY ), z( InZ )
		{
		}
	void SetfloatPoint( const float *v ) {  x=v[0]; y=v[1]; z=v[2]; }

	vec3( ) : x(0), y(0), z(0)
		{
		}

	// Operator Overloads
	inline bool operator== (const vec3& V2) const 
		{
		return (x == V2.x && y == V2.y && z == V2.z);
		}

	inline vec3 operator+ (const vec3& V2) const 
		{
		return vec3( x + V2.x,  y + V2.y,  z + V2.z);
		}
	inline vec3 operator- (const vec3& V2) const
		{
		return vec3( x - V2.x,  y - V2.y,  z - V2.z);
		}
	inline vec3 SubP(const float *v) const
		{
		  return vec3( x - v[0],  y - v[1],  z - v[2]);
		}

	inline vec3 operator- ( ) const
		{
		return vec3(-x, -y, -z);
		}

	inline vec3 operator/ (float S ) const
		{
		float fInv = 1.0 / S;
		return vec3 (x * fInv , y * fInv, z * fInv);
		}
	inline vec3 operator/ (const vec3& V2) const
		{
		return vec3 (x / V2.x,  y / V2.y,  z / V2.z);
		}
	inline vec3 operator* (const vec3& V2) const
		{
		return vec3 (x * V2.x,  y * V2.y,  z * V2.z);
		}
	inline vec3 operator* (float S) const
		{
		return vec3 (x * S,  y * S,  z * S);
		}
	inline vec3 operator+ (float S) const
		{
		return vec3 (x + S,  y + S,  z + S);
		}
	inline vec3 operator- (float S) const
		{
		return vec3 (x - S,  y - S,  z - S);
		}

	inline void operator+= ( const vec3& V2 )
		{
		x += V2.x;
		y += V2.y;
		z += V2.z;
		}
	inline void operator-= ( const vec3& V2 )
		{
		x -= V2.x;
		y -= V2.y;
		z -= V2.z;
		}

	inline float operator[] ( int i )
		{
		if ( i == 0 ) return x;
		else if ( i == 1 ) return y;
		else return z;
		}

	// Functions
	inline float Dot( const vec3 &V1 ) const
		{
		return V1.x*x + V1.y*y + V1.z*z;
		}

	// These require math.h for the sqrt function
	float Magnitude( ) const
		{
		//cmath cast added
		return  sqrtf(x*x + y*y + z*z);
		}

	inline void Normalize()
		{
		float fMag = ( x*x + y*y + z*z );
		if (fMag == 0) {return;}
		//cmath cast added
		float fMult = (1.0/sqrtf(fMag));            
		x *= fMult;
		y *= fMult;
		z *= fMult;
		return;
		}
};

inline vec3 Subtractfloatfloat(const float *d1, const float *d2)
{
  return vec3(d1[0]-d2[0], d1[1]-d2[1], d1[2]-d2[2]);
}

inline float clamp(float d, float min, float max) 
{
  const float t = d < min ? min : d;
  return t > max ? max : t;
}


inline void clamp(vec3 &v, float min, float max) 
{
  v.x = clamp(v.x,min,max);
  v.y = clamp(v.y,min,max);
  v.z = clamp(v.z,min,max);
}



#endif
