/*
	Program executes a discrete convolution of a kernel (arg2) over an input feature map (arg1)

	Program will print final result if compiled with -DPRINT flag
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ITERATIONS 1000

float *** convolve(float ***imap, float ***kernel, float ***omap, char type[], int icnls, int kcnls, int kh, int kw, int oh, int ow, int stride);
static __inline__ unsigned long long rdtsc(void);

int main(int argc, char *argv[]){
	FILE *kernelfile, *inputfile;
	int k1, k2, kc, i1, i2, ic, o1, o2, oc;
	char ktype[4], itype[4];
	int i, j, x, y;
	unsigned long long int iterations;
	float ***imap, ***kernel, ***omap;
	clock_t time;
	int stride;

/*
	Setup
*/

	if(argc != 4){
		printf("usage: ./conv <input feature map> <kernel> <stride>\n");
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

	stride = atoi(argv[3]);

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

	o1 = (i1 - k1) / stride + 1;
	o2 = (i2 - k2) / stride + 1;
	oc = ic * kc;

	if(!strcmp(itype, "CHW")){

		imap = (float***) malloc(sizeof(float**) * ic);
		kernel = (float***) malloc(sizeof(float**) * kc);
		omap = (float***) malloc(sizeof(float**) * oc);

                for(i = 0; i < oc; i++){
                        omap[i] = (float**) malloc(sizeof(float*) * o1);
                        for(j = 0; j < o1; j++){
                                omap[i][j] = (float*) malloc(sizeof(float) * o2);
                        }
                }

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
		omap = (float***) malloc(sizeof(float**) * o1);

                for(i = 0; i < o1; i++){
                        omap[i] = (float**) malloc(sizeof(float*) * o2);
                        for(j = 0; j < o2; j++){
                                omap[i][j] = (float*) malloc(sizeof(float) * oc);
                        }
                }

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

	convolve(imap, kernel, omap, itype, ic, kc, k1, k2, o1, o2, stride);

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

float *** convolve(float ***imap, float ***kernel, float ***omap, char type[],  int icnls, int kcnls, int kh, int kw, int oh, int ow, int stride){
	clock_t time;
	unsigned long long int iterations;
	int i, j, x, y, ic, kc;
	unsigned long long clocks;

        time = clock();
	clocks = rdtsc();

	if(!strcmp(type, "CHW")){
	        for(iterations = 0; iterations < ITERATIONS; iterations++){
			for(ic = 0; ic < icnls; ic++){
				for(kc = 0; kc < kcnls; kc++){
				        for(i = 0; i < oh; i++){
			        	        for(j = 0; j < ow; j++){
		        	        	omap[ic*icnls + kc][i][j] = 0;
		                	        	for(x = 0; x < kh; x++){
		                        	        	for(y = 0; y < kw; y++){
		                                	        	omap[ic*icnls + kc][i][j] += imap[ic][i*stride + x][j*stride + y] * kernel[kc][x][y];
								}
		                                	}
		                        	}
		                	}
				}
			}
		}
        } else { // HWC
		for(iterations = 0; iterations < ITERATIONS; iterations++){
			for(i = 0; i < oh; i++){
                                for(j = 0; j < ow; j++){
					for(ic = 0; ic < icnls; ic++){
	                                        for(kc = 0; kc < kcnls; kc++){
        	                                omap[i][j][ic*icnls + kc] = 0;
                	                                for(x = 0; x < kh; x++){
                        	                                for(y = 0; y < kw; y++){
                                	                                omap[i][j][ic*icnls + kc] += imap[i*stride + x][j*stride + y][ic] * kernel[x][y][kc];
								}
                                                        }
                                                }
                                        }
                                }
                        }

		}

        }

        time = clock() - time;
	clocks = rdtsc() - clocks;

        printf("Discrete convolution took %lf seconds for %i iterations\n", (double)time/CLOCKS_PER_SEC, ITERATIONS);
	printf("Clocks: %lli\n", clocks);

	return omap;
}

#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

#elif defined(__x86_64__)

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif
