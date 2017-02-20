/**************************************************************************//**
*
* \file        main.c
*
* \defgroup    main                 Main file for Project 3
*
* \details     This is the main file for project three. It is called along with
*              a filename and a time limit for solving the given instance. It
*              terminates when the solution is found or time expires.
*
*              This is also where the solution method for this project,
*              a greedy approach, is defined.
*
* \version     02/13/17  gcg  Initial version.
*
* \{
*
******************************************************************************/

// ***** Header files *********************************************************

// C Standard

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Modules

#include "Subset_Sum.h"

// ***** Local function prototypes ********************************************

//! The solver is defined using the macro provided by the Subset Sum module

SUBSETSUM_ALGORITHM(P3_Greedy);

// ***** Local variables ******************************************************

static Subset_Sum_t mtProblem;

/**************************************************************************//**
*
* \defgroup    main                   Main function
*
* \{
*
******************************************************************************/

/**************************************************************************//**
*
* \anchor      main
*
* \brief       Main function for Project 3
*
* \details     Creates a subset sum instance from the provided file and uses
*              the locally defined greedy search to solve it.
*
* \ref         Subset_Sum_Initialize
* \ref         Subset_Sum_ReadFromFile
* \ref         Subset_Sum_SetOutputFile
* \ref         Subset_Sum_SetSolver
* \ref         Subset_Sum_Solve
* \ref         Subset_Sum_Free
*
* \param[in]   argv[1]          Input file name
*
* \retval      int
*
******************************************************************************/

int main(int argc, char **argv)
{  
        
        // Verify all arguments were recieved
        
        if (argc != 2)
        {
            printf("Invalid arguments! \n");
            printf("Usage: P3 [input file name]\n");
            
            return -1;
        }
        
        // Initialize the problem
        
        Subset_Sum_Initialize(&mtProblem, argv[1]);
        Subset_Sum_SetSolver(&mtProblem, P3_Greedy);
        
        // Solve the problem
        
        Subset_Sum_Solve(&mtProblem);

        // Write outfile

        Subset_SumDisplayData(&mtProblem);
        Subset_SumWriteData(&mtProblem);
        
        // Cleanup
        
        Subset_Sum_Free(&mtProblem);
        
        return 0;
}

// \}

/**************************************************************************//**
*
* \defgroup    Algorithm              Algorithm definition
*
* \{
*
******************************************************************************/

/**************************************************************************//**
*
* \anchor      P3_Greedy
*
* \brief       Greedy Algorithm for solving a subset sum instance.
*
* \details     Solves an instance of Subset Sum by greedily adding any valid
*              elements
*
* \param[in]   zptInst            Instance to solve
*
* \retval      void
*
******************************************************************************/
SUBSETSUM_ALGORITHM(P3_Greedy)
{
    uint32_t xuwLoop;
    uint64_t xulTempSum = 0u;

    // Clear all selections

    for(xuwLoop = 0u; xuwLoop < zptInst->suwSize; xuwLoop++)
    {
        Subset_Sum_Select(zptInst, xuwLoop, EXCLUDED);
    }
    
    // Loop through the set, adding any element that is legal
    // TBD - Sorted?
    // TBD - Smallest first? Largest first?
    
    for(xuwLoop = 0u; xuwLoop < zptInst->suwSize; xuwLoop++)
    {
        // If the instance is solved, stop
        
        if (Subset_Sum_GetSum(zptInst) == zptInst->sulTarget)
        {
            
            break;
        }
        
        // Attempt to add the next element if it is legal. If not,
        // ignore it.
        
        xulTempSum = 
            Subset_Sum_GetSum(zptInst) + zptInst->sauwInputSet[xuwLoop];
        
        zptInst->saucSolution[xuwLoop] = 
            (xulTempSum <= zptInst->sulTarget) ? INCLUDED : EXCLUDED;
    }
}

// \}

// \}
