#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double mytimecheck(void);
int readit(char *whatever, double **pmat, int *pn);
int power(double *matrix, int n, double *vector, double *newvector);

int main(int argc, char  **argv)
{
	int code = 0;
	int readcode;
	double *matrix = NULL;
	int n = 0;
	double *vector = NULL, *newvector;
	double tstart, tend;
	int numcopies = 13, k;

	if (argc != 2){
		printf(" usage: power datafile\n"); code = 1000;  goto DONE;
	}

	readcode = readit(argv[1], &matrix, &n);
	if (readcode){
		goto DONE;
	}
	/** allocate auxiliary vectors for the power method **/

	vector = (double *)calloc(2 * n, sizeof(double));
	if (vector == NULL){
		printf("can't allocate vector\n"); code = 1; goto DONE;
	}
	newvector = vector + n;  /* what is this?*/
	printf("about to start running power code\n");

	tstart = mytimecheck();

	for (k = 0; k < numcopies; k++){
		printf("%d\n", k);
		code = power(matrix, n, vector, newvector);
		
	}

	tend = mytimecheck();
	printf("average cpu time: %f\n", (tend - tstart) / numcopies);

DONE:
	return code;
}

int power(double *matrix, int n, double *vector, double *newvector)
{
	int retcode = 0;
	int i, j, max_iters = 1000, iter;
	double oldnorm2, norm2, mult;
	double tolerance = 1e-10;
	double *nv, *v, *m, *v1,*v2,sum;

	/* choose random vector*/
	for (j = 0; j < n; j++){
		vector[j] = rand() /((double) RAND_MAX);
	}

	oldnorm2 = 0;
	for (iter = 0; iter < max_iters; iter++){
		m = matrix;
		nv = newvector;
		for (i = 0; i < n; i++){
			sum = 0;
			v = vector;
			for (j = 0; j < n; j++){
				sum += (*v) * (*m);
				v++;
				m++;
			}
			*nv = sum;
			++nv;
		}

		/* normalize */
		norm2 = 0;
		for (j = 0; j < n; j++) norm2 += newvector[j] * newvector[j];

		norm2 = sqrt(norm2);

		mult = 1 / norm2;

		for (j = 0; j < n; j++)
			vector[j] = mult*newvector[j];

		printf(" iter %d eig estimate: %.18e\n", iter, norm2);
		if (fabs(norm2 - oldnorm2) / norm2 < tolerance){
			printf("Done\n");

			v2 = vector;
			v1 = vector;
			for (i = 0; i < n; i++){
				for (j = 0; j < n; j++){
					*matrix= *matrix-v1[i]*v2[j]*norm2;
					matrix++;
				}
		}


			break;
		}

		oldnorm2 = norm2;
	}
	return retcode;
}

int readit(char *filename, double **pmat, int *pn)
{
	int code = 0;
	FILE *input = NULL;
	char buffer[100];
	int myn = 0;
	double *mymatrix = NULL;


	input = fopen(filename, "r");
	if (!input){
		printf("cannot open file %s\n", filename); code = 1; goto BACK;
	}

	fscanf(input, "%s", buffer);
	fscanf(input, "%s", buffer);
	myn = atoi(buffer);
	printf(" n = %d\n", myn);

	fscanf(input, "%s", buffer); fscanf(input, "%s", buffer); /** read the next two words but ignore **/
	mymatrix = (double *)calloc(myn*myn, sizeof(double));


	*pmat = mymatrix;
	*pn = myn;

	fscanf(input, "%s", buffer);
	for (int j = 0; j < myn*myn; j++){
		fscanf(input, "%s", buffer);
		mymatrix[j] = atof(buffer);
	}

BACK:
	return code;
}

#include<sys\timeb.h>

double mytimecheck(void)
{
   double seconds, millis;
   struct timeb mytimeb;

   ftime(&mytimeb);

   seconds = (double) mytimeb.time;
   millis = ( (double) mytimeb.millitm)/1000.00;

   return seconds+millis;
}
/*


	 


	double norm2 = 0;
	for(int j = 0; j < n; j++) norm2 += newvector[j]*newvector[j];

        mult = 1/sqrt(norm2);


	for(j = 0; j < n; j++) vector[j] = newvector[j]*mult;
	*/
