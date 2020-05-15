/*
	Prints out randomly generated multi-channel matrices

	5th arguement passed in denotes ordering of storage (0 = HWC; 1 = CHW)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	int h, w, c, sparsity, ordering;
	FILE *out;
	float ***mat;
	int i, j, k;
	int nnz;

	if(argc != 7){
		printf("usage: ./synth <channels> <height> <width> <nnz density (0 - 100)> <ordering (0 = HWC; 1 = CHW)> <output file name>\n");
		return 1;
	}

	c = atoi(argv[1]);
	h = atoi(argv[2]);
	w = atoi(argv[3]);
	sparsity = atoi(argv[4]);
	nnz = 0;
	ordering = atoi(argv[5]);

	out = fopen(argv[6], "w");

	if(out == NULL){
		printf("Failed to open output file\n");
	}

	srand(time(NULL));

	if(ordering == 1){ // CHW

		fprintf(out,"CHW\n");
		fprintf(out, "%i %i %i\n", c, h, w);
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
					fprintf(out,"%f ", mat[i][j][k]);
				}
				fprintf(out,"\n");
			}
			if(i != (c-1)){fprintf(out,"\n\n");}
		}

		printf("%i nonzero values with a sparsity of %f\n", nnz, (1 - ((float) nnz) / (h * w * c)));

		for(i = 0; i < c; i++){
			for(j = 0; j < h; j++){
				free(mat[i][j]);
			}
			free(mat[i]);
		}
		free(mat);

	} else { // HWC

		fprintf(out,"HWC\n");
		fprintf(out, "%i %i %i\n", h, w, c);
	        mat = (float***) malloc(sizeof(float**) * h);
        	for(i = 0; i < h; i++){
                	mat[i] = (float**) malloc(sizeof(float*) * w);
                	for(j = 0; j < w; j++){
                        	mat[i][j] = (float*) malloc(sizeof(float) * c);
                        	for(k = 0; k < c; k++){
                                	if(rand() % 100 < sparsity){
                                        	mat[i][j][k] = (float) (rand() % 10000) / 1000;
                                        	nnz++;
                                	} else {
                                        	mat[i][j][k] = 0;
                                	}
                                	fprintf(out,"%f ", mat[i][j][k]);
                        	}
                        	fprintf(out,"\n");
                	}
                	if(i != (h-1)){fprintf(out,"\n\n");}
        	}

	        printf("%i nonzero values with a sparsity of %f\n", nnz, (1 - ((float) nnz) / (h * w * c)));

        	for(i = 0; i < h; i++){
                	for(j = 0; j < w; j++){
                        	free(mat[i][j]);
                	}
                	free(mat[i]);
        	}
        	free(mat);

	}

	return 0;
}
