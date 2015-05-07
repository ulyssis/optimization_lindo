/*  formulation for chapter 1, 3.18 in thesis
 *
 *	c channels are available for a network composed with n nodes.
 *	Each node can only use one channel at one time.
 *	The two nodes workin on the same channel cause interfernce on each other no matter how far they are apart from.
 *	The transmission power is between the maximal permitted transmission power and a very small lowest power.
 *	sum_interference/transmissionPower is the ratio between the received interference caused by co-channel interfernce and transmission power.
 *	objective function is the total sum of sum_interference/transmissionPower.
 *
 *	Minmize:	sum(i\in n) f_i/p_i
 *	    s.t.	for any user, one channel is choosed and the transmission power is accordingly decided.
 *
 * compoundcoefficient means i put multiple constants together to make the codes easier.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* LINDO API header file */
#include "/home/li/work/tools/lindo/lindoapi/include/lindo.h"
/* Define a macro to declare variables for error checking */
#define APIERRORSETUP \
int nErrorCode; \
char cErrorMessage[LS_MAX_ERROR_MESSAGE_LENGTH] \

/* Define a macro to do our error checking */
#define APIERRORCHECK \
   if (nErrorCode) \
	   { \
		  if ( pEnv) \
		  { \
			 LSgetErrorMessage( pEnv, nErrorCode, \
			  cErrorMessage); \
			 printf("Errorcode=%d:  %s\n", nErrorCode, \
			  cErrorMessage); \
		  } else {\
			 printf( "Fatal Error\n"); \
		  } \
		  exit(1); \
	   } \

#define APIVERSION \
	{\
	   char szVersion[255], szBuild[255];\
	   LSgetVersionInfo(szVersion,szBuild);\
	   printf("\nLINDO API Version %s built on %s\n",szVersion,szBuild);\
	}\

		// index of numval. +1 in the end is to get the index in the array.
		int constIndex_h(int n, int i, int j)
			{
			int m = i*n + j;
			return m;
			}
		int constIndex_power(int n, int c, int i, int k)
			{
			int m = n*n + i*c + k;
			return m;
			}

		int constIndex_noise(int n, int c)
			{
			int m = n*n + n*c;
			return m;
			}
		int constIndex_lowPower(int n, int c)
			{
			int m = n*n + n*c +1;
			return m;
			}
		int constIndex_zero(int n, int c)
			{
			int m = n*n + n*c +2;
			return m;
			}
		int constIndex_one(int n, int c)
			{
			int m = n*n + n*c + 3;
			return m;
			}
		int constIndex_two(int n, int c)
			{
			int m = n*n + n*c + 4;
			return m;
			}

		// index of varval
		int index_x(int n, int c, int i, int k)
			{
			int m = i*c + k;
			return m;
			}
		int index_p(int n, int c, int i, int k)
			{
			int m = n*c + i*c + k;
			return m;
			}
		int index_alpha(int n, int c, int i, int j, int k)
			{
			int m = 2*n*c + (i*n+j)*c + k;
			return m;
			}
		int index_beta(int n, int c, int i, int j, int k)
			{
			int m = 2*n*c + n*n*c + (i*n+j)*c + k;
			return m;
			}
		int index_q(int n, int c, int i, int k)
			{
			int m = 2*n*c + 2*n*n*c + i*c + k;
			return m;
			}


static void CALLTYPE print_line_log(pLSmodel pModel, char *line, void *userdata)
	{
	  if (line)
	  {
		printf("%s",line);
	  } /*if*/
	} /*print_line*/

/* main entry point */
int main()
	{
	clock_t start = clock();//start recording Elapsed time of running this program
	int n = 10, c = 3;
	double lowPower =0.0;  //xxxxxxxx
	APIERRORSETUP;
	pLSenv pEnv;
	pLSmodel pModel;
	char MY_LICENSE_KEY[1024];

	/*	>>> Step 1 <<< Create a model in the environment.	*/
	nErrorCode = LSloadLicenseString("../../../license/lndapi60.lic", MY_LICENSE_KEY);
	if ( nErrorCode != LSERR_NO_ERROR)
		{
		printf( "Failed to load license key (error %d)\n", nErrorCode);
		exit(1);
		}
	APIVERSION;
	pEnv = LScreateEnv ( &nErrorCode, MY_LICENSE_KEY);
	if ( nErrorCode == LSERR_NO_VALID_LICENSE)
		{
		printf("Invalid License Key!\n");
		exit(1);
		}
	APIERRORCHECK;

	//record the initial value
	double initial[100];

	/* >>> Step 2 <<< Create a model in the environment. */
	pModel = LScreateModel(pEnv,&nErrorCode);
	APIERRORCHECK;
    nErrorCode = LSsetModelLogfunc(pModel, (printModelLOG_t) print_line_log, NULL);
    LSreadModelParameter(pModel,"lindo.par");
		{
		/* >>>> Step 3 <<< Set up the instruction list of the model. */
		int nobjs, ncons, nvars, nnums, lsize;
		int i, j, k, z;	// remember to modify pmax!
		int num_variables = 3*n*c + 2*n*n*c; //pow(n, 2)
		int num_constants = n*n + n*c + 5;	 // attenuation, maximal power, noise, lowPower, 0, 1, 2
		int num_constraints = 6*n*n*c + n + n*c; // n +n*c equality constraints

		/* Direction of optimization */
		int objsense[1];
		/* Constraint type */
		char ctype[num_constraints];
		/* Variable type, C = continuous, B = binary */
		char vtype[num_variables];
		/* index of instructions */
		int code[100000]; ///xxxxxxxx
		/* Variable index */
		int varndx[num_variables];
		/* Double Precision constants in the model, n*n+n*c+2+1 */
		double numval[num_constants];
		/* Starting point of variables */
		double varval[num_variables];
		/* index of start points and lengthes of object and constraints */
		int objs_beg[1], objs_length[1], cons_beg[num_constraints], cons_length[num_constraints];
		/* lower and upper bound */
		double lwrbnd[num_variables], uprbnd[num_variables];
		/* linearization level, automatic differentiation*/
		int nLinearz, nAutoDeriv;
		int ikod, iobj, icon;
		/* Number of constraints */
		ncons = num_constraints;
		/* Number of objectives */
		nobjs = 1;
		/* Number of variables */
		nvars = num_variables;
		/* Number of real number constants */
		nnums = num_constants;


		// -------------------------------------
		// ---------- read from Matlab results-------
		// -------------------------------------
		//	Double Precision constants in the model


	  	// read data from Gtilde.txt
		double Gtilde[n*n];
	    FILE *fp;
	    fp = fopen("/home/li/work/tools/lindo/lindoapi/samples/c/dica/Gtilde.txt", "rb");
//	    fp = fopen("Gtilde.txt", "rb");
	    if(fp == NULL)
	    	{
	        fprintf(stderr,"Cannot open file for reading");
	        exit(1);
	    	}
		i=0;
	 	while(!feof(fp))
		 	{
			if( fscanf(fp, "%lf", &Gtilde[i]) == 1 )
			   	{
			    i++;
			 	}
			}	// read from fp to Gtilde

	    if(fclose(fp) != 0)
	     	{
	        fprintf(stderr,"Could not close file properly!\n");
	        exit(1);
	     	}
		printf ("attenuation:\n");
	  	for (j=0; j<sizeof(Gtilde)/8; j++)
	  		{
//	  		if(j)
	  		printf ("%g\n", Gtilde[j]);

//	  		printf ("%g\n", 1e+10*Gtilde[j]);
	  		}
	  	printf ("\n");


	  	// read data from maxPermittedPower to array power, used as upper bound
	  	double power[n*c+1];
	  	fp = fopen("/home/li/work/tools/lindo/lindoapi/samples/c/dica/maxPermittedPower.txt", "rb");
//	  	fp = fopen("power.txt", "rb");
	  	if(fp == NULL)
			{
			fprintf(stderr,"Cannot open file for reading");
			exit(1);
			}
		i=0;

		while(!feof(fp))
			{
			if( fscanf(fp, "%lf", &power[i]) == 1 )
				{
				i++;
				}
			}

		if(fclose(fp) != 0)
			{
			fprintf(stderr,"Could not close file properly!\n");
			exit(1);
			}
		printf ("maximal permitted power level:\n");
	  	for (j=0; j<sizeof(power)/8 -1; j++)
	  		{
	  		printf ("%g\n",power[j]);
	  		}
	  	printf ("\n");

		//-------import constant numbers from file to constant vector numval in LINDO---------------------------
		// import Gtilde(gains) into numvals  // *(1e+10)
		for(i=0; i< n*n; i++)
			{
			numval[i] = Gtilde[i]*(1e+10);
			}
		// import powers
		k=0;
		for(i= n*n; i< n*n + n*c ; i++)
			{
			numval[i] = power[k++];
			}
		int currentIndex = n*n + n*c;
		numval[currentIndex] = power[currentIndex]; //noise
		numval[++currentIndex] = lowPower; //lowest power
		numval[++currentIndex] = 0;
		numval[++currentIndex] = 1;
		numval[++currentIndex] = 2;






		/*	Initiate variables:	*/

		/* Index and starting points of Variables */
		for(i=0; i<num_variables; i++)
			{
			varndx[i]=i;
            varval[i]=0;
			}
		// initialize the variables, each node randomly chooses one channel
		srand ( time(NULL) );
		int miniIndex;
		for(i = 0; i < n ; i++)
			{
			miniIndex = rand() % c;
			varval[i*c + miniIndex] = 1;
			}
		for (i = 0; i< n*c; i++)
			{
			if((i+1)%c==0)
			printf("varval[%d] = %f \n", i, varval[i]);
			else
			printf("varval[%d] = %f \t", i, varval[i]);
			}

//copy first n*c members of varval to intitial
		for (i=0; i<n*c; i++){
			initial[i] = varval[i];
		}

		// starting points of p
		k =0;
		for(i=n*c; i<2*n*c; i++){
			varval[i]=power[k++];
		}
		//\alpha
		for(i = 0; i < n ; i++){
			for(j = 0; j < n ; j++){
				for(k = 0; k < c ; k++){
					varval[index_alpha(n,c,i,j,k)] = varval[index_x(n, c, i, k)] * varval[index_x(n, c, j, k)];
				}
			}
		}
		// starting points of q
		k=0;
		for(i=2*n*c+2*n*n*c; i<3*n*c+2*n*n*c; i++){
			varval[i]=1.0/power[k++];
		}

		//\beta
		for(i = 0; i < n ; i++){
			for(j = 0; j < n ; j++){
				for(k = 0; k < c ; k++){
					varval[index_beta(n,c,i,j,k)] = varval[index_p(n, c, j, k)] * varval[index_alpha(n, c, i, j, k)];
				}
			}
		}

// print the varnum
		printf("\n #######################\n  print the vector numval: \n");
		printf("\n attenuation, maxPower and 5 others: \n");
	  	for (j=0; j<sizeof(numval)/8+1; j++)
	  		{
	  		if(j < n*n){
	  			if((j+1)%n==0){
	  				printf ("%g\n",numval[j]);
	  			}
	  			else
		  			printf ("%g\t",numval[j]);
	  		}
	  		if(j >= n*n){
	  			if((j+1)%c==0)
	  				printf ("%g\n",numval[j]);
				else
					printf ("%g\t",numval[j]);
	  			}
	  		}
	  	printf ("\n");

// print the start values of vector varval
		printf("\n #######################\n  print the start values of vector varval: \n");
		for (i = 0; i< num_variables; i++)
			{
			if((i+1)%c==0)
				if(i==n*c-1 || i==2*n*c-1 || i==2*n*c+n*n*c-1 || i==2*n*c+2*n*n*c-1 || i==3*n*c+3*n*n*c-1){ // new line after one chunk of variables
					printf("varval[%d] = %f \n\n", i, varval[i]);
				}
				else
					printf("varval[%d] = %f \n", i, varval[i]);
			else
			printf("varval[%d] = %f \t", i, varval[i]);
			}





		/* Lower & Upper bounds of variables */
		//x
		for(i=0; i < n*c; i++)
			{
			lwrbnd[i]=0;
			uprbnd[i]=1;
			}
		//p
		for(i=n*c; i < 2*n*c; i++)
			{
			lwrbnd[i]=lowPower;
			uprbnd[i]=power[i-n*c];
			}
		//\alpha
		for(i=2*n*c; i < 2*n*c+n*n*c; i++)
			{
			lwrbnd[i]=0;
			uprbnd[i]=1;
			}
		//\beta
		for(i = 0; i < n ; i++){
			for(j = 0; j < n ; j++){
				for(k = 0; k < c ; k++){
					lwrbnd[index_beta(n, c, i, j, k)] = 0;
					uprbnd[index_beta(n, c, i, j, k)] = varval[index_p(n, c, j, k)];
				}
			}
		}
		// q
		for(i=2*n*c+2*n*n*c; i < 3*n*c+2*n*n*c; i++)
			{
			lwrbnd[i]=1.0/power[i-2*n*c-2*n*n*c];
			uprbnd[i]=1.0/lowPower;
			}

//// print the start values of vector varval
//		printf("\n #######################\n print the lower bounds of vector varval: \n");
//		for (i = 0; i< num_variables; i++)
//			{
//			if((i+1)%c==0)
//				if(i==n*c-1 || i==2*n*c-1 || i==2*n*c+n*n*c-1 || i==2*n*c+2*n*n*c-1 || i==3*n*c+3*n*n*c-1){ // new line after one chunk of variables
//					printf("lwrbnd[%d] = %f \n\n", i, lwrbnd[i]);
//				}
//				else
//					printf("lwrbnd[%d] = %f \n", i, lwrbnd[i]);
//			else
//			printf("lwrbnd[%d] = %f \t", i, lwrbnd[i]);
//			}
//
//		printf("\n #######################\n  print the upper bounds of vector varval: \n");
//		for (i = 0; i< num_variables; i++)
//			{
//			if((i+1)%c==0)
//				if(i==n*c-1 || i==2*n*c-1 || i==2*n*c+n*n*c-1 || i==2*n*c+2*n*n*c-1 || i==3*n*c+3*n*n*c-1){ // new line after one chunk of variables
//					printf("uprbnd[%d] = %f \n\n", i, uprbnd[i]);
//				}
//				else
//					printf("uprbnd[%d] = %f \n", i, uprbnd[i]);
//			else
//			printf("uprbnd[%d] = %f \t", i, uprbnd[i]);
//			}



		/* Variable type. C= continuous, B = binary */
		//x
		for(i=0; i< n*c; i++)
			{
			vtype[i]='B';
			}
		//p
		for(i=n*c; i < 2*n*c; i++)
			{
			vtype[i]='C';
			}
		//alpha
		for(i=2*n*c; i < 2*n*c+n*n*c; i++)
			{
			vtype[i]='B';
			}
		//\beta and q
		for(i=2*n*c+n*n*c; i < num_variables; i++)
			{
			vtype[i]='C';
			}



		// -------------------------------------------------
		// ---------- finish reading data from file ---------
		// -------------------------------------------------

		printf ("OK1:\n");

        int compound, NoisePowerRatio; //the index for coefficient
		/* Count for instruction code */
		ikod = 0;
		/* Count for objective row */
		iobj = 0;
		/* Count for constraint row */
		icon = 0;

		/* Direction of optimization */
		objsense[iobj]= LS_MIN;
		/* Beginning position of objective */
		objs_beg[iobj]= ikod;

		/*
		 * min:
		 * \sum_{i\in N} (\sum_j \sum_k q_{i,k}*\beta_{i,j}^k * h_{i,j} * z + \sum_k q_{i,k}*q_{i,k} * N_0)
		 * j!=i, k\in \mathcal{C}
		 *
		 * h_{i,j} includes pass loss and shadowing
		 */

		for(i=0; i<n; i++)
			{
			for(j=0; j<n; j++ )
				{
				if( j!= i )
					{
					for(k=0; k<c; k++)
						{
						code[ikod++]= EP_PUSH_VAR;
						code[ikod++]= index_q(n,c,i,k);			 /* x_{i,k} */
						code[ikod++]= EP_PUSH_VAR;
						code[ikod++]= index_beta(n,c,i,j,k);			 /* x_{j,k} */
						code[ikod++]= EP_PUSH_NUM;
						code[ikod++]= constIndex_h(n, i, j);			 /* g_{i,j} */
						code[ikod++]= EP_MULTIPLY;
						code[ikod++]= EP_MULTIPLY;

						if (i!=0 || j!=1 || k!=0)	// opposite of (i==0 && j==1 && k==0)
							{
							code[ikod++]= EP_PLUS;
							}
						}
					}
				}
			for(k=0; k<c; k++)
				{
				code[ikod++]= EP_PUSH_VAR;
				code[ikod++]= index_q(n,c,i,k);			/* q_{i,k} 			*/
				code[ikod++]= EP_PUSH_VAR;
				code[ikod++]= index_x(n,c,i,k);			/* x_{i,k} 			*/
				code[ikod++]= EP_PUSH_NUM;
				code[ikod++]= constIndex_noise(n,c);			/* noise power	*/
				code[ikod++]= EP_MULTIPLY;
				code[ikod++]= EP_MULTIPLY;
				code[ikod++]= EP_PLUS;
				}
			}

		/* Length of objective */
		objs_length[iobj] = ikod - objs_beg[iobj];

		printf ("OK2:\n");


		/*	constraints!	*/
		/*
		 * 	x_{i,1} + x_{i,2}... + x_{i,c} == 1, i\in N
		 */
		//0, the sum
		for(i=0; i<n; i++)
			{
			/* Constraint type */
			ctype[icon]= 'E';
			cons_beg[icon]= ikod;
			for(j=0; j<c; j++)	/*dealing with channels*/
				{
				code[ikod++]= EP_PUSH_VAR;
				code[ikod++]= i*c+j;
				}
			for(k=1; k<c; k++)
				{
				code[ikod++]= EP_PLUS;
				}
			code[ikod++]= EP_PUSH_NUM;
			code[ikod++]= constIndex_one(n,c);
			code[ikod++]= EP_MINUS;

			cons_length[icon] = ikod - cons_beg[icon];
			/* Increment the constraint count */
			icon++;
			}

		//1
		for(i =0; i<n; i++){
			for(j =0; j<n; j++){
				for(k =0; k<c; k++){
					ctype[icon]= 'L';
					cons_beg[icon]= ikod;
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_x(n, c, j, k);
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_x(n, c, i, k);
					code[ikod++]= EP_PLUS;
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_alpha(n, c, i, j, k);
					code[ikod++]= EP_MINUS;
					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_one(n, c);
					code[ikod++]= EP_MINUS;

					cons_length[icon] = ikod - cons_beg[icon];
					/* Increment the constraint count */
					icon++;
				}
			}
		}

		//2
		for(i =0; i<n; i++){
			for(j =0; j<n; j++){
				for(k =0; k<c; k++){
					ctype[icon]= 'L';
					cons_beg[icon]= ikod;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_alpha(n, c, i, j, k);
					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_two(n,c);
					code[ikod++]= EP_MULTIPLY;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_x(n, c, j, k);
					code[ikod++]= EP_MINUS;
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_x(n, c, i, k);
					code[ikod++]= EP_MINUS;
					cons_length[icon] = ikod - cons_beg[icon];
					/* Increment the constraint count */
					icon++;
				}
			}
		}

		//3
		for(i =0; i<n; i++){
			for(j =0; j<n; j++){
				for(k =0; k<c; k++){
					ctype[icon]= 'L';
					cons_beg[icon]= ikod;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_beta(n, c, i, j, k);

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_alpha(n, c, i, j, k);
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_p(n, c, j, k);
					code[ikod++]= EP_MULTIPLY;

					code[ikod++]= EP_MINUS;

					cons_length[icon] = ikod - cons_beg[icon];
					/* Increment the constraint count */
					icon++;
				}
			}
		}

		//4
		for(i =0; i<n; i++){
			for(j =0; j<n; j++){
				for(k =0; k<c; k++){
					ctype[icon]= 'L';
					cons_beg[icon]= ikod;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_alpha(n, c, i, j, k);
					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_lowPower(n,c);
					code[ikod++]= EP_MULTIPLY;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_beta(n, c, i, j, k);
					code[ikod++]= EP_MINUS;
					cons_length[icon] = ikod - cons_beg[icon];
					/* Increment the constraint count */
					icon++;
				}
			}
		}

		//5
		for(i =0; i<n; i++){
			for(j =0; j<n; j++){
				for(k =0; k<c; k++){
					ctype[icon]= 'L';
					cons_beg[icon]= ikod;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_beta(n, c, i, j, k);
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_p(n, c, j, k);
					code[ikod++]= EP_MINUS;
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_alpha(n, c, i, j, k);
					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_lowPower(n,c);
					code[ikod++]= EP_MULTIPLY;
					code[ikod++]= EP_MINUS;
					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_lowPower(n,c);
					code[ikod++]= EP_PLUS;


					cons_length[icon] = ikod - cons_beg[icon];
					/* Increment the constraint count */
					icon++;
				}
			}
		}

		//6
		for(i =0; i<n; i++){
			for(j =0; j<n; j++){
				for(k =0; k<c; k++){
					ctype[icon]= 'L';
					cons_beg[icon]= ikod;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_p(n, c, j, k);
					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_beta(n, c, i, j, k);
					code[ikod++]= EP_MINUS;

					code[ikod++]= EP_PUSH_VAR;
					code[ikod++]= index_alpha(n, c, i, j, k);
					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_power(n,c,j,k);
					code[ikod++]= EP_MULTIPLY;
					code[ikod++]= EP_PLUS;

					code[ikod++]= EP_PUSH_NUM;
					code[ikod++]= constIndex_power(n,c,j,k);
					code[ikod++]= EP_MINUS;

					cons_length[icon] = ikod - cons_beg[icon];
					/* Increment the constraint count */
					icon++;
				}
			}
		}

		//p_i^k * q_i^k = 1
		for(i =0; i<n; i++){
			for(k =0; k<c; k++){
			ctype[icon]= 'E';
			cons_beg[icon]= ikod;

			code[ikod++]= EP_PUSH_VAR;
			code[ikod++]= index_q(n, c, i, k);
			code[ikod++]= EP_PUSH_VAR;
			code[ikod++]= index_p(n, c, i, k);
			code[ikod++]= EP_MULTIPLY;

			code[ikod++]= EP_PUSH_NUM;
			code[ikod++]= constIndex_one(n,c);
			code[ikod++]= EP_MINUS;

			cons_length[icon] = ikod - cons_beg[icon];
			/* Increment the constraint count */
			icon++;
				}
			}




		printf ("OK3:\n");


		/* Total number of items in the instruction list */
		lsize = ikod;
		printf ("ikod = %d \n", lsize);
		printf ("test = %f \n", 1.0/2.0);

		/* >>> Step 4 <<< Set the properties of the solvers	*/

		/* Set linearization level, before a call to LSloadInstruct.
		* If not specified, the solver will decide */
		/* linearization option is turned on! */
////		nLinearz = 0;
		nLinearz = 2;
		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_LINEARZ, nLinearz);
		APIERRORCHECK;
//
		/* Set up automatic differentiation, before a call to LSloadInstruct.
			If not specified, the numerical derivative will be applied */
//		nAutoDeriv = 0;
		nAutoDeriv = 2;
		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_AUTODERIV, nAutoDeriv);
		APIERRORCHECK;


//		/*	Should LSsolveMIP is called, use the following codes	*/
// 	   nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_MIP_TOPOPT, LS_METHOD_NLP);	//LS_METHOD_NLP is '4', represeting nonlinear solver
//  	   APIERRORCHECK;
//  	   nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_MIP_REOPT, LS_METHOD_NLP);
//  	   APIERRORCHECK;


		/*	Should LSsolveGOP is called, use the following codes */
		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_QUADCHK, 0);
		APIERRORCHECK;


		/* Pass the instruction list to problem structure
		* by a call to LSloadInstruct() */
		nErrorCode = LSloadInstruct (pModel, ncons, nobjs, nvars, nnums, objsense, ctype, vtype, code, lsize, varndx, numval, varval, objs_beg, objs_length, cons_beg, cons_length, lwrbnd, uprbnd);
		APIERRORCHECK;
		}


	/* >>> Step 5 <<< Perform the optimization	*/
    char *pszFname="autoGenerated.mpi";
    nErrorCode = LSwriteMPIFile(pModel, pszFname);
    APIERRORCHECK;
    printf("%s\n", pszFname);

//	nErrorCode = LSsolveMIP(pModel, NULL); //P491 manual
    nErrorCode = LSsolveGOP(pModel, NULL); //P491 manual
	APIERRORCHECK;

	// output matrix B to file "matrixB"
	remove("matrixB");
	FILE *matrixB_vriablePower, *matrixP_vriablePower, *flag_resolve;
	matrixB_vriablePower = fopen("/home/li/work/dev/DiCAPS/2011OtcGameOptReform/codes_cameraready/matrixB_vriablePower", "w");
	matrixP_vriablePower = fopen("/home/li/work/dev/DiCAPS/2011OtcGameOptReform/codes_cameraready/matrixP_vriablePower", "w");
	flag_resolve = fopen("/home/li/work/dev/DiCAPS/2011OtcGameOptReform/codes_cameraready/flag_resolve", "w");
	{
        int nLinearity;
		double objval, primal[2*n*c];
		/* Get the optimization result */
		LSgetInfo(pModel, LS_DINFO_MIP_OBJ, &objval);
		APIERRORCHECK;
		LSgetMIPPrimalSolution( pModel, primal);
		APIERRORCHECK;
		printf("\n\nObjective = %f \n",objval);
		int pt;
		for (pt=0; pt< n*c; pt++)
			{
			if (pt % c == c-1)
				{
				printf("x[%d] =  %f \n", pt, primal[pt]);
				}
			else
				{
				printf("x[%d] =  %f \t", pt, primal[pt]);
				}
			}
		printf("\n The following is writen into files to be used by matlab:\n");
		printf("\n channel usage:\n");
		for (pt=0; pt< n*c; pt++)
			{
			if (pt % c == c-1)
				{
				fprintf(matrixB_vriablePower, "%f\n", primal[pt]); //write to file matrixB in matlab's folder
				if(pt != n*c-1)
					{
					printf("%f; \n", primal[pt]);
					}
				else
				printf("%f]; \n", primal[pt]);
				}
			else
				{
				if(pt == 0)
					{
					printf("[");
					}
				printf("%f \t", primal[pt]);
				fprintf(matrixB_vriablePower, "%f\t", primal[pt]); //write to file matrixB in matlab's folder
				}
			}

		/*
		 *  judge whether calculated channel usage is identical with the initial inputs.
		 *  being identical means solver fails
		 *  undentical means solver achieves 'good' result
		 */
		int flag =0;
		int i;
		for(i=0; i<n*c; i++){
			flag += (initial[i]!=primal[i]);
		}
		fprintf(flag_resolve, "%d\n", flag);

		printf("\n power allocation:\n");
		for (pt=n*c; pt< 2*n*c; pt++)
			{
			if (pt % c == c-1)
				{
				fprintf(matrixP_vriablePower, "%f\n", primal[pt]); //write to file matrixB in matlab's folder
				if(pt != n*c-1)
					{
					printf("%f; \n", primal[pt]);
					}
				else
				printf("%f]; \n", primal[pt]);
				}
			else
				{
				if(pt == 0)
					{
					printf("[");
					}
				printf("%f \t", primal[pt]);
				fprintf(matrixP_vriablePower, "%f\t", primal[pt]); //write to file matrixB in matlab's folder
				}
			}



        nErrorCode = LSwriteSolution( pModel, "a.sol");

		/* Report the status of solution */
		if (nLinearity)
			  printf("\nModel has been completely linearized.\
					  \nSolution Status: Globally Optimal\n");
			  else
			  printf("\nModel is nonlinear.\
					  \nSolution Status: Locally Optimal\n\n");
		printf("Took %f seconds\n", ((double)clock()-start)/CLOCKS_PER_SEC);//printout the Elapsed time of running this program

		}
		fclose(matrixB_vriablePower);
		fclose(matrixP_vriablePower);


	/* >>> Step 7 <<< Delete the LINDO environment */
	  LSdeleteEnv(&pEnv);
	}















