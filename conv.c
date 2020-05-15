/*
	Program executes a discrete convolution of a kernel (arg2) over an input feature map (arg1)

	Program will print final result if compiled with -DPRINT flag
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ITERATIONS 1

float *** convolve(float ***imap, float ***kernel, float ***omap, char type[], int channels, int isize, int ksize, int osize);

int main(int argc, char *argv[]){
	FILE *kernelfile, *inputfile;
	int k1, k2, kc, i1, i2, ic, o1, o2, oc;
	char ktype[4], itype[4];
	int i, j, x, y;
	unsigned long long int iterations;
	float ***imap, ***kernel, ***omap;
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

	o1 = i1 - k1 + 1;
	o2 = i2 - k2 + 1;
	oc = ic;

	if(!strcmp(itype, "CHW")){

		imap = (float***) malloc(sizeof(float**) * ic);
		kernel = (float***) malloc(sizeof(float**) * kc);

  		for(i = 0; i < ic; i++){
			imap[i] = (float**) malloc(sizeof(float*) * i1);
			for(j = 0; j < i1; j++){
        			imap[i][j] = (float*) malloc(sizeof(float) * i2);
			}
  		}

  		for(i = 0; i < kc; i++){
  			kernel[i] = (float**) malloc(sizeof(float*) * k1);
			for(j = 0; j < k1; j++){
        			kernel[i][j] = (float*) malloc(sizeof(float) * k2);
      			}
		}

    		for(x = 0; x < kc; x++){
    			for(i = 0; i < k1; i++){
	    			for(j = 0; j < k2; j++){
		    			fscanf(kernelfile, "%f", kernel[x][i] + j);
  		  		}
  	  		}
    		}

    		for(x = 0; x < ic; x++){
  	  		for(i = 0; i < i1; i++){
        			for(j = 0; j < i2; j++){
          				fscanf(inputfile, "%f", imap[x][i] + j);
        			}
      			}
    		}

  	} else { // HWC

  		imap = (float***) malloc(sizeof(float**) * i1);
	  	kernel = (float***) malloc(sizeof(float**) * k1);

  		for(i = 0; i < i1; i++){
  			imap[i] = (float**) malloc(sizeof(float*) * i2);
      			for(j = 0; j < i2; j++){
        			imap[i][j] = (float*) malloc(sizeof(float) * ic);
      			}
  		}

  		for(i = 0; i < k1; i++){
  			kernel[i] = (float**) malloc(sizeof(float*) * k2);
      			for(j = 0; j < k2; j++){
        			kernel[i][j] = (float*) malloc(sizeof(float) * kc);
      			}
	  	}

    		for(x = 0; x < k1; x++){
    			for(i = 0; i < k2; i++){
	    			for(j = 0; j < kc; j++){
		    			fscanf(kernelfile, "%f", kernel[x][i] + j);
  		  		}
  	  		}
    		}

    		for(x = 0; x < i1; x++){
  	  		for(i = 0; i < i2; i++){
        			for(j = 0; j < ic; j++){
          				fscanf(inputfile, "%f", imap[x][i] + j);
        			}
      			}
    		}

  	}
/*
	Convolution algorithm
*/

	omap = convolve(imap, kernel, omap, itype, ic, i1, k1, o1);

#ifdef PRINT
	for(x = 0; x < oc; x++){
		for(i = 0; i < o1; i++){
			for(j = 0; j < o2; j++){
				if(!strcmp(itype, "CHW")){
					printf("%f ", omap[x][i][j]);
				} else {
					printf("%f ", omap[i][j][x]);
				}
			}
			printf("\n");
		}
		if(x != (oc-1)){printf("\n");}
	}
#endif
/*
	Destruction of dynamic data
*/
  	if(!strcmp(itype, "CHW")){
	  	for(i = 0; i < ic; i++){
      			for(j = 0; j < i1; j++){
        			free(imap[i][j]);
      			}
      			free(imap[i]);
    		}

    		for(i = 0; i < kc; i++){
      			for(j = 0; j < k1; j++){
        			free(kernel[i][j]);
      			}
      			free(kernel[i]);
    		}

	  	for(i = 0; i < oc; i++){
      			for(j = 0; j < o1; j++){
        			free(omap[i][j]);
      			}
		  	free(omap[i]);
	  	}

  	} else {

    		for(i = 0; i < i1; i++){
      			for(j = 0; j < i2; j++){
        			free(imap[i][j]);
      			}
      			free(imap[i]);
    		}

    		for(i = 0; i < k1; i++){
      			for(j = 0; j < k2; j++){
        			free(kernel[i][j]);
      			}
      			free(kernel[i]);
    		}

	  	for(i = 0; i < o1; i++){
      			for(j = 0; j < o2; j++){
        			free(omap[i][j]);
      			}
		  	free(omap[i]);
	  	}
  	}

	free(kernel);
	free(imap);
	free(omap);

	return 0;
}

float *** convolve(float ***imap, float ***kernel, float ***omap, char type[], int channels, int isize, int ksize, int osize){
	clock_t time;
	unsigned long long int iterations;
	int i, j, x, y, c;
	float ***temp;

	if(!strcmp(type, "CHW")){
		temp = (float***) malloc(sizeof(float**) * channels);

		for(i = 0; i < channels; i++){
			temp[i] = (float**) malloc(sizeof(float*) * osize);
			for(j = 0; j < osize; j++){
				temp[i][j] = (float*) malloc(sizeof(float) * osize);
			}
		}
	} else {
                temp = (float***) malloc(sizeof(float**) * osize);

                for(i = 0; i < osize; i++){
                        temp[i] = (float**) malloc(sizeof(float*) * osize);
                        for(j = 0; j < osize; j++){
                                temp[i][j] = (float*) malloc(sizeof(float) * channels);
                        }
                }
	}

        time = clock();

        for(iterations = 0; iterations < ITERATIONS; iterations++){

		if(!strcmp(type, "CHW")){
			for(c = 0; c < channels; c++){
			        for(i = 0; i < osize; i++){
		        	        for(j = 0; j < osize; j++){
		                	temp[c][i][j] = 0;
		                        	for(x = 0; x < ksize; x++){
		                                	for(y = 0; y < ksize; y++){
		                                        	temp[c][i][j] += imap[c][i + x][j + y] * kernel[c][x][y];
		                                	}
		                        	}
		                	}
				}
			}

        	} else { // HWC
			for(c = 0; c < channels; c++){
                                for(i = 0; i < osize; i++){
                                        for(j = 0; j < osize; j++){
                                        temp[i][j][c] = 0;
                                                for(x = 0; x < ksize; x++){
                                                        for(y = 0; y < ksize; y++){
                                                                temp[i][j][c] += imap[i + x][j + y][c] * kernel[x][y][c];
                                                        }
                                                }
                                        }
                                }
                        }

		}

        }

        time = clock() - time;

        printf("Discrete convolution took %lf seconds for %i iterations\n", (double)time/CLOCKS_PER_SEC, ITERATIONS);

	return temp;
}
