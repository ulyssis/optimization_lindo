
###################################################################
#                       LINDO-API
#                    	 dicaps
#                  Copyright (c) 2011
#
#         Di Li, James Gross at umic.rwth-aachen
#                li@umic.rwth-aachen.de
###################################################################
  
File   : dicaps.c
  Purpose: Solve a NLP using the instruction-list style interface.
  Model  : A nonlinear model with multiple local minimizers.
  5 nodes, 3 channels. all the variables and constants are given.
  for larger scenario, boost c++ lib can be used to generate the 

	minimize  max(u0 u2 u3 u4 u5)   
	s.t.    
		b0, b1, b2, b3, b4   are binary

		0 <= c0 <= 4
		0 <= c1 <= 4
		0 <= c2 <= 4
		0 <= c3 <= 4
		0 <= c4 <= 4

                0  <=  p0  <=  8
                0  <=  p1  <=  8
                0  <=  p2  <=  8
                0  <=  p3  <=  8
                0  <=  p4  <=  8

	switch(c0) {
		case 0:
			u0 = (		(c0==c1) * p1 * g10 + (c0==c2) * p2 * g20 + (c0==c3) * p3 * g30 + (c0==c4) * p1 * g40 )/p0/min(Vlookup (p0, Pmax0, timepoints, 0)，10）
			break;
		case 1:
			u0 = (		(c0==c1) * p1 * g10 + (c0==c2) * p2 * g20 + (c0==c3) * p3 * g30 + (c0==c4) * p1 * g40 )/p0/min(Vlookup (p0, Pmax1, timepoints, 0)，10）
			break；
		case 2:
			u0 = (		(c0==c1) * p1 * g10 + (c0==c2) * p2 * g20 + (c0==c3) * p3 * g30 + (c0==c4) * p1 * g40 )/p0/min(Vlookup (p0, Pmax2, timepoints, 0)，10）
			break；


		u1 = ((c1, c0) * p0 * g01 		       + (c1, c2) * p2 * g21 + (c1, c3) * p3 * g31 + (c1, c4) * p1 * g41 )/p1/
		u2 = ((c2, c0) * p0 * g01 + (c2, c1) * p1 * g12 + 		       (c2, c3) * p3 * g32 + (c2, c4) * p1 * g42 )/p2/
		u3 = ((c3, c0) * p0 * g03 + (c3, c1) * p1 * g13 + (c3, c2) * p2 * g23 			   + (c3, c4) * p4 * g43 )/p3/
		u4 = ((c4, c0) * p0 * g04 + (c4, c1) * p1 * g14 + (c4, c2) * p2 * g24 + (c4, c3) * p3 * g34			 )/p4/




		// channel gain
		//	g00 g01 g02 g03 g04	0         0.0010    0.0004    0.0010    0.0028
		//	g10 g11 g12 g13 g14	0.0010         0    0.0010    0.0004    0.0028
		// g = 	g20 g21 g22 g23 g24 =   0.0004    0.0010         0    0.0010    0.0028
		//	g30 g31 g32 g33 g34	0.0010    0.0004    0.0010         0    0.0028
		//	g40 g41 g42 g43 g44	0.0028    0.0028    0.0028    0.0028         0
		
		// possible interference
		pg0	0            0.0010*p1    0.0004*p2    0.0010*p3    0.0028*p4   
		pg1     0.0010*p0            0    0.0010*p2    0.0004*p3    0.0028*p4
	p.g =	pg2 =   0.0004*p0    0.0010*p1            0    0.0010*p3    0.0028*p4
		pg3     0.0010*p0    0.0004*p1    0.0010*p2            0    0.0028*p4
		pg4     0.0028*p0    0.0028*p1    0.0028*p2    0.0028*p3            0

		// F illustrates the interferce relations
		// F = (B* B' ~= 0);
		// F = F - eye(n);

		F0	0     0     1     0     0
		F1	0     0     0     1     0
	F =     F2 =	1     0     0     0     0
		F3	0     1     0     0     0
		F4	0     0     0     0     0	

		inf0 = SumIf(1, F0 , pg0)
		inf1 = SumIf(1, F1 , pg1)
		inf2 = SumIf(1, F2 , pg2)
		inf3 = SumIf(1, F3 , pg3)
		inf4 = SumIf(1, F4 , pg4)
	
		// judge channel
		c0 = SumIf(1, B0, (0 1 2))
		...
		c4 = SumIf(1, B4, (0 1 2))	
	
		// ti
		Pmax0 = (8 8 8 4 4 4 8 4 4 4 4)
		Pmax1 = (4 4 4 8 8 4 8 8 4 8 8)
		Pmax2 = (8 8 8 4 8 4 4 4 4 4 8)
		timepoints = (1 2 3 4 5 6 7 8 9 10 11)
	
		switch c0 {	
		case 0: t0 = Vlookup (p0, Pmax0 , timepoints , 0)
		case 1: t0 = Vlookup (p0, Pmax1 , timepoints , 0)
		case 2: t0 = Vlookup (p0, Pmax2 , timepoints , 0) }
		switch c1
		...
		switch c2
		...
		switch c3
		...
		switch c4 {
		case 0: t0 = Vlookup (p4, Pmax0 , timepoints , 0)
		case 1: t0 = Vlookup (p4, Pmax1 , timepoints , 0)
		case 2: t0 = Vlookup (p4, Pmax2 , timepoints , 0) }

		t0 = min (t0,10)
		t1 = min (t1,10)
		t2 = min (t2,10)
		t3 = min (t3,10)
		t4 = min (t4,10)

		// utility
		u0 = inf0/p0/t0
		u1 = inf1/p1/t1
		u2 = inf2/p2/t2
		u3 = inf3/p3/t3
		u4 = inf4/p4/t4

                0  <=  p0  <=  8
                0  <=  p1  <=  8
                0  <=  p2  <=  8
                0  <=  p3  <=  8
                0  <=  p4  <=  8


#include <stdio.h>
#include <stdlib.h>
/* LINDO API header file */
#include "lindo.h"

/* Define a macro to declare variables for
    error checking */
#define APIERRORSETUP  \
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
         printf("nErrorCode=%d:  %s\n", nErrorCode, \
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

 /*****************************************************************
  * Step 1: Create a model in the environment.
  *****************************************************************/
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
      printf( "Invalid License Key!\n");
      exit( 1);
   }
   APIERRORCHECK;
/* >>> Step 2 <<< Create a model in the environment. */
   pModel = LScreateModel(pEnv,&nErrorCode);
   APIERRORCHECK;
   {
/*
 * >>>> Step 3 <<< Set up the instruction list of the model.
 */
      int nobjs, ncons, nvars, nnums, lsize;
      int objsense[1];
      char ctype[6], vtype[20];			//// ctype[6] stores 6 constraint relations
      int code[1000], varndx[20];			//// the size of code need consideration
      double numval[8],varval[30];
      int objs_beg[1], objs_length[1], cons_beg[6], cons_length[6];
      double lwrbnd[2], uprbnd[2];
      int nLinearz, nAutoDeriv;
      int ikod, iobj, icon;
      /* Number of constraints */
      ncons = ;
      /* Number of objectives */
      nobjs = 1;
      /* Number of variables */
      nvars = 10;			//power and B
      /* Number of real number constants */
      nnums = 5;
      /* Variable index */
      varndx[0] =0;
      varndx[1] =1;
      varndx[2] =2;
      varndx[3] =3;
      varndx[4] =4;
      varndx[5] =5;
      varndx[6] =6;
      varndx[7] =7;
      varndx[8] =8;
      varndx[9] =9;
//      varndx[10]=10;
//      varndx[11]=11;
//      varndx[12]=12;
//      varndx[13]=13;
//      varndx[14]=14;
//      varndx[15]=15;
//      varndx[16]=16;
//      varndx[17]=17;
//      varndx[18]=18;
//      varndx[19]=19;
      /* Lower bounds of variables */
      lwrbnd[0]=0.0;
      lwrbnd[1]=0.0;
      lwrbnd[2]=0.0;
      lwrbnd[3]=0.0;
      lwrbnd[4]=0.0;

      /* Upper bounds of variables */
      uprbnd[0]=8.0;
      uprbnd[1]=8.0;
      uprbnd[2]=8.0;
      uprbnd[3]=8.0;
      uprbnd[4]=8.0;
      
      /* Starting point of variables */
      varval[0]=4.0;
      varval[1]=4.0;
      varval[2]=4.0;
      varval[3]=4.0;
      varval[4]=8.0;

      varval[5]=0;
      varval[6]=1;
      varval[7]=0;
      varval[8]=0;
      varval[9]=0;
      varval[10]=1;
      varval[11]=0;
      varval[12]=1;
      varval[13]=0;
      varval[14]=0;
      varval[15]=0;
      varval[16]=1;
      varval[17]=1;
      varval[18]=0;
      varval[19]=0;

      /* Variable type, C= continuous, B = binary */
      vtype[0] = 'C';
      vtype[1] = 'C';
      vtype[2] = 'C';
      vtype[3] = 'C';
      vtype[4] = 'C';

      vtype[5] = 'B';
      vtype[6] = 'B';
      vtype[7] = 'B';
      vtype[8] = 'B';
      vtype[9] = 'B';
      vtype[10] = 'B';
      vtype[11] = 'B';
      vtype[12] = 'B';
      vtype[13] = 'B';
      vtype[14] = 'B';
      vtype[15] = 'B';
      vtype[16] = 'B';
      vtype[17] = 'B';
      vtype[18] = 'B';
      vtype[19] = 'B';

  /* Double Precision constants in the model */
      numval[0]=1.0;
      numval[1]=0.4;
      numval[2]=6.0;
      numval[3]=4.0;
      numval[4]=2.0;
      /* Count for instruction code */		//// stores the indices of all instructions
    ikod = 0;
      /* Count for objective row */
    iobj = 0;
      /* Count for constraint row */		//// stores the indices of all constraints
    icon = 0;
      /*
       *  Instruction code of the objective:
       *
       *  minimize  max(u0 u2 u3 u4 u5)
       */
      /* Direction of optimization */
      objsense[iobj]= LS_MIN;
      /* Beginning position of objective */
      objs_beg[iobj]=ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    0;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    0;
      code[ikod++]=  EP_PLUS;
      code[ikod++]=  EP_ABS;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    1;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]= EP_MULTIPLY;
      code[ikod++]= EP_PLUS;				/////////////
  /* Length of objective */
      objs_length[iobj] = ikod - objs_beg[iobj];

      /*
       *  Instruction code of constraint 0:		
       *
       *  x0  + x1 - 4 <= 0;
       */
  /* Constraint type */
      ctype[icon]= 'L';   /* less or than or equal to */
      /* Beginning position of constraint 0 */
      cons_beg[icon]= ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    0;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_PLUS;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    3;
      code[ikod++]=  EP_MINUS;
  /* Length of constraint 0 */
      cons_length[icon] = ikod - cons_beg[icon];
      /* Increment the constraint count */
  icon++;
      /*
       *  Instruction code of constraint 1:
       *
       *  x0 * x1      + x1 - 6 <= 0;
       */
  /* Constraint type */
      ctype[icon]= 'L';   /* less than or equal to */
      /* Beginning position of constraint 1 */
      cons_beg[icon]=  ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    0;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_MULTIPLY;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_PLUS;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    2;
      code[ikod++]=  EP_MINUS;
      /* Length of constraint 1 */
      cons_length[icon] = ikod - cons_beg[icon];
      /* Increment the constraint count */
  icon++;
      /*
       *  Instruction code of constraint 2:
       *
       *  x0 * x1           <= 0;
       */
  /* Constraint type */
      ctype[icon]= 'L';   /* less than or equal to */
      /* Beginning position of constraint 2 */
      cons_beg[icon]=  ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    0;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_MULTIPLY;
  /* Length of constraint 2 */
      cons_length[icon] = ikod - cons_beg[icon];
      /* Increment the constraint count */
      icon++;
      /*
       *  Instruction code of constraint 3:
     *
       *  max(x0 , x1 + 1)        >= 0;
       */
  /* Constraint type */
      ctype[icon]= 'G';   /* greater than or equal to */
      /* Beginning position of constraint 3 */
      cons_beg[icon]=  ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    0;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    0;
      code[ikod++]=  EP_PLUS;
      code[ikod++]=  EP_MAX;
      code[ikod++]=    2;				//The operator is followed by an integer specifying the vector length n

  /* Length of constraint 3 */
      cons_length[icon] = ikod - cons_beg[icon];
      /* Increment the constraint count */
  icon++;
      /*
       *  Instruction code of constraint 4:
       *
       *  if(x1, 1, x1)        <= 0;
       */
  /* Constraint type */
      ctype[icon]= 'L';  /* less than or equal to */
      /* Beginning position of constraint 4 */
      cons_beg[icon]=  ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    0;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_IF;
  /* Length of constraint 4 */
      cons_length[icon] = ikod - cons_beg[icon];
      /* Increment the constraint count */
  icon++;
      /*
       *  Instruction code of constraint 5:
       *
       *  (x1 * 2 * x1  -  x1) * x0      <= 0;				/// x1 * 2 * x1 * x0 - x1 * x0
       */
  /* Constraint type */
      ctype[icon]= 'L';  /* less than or equal to */
      /* Beginning position of constraint 5 */
      cons_beg[icon]=  ikod;
      /* Instruction list code */
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_PUSH_NUM;
      code[ikod++]=    4;
      code[ikod++]=  EP_MULTIPLY;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_MULTIPLY;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    1;
      code[ikod++]=  EP_MINUS;
      code[ikod++]=  EP_PUSH_VAR;
      code[ikod++]=    0;
      code[ikod++]=  EP_MULTIPLY;
  /* Length of constraint 5 */
      cons_length[icon] = ikod - cons_beg[icon];

      /* Total number of items in the instruction list */
      lsize = ikod;
      /* Set linearization level, before a call to LSloadInstruct.
       * If not specified, the solver will decide */
      nLinearz = 1;
      nErrorCode = LSsetModelIntParameter (pModel,
                    LS_IPARAM_NLP_LINEARZ, nLinearz);
      APIERRORCHECK;

      /* Set up automatic differentiation, before a call to
       * LSloadInstruct. If not specified, the numerical derivative
       * will be applied */
      nAutoDeriv = 1;
      nErrorCode = LSsetModelIntParameter (pModel,
                    LS_IPARAM_NLP_AUTODERIV, nAutoDeriv);
      APIERRORCHECK;
      /* Pass the instruction list to problem structure
       * by a call to LSloadInstruct() */
      nErrorCode = LSloadInstruct (pModel, ncons, nobjs, nvars, nnums,
                    objsense, ctype,  vtype, code, lsize, varndx,
                    numval, varval, objs_beg, objs_length, cons_beg,
                    cons_length, lwrbnd, uprbnd);
      APIERRORCHECK;
   }
/*
 * >>> Step 5 <<< Perform the optimization using the MIP solver
 */
   nErrorCode = LSsolveMIP(pModel, NULL);
   APIERRORCHECK;
   {
      int nLinearity;
      double objval=0.0, primal[100];
      /* Get the optimization result */
      LSgetInfo(pModel, LS_DINFO_MIP_OBJ, &objval);
      APIERRORCHECK;
      LSgetMIPPrimalSolution( pModel, primal) ;
      APIERRORCHECK;
      printf("\n\nObjective = %f \n",objval);
      printf("x[0] = %f \n",primal[0]);
      printf("x[1] = %f \n",primal[1]);

      /* Get the linearity of the solved model */
      nErrorCode = LSgetModelIntParameter (pModel,
                   LS_IPARAM_NLP_LINEARITY, &nLinearity);
      APIERRORCHECK;
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
