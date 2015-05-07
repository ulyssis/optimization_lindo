/*Mini is the name means the obj is shorter for debugging.
/*
minmize:	sum(i\in n) f_i/p_i
s.t. 
		for any user, one channel is choosed and the transmission power is accordingly decided.
*/

#include <stdio.h>
#include <stdlib.h>
/* LINDO API header file */
#include "/home/li/work/tools/lindo/lindoapi/include/lindo.h"
//#include "lindo.h"
/* Define a macro to declare variables for error checking */
#define APIERRORSETUP \
int nErrorCode; \
char cErrorMessage[LS_MAX_ERROR_MESSAGE_LENGTH] \


/* Define a macro to do our error checking */
/* this is a macro! */
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

/* main entry point */
int main()
{
APIERRORSETUP;
pLSenv pEnv;
pLSmodel pModel;
char MY_LICENSE_KEY[1024];
/**********************************************************
* Step 1: Create a model in the environment.
**********************************************************/
nErrorCode = LSloadLicenseString("../../../license/lndapi60.lic",MY_LICENSE_KEY);
if ( nErrorCode != LSERR_NO_ERROR)
{
   printf( "Failed to load license key (error %d)\n",nErrorCode);
   exit( 1);
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
{
/* >>>> Step 3 <<< Set up the instruction list of the model. */
int nobjs, ncons, nvars, nnums, lsize;

/* Direction of optimization */
int objsense[1];		
/* Constraint type */
char ctype[10];
/* Variable type, C = continuous, B = binary */
char vtype[6];
/* index of instructions */
int code[1000];
/* Variable index */
int varndx[6];
/* Double Precision constants in the model, 2 power+10 channel gain + 1 x */
double numval[4];
/* Starting point of variables */
double varval[6];
/* index of start points and lengthes of object and constraints */
int objs_beg[1], objs_length[1], cons_beg[10], cons_length[10];
/* lower and upper bound */
double lwrbnd[6], uprbnd[6];
/* linearization level, automatic differentiation*/
int nLinearz, nAutoDeriv;
int ikod, iobj, icon;
/* Number of constraints */
ncons = 10;
/* Number of objectives */
nobjs = 1;
/* Number of variables */
nvars = 6;
/* Number of real number constants */
nnums = 4;
/* Variable index  */
varndx[0]=0;	//x1
varndx[1]=1;	//x2
varndx[2]=2;	//x3
varndx[3]=3;	//x4
varndx[4]=4;	//v1
varndx[5]=5;	//v2

/* Lower bounds of variables */
lwrbnd[0]=0;
lwrbnd[1]=0;
lwrbnd[2]=0;
lwrbnd[3]=0;
lwrbnd[4]=6;
lwrbnd[5]=6;
/* Upper bounds of variables */
uprbnd[0]=1;
uprbnd[1]=1;
uprbnd[2]=1;
uprbnd[3]=1;
uprbnd[4]=7;
uprbnd[5]=7;
// Starting point of variables
/*varval[0] = 1;
varval[1] = 0;
varval[2] = 1;
varval[3] = 0;

varval[4] = 1;
varval[5] = 0;
varval[6] = 1;
varval[7] = 0;
varval[8] = 1;
varval[9] = 0;

varval[10]=6;
varval[11]=6;
varval[12]=6;
varval[13]=6;
varval[14]=6;*/

/* Variable type, C= continuous, B = binary */
vtype[0] = 'B';
vtype[1] = 'B';
vtype[2] = 'B';
vtype[3] = 'B';
vtype[4] = 'C';
vtype[5] = 'C';

/* Double Precision constants in the model */
numval[0]= 7;
numval[1]= 6;
numval[2]= 0.5;	//21, 12
numval[3]= 1;

/* Count for instruction code */
ikod = 0;
/* Count for objective row */
iobj = 0;
/* Count for constraint row */
icon = 0;
/*
* Instruction code of the objective:

* max   	Pmax*X2*h21*(X1*X2')/Pmax*X1 + Pmax*X1*h12*(X1*X2')/Pmax*X2
	
	=	v2*h21*x1*x3/v1 + v2*h21*x2*x4/v1 + v1*h12*x1*x3/v2 + v1*h12*x2*x4/v2
*/

/* Direction of optimization */
objsense[iobj]= LS_MIN;
/* Beginning position of objective */
objs_beg[iobj]= ikod;
/* Instruction list code */
/* the 1st line of the obj */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_DIVIDE;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;
/* Length of objective */
objs_length[iobj] = ikod - objs_beg[iobj];


//**Here starts the constraints!**//

/* Constraints:
v1 >= 6x1;
v1 >= 7x2;
v1 <= 6x1 + 7x2;

v2 >= 6x3;
v2 >= 7x4;
v2 <= 6x3 + 7x4;

x1 + x2 <= 1;
max(x1, x2) >=1;

x3 + x4 <= 1;
max(x3, x4) >=1;
*/

//**          v1		**//
/*
* Instruction code of constraint 0:
* v1 - 6x1 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 0 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 0 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 1:
* v1 - 7x2 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 1 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 1 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 2:
* v1 - 6x1 - 7x2 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 2 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 2 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          v2		**//
/*
* Instruction code of constraint 3:
* v2 - 6x3 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 3 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 3 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 4:
* v2 - 7x4 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 4 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 4 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 5:
* v2 - 6x3 - 7x4 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 5 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 5 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


//**          X1		**//
/*
* Instruction code of constraint 15:
* x1 + x2 - 1 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 15 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MINUS;
/* Length of constraint 15 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 16:
* max (x1, x2) -1 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 16 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_MAX;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MINUS;
/* Length of constraint 16 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          X2		**//
/*
* Instruction code of constraint 17:
* x3 + x4 - 1 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 17 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MINUS;
/* Length of constraint 17 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 18:
* max (x3, x4) -1 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 18 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MAX;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MINUS;
/* Length of constraint 18 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/* Total number of items in the instruction list */
lsize = ikod;
/* Set linearization level, before a call to  .
* If not specified, the solver will decide */
/* linearization option is turned on! */
nLinearz = 1;
nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_LINEARZ, nLinearz);
APIERRORCHECK;

/* Set up automatic differentiation, before a call to LSloadInstruct.
	If not specified, the numerical derivative will be applied */
nAutoDeriv = 1;
nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_AUTODERIV, nAutoDeriv);
APIERRORCHECK;

/* Pass the instruction list to problem structure
* by a call to LSloadInstruct() */
nErrorCode = LSloadInstruct (pModel, ncons, nobjs, nvars, nnums, objsense, ctype, vtype, code, lsize, varndx, numval, varval, objs_beg, objs_length, cons_beg, cons_length, lwrbnd, uprbnd);
APIERRORCHECK;
}

/*
* >>> Step 5 <<< Perform the optimization using the MIP solver
*/
nErrorCode = LSsolveMIP(pModel, NULL); //P491 manual
APIERRORCHECK;
{
int nLinearity;
double objval, primal[15];
/* Get the optimization result */
LSgetInfo(pModel, LS_DINFO_MIP_OBJ, &objval);
APIERRORCHECK;
LSgetMIPPrimalSolution( pModel, primal);
APIERRORCHECK;
printf("\n\nObjective = %f \n",objval);
printf("x[1] = %f \n",primal[0]);
printf("x[2] = %f \n",primal[1]);
printf("x[3] = %f \n",primal[2]);
printf("x[4] = %f \n",primal[3]);
printf("v[1] = %f \n",primal[4]);
printf("v[2] = %f \n",primal[5]);


/* Get the linearity of the solved model, to check
the characteristic of the solved model */

nErrorCode = LSgetModelIntParameter (pModel, LS_IPARAM_NLP_LINEARITY, &nLinearity);
//APIERRORCHECK;
/* Report the status of solution */
if (nLinearity)
      printf("\nModel has been completely linearized.\
              \nSolution Status: Globally Optimal\n");
      else
      printf("\nModel is nonlinear.\
              \nSolution Status: Locally Optimal\n\n");
}

/* >>> Step 7 <<< Delete the LINDO environment */
  LSdeleteEnv(&pEnv);

 /* Wait until user presses the Enter key */
  printf("Press <Enter> ...");
  getchar();
}


