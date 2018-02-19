#ifndef VEC4_H_
#define VEC4_H_

#include <ios>
#include <ostream>
#include <math.h>

using namespace std;

/* ------------------------------------------------------------------- */

template <class T>

class mat4x4 {

  // entries come in OpenGL order:
  //    0 4 8  12
  //    1 5 9  13
  //    2 6 10 14
  //    3 7 11 15

 
 public:
  T *e;
  mat4x4();
  mat4x4 ( const mat4x4<T> &m );

  mat4x4<T> & operator= ( mat4x4 m );

  ~mat4x4();

  mat4x4<T> & lmul ( mat4x4<T> m );
  mat4x4<T> & rmul ( mat4x4<T> m );

  T & operator() ( int i, int j );

  T *pointer();
};

template <class T>
mat4x4<T> operator* ( mat4x4<T> m, mat4x4<T> n );

template <class T>
mat4x4<T> Translation ( T a, T b, T c );

template <class T>
mat4x4<T> Identity ();

template <class T>
mat4x4<T> Rotation ( T x, T y, T z, double alpha );

template <class T>
mat4x4<T> Scale ( T a );

template <class T>
mat4x4<T> Scale ( T a, T b, T c );

template <class T>
std::ostream & operator << ( std::ostream &o, mat4x4<T> m );

/* ------------------------------------------------------------------- */

template <class T>

class vec3d {
 public:
  T *coord;
  vec3d ();
  vec3d ( T x, T y, T z );
  vec3d ( const vec3d<T> & v );

  ~vec3d();

  vec3d<T> & operator += ( vec3d<T> v );
  vec3d<T> & operator -= ( vec3d<T> v );
  vec3d<T> & operator |= ( vec3d<T> v );
  vec3d<T> & operator &= ( vec3d<T> v );
  vec3d<T> & operator *= ( T c );
  vec3d<T> & operator = ( vec3d<T> v );

  // allocate and return pointer to coordinate vector
  T *pointer();

  T & operator[] ( int i );
  T maxV();
  T minV();

  vec3d<T> & normalize();
};

template <class T>
vec3d<T> operator+ ( vec3d<T> v, vec3d<T> w );

template <class T>
vec3d<T> operator- ( vec3d<T> v, vec3d<T> w );

// scalar product
template <class T>
T operator* ( vec3d<T> v, vec3d<T> w );

// vector product
template <class T>
vec3d<T> operator^ ( vec3d<T> v, vec3d<T> w );

// max
template <class T>
vec3d<T> operator| ( vec3d<T> v, vec3d<T> w );

// min
template <class T>
vec3d<T> operator& ( vec3d<T> v, vec3d<T> w );

template <class T>
vec3d<T> operator* ( T c, vec3d<T> v );

template <class T>
std::ostream & operator<< ( std::ostream &o, vec3d<T> v );

template <class T>
bool operator== ( vec3d<T> v, vec3d<T> w );

/* ------------------------------------------------------------------- */

template <class T>

class vec3dh {

  vec3d<T> xyz;
  T h;

 public:

  vec3dh ();
  vec3dh ( T x, T y, T z );
  vec3dh ( T x, T y, T z, T hh );
  vec3dh ( vec3d<T> v, T hh );
  vec3dh ( vec3d<T> v );

  // allocate and return pointer to coordinate vector
  T *pointer();

  T & operator[] ( int i );

  vec3dh<T> & operator*= ( mat4x4<T> m );
 };
 
template <class T>
vec3dh<T> operator* ( mat4x4<T> m, vec3dh<T> v );

template <class T>
std::ostream & operator<< ( std::ostream &o, vec3dh<T> v );


/* ------------------------------------------------------------------- */

/* ---------------- functions related to mat4x4 template ------------- */

template <class T>
mat4x4<T>::mat4x4 ()
{
  e = new T[16];
}

template <class T>
mat4x4<T>::mat4x4 ( const mat4x4<T> &m )
{
  e = new T[16];
  for ( int i=0; i<16; i++ )
    e[i] = m.e[i];
}

template <class T>
mat4x4<T> & mat4x4<T>::operator= ( mat4x4<T> m )
{
  if (!e)
    e = new T[16];
  for ( int i=0; i<16; i++ )
    e[i] = m.e[i];
  return *this;
}

template <class T>
mat4x4<T>::~mat4x4<T>()
{
  delete[] e;
  e = NULL;
}

static int IX ( int i, int j )
{
  return j|(i<<2);
}

template <class T>
mat4x4<T> & mat4x4<T>::lmul ( mat4x4<T> m )
{
  int i,j,k;

  T *res = new T[16];

  for ( i=0; i<4; i++ )
    for ( j=0; j<4; j++ )
      {
	res[IX(i,j)]=0;
	for ( k=0; k<4; k++ )
	  res[IX(i,j)] += m.e[IX(i,k)]*e[IX(k,j)];
      }
  
  for ( i=0; i<16; i++ )
    e[i] = res[i];

  delete[] res;

  return *this;
}

template <class T>
mat4x4<T> & mat4x4<T>::rmul ( mat4x4<T> m )
{
  int i,j,k;

  T *res = new T[16];

  for ( i=0; i<4; i++ )
    for ( j=0; j<4; j++ )
      {
	res[IX(i,j)]=0;
	for ( k=0; k<4; k++ )
	  res[IX(i,j)] += e[IX(i,k)]*m.e[IX(k,j)];
      }
  
  for ( i=0; i<16; i++ )
    e[i] = res[i];

  delete[] res;

  return *this;
}

template <class T>
T & mat4x4<T>::operator() ( int i, int j )
{
  return e[IX(i,j)];
}

template <class T>
mat4x4<T> operator* ( mat4x4<T> m, mat4x4<T> n )
{
  int i,j,k;

  mat4x4<T> res;

  for ( i=0; i<4; i++ )
    for ( j=0; j<4; j++ )
      {
	res(i,j)=0;
	for ( k=0; k<4; k++ )
	  res(i,j) += m(i,k)*n(k,j);
      }

  return res;
}

template <class T>
T * mat4x4<T>::pointer()
{
  T *res = new T[16];
  for ( int i=0; i<16; i++ )
    res[i] = e[i];
  return res;
}

template <class T> 
mat4x4<T> Identity ()
{
  mat4x4<T> res;
  int i,j;
  for ( i=0; i<4; i++ )
    for ( j=0; j<4; j++ )
      res(i,j) = (i==j);
  return res;
}

template <class T>
mat4x4<T> Translation ( T a, T b, T c )
{
  mat4x4<T> res = Identity<T>();
  res(0,3) = a;
  res(1,3) = b;
  res(2,3) = c;
  return res;
}

template <class T>
mat4x4<T> Scale ( T a, T b, T c )
{
  mat4x4<T> res;
  int i,j;
  for ( i=0; i<4; i++ )
    for ( j=0; j<4; j++ )
      res(i,j) = 0;
  res(0,0) = a;
  res(0,0) = b;
  res(0,0) = c;
  return res; 
}

template <class T>
mat4x4<T> Scale ( T a )
{
  return Scale(a,a,a);
}

template <class T> 
std::ostream & operator << ( std::ostream &o, mat4x4<T> m )
{
  int i,j;
  for ( i=0; i<4; i++ )
    {
      for ( j=0; j<4; j++ )
	o << m(i,j) << " ";
      o << endl;
    }
  return o;
}

template <class T>
mat4x4<T> Rotation ( T x, T y, T z, double alpha )
{
  mat4x4<T> res;
  res(0,0) = 1+(1-cos(alpha))*(x*x-1);
  res(1,1) = 1+(1-cos(alpha))*(y*y-1);
  res(2,2) = 1+(1-cos(alpha))*(z*z-1);
  res(0,1) = -z*sin(alpha)+(1-cos(alpha))*x*y;
  res(0,2) = y*sin(alpha)+(1-cos(alpha))*x*z;
  res(1,0) = z*sin(alpha)+(1-cos(alpha))*x*y;
  res(1,2) = -x*sin(alpha)+(1-cos(alpha))*y*z;
  res(2,0) = -y*sin(alpha)+(1-cos(alpha))*x*z;
  res(2,1) = x*sin(alpha)+(1-cos(alpha))*y*z;
  res(3,3) = 1;
  res(0,3) = res(1,3) = res(2,3) = res(3,0) = res(3,1) = res(3,2) = 0;
  return res;
}

/* ------------------------------------------------------------------- */

/* ---------------- functions related to vec3d template ------------- */

template <class T>
vec3d<T>::vec3d()
{
  coord = new T[3];
}

template <class T>
T vec3d<T>::minV()
{
  if (coord[0]<coord[1] && coord[0]<coord[2])
    return coord[0];
  return coord[1]<coord[2] ? coord[1] : coord[2];
}

template <class T>
T vec3d<T>::maxV()
{
  if (coord[0]>coord[1] && coord[0]>coord[2])
    return coord[0];
  return coord[1]>coord[2] ? coord[1] : coord[2];
}

template <class T>
vec3d<T>::vec3d ( T x, T y, T z )
{
  coord = new T[3];
  coord[0] = x;
  coord[1] = y;
  coord[2] = z;
}

template <class T>
vec3d<T>::vec3d ( const vec3d<T>&v )
{
  coord = new T[3];
  coord[0] = v.coord[0];
  coord[1] = v.coord[1];
  coord[2] = v.coord[2];
}

template <class T>
vec3d<T>::~vec3d()
{
  delete[] coord;
  coord = 0;
}

template <class T>
vec3d<T> & vec3d<T>::operator += ( vec3d<T> v )
{
  coord[0] += v.coord[0];
  coord[1] += v.coord[1];
  coord[2] += v.coord[2];
  return *this;
}

template <class T>
vec3d<T> & vec3d<T>::operator -= ( vec3d<T> v )
{
  coord[0] -= v.coord[0];
  coord[1] -= v.coord[1];
  coord[2] -= v.coord[2];
  return *this;
}

template <class T>
vec3d<T> & vec3d<T>::operator = ( vec3d<T> v )
{
  if (!coord)
    coord = new T[3];
  coord[0] = v.coord[0];
  coord[1] = v.coord[1];
  coord[2] = v.coord[2];
  return *this;
}

template <class T>
vec3d<T> & vec3d<T>::operator*= ( T a )
{
  coord[0] *= a;
  coord[1] *= a;
  coord[2] *= a;
  return *this;
}

template <class T>
vec3d<T> & vec3d<T>::operator |= ( vec3d<T> v )
{
  if (v[0]>coord[0])
    coord[0] = v[0];
  if (v[1]>coord[1])
    coord[1] = v[1];
  if (v[2]>coord[2])
    coord[2] = v[2];
  return *this;
}

template <class T>
vec3d<T> & vec3d<T>::operator &= ( vec3d<T> v )
{
  if (v[0]<coord[0])
    coord[0] = v[0];
  if (v[1]<coord[1])
    coord[1] = v[1];
  if (v[2]<coord[2])
    coord[2] = v[2];
  return *this;
}

template <class T>
vec3d<T> & vec3d<T>::normalize ( )
{
  T l = sqrt(coord[0]*coord[0]+coord[1]*coord[1]+coord[2]*coord[2]);
  coord[0] = coord[0]/l;
  coord[1] = coord[1]/l;
  coord[2] = coord[2]/l;  
  return *this;
}

template <class T>
bool operator== ( vec3d<T> v, vec3d<T> w )
{
  return v[0]==w[0] && v[1]==w[1] && v[2]==w[2];
}

template <class T>
T * vec3d<T>::pointer()
{
  return coord;
}

template <class T>
T & vec3d<T>::operator[] ( int i )
{
  return coord[i];
}

template <class T>
vec3d<T> operator+ ( vec3d<T> v, vec3d<T> w )
{
  return vec3d<T>(v[0]+w[0],v[1]+w[1],v[2]+w[2]);
}

template <class T>
vec3d<T> operator- ( vec3d<T> v, vec3d<T> w )
{
  return vec3d<T>(v[0]-w[0],v[1]-w[1],v[2]-w[2]);
}

template <class T>
T operator* ( vec3d<T> v, vec3d<T> w )
{
  return v[0]*w[0]+v[1]*w[1]+v[2]*w[2];
}

template <class T>
vec3d<T> operator* ( T c, vec3d<T> v )
{
  return vec3d<T>(c*v[0],c*v[1],c*v[2]);
}

template <class T>
vec3d<T> operator^ ( vec3d<T> v, vec3d<T> w )
{
  return vec3d<T>(v[1]*w[2]-v[2]*w[1], v[2]*w[0]-v[0]*w[2], v[0]*w[1]-v[1]*w[0]);
}

template <class T>
vec3d<T> operator| ( vec3d<T> v, vec3d<T> w )
{
  return vec3d<T>(
		  v[0]>w[0] ? v[0] : w[0],
		  v[1]>w[1] ? v[1] : w[1],
		  v[2]>w[2] ? v[2] : w[2]
		  );
}

template <class T>
vec3d<T> operator& ( vec3d<T> v, vec3d<T> w )
{
  return vec3d<T>(
		  v[0]<w[0] ? v[0] : w[0],
		  v[1]<w[1] ? v[1] : w[1],
		  v[2]<w[2] ? v[2] : w[2]
		  );
}

template <class T>
std::ostream & operator<< ( std::ostream &o, vec3d<T> v )
{
  o << v[0] << " " << v[1] << " " << v[2];
  return o;
}


/* ------------------------------------------------------------------- */

/* ---------------- functions related to vec3dh template ------------- */

template <class T>
vec3dh<T>::vec3dh() : xyz(), h(1)
{}

template <class T>
vec3dh<T>::vec3dh ( T x, T y, T z ) : xyz(x,y,z), h(1)
{}

template <class T>
vec3dh<T>::vec3dh ( T x, T y, T z, T hh ) : xyz(x,y,z), h(hh)
{}

template <class T>
vec3dh<T>::vec3dh ( vec3d<T> v, T hh ) : xyz(v), h(hh)
{}

template <class T>
vec3dh<T>::vec3dh ( vec3d<T> v ) : xyz(v), h(1)
{}

template <class T>
T *vec3dh<T>::pointer()
{
  T *res = new T[4];
  res[0] = xyz[0];
  res[1] = xyz[1];
  res[2] = xyz[2];
  res[3] = h;
  return res;
}

template <class T>
T & vec3dh<T>::operator[] ( int i )
{
  return (i<3) ? xyz[i] : h;
}

template <class T>
vec3dh<T> & vec3dh<T>::operator*= ( mat4x4<T> m )
{
  T res[4];
  res[0] = m(0,0)*xyz[0]+m(0,1)*xyz[1]+m(0,2)*xyz[2]+m(0,3)*h;
  res[1] = m(1,0)*xyz[1]+m(1,1)*xyz[1]+m(1,2)*xyz[2]+m(1,3)*h;
  res[2] = m(2,0)*xyz[2]+m(2,1)*xyz[1]+m(2,2)*xyz[2]+m(2,3)*h;
  res[3] = m(3,0)*xyz[3]+m(3,1)*xyz[1]+m(3,2)*xyz[2]+m(3,3)*h;
  xyz[0] = res[0];
  xyz[1] = res[1];
  xyz[2] = res[2];
  h = res[3];
  return *this;
}

template <class T>
vec3dh<T> operator* ( mat4x4<T> m, vec3dh<T> v )
{
  return vec3dh<T>(
		   m(0,0)*v[0]+m(0,1)*v[1]+m(0,2)*v[2]+m(0,3)*v[3],
		   m(1,0)*v[1]+m(1,1)*v[1]+m(1,2)*v[2]+m(1,3)*v[3],
		   m(2,0)*v[2]+m(2,1)*v[1]+m(2,2)*v[2]+m(2,3)*v[3],
		   m(3,0)*v[3]+m(3,1)*v[1]+m(3,2)*v[2]+m(3,3)*v[3]
		   );
}


template <class T>
std::ostream & operator<< ( std::ostream &o, vec3dh<T> v )
{
  o << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
  return o;
}

/* ------------------------------------------------------------------- */

#endif /*VEC4_H_*/
