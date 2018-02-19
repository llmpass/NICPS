
/* orient2.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <mesh.h>

/* ------------------------------------ */

M_mesh m;
int *stack;
int sp;
unsigned char *flags;

/* ------------------------------------ */

void init_all ( char *fname )
{
  FILE *fp = fopen(fname,"r");
  if (!fp)
    {
      printf("Can't open input file %s\n",fname);
      exit(1);
    }
  m = M_read_mesh(fp);
  fclose(fp);
  sp = 0;
  stack = (int*)malloc(m.triangles*sizeof(int));
  flags = (unsigned char*)malloc(m.triangles*sizeof(unsigned char));
  memset(flags,0,m.triangles*sizeof(unsigned char));
}

/* ------------------------------------ */

void push ( int i )
{
  if (flags[i]) return;
  assert(sp<m.triangles);
  stack[sp++] = i;
}

/* ------------------------------------ */

int pop()
{
  assert(sp>0);
  return stack[--sp];
}

/* ------------------------------------ */

void clean_up ( char *fname )
{
  FILE *fp;
  free(stack);
  fp = fopen(fname,"w");
  if (!fp)
    {
      printf("Can't open output file %s\n",fname);
      exit(1);
    }
  M_write_mesh(fp,m);
  fclose(fp);
}

/* ------------------------------------ */

int reorients = 0;

int reorient ( int i )
{
  int tmp;
  reorients++;
  tmp = m.triangle[i][0];
  m.triangle[i][0] = m.triangle[i][1];
  m.triangle[i][1] = tmp;
  tmp = m.adj[i][0];
  m.adj[i][0] = m.adj[i][1];
  m.adj[i][1] = tmp;
}

/* ------------------------------------ */

int isalong ( int v1, int v2, int t )
{
  return which(v2,m.triangle[t])==(which(v1,m.triangle[t])+1)%3;
}

/* ------------------------------------ */

void _traverse()
{
  int i,j,k;

  while(sp>0)
    {
      i = pop();
      flags[i] = 1;

      for ( j=0; j<3; j++ )
	{
	  if ((k=(m.adj[i][j]))==-1) 
	    continue;

	  if (flags[k])
	    continue;

	  /* fix the orientation of k if necessary */

	  if (isalong(m.triangle[i][(j+1)%3],m.triangle[i][(j+2)%3],k))
	    reorient(k);

	  push(k);
	}
    }
}

/* ------------------------------------ */

void traverse ( int i )
{
  assert(!sp);
  push(i);
  _traverse();
}

/* ------------------------------------ */

void traverse_all()
{
  int i,cs;

  for ( cs=i=0; i<m.triangles; i++ )
    if (!flags[i])
      {
	cs++;
	traverse(i);
      }
  printf("Connected components: %d, reorients: %d\n",cs,reorients);
}

/* ------------------------------------ */

int main ( int argc, char *argv[] )
{
  if (argc!=3)
    {
      printf("Usage : orient <input file> <output file>\n");
      return 0;
    }
  init_all(argv[1]);
  traverse_all();
  clean_up(argv[2]);
  return 0;
}
