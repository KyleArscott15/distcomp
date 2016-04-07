#ifndef vec3_h
#define vec3_h

#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif

// http://stackoverflow.com/questions/13706809/structs-in-c-with-initial-values
typedef struct {
  float x, y, z;
}  vec3;

// SetDoublePoint
#define SET_POINT(p,v) { p.x=v[0]; p.y=v[1]; p.z=v[2]; } 

// RETURNS VECTOR
//XXX used them like this  "vec3 my_vector = PLUS(v1,v2);"
#define EQUAL(v1,v2) (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) //inline bool operator== (const vec3& V2) const
#define PLUS(v1,v2)  {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z} // inline vec3 operator+ (const vec3& V2) const //XXX different behavior then before, now it modifies v1 in place instead of returning new vec3
#define PLUS_SET(v1,v2) {v1.x+=v2.x; v1.y+=v2.y; v1.z+=v2.z;}


#define SUB(v1,v2)  {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z} // inline vec3 operator- (const vec3& V2) const //XXX different behavior then before, now it modifies v1 in place instead of returning new vec3
#define SUBP(v1,arr) {v1.x-arr[0], v1.y-arr[1], v1.z-arr[2]} // inline vec3 SubP(const double *v) const
#define NEG(v1)      {-v1.x, -v1.y, -v1.z} //inline vec3 operator- ( ) const
#define DIVK(v1,k)  { v1.x/k, v1.y/k, v1.z/k} // inline vec3 operator/ (double S ) const // XXX need more efficient
#define DIV(v1,v2) { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z}  // inline vec3 operator/ (const vec3& V2) const
#define MUL(v1,v2) { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z} // inline vec3 operator* (const vec3& V2) const
#define MULK(v1,k)  { v1.x*k, v1.y*k, v1.z*k} // inline vec3 operator* (double S) const

#define MULK_SET(v1,k) { v1.x*=k; v1.y*=k; v1.z*=k;}

#define ADD(v1,k)  { v1.x+k, v1.y+k, v1.z+k} // inline vec3 operator+ (double S) const
#define ADD_SET(v1,k) { v1.x+=k; v1.y+=k; v1.z+=k;}

#define SUBK(v1,k) { v1.x-k, v1.y-k, v1.z-k} 	// inline vec3 operator- (double S) const

// MODIFIES FIRST VECTOR
#define PLUSEQUAL(v1,v2)  { v1.x=v1.x+v2.x; v1.y=v1.y+v2.y; v1.z=v1.z+v2.z; } // inline void operator+= ( const vec3& V2 )
#define SUBEQUAL(v1,v2)  { v1.x=v1.x+v2.x; v1.y=v1.y+v2.y; v1.z=v1.z+v2.z; } // inline void operator-= ( const vec3& V2 )
//TODO KA is this necessary to convert? inline double operator[] ( int i )
#define DOTSELF(d,v1)         {  d=( v1.x*v1.x + v1.y*v1.y + v1.z*v1.z );    }
#define DOT(d,v1,v2)         {  d=( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z );    }
#define DOTRET(v1,v2)        ( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z )
#define MAGNITUDE(m,p) 	({ m=sqrtf( p.x*p.x + p.y*p.y + p.z*p.z ); })
#define MAGNITUDE_RET(p) 	(sqrtf(p.x*p.x + p.y*p.y + p.z*p.z))
#define NORMALIZE(p) {					\
    float fMag = ( (p).x*(p).x + (p).y*(p).y + (p).z*(p).z );	\
    if (fMag != 0)					\
      {							\
	float fMult = 1.0/sqrtf(fMag);			\
	(p).x *= fMult;					\
	(p).y *= fMult;					\
	(p).z *= fMult;					\
      }							\
  }
#define VECESUBDUBDUB(v,d1,d2)  v.x = (d1[i*3]-d2[0]); v.y = (d1[i*3 + 1]-d2[1]); v.z = (d1[i * 3 + 2]-d2[2]);// inline vec3 SubtractDoubleDouble(const double *d1, const double *d2) 
// Not SubP!
#define SUBTRACT_POINT(p,v,u)			\
  {						\
  p.x=(v[0])-(u[0]);				\
  p.y=(v[1])-(u[1]);				\
  p.z=(v[2])-(u[2]);				\
}
#define MAX(a,b)      ( ((a)>(b))? (a):(b))
#define VEC(v,a,b,c) vec3 v= {a,b,c}
#define CLAMP(d, min, max) { \
	float t=(d < min ? min : d); \
	d=(t > max ? max : t);\
}
#define CLAMPVEC(v1, min, max) { \
	CLAMP(v1.x,min,max); \
	CLAMP(v1.y,min,max); \
	CLAMP(v1.z,min,max);  \
}

// Defined by Kyle
#define COPY(v1) {v1.x, v1.y, v1.z}

#define SHOW(v1)     {printf("[%f,%f,%f]\n", v1.x, v1.y, v1.z); fflush(stdout);}
#define SHOWMSG(msg,v1) {printf("%s: [%f,%f,%f]\n", msg, v1.x, v1.y, v1.z); fflush(stdout);}

#endif
