/*
	Prints out randomly generated matrices in CHW
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	int h, w, c, sparsity;
	float ***mat;
	int i, j, k;
	int nnz;

	if(argc != 5){
		printf("usage: ./synth <channels> <height> <width> <sparsity (0 - 100)>\n");
		return 1;
	}

	c = atoi(argv[1]);
	h = atoi(argv[2]);
	w = atoi(argv[3]);
	sparsity = atoi(argv[4]);
	nnz = 0;

	srand(time(NULL));

	mat = (float***) malloc(sizeof(float**) * c);
	for(i = 0; i < c; i++){
		mat[i] = (float**) malloc(sizeof(float*) * h);
		for(j = 0; j < h; j++){
			mat[i][j] = (float*) malloc(sizeof(float) * w);
			for(k = 0; k < w; k++){
				if(rand() % 100 < sparsity){
					mat[i][j][k] = (float) (rand() % 10000) / 1000;
					nnz++;
				} else {
					mat[i][j][k] = 0;
				}
				printf("%f ", mat[i][j][k]);
			}
			printf("\n");
		}
		if(i != (c-1)){printf("\n\n");}
	}

	printf("%i nonzero values with a sparsity of %f\n", nnz, (1 - ((float) nnz) / (h * w * c)));

	for(i = 0; i < c; i++){
		for(j = 0; j < h; j++){
			free(mat[i][j]);
		}
		free(mat[i]);
	}
	free(mat);

	return 0;
}
