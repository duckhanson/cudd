/**CFile***********************************************************************

  FileName    [main.c]

  PackageName [ntr]

  Synopsis    [Main program for the nanotrav program.]

  Description []

  SeeAlso     []

  Author      [Fabio Somenzi/Priyank Kalla]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  Neither the name of the University of Colorado nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.]

******************************************************************************/

#include "ntr.h"
#include "cuddInt.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

#define NTR_VERSION\
    "Nanotrav Version #0.12, Release date 2003/12/31"

#define BUFLENGTH 8192

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

#ifndef lint
static char rcsid[] UTIL_UNUSED = "$Id: main.c,v 1.38 2004/08/13 18:28:28 fabio Exp fabio $";
#endif

static  char    buffer[BUFLENGTH];
#ifdef DD_DEBUG
extern  st_table *checkMinterms (BnetNetwork *net, DdManager *dd, st_table *previous);
#endif

/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/

/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/
/*-- Priyank: We don't need any of these--*/
/***************
static NtrOptions * mainInit ();
static void ntrReadOptions (int argc, char **argv, NtrOptions *option);
static void ntrReadOptionsFile (char *name, char ***argv, int *argc);
static char* readLine (FILE *fp);
static FILE * open_file (char *filename, const char *mode);
static int reorder (BnetNetwork *net, DdManager *dd, NtrOptions *option);
static void freeOption (NtrOptions *option);
static DdManager * startCudd (NtrOptions *option, int nvars);
static int ntrReadTree (DdManager *dd, char *treefile, int nvars);
******************/

/**AutomaticEnd***************************************************************/

/*---------------------------------------------------------------------------*/
/* Definition of exported functions                                          */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis    [Main program for ntr.]

  Description [Main program for ntr. Performs
  initialization. Introduces variables, builds ROBDDs for some
  functions, and prints-out the BDD structure. A sample file given to
  the 5740 class.]

  SideEffects [None]

  SeeAlso     []

******************************************************************************/
int
main(
  int  argc,
  char ** argv)
{
    NtrOptions	*option;	/* options */
    DdManager	*dd;		/* pointer to DD manager */
    int		exitval;	/* return value of Cudd_CheckZeroRef */
    int		ok;		/* overall return value from main() */
    int		result;		/* stores the return value of functions */
    int		i;		/* loop index */
    int		j;		/* loop index */
    double	*signatures;	/* array of signatures */
    int		pr;		/* verbosity level */
    int		reencoded;	/* linear transformations attempted */


    /****** Priyank's additions *******/
    DdNode *one, *zero;
    DdNode *a, *b, *c, *f, *g, *h;
    DdNode *f_a, *f_abar;
    /*********************************/


    /* Initialize manager. We start with 0 variables */
    dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    /*startCudd(option,net1->ninputs);*/
    if (dd == NULL) { exit(2); }


    /************ lets do our work here *************/
    one = Cudd_ReadOne( dd );
    Cudd_Ref(one); /* referenced for the first time */

    zero = Cudd_Not( one ); /* not the same as Cudd_ReadZero */
    /* Cudd_Ref(zero);*/ 
    /* reference count increment not needed because Cudd_Not returns
       the projection */ 

    /***** Add a new variable ***********/
    a = Cudd_bddNewVar(dd);
    b = Cudd_bddNewVar(dd);
    c = Cudd_bddNewVar(dd);

    f = Cudd_bddIte(dd, a, b, c); /* compute ITE(a, b, c); */
    Cudd_Ref(f);
    
    /* print BDD structure of f */
    printf("Printing the BDD for f: ptr to the nodes, T & E children\n");
    Cudd_PrintDebug( dd, f, 3, 3); 

    f_a = Cudd_Cofactor(dd, f, a); Cudd_Ref(f_a);
    /* f_a = +ve cofactor of f w.r.t. a: f_a = b */

    f_abar = Cudd_Cofactor(dd, f, Cudd_Not(a) ); Cudd_Ref(f_abar);
    /* f_abar = -ve cofactor of f w.r.t. a: f_abar = c */

    g = Cudd_bddAnd(dd, f_a, f_abar); Cudd_Ref(g);
    printf("Printing the BDD for g = b AND c\n");
    Cudd_PrintDebug( dd, g, 3, 3); 

    h = Cudd_bddAnd(dd, b, c); Cudd_Ref(h);

    if( h == g ){
      printf("Equivalence h = g means same DdNode pointers\n");
    }
    else{
      printf("Equivalence h = g does not mean same DdNode pointers? Something must be wrong\n");
    }


    exit(0);

} /* end of main */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
