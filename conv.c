#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 1000000

int main(int argc, char *argv[]){
	FILE *kernelfile, *inputfile;
	int k1, k2, i1, i2, o1, o2;
	int i, j, x, y;
	unsigned long long int iterations;
	int **imap, **kernel, **omap;
	clock_t time;

/*
	Setup
*/

	if(argc != 3){
		printf("usage: ./conv <input feature map> <kernel>\n");
		exit(1);
	}

	inputfile = fopen(argv[1], "r");
	if(inputfile == NULL){
		printf("Couldn't open input feature map file\n");
		exit(1);
	}

	kernelfile = fopen(argv[2], "r");
	if(kernelfile == NULL){
		printf("Couldn't open kernel file\n");
		exit(1);
	}

	fscanf(inputfile, "%i %i", &i1, &i2);

	fscanf(kernelfile, "%i %i", &k1, &k2);

	o1 = i1 - k1 + 1;
	o2 = i2 - k2 + 1;

	imap = (int**) malloc(sizeof(int*) * i1);
	kernel = (int**) malloc(sizeof(int*) * k1);
	omap = (int**) malloc(sizeof(int*) * o1);

	for(i = 0; i < i1; i++){
		imap[i] = (int*) malloc(sizeof(int) * i2);
	}

	for(i = 0; i < k1; i++){
		kernel[i] = (int*) malloc(sizeof(int) * k2);
	}

	for(i = 0; i < o1; i++){
		omap[i] = (int*) malloc(sizeof(int) * o2);
	}

	for(i = 0; i < k1; i++){
		for(j = 0; j < k2; j++){
			fscanf(kernelfile, "%i", kernel[i] + j);
		}
	}

	for(i = 0; i < i1; i++){
                for(j = 0; j < i2; j++){
                        fscanf(inputfile, "%i", imap[i] + j);
                }
        }
/*
	Convolution algorithm
*/

	time = clock();

	for(iterations = 0; iterations < ITERATIONS; iterations++){

	for(i = 0; i < o1; i++){
		for(j = 0; j < o2; j++){
		omap[i][j] = 0;
			for(x = 0; x < k1; x++){
				for(y = 0; y < k2; y++){
					omap[i][j] += imap[i + x][j + y] * kernel[x][y];
				}
			}
		}
	}

	}

	time = clock() - time;

	printf("Discrete convolution took %lf seconds for %i iterations\n", (double)time/CLOCKS_PER_SEC, ITERATIONS);

#ifdef PRINT
	for(i = 0; i < o1; i++){
		for(j = 0; j < o2; j++){
			printf("%i ", omap[i][j]);
		}
		printf("\n");
	}
#endif
/*
	Destruction of dynamic data
*/

	for(i = 0; i < i1; i++){
                free(imap[i]);
        }

        for(i = 0; i < k1; i++){
                free(kernel[i]);
        }

	for(i = 0; i < o1; i++){
		free(omap[i]);
	}

	free(kernel);
	free(imap);
	free(omap);

	return 0;
}
