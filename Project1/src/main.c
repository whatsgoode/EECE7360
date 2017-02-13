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
#include <time.h>
#include <stdbool.h>

// Modules

#include "Subset_Sum.h"

// ***** Local function prototypes ********************************************

//! The solver is defined using the macro provided by the Subset Sum module

SUBSETSUM_ALGORITHM(P1_Exhaustive);

// ***** Local variables ******************************************************

static Subset_Sum_t mtProblem;
static uint32_t muwTimeLimit;

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

        // Set the time limit

        muwTimeLimit = atoi(argv[2]);
        
        // Initialize the problem
        
        Subset_Sum_Initialize(&mtProblem, argv[1]);
        Subset_Sum_SetSolver(&mtProblem, P1_Exhaustive);
        
        // Solve the problem
        
        Subset_Sum_Solve(&mtProblem);

        // Write outfile

        Subset_SumDisplayData(&mtProblem);
        Subset_SumWriteData(&mtProblem, "test");
        
        // Cleanup
        
        Subset_Sum_Free(&mtProblem);
        
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
* \param[in]   zptInst            Instance to solve
*
* \retval      void
*
******************************************************************************/
SUBSETSUM_ALGORITHM(P1_Exhaustive)
{
    uint32_t xuwLoop;
    time_t xtStartTime, xtCurrTime;
    bool xbDone = false;

    // Clear all selections

    for(xuwLoop = 0u; xuwLoop < zptInst->suwSize; xuwLoop++)
    {
        Subset_Sum_Select(zptInst, xuwLoop, EXCLUDED);
    }

    // Start the timer

    xtStartTime = time(NULL);
    xtCurrTime = 0u;

    // The loop to find the subsets treats the inpur array elements
    // as the digits in a binary number. This way, every combination
    // is tested as it "counts"

    while((Subset_Sum_GetSum(zptInst) != zptInst->sulTarget) &&
          (xtCurrTime < muwTimeLimit) &&
          (xbDone == false))
    {
          // Find next subset

          for(xuwLoop = 0; xuwLoop < zptInst->suwSize; xuwLoop++)
          {
              if(zptInst->saucSolution[xuwLoop] == INCLUDED)
              {
                  zptInst->saucSolution[xuwLoop] = EXCLUDED;
                  if (xuwLoop == (zptInst->suwSize - 1u))
                  {
                      xbDone = true;
                      break;
                  }
              }
              else
              {
                  zptInst->saucSolution[xuwLoop] = INCLUDED;
                  break;
              }
          }

          // Update the elapsed time

          xtCurrTime = time(NULL) - xtStartTime;
    }

    // Update the total elapsed time

    zptInst->suwTime = xtCurrTime;
}

// \}

// \}
