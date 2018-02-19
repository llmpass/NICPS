
#include <stdlib.h>
#include <stdio.h>

#include <shared.h>

typedef int tri_int[3];
typedef double tri_double[3];

int main ( int argc, char *argv[] )
{
  int t,v,i;
  FILE *fp;
  tri_int *ti;
  tri_double *vi;

  if (argc!=3)
    {
      fprintf(stderr,"Usage: t2iv <input .t file name> <output .t file name>\n");
      return 0;
    }

  fp = fopen(argv[1],"r");

  if (!fp)
    {
      fprintf(stderr,"Can't open input file %s\n",argv[1]);
      return 0;
    }

  fscanf(fp,"%d %d",&t,&v);

  ti = (tri_int*)malloc(t*sizeof(tri_int));
  vi = (tri_double*)malloc(v*sizeof(tri_double));

  for ( i=0; i<t; i++ )
    fscanf(fp,"%d %d %d",&ti[i][0],&ti[i][1],&ti[i][2]);

  for ( i=0; i<v; i++ )
    fscanf(fp,"%lf %lf %lf",&vi[i][0],&vi[i][1],&vi[i][2]);

  fclose(fp);

  fp = fopen(argv[2],"w");

  fprintf(fp,"%d %d\n\n",t,v);

  fprintf(fp,"%d %d %d\n",ti[0][1],ti[0][0],ti[0][2]);

  for ( i=1; i<t; i++ )
    fprintf(fp,"%d %d %d\n",ti[i][1],ti[i][0],ti[i][2]);

  fprintf(fp,"\n");

  for ( i=0; i<v; i++ )
    fprintf(fp,PFORMAT,vi[i][0],vi[i][1],vi[i][2]);

  fclose(fp);

  return 0;
}
