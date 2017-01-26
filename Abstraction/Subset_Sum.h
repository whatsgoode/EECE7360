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

//! The Subset_Sum_t typedef exists just to hide the void pointer from the
//! higher level application

typedef void * Subset_Sum_t;

//! The solver function and macro are used to easily create and provide
//! solution functions to the solver.

typedef void (*Algorithm_t)(uint32_t * zauwInput, uint32_t zuwSize,
                                 uint32_t * zauwSolution, uint32_t zuwTarget);

#define SUBSETSUM_ALGORITHM(Func)                                             \
       void (Func) (uint32_t * zauwInput, uint32_t zuwSize,                   \
                                uint32_t * zauwSolution, uint32_t zuwTarget)

// ***** Function prototypes **************************************************

// Initialization functions

void Subset_Sum_Initialize (Subset_Sum_t zptHandle);

// Control functions

void Subset_Sum_Solve (Subset_Sum_t zptHandle);

// Input functions

void Subset_Sum_ReadFromFile (Subset_Sum_t zptHandle, char * zpsFilePath);
void Subset_Sum_ReadFromArray (Subset_Sum_t zptHandle, uint32_t * zpauwData);

void Subset_Sum_SetOutputFile (Subset_Sum_t zptHandle, char * zpsFilePath);
void Subset_Sum_SetSolver (Subset_Sum_t zptHandle, Algorithm_t ztSolver);

// Print Functions

void Subset_SumDisplayData (Subset_Sum_t zptHandle);
void Subset_SumWriteData (Subset_Sum_t zptHandle);

// Cleanup functions

void Subset_Sum_Free (Subset_Sum_t zptHandle);

#endif _SUBSUM_H
