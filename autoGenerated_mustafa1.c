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
#include "lindo.h"
/* Define a macro to declare variables for error checking */
#define APIERRORSETUP \
int nErrorCode; \
char cErrorMessage[LS_MAX_ERROR_MESSAGE_LENGTH] \

/* Define a macro to do our error checking */
#define APIERRORCHECK  \
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

		// obtain the index of a numval based on the subindex in the Gtilde matrix.
		int index_pathlossMatrix2array(int n, int i, int j)
			{
			int a, b;
			if(i!=j)
				{
				if(i<j)
					{
					a=i;
					b=j;
					}
				if(i>j)
					{
					a=j;
					b=i;
					}
				int m = 1/2*((n-1)+(n-a))*a + b-a-1;
				return m;
				}
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
	int n = 15, c = 3;
	APIERRORSETUP;
	pLSenv pEnv;
	pLSmodel pModel;
	char MY_LICENSE_KEY[1024];

	/*	>>> Step 1 <<< Create a model in the environment.	*/
//	nErrorCode = LSloadLicenseString("d:/usr/Lindoapi/7.0/license/lndapi70.lic", MY_LICENSE_KEY);
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
		int num_constants = c + 0.5*n*(n-1) + 1; //
		int num_constraints = n; // for every node, e.g. x0+x1+x3=1

		/* Direction of optimization */
		int objsense[1];
		/* Constraint type */
		char ctype[num_constants];
		/* Variable type, C = continuous, B = binary */
		char vtype[num_variables];
		/* index of instructions */
		int code[100000];
		/* Variable index */
		int varndx[num_variables];
		/* Double Precision constants in the model, c + 0.5*n*(n-1) + 1 */
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

		/* Indes for Variables */
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

		//	Double Precision constants in the model
		float Gtilde[] = {0.001775,0.01668,0.01698,0.02993,0.00483,0.006406,0.00163,0.00468,0.003023,0.001496,0.003921,0.009819,0.001105,0.06295,0.0009249,0.007844,0.1918,0.003905,0.005581           ,0.0009435,0.003764,0.00167,0.003087,0.004185,4.6,0.05824,0.0006921,0.02702,0.1006,0.01232,0.001898,0.005104,0.001757,0.01091,0.002478,0.002152,0.001433                    ,0.01741,0.1794,0.009238,0.009035,0.0009386,0.001596,0.03027,0.001231,0.001919,0.004928,0.001862,0.008365,0.001495,0.005065,0.005709,0.01032,0.03397                           ,0.1078,0.1106,0.2998,0.003824,0.007157,0.007503,0.005835,0.0127,0.1056,0.00514,0.06585,0.003852,0.1309,0.01014,0.05,0.04381                                ,0.02609,0.03015,0.001663,0.003079,0.01723,0.002251,0.00404,0.01446,0.002877,0.02582,0.002235,0.01499,0.01005,0.02371,0.0683                                           ,1.834,0.003399,0.003854,0.0104,0.008334,0.008335,0.03063,0.01871,0.01117,0.01198,0.05238,0.003326,1.348,0.09412                                                  ,0.004554,0.005754,0.007445,0.01035,0.01347,0.07379,0.01241,0.01915,0.008522,0.1506,0.004575,0.2038,0.05172                                                            ,0.03584,0.0007208,0.04577,0.09229,0.01223,0.002262,0.004609,0.002107,0.01144,0.002166,0.002362,0.001511                                                                       ,0.0009159,0.008644,0.1426,0.02995,0.001503,0.01925,0.001328,0.01966,0.008497,0.002595,0.001974                                                                                  ,0.001171,0.001269,0.002729,0.003467,0.002728,0.002724,0.003136,0.001647,0.0174,0.06899                                                                                         ,0.03481,0.01479,0.008395,0.003996,0.007791,0.01862,0.001616,0.00558,0.002758                                                                                           ,0.09649,0.003083,0.01775,0.002664,0.07196,0.005277,0.005038,0.003159                                                                                                    ,0.004373,0.0858,0.003479,4.417,0.01025,0.0147,0.009153                                                                                                                    ,0.001963,4.206,0.005988,0.0009088,0.02195,0.007697                                                                                                                       ,0.001612,0.04847,0.07378,0.007186,0.007293                                                                                                                                      ,0.004642,0.0007764,0.01359,0.005465                                                                                                                                   ,0.007472,0.02193,0.0116                                                                                                                                                       ,0.002567,0.003035                                                                                                                                                 ,0.2209
};

		printf ("Gtilde:\n");
	  	for (j=0; j<sizeof(Gtilde)/8; j++)
	  		{
	  		printf ("%g\n", Gtilde[j]);
	  		}
	  	printf ("\n");


		// corresponds to the dimension of pmax
		int pmax[]={6.0, 7.0, 8.0, 9.0, 10.0};

		printf ("power:\n");
	  	for (j=0; j<sizeof(pmax)/4; j++)
	  		{
	  		printf ("%g\n",pmax[j]);
	  		}
	  	printf ("\n");


		// import Gtilde(gains) into numvals
		for(i=0; i< 0.5*n*(n-1); i++)
			{
			numval[i] = Gtilde[i];
			}

		// import power-levels into numvals
		k = 0;
		for(i = 0.5*n*(n-1); i < 0.5*n*(n-1) + c; i++)
			{
			numval[i] = pmax[k++];
			}
		// import the last constant 1 into numvals
		numval[n*(n-1)/2 + c] = 1;


        
		int varIndex;
		int constantIndex;
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
		 * min \sum x_{i,k}* g_{i,j}* x_{j,k}, j!=i, k\in \mathcal{C}
		 */
		for(i=0; i<n; i++)
			{
			for(j=0; j<n; j++ )
				{
				if( j!= i )
					{
					constantIndex = index_pathlossMatrix2array(n, i, j);
					for(k=0; k<c; k++)
						{
						code[ikod++]= EP_PUSH_VAR;
						code[ikod++]= i*c+k;
						code[ikod++]= EP_PUSH_VAR;
						code[ikod++]= j*c+k;
						code[ikod++]= EP_PUSH_NUM;
						code[ikod++]= constantIndex;
						code[ikod++]= EP_MULTIPLY;
						code[ikod++]= EP_MULTIPLY;
						if (i!=0 || j!=1 || k!=0)
							{
							code[ikod++]= EP_PLUS;
							}
						}
					}
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
			for(j=0; j<c; j++)	//dealing with channels
				{
				code[ikod++]= EP_PUSH_VAR;
				code[ikod++]= i*c+j;
				}
			for(k=1; k<c; k++)
				{
				code[ikod++]= EP_PLUS;
				}
			code[ikod++]= EP_PUSH_NUM;
			code[ikod++]= 0.5*n*(n-1)+c;
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
		//nLinearz = 0;
		//nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_LINEARZ, nLinearz);
		//APIERRORCHECK;

//		/* Set up automatic differentiation, before a call to LSloadInstruct.
//			If not specified, the numerical derivative will be applied */
		nAutoDeriv = 2;
		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_AUTODERIV, nAutoDeriv);
		APIERRORCHECK;


//		/*	Should LSsolveMIP is called, uncomment the following codes	*/
//		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_MIP_TOPOPT, LS_METHOD_NLP);	//LS_METHOD_NLP is '4', represeting nonlinear solver
//		APIERRORCHECK;
//		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_MIP_REOPT, LS_METHOD_NLP);
//		APIERRORCHECK;


		/*	Should LSsolveGOP is called, uncomment the following codes	*/
		nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_QUADCHK, 0);
		APIERRORCHECK;

        
		/* Pass the instruction list to problem structure
		* by a call to LSloadInstruct() */
		nErrorCode = LSloadInstruct (pModel, ncons, nobjs, nvars, nnums, objsense, ctype, vtype, code, lsize, varndx, numval, varval, objs_beg, objs_length, cons_beg, cons_length, lwrbnd, uprbnd);
		APIERRORCHECK;        
        
		}


	/* >>> Step 5 <<< Perform the optimization	*/
    char *pszFname="autoGenerated_mustafa1.mpi";
    nErrorCode = LSwriteMPIFile( pModel, pszFname);
    APIERRORCHECK;
    printf("%s\n", pszFname);
    
	nErrorCode = LSsolveGOP(pModel, NULL); //P491 manual
    //nErrorCode = LSsolveMIP(pModel, NULL); //P491 manual
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
		printf("\n");

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













