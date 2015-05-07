/*
minmize:	sum(i\in n) f_i/p_i
s.t. 
		for any user, one channel is choosed and the transmission power is accordingly decided.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
	/******************************
	*read data from Gtilde.txt to f
	*******************************/
/*	float *f;
	float g[25];
	FILE *pFile;
	pFile = fopen ("/home/li/work/tools/lindo/lindoapi/samples/c/dica/Gtilde.txt","r");
	//read data from file to f
	int i;	
	for(i = 0; i < 25; i++) 
	{
	fscanf (pFile, "%f", f);
	g[i]=*(f++);
	}
	fclose (pFile);
*/	
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
char ctype[65];
/* Variable type, C = continuous, B = binary */
char vtype[85];
/* index of instructions */
int code[4000];
/* Variable index */
int varndx[85];
/* Double Precision constants in the model, 3 power+45 channel gain + 1 x */
double numval[49];
/* Starting point of variables */
double varval[85];
/* index of start points and lengthes of object and constraints */
int objs_beg[1], objs_length[1], cons_beg[65], cons_length[65];
/* lower and upper bound */
double lwrbnd[85], uprbnd[85];
/* linearization level, automatic differentiation*/
int nLinearz, nAutoDeriv;
int ikod, iobj, icon;
//nErrorCode = LSloadInstruct (pModel, ncons, nobjs, nvars, nnums, objsense, ctype, vtype, code, lsize, varndx, numval, varval, objs_beg, objs_length, cons_beg, cons_length, lwrbnd, uprbnd);
/* Number of constraints */
ncons = 65;
/* Number of objectives */
nobjs = 1;
/* Number of variables */
nvars = 85;
/* Number of real number constants */
nnums = 49;
/* Variable index  */
/*
varndx[0]=0;	//x0
varndx[1]=1;	//x1
varndx[2]=2;	//x2
varndx[3]=3;	//x3
varndx[4]=4;	//x4
varndx[5]=5;	//x5
varndx[6]=6;	//x6
varndx[7]=7;	//x7
varndx[8]=8;	//x8
varndx[9]=9;	//x9
varndx[10]=10;	//x10
varndx[11]=11;	//x11
varndx[12]=12;	//x12
varndx[13]=13;	//x13
varndx[14]=14;	//x14
varndx[15]=15;	//x15
varndx[16]=16;	//x16
varndx[17]=17;	//x17
varndx[18]=18;	//x18
varndx[19]=19;	//x19
varndx[20]=20;	//x20
varndx[21]=21;	//x21
varndx[22]=22;	//x22
varndx[23]=23;	//X23
varndx[24]=24;	//X24
varndx[25]=25;	//X25
varndx[26]=26;	//X26
varndx[27]=27;	//X27
varndx[28]=28;	//X28
varndx[29]=29;	//X29
--
varndx[30]=30;	//v0
varndx[31]=31;	//v1
varndx[32]=32;	//v2
varndx[33]=33;	//v3
varndx[34]=34;	//v4
varndx[35]=35;	//v5
varndx[36]=36;	//v6
varndx[37]=37;	//v7
varndx[38]=38;	//v8
varndx[39]=39;	//v9
--
#45
varndx[40]=40;  //h01
varndx[41]=41;  //h02
varndx[42]=42;  //h03
varndx[43]=43;  //h04
varndx[44]=44;  //h05
varndx[45]=45;  //h06
varndx[46]=46;  //h07
varndx[47]=47;  //h08
varndx[48]=48;  //h09
--
varndx[49]=49;  //h12
varndx[50]=50;  //h13
varndx[51]=51;  //h14
varndx[52]=52;  //h15
varndx[53]=53;  //h16
varndx[54]=54;  //h17
varndx[55]=55;  //h18
varndx[56]=56;  //h19
--
varndx[57]=57;  //h23
varndx[58]=58;  //h24
varndx[59]=59;  //h25
varndx[60]=60;  //h26
varndx[61]=61;  //h27
varndx[62]=62;  //h28
varndx[63]=63;  //h29
--
varndx[64]=64;  //h34
varndx[65]=65;  //h35
varndx[66]=66;  //h36
varndx[67]=67;  //h37
varndx[68]=68;  //h38
varndx[69]=69;  //h39
--
varndx[70]=70;  //h45
varndx[71]=71;  //h46
varndx[72]=72;  //h47
varndx[73]=73;  //h48
varndx[74]=74;  //h49
--
varndx[75]=75;  //h56
varndx[76]=76;  //h57
varndx[77]=77;  //h58
varndx[78]=78;  //h59
--
varndx[79]=79;  //h67
varndx[80]=80;  //h68
varndx[81]=81;  //h69
--
varndx[82]=82;  //h78
varndx[83]=83;  //h79
--
varndx[84]=84;  //h89

*/
int k;
for(k=0; k<85; k++)
{
varndx[k]=k;
}

/* Lower bounds of variables */
int m;
for(m=0; m < 85; m++)
{
if (m<30|| m > 39)
	lwrbnd[m]=0;
else
	lwrbnd[m]=5;
}

/* Upper bounds of variables */
int n;
for(n=0; n<85; n++)
{
	if (n<30 || n> 39)
		uprbnd[n]=1;
else
	uprbnd[n]=7;
}
// Starting point of variables
/*
varval[0] = 1;
...

varval[31]=7;
...
*/
/* a function, to generate a random number among 0, 1, 2
 * the parameter is used to make different random seed.
 */

int gen_rand(int r)
{
	int iSecret;
	srand ( r+1);
	iSecret = rand() % 3;
	return iSecret;
}

int r, miniIndex;
for(r = 0; r < 30 ; r++)
	{
	if(r%3 == 0)
		{
		int s = r;
		for (s; s<r+3; s++)
			{
			varval[s] = 0;
			}
		miniIndex = gen_rand(r);
		varval[r+miniIndex] = 1;
		}
	}

//varval
printf("varval[0] = %f \t",varval[0]);
printf("varval[1] = %f \t",varval[1]);
printf("varval[2] = %f \n",varval[2]);
printf("varval[3] = %f \t",varval[3]);
printf("varval[4] = %f \t",varval[4]);
printf("varval[5] = %f \n",varval[5]);
printf("varval[6] = %f \t",varval[6]);
printf("varval[7] = %f \t",varval[7]);
printf("varval[8] = %f \n",varval[8]);
printf("varval[9] = %f \t",varval[9]);
printf("varval[10] = %f \t",varval[10]);
printf("varval[11] = %f \n",varval[11]);
printf("varval[12] = %f \t",varval[12]);
printf("varval[13] = %f \t",varval[13]);
printf("varval[14] = %f \n",varval[14]);
printf("varval[15] = %f \t",varval[15]);
printf("varval[16] = %f \t",varval[16]);
printf("varval[17] = %f \n",varval[17]);
printf("varval[18] = %f \t",varval[18]);
printf("varval[19] = %f \t",varval[19]);
printf("varval[20] = %f \n",varval[20]);
printf("varval[21] = %f \t",varval[21]);
printf("varval[22] = %f \t",varval[22]);
printf("varval[23] = %f \n",varval[23]);
printf("varval[24] = %f \t",varval[24]);
printf("varval[25] = %f \t",varval[25]);
printf("varval[26] = %f \n",varval[26]);
printf("varval[27] = %f \t",varval[27]);
printf("varval[28] = %f \t",varval[28]);
printf("varval[29] = %f \n",varval[29]);
/* Variable type, C= continuous, B = binary */
int i;
for(i=0; i<sizeof(vtype); i++)
	{
	if (i<30)
	vtype[i]='B';
	else
	vtype[i]='C';
	}


/* Double Precision constants in the model */
float Gtilde[100] ={
		0,0.00119454,0.003084523,0.001789721,0.001429241,0.002128677,0.0007753891,0.00150268,0.002309735,0.01285306,0.00119454,0,0.0005759919,0.001217331,0.002363097,0.001457377,0.001218153,0.01319125,0.001080645,0.002217608,0.003084523,0.0005759919,0,0.01410499,0.003896949,0.01312689,0.002818726,0.00148611,0.03154341,0.0104927,0.001789721,0.001217331,0.01410499,0,0.07541968,4.456252,0.03485465,0.006324652,1.003294,0.0143797,0.001429241,0.002363097,0.003896949,0.07541968,0,0.1052741,0.08703297,0.02733769,0.0299052,0.01002421,0.002128677,0.001457377,0.01312689,4.456252,0.1052741,0,0.02832995,0.008301594,0.7209691,0.02165002,0.0007753891,0.001218153,0.002818726,0.03485465,0.08703297,0.02832995,0,0.007347204,0.01506862,0.003450986,0.00150268,0.01319125,0.00148611,0.006324652,0.02733769,0.008301594,0.007347204,0,0.004649224,0.006833947,0.002309735,0.001080645,0.03154341,1.003294,0.0299052,0.7209691,0.01506862,0.004649224,0,0.02057808,0.01285306,0.002217608,0.0104927,0.0143797,0.01002421,0.02165002,0.003450986,0.006833947,0.02057808,0
};

numval[0]= 1;
numval[1]= Gtilde[1];	//10, 01
numval[2]= Gtilde[2];	//20, 02
numval[3]= Gtilde[3];	//30, 03
numval[4]= Gtilde[4];	//40, 04
numval[5]= Gtilde[5];	//50, 05
numval[6]= Gtilde[6];	//60, 06
numval[7]= Gtilde[7];	//70, 07
numval[8]= Gtilde[8];	//80, 08
numval[9]= Gtilde[9];	//90, 09
numval[10]= Gtilde[12];	//21, 12
numval[11]= Gtilde[13];	//31, 13
numval[12]= Gtilde[14];	//41, 14
numval[13]= Gtilde[15];	//51, 15
numval[14]= Gtilde[16];	//61, 16
numval[15]= Gtilde[17];	//71, 17
numval[16]= Gtilde[18];	//81, 18
numval[17]= Gtilde[19];	//91, 19
numval[18]= Gtilde[23];	//32, 23
numval[19]= Gtilde[24];	//42, 24
numval[20]= Gtilde[25];	//52, 25
numval[21]= Gtilde[26];	//62, 26
numval[22]= Gtilde[27];	//72, 27
numval[23]= Gtilde[28];	//82, 28
numval[24]= Gtilde[29];	//92, 29
numval[25]= Gtilde[34];	//43, 34
numval[26]= Gtilde[35];	//53, 35
numval[27]= Gtilde[36];	//63, 36
numval[28]= Gtilde[37];	//73, 37
numval[39]= Gtilde[38];	//83, 38
numval[30]= Gtilde[39];	//93, 39
numval[31]= Gtilde[45];	//54, 45
numval[32]= Gtilde[46];	//64, 46
numval[33]= Gtilde[47];	//74, 47
numval[34]= Gtilde[48];	//84, 48
numval[35]= Gtilde[49];	//94, 49
numval[36]= Gtilde[56];	//65, 56
numval[37]= Gtilde[57];	//75, 57
numval[38]= Gtilde[58];	//85, 58
numval[39]= Gtilde[59];	//95, 59
numval[40]= Gtilde[67];	//76, 67
numval[41]= Gtilde[68];	//86, 68
numval[42]= Gtilde[69];	//96, 69
numval[43]= Gtilde[78];	//87, 78
numval[44]= Gtilde[79];	//97, 79
numval[45]= Gtilde[89];	//98, 89
numval[46]= 5;
numval[47]= 6;
numval[48]= 7;

/* Count for instruction code */
ikod = 0;
/* Count for objective row */
iobj = 0;
/* Count for constraint row */
icon = 0;
/*
* Instruction code of the objective:
*
* max
* 		(v1*h10*g10 + v2*h20*g20 + v3*h30*g30 + v4*h40*g40 + v5*h50*g50 + v6*h60*g60 + v7*h70*g70 + v8*h80*g80 + v9*h90*g90)/v0 +
*		(v0*h01*g01 + v2*h21*g21 + v3*h31*g31 + v4*h41*g41 + v5*h51*g51 + v6*h61*g61 + v7*h71*g71 + v8*h81*g81 + v9*h91*g91)/v1 +
*		(v0*h02*g02 + v1*h12*g12 + v3*h32*g32 + v4*h42*g42 + v5*h52*g52 + v6*h62*g62 + v7*h72*g72 + v8*h82*g82 + v9*h92*g92)/v2 +
*		(v0*h03*g03 + v1*h13*g13 + v2*h23*g23 + v4*h43*g43 + v5*h53*g53 + v6*h63*g63 + v7*h73*g73 + v8*h83*g83 + v9*h93*g93)/v3 +
*		(v0*h04*g04 + v1*h14*g14 + v2*h24*g24 + v3*h34*g34 + v5*h54*g54 + v6*h64*g64 + v7*h74*g74 + v8*h84*g84 + v9*h94*g94)/v4 +
*		(v0*h05*g05 + v1*h15*g15 + v2*h25*g25 + v3*h35*g35 + v4*h45*g45 + v6*h65*g65 + v7*h75*g75 + v8*h85*g85 + v9*h95*g95)/v5 +
* 		(v0*h06*g06 + v1*h16*g16 + v2*h26*g26 + v3*h36*g36 + v4*h46*g46 + v5*h56*g56 + v7*h76*g76 + v8*h86*g86 + v9*h96*g96)/v6 +
*		(v0*h07*g07 + v1*h17*g17 + v2*h27*g27 + v3*h37*g37 + v4*h47*g47 + v5*h57*g57 + v6*h67*g67 + v8*h87*g87 + v9*h97*g97)/v7 +
*		(v0*h08*g08 + v1*h18*g18 + v2*h28*g28 + v3*h38*g38 + v4*h48*g48 + v5*h58*g58 + v6*h68*g68 + v7*h78*g78 + v9*h98*g98)/v8 +
*		(v0*h09*g09 + v1*h19*g19 + v2*h29*g29 + v3*h39*g39 + v4*h49*g49 + v5*h59*g59 + v6*h69*g69 + v7*h79*g79 + v8*h89*g89)/v9
*
*s.t.	v0=pmax*X0,...v9=pmax*X9				#10
*		h01=X0*X1',...h89=X8*X9'				#45

/* Direction of optimization */
objsense[iobj]= LS_MIN;
/* Beginning position of objective */
objs_beg[iobj]= ikod;
/* Instruction list code */
/* the 1st line of the obj */
//v0
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 40;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;


code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 41;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 42;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 43;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 44;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 45;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 30;
code[ikod++]= EP_DIVIDE;


// v1
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 40;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 49;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 50;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 51;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 52;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 53;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 54;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 55;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 56;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

//V2
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 41;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 49;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 57;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 58;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 59;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 60;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 61;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 62;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 63;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

//V3
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 42;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 50;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 57;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 64;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 65;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 66;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 67;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 68;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 69;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 30;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

//v4
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 43;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 51;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 58;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 64;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 70;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 31;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 71;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 32;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 72;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 33;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 73;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 34;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 74;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 35;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


//v5
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 44;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 52;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 59;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 65;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 70;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 31;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 75;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 36;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 76;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 37;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 77;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 38;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 78;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 39;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

//v6
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 45;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 53;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 60;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 66;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 71;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 32;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 75;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 36;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 79;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 40;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 80;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 41;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 81;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 42;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

//v7
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 54;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 61;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 67;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 72;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 33;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 76;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 37;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 79;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 40;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 82;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 43;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 83;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 44;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


//v8
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 55;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 62;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 68;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 73;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 34;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 77;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 38;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 80;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 41;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 82;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 43;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 84;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 45;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


//v9
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 56;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 63;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 69;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 30;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 74;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 35;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 78;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 39;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 81;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 42;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 83;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 44;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;

code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 84;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 45;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_DIVIDE;
code[ikod++]= EP_PLUS;


/* Length of objective */
objs_length[iobj] = ikod - objs_beg[iobj];


//**Here starts the constraints!**//
/*
v0 == 5x0+ 6x1 + 7x2;
v1 == 5x3+ 6x4 + 7x5;
v2 == 5x6+ 6x7 + 7x8;
v3 == 5x9+ 6x10 + 7x11;
v4 == 5x12+ 6x13 + 7x14;
v5 == 5x15+ 6x16 + 7x17;
v6 == 5x18+ 6x19 + 7x20;
v7 == 5x21+ 6x22 + 7x23;
v8 == 5x24+ 6x25 + 7x26;
v9 == 5x27+ 6x28 + 7x29;

1 == x0+ x1 + x2;
1 == x3+ x4 + x5;
1 == x6+ x7 + x8;
1 == x9+ x10 + x11;
1 == x12+ x13 + x14;
1 == x15+ x16 + x17;
1 == x18+ x19 + x20;
1 == x21+ x22 + x23;
1 == x24+ x25 + x26;
1 == x27+ x28 + x19;
--
h01 == x0*x3 + x1*x4 +x2*x5;
h02 == x0*x6 + x1*x7 +x2*x8;
h03 == x0*x9 + x1*x10 +x2*x11;
h04 == x0*x12 + x1*x13 +x2*x14;
h05 == x0*x15 + x1*x16 +x2*x17;
h06 == x0*x18 + x1*x19 +x2*x20;
h07 == x0*x21 + x1*x22 +x2*x23;
h08 == x0*x24 + x1*x25 +x2*x26;
h09 == x0*x27 + x1*x28 +x2*x29;
--
h12 == x3*x6 + x4*x7 + x5*x8;
h13 == x3*x9 + x4*x10 + x5*x11;
h14 == x3*x12 + x4*x13 + x5*x14;
h15 == x3*x15 + x4*x16 + x5*x17;
h16 == x3*x18 + x4*x19 + x5*x20;
h17 == x3*x21 + x4*x22 + x5*x23;
h18 == x3*x24 + x4*x25 + x5*x26;
h19 == x3*x27 + x4*x28 + x5*x29;
--
h23 ==x6*x9 + x7*x10 * x8*x11;
h24 ==x6*x12 + x7*x13 * x8*x14;
h25 ==x6*x15 + x7*x16 * x8*x17;
h26 ==x6*x18 + x7*x19 * x8*x20;
h27 ==x6*x21 + x7*x22 * x8*x23;
h28 ==x6*x24 + x7*x25 * x8*x26;
h29 ==x6*x27 + x7*x28 * x8*x29;
--
h34 ==x9*x12 + x10*x13 * x11*x14;
h35 ==x9*x15 + x10*x16 * x11*x17;
h36 ==x9*x18 + x10*x19 * x11*x20;
h37 ==x9*x21 + x10*x22 * x11*x23;
h38 ==x9*x24 + x10*x25 * x11*x26;
h39 ==x9*x27 + x10*x28 * x11*x29;
--
h45 ==x12*x15 + x13*x16 * x14*x17;
h46 ==x12*x18 + x13*x19 * x14*x20;
h47 ==x12*x21 + x13*x22 * x14*x23;
h48 ==x12*x24 + x13*x25 * x14*x26;
h49 ==x12*x27 + x13*x28 * x14*x29;
--
h56 ==x15*x18 + x16*x19 * x17*x20;
h57 ==x15*x21 + x16*x22 * x17*x23;
h58 ==x15*x24 + x16*x25 * x17*x26;
h59 ==x15*x27 + x16*x28 * x17*x29;
--
h67 ==x18*x21 + x19*x22 * x20*x23;
h68 ==x18*x24 + x19*x25 * x20*x26;
h69 ==x18*x27 + x19*x28 * x20*x29;
--
h78 ==x21*x24 + x22*x25 * x23*x26;
h79 ==x21*x27 + x22*x28 * x23*x29;
--
h89 ==x24*x27 + x25*x28 * x26*x29;
--

/*
 * v0 == 5x0+ 6x1 + 7x2;
v1 == 5x3+ 6x4 + 7x5;
v2 == 5x6+ 6x7 + 7x8;
v3 == 5x9+ 6x10 + 7x11;
v4 == 5x12+ 6x13 + 7x14;
v5 == 5x15+ 6x16 + 7x17;
v6 == 5x18+ 6x19 + 7x20;
v7 == 5x21+ 6x22 + 7x23;
v8 == 5x24+ 6x25 + 7x26;
v9 == 5x27+ 6x28 + 7x29;
 */

/*
* v0 - 5x0 - 6x1 - 7x2 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 30;

code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* v1 - 5x3 - 6x4 - 7x5 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 31;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* v2 - 5x6 - 6x7 - 7x8 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 32;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* v3 - 5x9 - 6x10 - 7x11 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 33;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* v4 - 5x12 - 6x13 - 7x14 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 34;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* v5 - 5x15 - 6x16 - 7x17 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 35;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;



/*
* v6 - 5x18 - 6x19 - 7x20 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 36;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* v7 - 5x21 - 6x22 - 7x23 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 37;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* v8 - 5x24 - 6x25 - 7x26 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 38;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;



/*
* v9 - 5x27 - 6x28 - 7x29 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 39;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 46;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 47;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 48;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_MINUS;

cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;



/*---------------------------------
  * 1 == x0+ x1 + x2;
1 == x3+ x4 + x5;
1 == x6+ x7 + x8;
1 == x9+ x10 + x11;
1 == x12+ x13 + x14;
1 == x15+ x16 + x17;
1 == x18+ x19 + x20;
1 == x21+ x22 + x23;
1 == x24+ x25 + x26;
1 == x27+ x28 + x19;
  */
/*
* x0 + x1 + x2 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3 + x4 + x5 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6 + x7 + x8 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x9 + x10 + x11 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x12 + x13 + x14 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x15 + x16 + x17 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x18 + x19 + x20 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x21 + x22 + x23 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x24 + x25 + x26 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x27 + x28 + x29 -1 == 0;
/* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_PUSH_NUM;
code[ikod++]= 0;
code[ikod++]= EP_MINUS;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PLUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;
/*
 * h01 == x0*x3 + x1*x4 +x2*x5;
h02 == x0*x6 + x1*x7 +x2*x8;
h03 == x0*x9 + x1*x10 +x2*x11;
h04 == x0*x12 + x1*x13 +x2*x14;
h05 == x0*x15 + x1*x16 +x2*x17;
h06 == x0*x18 + x1*x19 +x2*x20;
h07 == x0*x21 + x1*x22 +x2*x23;
h08 == x0*x24 + x1*x25 +x2*x26;
h09 == x0*x27 + x1*x28 +x2*x29;
 */

/*
* x0*x3 + x1*x4 +x2*x5 - h01 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 40;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x6 + x1*x7 +x2*x8 - h02 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 41;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x9 + x1*x10 +x2*x11 -h03 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 42;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x12 + x1*x13 +x2*x14 -h04 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 43;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x15 + x1*x16 +x2*x17 -h05 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 44;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x18 + x1*x19 +x2*x20 -h06 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 45;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x21 + x1*x22 +x2*x23 -h07 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 46;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x24 + x1*x25 +x2*x26 -h08 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 47;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x0*x27 + x1*x28 +x2*x29 -h09 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 0;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 1;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 2;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 48;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
 * h12 == x3*x6 + x4*x7 + x5*x8;
h13 == x3*x9 + x4*x10 + x5*x11;
h14 == x3*x12 + x4*x13 + x5*x14;
h15 == x3*x15 + x4*x16 + x5*x17;
h16 == x3*x18 + x4*x19 + x5*x20;
h17 == x3*x21 + x4*x22 + x5*x23;
h18 == x3*x24 + x4*x25 + x5*x26;
h19 == x3*x27 + x4*x28 + x5*x29;
 */
/*
* x3*x6 + x4*x7 + x5*x8 -h12 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 49;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x9 + x4*x10 + x5*x11 -h13== 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 50;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x12 + x4*x13 + x5*x14 - h14== 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 51;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x15 + x4*x16 + x5*x17 - h15 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 52;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x18 + x4*x19 + x5*x20 - h16 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 53;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x21 + x4*x22 + x5*x23 - h17 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 54;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x24 + x4*x25 + x5*x26 - h18 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 55;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x3*x27 + x4*x28 + x5*x29 - h19 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 3;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 4;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 5;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 56;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
 * h23 ==x6*x9 + x7*x10 * x8*x11;
h24 ==x6*x12 + x7*x13 * x8*x14;
h25 ==x6*x15 + x7*x16 * x8*x17;
h26 ==x6*x18 + x7*x19 * x8*x20;
h27 ==x6*x21 + x7*x22 * x8*x23;
h28 ==x6*x24 + x7*x25 * x8*x26;
h29 ==x6*x27 + x7*x28 * x8*x29;
 */
/*
* x6*x9 + x7*x10 * x8*x11 - h23 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 57;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6*x12 + x7*x13 * x8*x14 - h24 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 58;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6*x15 + x7*x16 * x8*x17 - h25 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 59;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6*x18 + x7*x19 * x8*x20 - h26 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 60;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6*x21 + x7*x22 * x8*x23 - h27 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 61;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6*x24 + x7*x25 * x8*x26 - h28 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 62;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x6*x27 + x7*x28 * x8*x29 - h29 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 6;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 7;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 8;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 63;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
 * h34 ==x9*x12 + x10*x13 * x11*x14;
h35 ==x9*x15 + x10*x16 * x11*x17;
h36 ==x9*x18 + x10*x19 * x11*x20;
h37 ==x9*x21 + x10*x22 * x11*x23;
h38 ==x9*x24 + x10*x25 * x11*x26;
h39 ==x9*x27 + x10*x28 * x11*x29;
 */

/*
* x9*x12 + x10*x13 * x11*x14 - h34 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 64;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x9*x15 + x10*x16 * x11*x17 - h35 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 65;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x9*x18 + x10*x19 * x11*x20 - h36 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 66;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x9*x21 + x10*x22 * x11*x23 - h37 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 67;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x9*x24 + x10*x25 * x11*x26 - h38 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 68;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x9*x27 + x10*x28 * x11*x29 - h39 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 9;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 10;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 11;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 69;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
 * h45 ==x12*x15 + x13*x16 * x14*x17;
h46 ==x12*x18 + x13*x19 * x14*x20;
h47 ==x12*x21 + x13*x22 * x14*x23;
h48 ==x12*x24 + x13*x25 * x14*x26;
h49 ==x12*x27 + x13*x28 * x14*x29;
 */
/*
* x12*x15 + x13*x16 * x14*x17 - h45 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 70;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x12*x18 + x13*x19 * x14*x20 - h46 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 71;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x12*x21 + x13*x22 * x14*x23 - h47 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 72;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x12*x24 + x13*x25 * x14*x26 - h48 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 73;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
* x12*x27 + x13*x28 * x14*x29 - h49 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 12;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 13;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 14;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 74;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
 * h56 ==x15*x18 + x16*x19 * x17*x20;
h57 ==x15*x21 + x16*x22 * x17*x23;
h58 ==x15*x24 + x16*x25 * x17*x26;
h59 ==x15*x27 + x16*x28 * x17*x29;
 */

/*
* x15*x18 + x16*x19 * x17*x20 - h56 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 75;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x15*x21 + x16*x22 * x17*x23 - h57 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 76;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x15*x24 + x16*x25 * x17*x26 - h58 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 77;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x15*x27 + x16*x28 * x17*x29 - h59 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 15;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 16;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 17;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 78;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
 * h67 ==x18*x21 + x19*x22 * x20*x23;
h68 ==x18*x24 + x19*x25 * x20*x26;
h69 ==x18*x27 + x19*x28 * x20*x29;
 */
/*
* x18*x21 + x19*x22 * x20*x23 - h67 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 79;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x18*x24 + x19*x25 * x20*x26 - h68 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 80;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x18*x27 + x19*x28 * x20*x29 - h69 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 18;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 19;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 20;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 81;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;

/*
 * h78 ==x21*x24 + x22*x25 * x23*x26;
	h79 ==x21*x27 + x22*x28 * x23*x29;
 */
/*
* x21*x24 + x22*x25 * x23*x26 - h78 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 82;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x21*x27 + x22*x28 * x23*x29 - h79 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 21;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 22;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 23;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 83;
code[ikod++]= EP_MINUS;
cons_length[icon] = ikod - cons_beg[icon];
/* Increment the constraint count */
icon++;


/*
* x24*x27 + x25*x28 * x26*x29 - h89 == 0;
* /* Constraint type */
ctype[icon]= 'E';
cons_beg[icon]= ikod;
/* Instruction list code */
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 24;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 27;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 25;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 28;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 26;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 29;
code[ikod++]= EP_MULTIPLY;
code[ikod++]= EP_PLUS;
code[ikod++]= EP_PUSH_VAR;
code[ikod++]= 84;
code[ikod++]= EP_MINUS;
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
double objval, primal[85];
/* Get the optimization result */
LSgetInfo(pModel, LS_DINFO_MIP_OBJ, &objval);
APIERRORCHECK;
LSgetMIPPrimalSolution( pModel, primal);
APIERRORCHECK;
printf("\n\nObjective = %f \n",objval);
int pt;
for (pt=0; pt<85; pt++)
{
	if (pt % 3 == 2)
	{
	printf("x[%d] =  %f \n", pt, primal[pt]);
	}
	else{
		printf("x[%d] =  %f \t", pt, primal[pt]);
		}
	if(pt==29 ||pt==39)
	{
		printf("\n");
	}
}
/*
printf("\n");
printf("x[0] = %f \t",primal[0]);
...
printf("x[28] = %f \n",primal[30]);
*/

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
//  printf("Press <Enter> ...");
//  getchar();
}


