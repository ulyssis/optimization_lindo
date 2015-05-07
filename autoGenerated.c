/*
 *	c channels are available for a network composed with n nodes.
 *	Each node can only use one channel at one time.
 *	The two nodes workin on the same channel can cause interfernce on each other no matter how far they are apart from.
 *	The transmission power is fixed, and dependent only with channel.
 *	sum_interference/transmissionPower is the ratio between the received interference caused by co-channel interfernce and transmission power.
 *	objective function is the total sum of sum_interference/transmissionPower.
 *
 *	Minmize:	sum(i\in n) f_i/p_i
 *	    s.t.	for any user, one channel is choosed and the transmission power is accordingly decided.
 *
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

		// obtain the index of a numval based on the subindex in the Gtilde matrix. +1 in the end is to get the index in the array.
		int indexInarray_compound(int n, int c, int i, int j, int k)
			{
			int m = i*n*c + j*c + k;
			return m;
			}

		int indexInarray_NoisePowerRatio(int n, int c, int i, int k)
			{
			int m = n*n*c + i*c + k;
			return m;
			}

//		int indexInarray_noisePowerRatio(int n, int c, int i, int k)
//			{
//			int m = n*n + 1 + n*n*c + i*c + k;
//			return m;
//			}


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
	int n = 16, c = 4;
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

	/* >>> Step 2 <<< Create a model in the environment. */
	pModel = LScreateModel(pEnv,&nErrorCode);
	APIERRORCHECK;
    nErrorCode = LSsetModelLogfunc(pModel, (printModelLOG_t) print_line_log, NULL);
    LSreadModelParameter(pModel,"lindo.par");
		{
		/* >>>> Step 3 <<< Set up the instruction list of the model. */
		int nobjs, ncons, nvars, nnums, lsize;
		int i, j, k, z;	// remember to modify pmax!
		int num_variables = n*c; //	The binary variables storing channel usage info for every channel on each node
		int num_constants = n*n*c + n*c + 1; // n*n*c compounded coefficients, n*c noise power ratio, 1 constant(1)
		int num_constraints = n; // for every node, e.g. x0+x1+x3=1

		/* Direction of optimization */
		int objsense[1];
		/* Constraint type */
		char ctype[num_constants];
		/* Variable type, C = continuous, B = binary */
		char vtype[num_variables];
		/* index of instructions */
		int code[20000];
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

		/*	Initiate variables:	*/

		/* Index for Variables */
		for(k=0; k<n*c; k++)
			{
			varndx[k]=k;
            varval[k]=0;
			}

		/* Lower & Upper bounds of variables */
		int m;
		for(m=0; m < n*c; m++)
			{
			lwrbnd[m]=0;
			}
		for(i=0; i < n*c; i++)
			{
			uprbnd[i]=1;
			}


		/* initialize the variables, each node randomly chooses one channel*/
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

		/* Variable type. C= continuous, B = binary */
		for(i=0; i< n*c; i++)
			{
			vtype[i]='B';
			}

		// -------------------------------------
		// ---------- read data from file-------
		// -------------------------------------
		//	Double Precision constants in the model


	  	// read data from compoundMatrixInOneRow.txt
		double Gtilde[n*n*c];
	    FILE *fp;
	    fp = fopen("/home/li/work/tools/lindo/lindoapi/samples/c/dica/compoundCoefficient.txt", "rb");
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
		printf ("compoundMatrix:\n");
	  	for (j=0; j<sizeof(Gtilde)/8; j++)
	  		{
	  		printf ("%g\n", Gtilde[j]);

//	  		printf ("%g\n", 1e+10*Gtilde[j]);
	  		}
	  	printf ("\n");


	  	// read data from NoisePowerRatioInOneRow.txt to array power
	  	double power[n*c];
	  	fp = fopen("/home/li/work/tools/lindo/lindoapi/samples/c/dica/NoisePowerRatio.txt", "rb");
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
		printf ("NoisePowerRatio:\n");
	  	for (j=0; j<sizeof(power)/8; j++)
	  		{
	  		printf ("%g\n",power[j]);
	  		}
	  	printf ("\n");


	  	//----
		//-------import constant numbers from file to LINDO---------------------------
	  	//----

		// import Gtilde(gains) into numvals
		for(i=0; i< n*n*c; i++)
			{
			numval[i] = Gtilde[i]*(1e+10);
			}

		// import powers
		k=0;
		for(i= n*n*c; i< n*n*c + n*c ; i++)
			{
			numval[i] = power[k++]*(1e+10);
			}

		numval[n*n*c + n*c] = 1;
		// -------------------------------------------------
		// ---------- finish reading data from file ---------
		// -------------------------------------------------



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
		 * \sum_{i\in N} (\sum_j \sum_k x_{i,k}*x_{j,k}*h_{i,j,k} + \sum_k x_{i,k}*r_{i,k, N_0})
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
						compound = indexInarray_compound(n, c, i, j, k);

						code[ikod++]= EP_PUSH_VAR;
						code[ikod++]= i*c+k;			 /* x_{i,k} */
						code[ikod++]= EP_PUSH_VAR;
						code[ikod++]= j*c+k;			 /* x_{j,k} */
						code[ikod++]= EP_PUSH_NUM;
						code[ikod++]= compound;			 /* g_{i,j} */
						code[ikod++]= EP_MULTIPLY;
						code[ikod++]= EP_MULTIPLY;

						if (i!=0 || j!=1 || k!=0)	// (i==0 && j==1 && k==0)
							{
							code[ikod++]= EP_PLUS;
							}
						}
					}
				}
			for(k=0; k<c; k++)
				{
				NoisePowerRatio = indexInarray_NoisePowerRatio(n, c, i, k);

				code[ikod++]= EP_PUSH_VAR;
				code[ikod++]= i*c+k;			/* x_{i,k} 			*/
				code[ikod++]= EP_PUSH_NUM;
				code[ikod++]= NoisePowerRatio;			/* noise power ratio	*/
				code[ikod++]= EP_MULTIPLY;
				code[ikod++]= EP_PLUS;
				}
			}

		/* Length of objective */
		objs_length[iobj] = ikod - objs_beg[iobj];


		/*	constraints!	*/
		/*
		 * 	x_{i,1} + x_{i,2}... + x_{i,c} == 1, i\in N
		 */
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
			code[ikod++]= n*n*c + n*c;
			code[ikod++]= EP_MINUS;

			cons_length[icon] = ikod - cons_beg[icon];
			/* Increment the constraint count */
			icon++;
			}

		/* Total number of items in the instruction list */
		lsize = ikod;


		/* >>> Step 4 <<< Set the properties of the solvers	*/

		/* Set linearization level, before a call to LSloadInstruct.
		* If not specified, the solver will decide */
		/* linearization option is turned on! */
////		nLinearz = 0;
//		nLinearz = 1;
//		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_LINEARZ, nLinearz);
//		APIERRORCHECK;
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
		{
        int nLinearity;
		double objval, primal[n*c];
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
		printf("\n The following is for matlab:\n");

		for (pt=0; pt< n*c; pt++)
			{
			if (pt % c == c-1)
				{
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

	/* >>> Step 7 <<< Delete the LINDO environment */
	  LSdeleteEnv(&pEnv);
	}















