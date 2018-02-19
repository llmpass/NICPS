#ifndef VEC3F_H
#define VEC3F_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "tnt_matrix.h"
#include "tnt_sparse_matrix.h"
#include "jama_eig.h"
#include "jama_svd.h"

using namespace std;

class Vec3f {
public:
  float x,y,z;

  Vec3f() {x=y=z=0;}
  Vec3f(float x,float y, float z) : x(x),y(y),z(z) {};
  Vec3f(float *f) : x(f[0]),y(f[1]),z(f[2]) {};

  inline const float &operator[](const int i) const
  { return *(&x+i); }; 

  inline float &operator[](const int i)
  { return *(&x+i); }; 

  inline Vec3f &operator=(const Vec3f &b)
  { x = b.x; y = b.y; z = b.z; return *this;};
};

#define Epsilon 1E-5
#define Infinity HUGE_VAL

inline float Dot(const Vec3f &a, const Vec3f &b)
{ return a.x*b.x+a.y*b.y+a.z*b.z; };

inline Vec3f Product(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.x*b.x,a.y*b.y,a.z*b.z); };

inline Vec3f Cross(const Vec3f &a, const Vec3f &b){ 
	return Vec3f(a.y*b.z-a.z*b.y,
							 a.z*b.x-a.x*b.z,
							 a.x*b.y-a.y*b.x); };

inline Vec3f operator-(const Vec3f &v)
{ return Vec3f(-v.x,-v.y,-v.z); };

inline float Length(const Vec3f &v)
{ return sqrt(Dot(v,v)); };

inline Vec3f operator*(const float d, const Vec3f &v)
{ return Vec3f(d*v.x, d*v.y, d*v.z); };

inline Vec3f operator*(const Vec3f &v, const float d)
{ return Vec3f(d*v.x, d*v.y, d*v.z); };

inline void operator*=(Vec3f &v, const float d)
{ v.x *= d; v.y*=d; v.z*=d; };

inline void operator*=(Vec3f &v, const Vec3f &f)
{ v.x *= f.x; v.y*=f.y; v.z*=f.z; };

inline Vec3f operator/(const Vec3f &v, const float d)
{ return (1/d)*v; };

inline void operator/=(Vec3f &v, const float d)
{ v *= (1/d); };

inline Vec3f operator+(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.x+b.x, a.y+b.y, a.z+b.z); };

inline Vec3f operator-(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.x-b.x, a.y-b.y, a.z-b.z); };

inline void Normalize(Vec3f &v)
{ v *= (1.0f/Length(v)); };

inline Vector<float> ConjugateGradient(Sparse_Matrix<float>& A, Vector<float> b, 
																			 Vector<float> x, int iMax=200, float eps=0.01) {
	int i = 0;
	Vector<float> r = b - A*x;
	Vector<float> d = r;
	float delta_new = dot_prod(r, r);
	float delta0 = delta_new, delta_old;
	Vector<float> q;
	float alpha, beta;
	Vector<float> xx(d.dim());
	xx = x;
	while (i < iMax && delta_new > eps*eps*delta0){
		q = A*d;
		alpha = delta_new / (dot_prod(d, q));
		xx = xx + alpha*d;
		if ((i%50)==0) r = b - A*xx;
		else r = r - alpha*q;
		delta_old = delta_new;
		delta_new = dot_prod(r, r);
		beta = delta_new / delta_old;
		d = r + beta*d;
		i++;
	}
	return xx;
}

#endif