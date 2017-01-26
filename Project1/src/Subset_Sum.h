/**************************************************************************//**
*
* \file        Subset_Sum.h
*
* \version     01/21/16  gcg  Initial version.
*
******************************************************************************/

#ifndef _SUBSUM_H
#define _SUBSUM_H

// ***** Header files *********************************************************

// Basic types

#include <stdint.h>

// ***** Definitions **********************************************************

//! The Subset_Sum_t struct contains all valid problem info. I might eventually
//! make this private as origionally intended but for now this is easier.

typedef struct Subset_Sum_s Subset_Sum_t;

typedef void (*Algorithm_t)(Subset_Sum_t * zptInst);

struct Subset_Sum_s
{
    uint32_t * sauwInputSet;
    uint32_t * sauwSolution;
    uint32_t suwSize;
    uint32_t suwTarget;
    Algorithm_t spfSolver;
    
};

//! The solver function and macro are used to easily create and provide
//! solution functions to the solver.

#define SUBSETSUM_ALGORITHM(Func)     void (Func) (Subset_Sum_t * zptInst)

// ***** Function prototypes **************************************************

// Initialization functions

void Subset_Sum_Initialize (Subset_Sum_t * zptHandle, char * zpsFilePath);

// Control functions

void Subset_Sum_Solve (Subset_Sum_t * zptHandle);

// Input functions

void Subset_Sum_SetSolver (Subset_Sum_t * zptHandle, Algorithm_t ztSolver);

// Print Functions

void Subset_SumDisplayData (Subset_Sum_t * zptHandle);
void Subset_SumWriteData (Subset_Sum_t * zptHandle, char * zpnFile);

// Cleanup functions

void Subset_Sum_Free (Subset_Sum_t * zptHandle);

#endif // !defined _SUBSUM_H
