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
char ctype[25];
/* Variable type, C = continuous, B = binary */
char vtype[15];
/* index of instructions */
int code[2000];
/* Variable index */
int varndx[15];
/* Double Precision constants in the model, 2 power+10 channel gain + 1 x */
double numval[13];
/* Starting point of variables */
double varval[15];
/* index of start points and lengthes of object and constraints */
int objs_beg[1], objs_length[1], cons_beg[25], cons_length[25];
/* lower and upper bound */
double lwrbnd[15], uprbnd[15];
/* linearization level, automatic differentiation*/
int nLinearz, nAutoDeriv;
int ikod, iobj, icon;
/* Number of constraints */
ncons = 25;
/* Number of objectives */
nobjs = 1;
/* Number of variables */
nvars = 15;
/* Number of real number constants */
nnums = 13;
/* Variable index  */

varndx[0]=0;	//x1
varndx[1]=1;	//x2
varndx[2]=2;	//x3
varndx[3]=3;	//x4
varndx[4]=4;	//x5
varndx[5]=5;	//x6
varndx[6]=6;	//x7
varndx[7]=7;	//x8
varndx[8]=8;	//x9
varndx[9]=9;	//x10
varndx[10]=10;	//v1
varndx[11]=11;	//v2
varndx[12]=12;	//v3
varndx[13]=13;	//v4
varndx[14]=14;	//v5

/* Lower bounds of variables */
lwrbnd[0]=0;
lwrbnd[1]=0;
lwrbnd[2]=0;
lwrbnd[3]=0;
lwrbnd[4]=0;
lwrbnd[5]=0;
lwrbnd[6]=0;
lwrbnd[7]=0;
lwrbnd[8]=0;
lwrbnd[9]=0;
lwrbnd[10]=6;
lwrbnd[11]=6;
lwrbnd[12]=6;
lwrbnd[13]=6;
lwrbnd[14]=6;
/* Upper bounds of variables */
uprbnd[0]=1;
uprbnd[1]=1;
uprbnd[2]=1;
uprbnd[3]=1;
uprbnd[4]=1;
uprbnd[5]=1;
uprbnd[6]=1;
uprbnd[7]=1;
uprbnd[8]=1;
uprbnd[9]=1;
uprbnd[10]=7;
uprbnd[11]=7;
uprbnd[12]=7;
uprbnd[13]=7;
uprbnd[14]=7;
// Starting point of variables
varval[0] = 1;
varval[1] = 0;
varval[2] = 1;
varval[3] = 0;
varval[4] = 1;
varval[5] = 0;
varval[6] = 1;
varval[7] = 0;
varval[8] = 1;
varval[9] = 1;

varval[10]=6;
varval[11]=6;
varval[12]=6;
varval[13]=6;
varval[14]=6;

/* Variable type, C= continuous, B = binary */
vtype[0] = 'B';
vtype[1] = 'B';
vtype[2] = 'B';
vtype[3] = 'B';
vtype[4] = 'B';
vtype[5] = 'B';
vtype[6] = 'B';
vtype[7] = 'B';
vtype[8] = 'B';
vtype[9] = 'B';
vtype[10] = 'I';
vtype[11] = 'I';
vtype[12] = 'I';
vtype[13] = 'I';
vtype[14] = 'I';

/* Double Precision constants in the model */
numval[0]= 6;
numval[1]= 7;
/*Gtilde =

         0    0.0011    0.0035    0.0030    0.4332
    0.0011         0    0.0060    0.0041    0.0017
    0.0035    0.0060         0    0.0019    0.0059
    0.0030    0.0041    0.0019         0    0.0047
    0.4332    0.0017    0.0059    0.0047         0


         0    0.0123    0.0074    0.0037    0.3136
    0.0123         0    0.0014    0.0012    0.0215
    0.0074    0.0014         0    0.0015    0.0035
    0.0037    0.0012    0.0015         0    0.0044
    0.3136    0.0215    0.0035    0.0044         0
*/

numval[2]=0.0823;	//21, 12
numval[3]=0.0674;	//31, 13
numval[4]=0.1137;	//41, 14
numval[5]=0.3136;	//51, 15
numval[6]=0.1014;	//32, 23
numval[7]=0.0912;	//42, 24
numval[8]=0.1215;	//52, 25
numval[9]=0.0615;	//43, 34
numval[10]=0.2035;	//53, 35
numval[11]=0.0944;	//54, 45
numval[12]=1;	

/* Count for instruction code */
ikod = 0;
/* Count for objective row */
iobj = 0;
/* Count for constraint row */
icon = 0;
/*
* Instruction code of the objective:
*
* max   	(Pmax*X2*h21*(X1*X2') + Pmax*X3*h31*(X1*X3') + Pmax*X4*h41*(X1*X4') + Pmax*X5*h51*(X1*X5'))/Pmax*X1 +
*		(Pmax*X1*h12*(X1*X2') + Pmax*X3*h32*(X3*X2') + Pmax*X4*h42*(X4*X2') + Pmax*X5*h52*(X5*X2'))/Pmax*X2 +
*		(Pmax*X1*h13*(X1*X3') + Pmax*X2*h23*(X2*X3') + Pmax*X4*h43*(X4*X3') + Pmax*X5*h53*(X5*X3'))/Pmax*X3 +
*		(Pmax*X1*h14*(X1*X4') + Pmax*X2*h24*(X2*X4') + Pmax*X3*h34*(X3*X4') + Pmax*X5*h54*(X5*X4'))/Pmax*X4 +
*		(Pmax*X1*h15*(X1*X5') + Pmax*X2*h25*(X2*X5') + Pmax*X3*h35*(X3*X5') + Pmax*X4*h45*(X4*X5'))/Pmax*X5

*	After introducing aucxiliary variable v1, v2, v3, v4 and v5, the object function becomes posynomial: (notice X1 is vector and x1, x2 are elements of it.)

	v2*h21*x1*x3/v1 + v2*h21*x2*x4/v1 + v3*h31*x1*x5/v1 + v3*h31*x2*x6/v1 + v4*h41*x1*x7/v1 + v4*h41*x2*x8/v1 + v5*h51*x1*x9/v1 + v5*h51*x2*x10/v1 +
	v1*h12*x1*x3/v2 + v1*h12*x2*x4/v2 + v3*h32*x3*x5/v2 + v3*h32*x4*x6/v2 + v4*h42*x3*x7/v2 + v4*h42*x4*x8/v2 + v5*h52*x3*x9/v2 + v5*h52*x4*x10/v2 +
	v1*h13*x1*x5/v3 + v1*h13*x2*x6/v3 + v2*h23*x3*x5/v3 + v2*h23*x4*x6/v3 + v4*h43*x5*x7/v3 + v4*h43*x6*x8/v3 + v5*h53*x5*x9/v3 + v5*h53*x6*x10/v3 +
	v1*h14*x1*x7/v4 + v1*h14*x2*x8/v4 + v2*h24*x3*x7/v4 + v2*h24*x4*x8/v4 + v3*h34*x5*x7/v4 + v3*h34*x6*x8/v4 + v5*h54*x9*x7/v4 + v5*h54*x10*x8/v4 +
	v1*h15*x1*x9/v5 + v1*h15*x2*x10/v5 + v2*h25*x3*x9/v5 + v2*h25*x4*x10/v5 + v3*h35*x5*x9/v5 + v3*h35*x6*x10/v5 + v4*h45*x7*x9/v5 + v4*h45*x8*x10/v5
*/

/* Direction of optimization */
objsense[iobj]= LS_MIN;
/* Beginning position of objective */
objs_beg[iobj]= ikod;
/* Instruction list code */
/* the 1st line of the obj */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

/* the 2nd line of the obj */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

/* the 3rd line of the obj */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;
/* the 4th line of the obj */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;
/* the 5th line of the obj */
//v1*h15*x1*x9/v5 + v1*h15*x2*x10/v5 + v2*h25*x3*x9/v5 + v2*h25*x4*x10/v5 + v3*h35*x5*x9/v5 + v3*h35*x6*x10/v5 + v4*h45*x7*x9/v5 + v4*h45*x8*x10/v5
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;
// v3*h35*x5*x9/v5 + v3*h35*x6*x10/v5 + v4*h45*x7*x9/v5 + v4*h45*x8*x10/v5

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;
/* Length of objective */
objs_length[iobj] = ikod - objs_beg[iobj];


//**Here starts the constraints!**//

/* Constraints:
v1 == 6x1 + 7x2;

v2 == 6x3 + 7x4;

v3 == 6x5 + 7x6;

v4 == 6x7 + 7x8;

v5 == 6x9 + 7x10;

x1 + x2 <= 1;
max(x1, x2) >=1;

x3 + x4 <= 1;
max(x3, x4) >=1;

x5 + x6 <= 1;
max(x5, x6) >=1;

x7 + x8 <= 1;
max(x7, x8) >=1;

x9 + x10 <= 1;
max(x9, x10) >=1;
*/

//**          v1		**//
/*

/*
* Instruction code of constraint 2:
* v1 - 6x1 - 7x2 == 0;
*/
/* Constraint type */
ctype[icon]= 'E';
/* greater than or equal to */
/* Beginning position of constraint 2 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
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
* Instruction code of constraint 5:
* v2 - 6x3 - 7x4 == 0;
*/
/* Constraint type */
ctype[icon]= 'E';
/* greater than or equal to */
/* Beginning position of constraint 5 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 5 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


//**          v3		**//
/*
/*
* Instruction code of constraint 8:
* v3 - 6x5 - 7x6 == 0;
*/
/* Constraint type */
ctype[icon]= 'E';
/* greater than or equal to */
/* Beginning position of constraint 8 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 8 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          v4		**//
/*
/*
* Instruction code of constraint 11:
* v4 - 6x7 - 7x8 == 0;
*/
/* Constraint type */
ctype[icon]= 'E';
/* greater than or equal to */
/* Beginning position of constraint 11 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 11 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          v5		**//
/*
/*
* Instruction code of constraint 14:
* v5 - 6x9 - 7x10 == 0;
*/
/* Constraint type */
ctype[icon]= 'E';
/* greater than or equal to */
/* Beginning position of constraint 14 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
/* Length of constraint 14 */
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
code[ikod++]= 12;
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
code[ikod++]= 12;
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
code[ikod++]= 12;
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
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 18 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          X3		**//
/*
* Instruction code of constraint 19:
* x5 + x6 - 1 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 19 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 19 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 20:
* max (x5, x6) -1 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 20 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MAX;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 20 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          X4		**//
/*
* Instruction code of constraint 21:
* x7 + x8 - 1 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 21 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 21 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 22:
* max (x5, x6) -1 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 22 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MAX;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 22 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

//**          X5		**//
/*
* Instruction code of constraint 23:
* x9 + x10 - 1 <= 0;
*/
/* Constraint type */
ctype[icon]= 'L';
/* greater than or equal to */
/* Beginning position of constraint 23 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 23 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* Instruction code of constraint 24:
* max (x5, x6) -1 >= 0;
*/
/* Constraint type */
ctype[icon]= 'G';
/* greater than or equal to */
/* Beginning position of constraint 24 */
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MAX;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MINUS;
/* Length of constraint 24 */
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/* Total number of items in the instruction list */
lsize = ikod;
/* Set linearization level, before a call to  .
* If not specified, the solver will decide */
/* linearization option is turned on! */
nLinearz = 0;
nErrorCode = LSsetModelIntParameter (pModel, LS_IPARAM_NLP_LINEARZ, nLinearz);
APIERRORCHECK;

/* Set up automatic differentiation, before a call to LSloadInstruct.
	If not specified, the numerical derivative will be applied */
/* The numerical derivative is turned off */
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
double objval, primal[20];
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
printf("x[5] = %f \n",primal[4]);
printf("x[6] = %f \n",primal[5]);
printf("x[7] = %f \n",primal[6]);
printf("x[8] = %f \n",primal[7]);
printf("x[9] = %f \n",primal[8]);
printf("x[10] = %f \n",primal[9]);
printf("v[1] = %f \n",primal[10]);
printf("v[2] = %f \n",primal[11]);
printf("v[3] = %f \n",primal[12]);
printf("v[4] = %f \n",primal[13]);
printf("v[5] = %f \n",primal[14]);

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


