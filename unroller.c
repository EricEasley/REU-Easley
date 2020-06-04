/*
	Program generates a linear verion of a looped discrete convolution of a kernel (arg2) over an input feature map (arg1)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ITERATIONS 1

float *** convolve(float ***imap, float ***kernel, float ***omap, char type[], int icnls, int kcnls, int kh, int kw, int oh, int ow, int stride);

int main(int argc, char *argv[]){
	FILE *kernelfile, *inputfile;
	int k1, k2, kc, i1, i2, ic, o1, o2, oc;
	char ktype[4], itype[4];
	int i, j, x, y;
	unsigned long long int iterations;
	float ***imap, ***kernel, ***omap;
	int stride, padding;

/*
	Setup
*/

	if(argc != 5){
		printf("usage: ./unroller <input feature map> <kernel> <stride> <padding>\n");
		exit(1);
	}

	stride = atoi(argv[3]);
	padding = atoi(argv[4]);

	if(stride <= 0){
		printf("Stride must be a positive integer\n");
		exit(1);
	}

	if(padding < 0){
		printf("Padding must be a non-negative integer\n");
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

	fscanf(inputfile, "%s", itype);

	fscanf(kernelfile, "%s", ktype);

	if(!strcmp(itype, "CHW")){
		fscanf(inputfile, "%i %i %i", &ic, &i1, &i2);
	} else {
		fscanf(inputfile, "%i %i %i", &i1, &i2, &ic);
	}

	if(!strcmp(ktype, "CHW")){
		fscanf(kernelfile, "%i %i %i", &kc, &k1, &k2);
	} else {
		fscanf(kernelfile, "%i %i %i", &k1, &k2, &kc);
	}

	i1 = i1 + 2*padding;
	i2 = i2 + 2*padding;

	o1 = (i1 - k1) / stride + 1;
	o2 = (i2 - k2) / stride + 1;
	oc = kc;

/*
	Convolution algorithm
*/

	convolve(imap, kernel, omap, itype, ic, kc, k1, k2, o1, o2, stride);

/*
	Destruction of dynamic data
*/

	fclose(kernelfile);
	fclose(inputfile);

	return 0;
}

float *** convolve(float ***imap, float ***kernel, float ***omap, char type[],  int icnls, int kcnls, int kh, int kw, int oh, int ow, int stride){
	unsigned long long int iterations;
	int i, j, x, y, ic, kc;

	if(!strcmp(type, "CHW")){
	        for(iterations = 0; iterations < ITERATIONS; iterations++){
			printf("\t\t\tfor(ic = 0; ic < icnls; ic++){\n");
				printf("\t\t\t\tfor(kc = 0; kc < kcnls; kc++){\n");
				        printf("\t\t\t\t\tfor(i = 0; i < oh; i++){\n");
			        	        printf("\t\t\t\t\t\tfor(j = 0; j < ow; j++){\n");
		        	        	printf("\t\t\t\t\t\t\tomap[kc][i][j] = 0;\n");
		                	        	for(x = 0; x < kh; x++){
		                        	        	for(y = 0; y < kw; y++){
		                                	        	printf("\t\t\t\t\t\t\tomap[kc][i][j] += imap[ic][i*stride + %i][j*stride + %i] * kernel[kc][%i][%i];\n", x, y, x, y);
								}
		                                	}
		                        	printf("\t\t\t\t\t\t}\n");
		                	printf("\t\t\t\t\t}\n");
				printf("\t\t\t\t}\n");
			printf("\t\t\t}\n");
		}
        } else { // HWC
		for(iterations = 0; iterations < ITERATIONS; iterations++){
			printf("\t\t\tfor(i = 0; i < oh; i++){\n");
                                printf("\t\t\t\tfor(j = 0; j < ow; j++){\n");
					printf("\t\t\t\t\tfor(ic = 0; ic < icnls; ic++){\n");
	                                        printf("\t\t\t\t\t\tfor(kc = 0; kc < kcnls; kc++){\n");
        	                                printf("\t\t\t\t\t\t\tomap[i][j][kc] = 0;\n");
                	                                for(x = 0; x < kh; x++){
                        	                                for(y = 0; y < kw; y++){
                                	                                printf("\t\t\t\t\t\t\tomap[i][j][kc] += imap[i*stride + %i][j*stride+ %i][ic] * kernel[%i][%i][kc];\n", x, y, x, y);
								}
                                                        }
                                                printf("\t\t\t\t\t\t}\n");
                                        printf("\t\t\t\t\t}\n");
                                printf("\t\t\t\t}\n");
                        printf("\t\t\t}\n");

		}

        }

	return omap;
}

