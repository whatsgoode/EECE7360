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
    char sacName[32u];
    uint32_t suwTime;
    uint32_t * sauwInputSet;
    uint8_t * saucSolution;
    uint32_t suwSize;
    uint64_t sulTarget;
    Algorithm_t spfSolver; 
};

//! The solver function and macro are used to easily create and provide
//! solution functions to the solver.

#define SUBSETSUM_ALGORITHM(Func)     void (Func) (Subset_Sum_t * zptInst)

// Enum for storing state of input set elements in the solution

enum
{
    EXCLUDED,
    INCLUDED
};

// ***** Function prototypes **************************************************

// Initialization functions

void Subset_Sum_Initialize (Subset_Sum_t * zptHandle, char * zpsFilePath);

// Control functions

void Subset_Sum_Solve (Subset_Sum_t * zptHandle);
uint32_t Subset_Sum_GetSum (Subset_Sum_t * zptHandle);

// Input functions

void Subset_Sum_SetSolver (Subset_Sum_t * zptHandle, Algorithm_t ztSolver);
void Subset_Sum_Select (Subset_Sum_t * zptHandle, 
                                    uint32_t zpuwIndex, uint8_t zeState);

// Print Functions

void Subset_SumDisplayData (Subset_Sum_t * zptHandle);
void Subset_SumWriteData (Subset_Sum_t * zptHandle, char * zpnFldr);

// Cleanup functions

void Subset_Sum_Free (Subset_Sum_t * zptHandle);

#endif // !defined _SUBSUM_H

