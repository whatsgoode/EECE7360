/**************************************************************************//**
*
* \file        main.c
*
* \defgroup    main                 Main file for Project 1
*
* \details     This is the main file for project one. It is called along with
*              a filename and a time limit for solving the given instance. It
*              terminates when the solution is found or time expires.
*
*              This is also where the solution method for this project,
*              an exhaustive approach, is defined.
*
* \version     01/22/17  gcg  Initial version.
*
* \{
*
******************************************************************************/

// ***** Header files *********************************************************

// C Standard

#include <stdio.h>
#include <stdlib.h>

// Modules

#include "Subset_Sum.h"

// ***** Local function prototypes ********************************************

//! The solver is defined using the macro provided by the Subset Sum module

SUBSETSUM_ALGORITHM(P1_Exhaustive);

// ***** Local variables ******************************************************

static Subset_Sum_t mptProblem;

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
* \brief       Main function for Project 1
*
* \details     Creates a subset sum instance from the provided file and uses
*              the locally defined exhaustive search to solve it.
*
* \ref         Subset_Sum_Initialize
* \ref         Subset_Sum_ReadFromFile
* \ref         Subset_Sum_SetOutputFile
* \ref         Subset_Sum_SetSolver
* \ref         Subset_Sum_Solve
* \ref         Subset_Sum_Free
*
* \param[in]   argv[1]          Input file name
* \param[in]   argv[2]          Runtime limit
*
* \retval      int
*
******************************************************************************/

int main(int argc, char **argv)
{
        
        
        // Verify all arguments were recieved
        
        if (argc != 3)
        {
            printf("Invalid arguments! \n");
            printf("Usage: P1 [input file name] [time limit (sec)]\n");
            
            return -1;
        }
        
        // Initialize the problem
        
        Subset_Sum_Initialize(mptProblem);
        Subset_Sum_ReadFromFile(mptProblem, argv[1]);
        Subset_Sum_SetSolver(mptProblem, P1_Exhaustive);
        
        // Solve the problem
        
        Subset_Sum_Solve(mptProblem);
        
        // Cleanup
        
        Subset_Sum_Free(mptProblem);
        
        // TBD: Print some info to console ??
        
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
* \anchor      P1_Exhaustive
*
* \brief       Exhaustive Algorithm for solving a subset sum instance.
*
* \details     Solves an instance of Subset Sum by trying every solution until
*              one works.
*
* \param[in]   zauwInput          Input set
* \param[in]   zuwSize            Input size
* \param[in]   zauwSolution       Solution set
* \param[in]   zuwTarget          Target sum
*
* \retval      void
*
******************************************************************************/
SUBSETSUM_ALGORITHM(P1_Exhaustive)
{
    
    
}

// \}
