/**************************************************************************//**
*
* \file        main.c
*
* \defgroup    main                 Main file for Project 5
*
* \details     This is the main file for project five. It is called along with
*              a filename and a time limit for solving the given instance. It
*              terminates when the solution is found or time expires.
*
*              This is also where the solution methods for this project,
*              a greedy & random initial solution with a "1OPT" neighborhood and
*              a basic tabu list implementation are defined. 
*
* \version     04/19/17  gcg  Initial version.
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

//! The solvers are defined using the macro provided by the Subset Sum module

SUBSETSUM_ALGORITHM(P5_Greedy);
SUBSETSUM_ALGORITHM(P5_Random);
SUBSETSUM_ALGORITHM(P5_Tabu);

//! Helper funtions - 1OPT is here since is is called twice

static uint32_t P5__1OPT(Subset_Sum_t * zptInst);

// ***** Local variables ******************************************************

static Subset_Sum_t mtProblem_Greedy;
static Subset_Sum_t mtProblem_Random;
static Subset_Sum_t mtProblem_Tabu;

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
* \brief       Main function for Project 5
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
        
        if (argc != 3)
        {
            printf("Invalid arguments! \n");
            printf("Usage: P5 [input file name] [time limit (sec)]\n");
            
            return -1;
        }
        
        // Set the time limit

        muwTimeLimit = atoi(argv[2]);
        
        // Initialize the problems
        
        Subset_Sum_Initialize(&mtProblem_Greedy, argv[1]);
        Subset_Sum_SetSolver(&mtProblem_Greedy, P5_Greedy);
        
        Subset_Sum_Initialize(&mtProblem_Random, argv[1]);
        Subset_Sum_SetSolver(&mtProblem_Random, P5_Random);
        
        Subset_Sum_Initialize(&mtProblem_Tabu, argv[1]);
        Subset_Sum_SetSolver(&mtProblem_Tabu, P5_Tabu);
        
        // Initialize random numbers
        
        srand(time(NULL));
        
        // Solve the problems and write outfiles
        
        Subset_Sum_Solve(&mtProblem_Greedy);
        printf("%s Greedy solved\r\n", argv[1]);
        Subset_SumWriteData(&mtProblem);
        
        Subset_Sum_Solve(&mtProblem_Random);
        printf("%s Random solved\r\n", argv[1]);
        Subset_SumWriteData(&mtProblem);
        
        Subset_Sum_Solve(&mtProblem_Tabu);
        printf("%s Tabu solved\r\n", argv[1]);
        Subset_SumWriteData(&mtProblem);
        
        // Cleanup
        
        Subset_Sum_Free(&mtProblem_Greedy);
        Subset_Sum_Free(&mtProblem_Random);
        Subset_Sum_Free(&mtProblem_Tabu);
        
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
* \anchor      P5_Greedy
*
* \brief       Greedy Algorithm for solving a subset sum instance.
*
* \details     Solves an instance of Subset Sum by greedily adding any valid
*              elements. Improve the solution using its 1OPT neighborhood
*
* \param[in]   zptInst            Instance to solve
*
* \retval      void
*
******************************************************************************/
SUBSETSUM_ALGORITHM(P5_Greedy)
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
    
    // Improve the solution if time remains
    
    P5__1OPT(zptInst);
}

/**************************************************************************//**
*
* \anchor      P5_Random
*
* \brief       Random Algorithm for solving a subset sum instance.
*
* \details     Solves an instance of Subset Sum by randomly adding any valid
*              elements. Improve the solution using its 1OPT neighborhood
*
* \param[in]   zptInst            Instance to solve
*
* \retval      void
*
******************************************************************************/
SUBSETSUM_ALGORITHM(P5_Random)
{
    uint32_t xuwLoop;
    uint64_t xulTempSum = 0u;
    time_t xtStartTime, xtCurrTime;
    bool xbDone = false;
    int xwRand;

    // Clear all selections

    for(xuwLoop = 0u; xuwLoop < zptInst->suwSize; xuwLoop++)
    {
        Subset_Sum_Select(zptInst, xuwLoop, EXCLUDED);
    }
    
    // Loop through the set, randomly adding any element that is legal
    
    for(xuwLoop = 0u; xuwLoop < zptInst->suwSize; xuwLoop++)
    {
        // If the instance is solved, stop
        
        if (Subset_Sum_GetSum(zptInst) == zptInst->sulTarget)
        {
            
            break;
        }
        
        // Attempt to randomly add the next element if it is legal. If not,
        // ignore it.
        
        xulTempSum = 
            Subset_Sum_GetSum(zptInst) + zptInst->sauwInputSet[xuwLoop];
        
        if(xulTempSum <= zptInst->sulTarget)
        {
            xwRand = rand() % 2;  // Pseudo random 0 or 1
            
            zptInst->saucSolution[xuwLoop] = (xwRand > 0) ? INCLUDED : EXCLUDED;
        }
    }
    
    // Improve the solution if time remains
    
    P5__1OPT(zptInst);
}

/**************************************************************************//**
*
* \anchor      P5_Tabu
*
* \brief       Greedy Algorithm for solving a subset sum instance.
*
* \details     Solves an instance of Subset Sum by greedily adding any valid
*              elements. Improve the solution using its 1OPT neighborhood and a
*              tabu list.
*
* \param[in]   zptInst            Instance to solve
*
* \retval      void
*
******************************************************************************/
SUBSETSUM_ALGORITHM(P5_Tabu)
{
    uint32_t xuwLoop;
    uint64_t xulTempSum = 0u;
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
    
    // TBD - Tabu list
}

// \}

/**************************************************************************//**
*
* \defgroup    Local              Helper definition
*
* \{
*
******************************************************************************/

/**************************************************************************//**
*
* \anchor      P5__1OPT
*
* \brief       1OPT neighborhood exploration for a subset sum instance
*
* \details     Improves an instance of Subset Sum by swapping any valid
*              elements.
*
* \param[in]   zptInst            Instance to solve
*
* \retval      void
*
******************************************************************************/

static uint32_t P5__1OPT(Subset_Sum_t * zptInst)
{
    uint32_t xuwLoop;
    time_t xtStartTime, xtCurrTime;
    bool xbDone = false;
    
    // Start the timer

    xtStartTime = time(NULL);
    xtCurrTime = 0u;
    
}

// \}

// \}
