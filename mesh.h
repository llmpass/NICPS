
/* --- mesh.h --- */

#include <stdio.h>
#include <math.h>

#ifndef __MESH_H

#define __MESH_H

/* -------------------------------- */
/* -------------------------------- */

typedef double M_point[3];

/* -------------------------------- */

typedef int M_int_triple[3];

/* -------------------------------- */

typedef struct {
  int t,ix;
}
M_pair;

/* -------------------------------- */

typedef struct
{
  int vertices;
  M_point *vertex;
  int *deg;       /* degree of a vertex */
  M_pair **inc;   /* incident triangles */

  int triangles;
  M_int_triple *triangle;
  M_int_triple *adj;

  int internal_edges;
  int external_edges;
  int edges;

  M_point *normal;
}
M_mesh;

/* -------------------------------- */

#define MF_DEG       0x1
#define MF_INC       0x2
#define MF_ADJ       0x4
#define MF_NORMAL    0x8
#define MF_EDGE_STAT 0x10

extern void M_point_subtract ( M_point t, M_point a, M_point b );

extern double M_point_dot ( M_point a, M_point b );

extern void M_point_add ( M_point t, M_point a, M_point b );

extern void M_point_scale ( M_point t, double c, M_point a );

extern void M_point_cross ( M_point t, M_point a, M_point b );

extern void M_point_normalize ( M_point p );

extern void M_point_scale_and_add ( M_point t, double c, M_point a );

extern M_mesh M_read_mesh ( FILE *fp );

extern M_mesh M_read_mesh_X ( FILE *fp, unsigned int flags );

extern void M_write_mesh ( FILE *fp, M_mesh m );

extern void M_free_mesh ( M_mesh *m );

extern void M_quantize ( M_mesh *m, double qsize );

extern int which ( int i, M_int_triple t );

extern double M_point_dist ( M_point p1, M_point p2 );

extern double M_point_area ( M_point p1, M_point p2, M_point p3 );

extern double M_point_norm ( M_point p );

/* -------------------------------- */
/* -------------------------------- */

#endif
