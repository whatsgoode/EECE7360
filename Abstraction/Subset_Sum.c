/**************************************************************************//**
*
* \file        Subset_Sum.c
*
* \defgroup    Subset_Sum    Generic module to represent a subset sum problem
*
* \details     This module is intended to provide a generic framework for
*              processing an instance of a subset sum problem. The values and
*              best solution are tracked, along with a pointer to a solver
*              function that can be changed as needed.
*
*              Also included are some generic printing functions for reporting
*              data.
*
* \version     01/21/17  gcg  Initial version.
* \version     01/25/17  gcg  Initial draft, needs testing
*
* \{
*
******************************************************************************/

// ***** Header files *********************************************************

#define _GNU_SOURCE

// C Standard

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

// Modules

#include "Subset_Sum.h"

// ***** Definitions **********************************************************

// ***** Local Functions ******************************************************

static void SS__Parse_Info (Subset_Sum_t * zptInst, char * zpnInfo);
static void SS__Parse_Entry (Subset_Sum_t * zptInst, 
                                      uint32_t zuwIndex, char * zpnLine);
static void SS__Write (Subset_Sum_t * zptHandle, int ziFd);

/**************************************************************************//**
*
* \defgroup    Subset_Sum Init       Initialization Functions
*
* \{
*
******************************************************************************/
/**************************************************************************//**
*
* \anchor      Subset_Sum_Initialize
*
* \brief       Parse the given input file
*
* \details     Chops the input file into lines and uses that info fill the
*              given Subset_Sub object pointer.
*
* \ref         SS__Parse_Info
* \ref         SS__Parse_Entry
*
* \param[in]   zptHandle            Problem instance
* \param[in]   zpsFilePath          File path
*
* \retval      void
*
******************************************************************************/

void Subset_Sum_Initialize (Subset_Sum_t * zptHandle, char * zpsFilePath)
{
    char xaacInputFile[256u][64u];
    char xcRead;
    uint32_t xuwSize = 0;
    uint8_t xucLineIndex = 0;

    // Pull the name of the instance

    // Make sure the file exists in the first place
    // Extract base name of file
    assert(access(zpsFilePath, F_OK) != 1);
    char *baseName = basename(zpsFilePath);
    sscanf(baseName, "%s.dat", zptHandle->sacName);
    while(zptHandle->sacName[++xucLineIndex] != '.'){};
    zptHandle->sacName[xucLineIndex] = '\0';
    xucLineIndex = 0;

    // Open the maps file

    int xiFD = open(zpsFilePath, O_RDONLY);

    // Chop the file into lines

    while(read(xiFD, &xcRead, 1u) != 0u)
    {
        xaacInputFile[xuwSize][xucLineIndex++] = xcRead;

        if(xcRead == '\n')
        {
            xaacInputFile[xuwSize++][xucLineIndex] = '\0';
            xucLineIndex = 0u;
        }
    }

    // Add last null on final string
    
    xaacInputFile[xuwSize][xucLineIndex] = '\0';

    // Read the problem info

    SS__Parse_Info(zptHandle, &(xaacInputFile[0u][0u]));
    
    // Now that the instance size is known, allocate space for the input
    // set
    
    zptHandle->sauwInputSet = 
            (uint32_t *)malloc(zptHandle->suwSize * sizeof(uint32_t));

    zptHandle->saucSolution = (uint8_t *)calloc(zptHandle->suwSize, 1u);
    
    // Read the set members one at a time

    for (xucLineIndex = 1u; xucLineIndex < xuwSize; xucLineIndex++)
    {
        
                SS__Parse_Entry(zptHandle, 
                    (xucLineIndex - 1u), &(xaacInputFile[xucLineIndex][0u]));
    }

    // Close the file

    close(xiFD);
}

// \}

/**************************************************************************//**
*
* \defgroup    Subset_Sum Control       Control Functions
*
* \{
*
******************************************************************************/
/**************************************************************************//**
*
* \anchor      Subset_Sum_Solve
*
* \brief       Attempt to solve the instance
*
* \details     Simply call the user supplied solver on the given instance
*
* \param[in]   zptHandle            Problem instance
*
* \retval      void
*
******************************************************************************/

void Subset_Sum_Solve (Subset_Sum_t * zptHandle)
{
    // Call the solver function
    
    (zptHandle->spfSolver)(zptHandle);
}

/**************************************************************************//**
*
* \anchor      Subset_Sum_GetSum
*
* \brief       Get the current sum of the solution
*
* \details     Returns the current sum of the selected items
*
* \param[in]   zptHandle            Problem instance
*
* \retval      uint32_t
*
******************************************************************************/

uint32_t Subset_Sum_GetSum (Subset_Sum_t * zptHandle)
{
    uint32_t xuwSum = 0u;
    uint32_t xuwLoop;

    // Sum enabled elements
    
    for (xuwLoop = 0u; xuwLoop < zptHandle->suwSize; xuwLoop++)
    {

        if(zptHandle->saucSolution[xuwLoop] == INCLUDED)
        {
            xuwSum += zptHandle->sauwInputSet[xuwLoop];
        }
    }

    return xuwSum;
}

// \}

/**************************************************************************//**
*
* \defgroup    Subset_Sum Input          Input Functions
*
* \{
*
******************************************************************************/
/**************************************************************************//**
*
* \anchor      Subset_Sum_SetSolver
*
* \brief       Set the solver function
*
* \details     Sets the solver function in the given instance handle.
*
* \param[in]   zptHandle            Problem instance
* \param[in]   ztSolver             Solution function pointer
*
* \retval      void
*
******************************************************************************/

void Subset_Sum_SetSolver (Subset_Sum_t * zptHandle, Algorithm_t ztSolver)
{
    // Set the solver function
    
    zptHandle->spfSolver = ztSolver;
}

/**************************************************************************//**
*
* \anchor      Subset_Sum_Select
*
* \brief       Add the given element to the solution
*
* \details     Sets the inlude state of the given element.
*
* \param[in]   zptHandle            Problem instance
* \param[in]   zpuwIndex            Item to set
* \param[in]   zeState              State to set
*
* \retval      void
*
******************************************************************************/

void Subset_Sum_Select (Subset_Sum_t * zptHandle, 
                                    uint32_t zpuwIndex, uint8_t zeState)
{
    zptHandle->saucSolution[zpuwIndex] = zeState;
}

// \}

/**************************************************************************//**
*
* \defgroup    Subset_Sum Print       Print Functions
*
* \{
*
******************************************************************************/
/**************************************************************************//**
*
* \anchor      Subset_SumDisplayData
*
* \brief       Print solution to console.
*
* \details     Prints the solution of the given instance to the console in the
*              same formatting as it would be written to an outfile.
*
* \param[in]   zptHandle            Problem instance
*
* \retval      void
*
******************************************************************************/

void Subset_SumDisplayData (Subset_Sum_t * zptHandle)
{
    // Call generic with file pointer to STDOUT

    SS__Write(zptHandle, STDOUT_FILENO);
}

/**************************************************************************//**
*
* \anchor      Subset_SumWriteData
*
* \brief       Write solution to output file.
*
* \details     Prints the solution of the given instance to the given outfile.
*
* \param[in]   zptHandle            Problem instance
* \param[in]   zpnFile              File path
*
* \retval      void
*
******************************************************************************/

void Subset_SumWriteData (Subset_Sum_t * zptHandle, char * zpnFldr)
{
    char xacFileName[128u];

    // Create file

    sprintf(xacFileName, "../outputs/%s/%s.out", zpnFldr, zptHandle->sacName);
    int xtFile = open(xacFileName, O_RDWR | O_CREAT | O_APPEND, 0777);

    // Call generic write

    SS__Write(zptHandle, xtFile);

    // Close the file

    close(xtFile);
}

// \}

/**************************************************************************//**
*
* \defgroup    Subset_Sum Print       Print Functions
*
* \{
*
******************************************************************************/
/**************************************************************************//**
*
* \anchor      Subset_Sum_Free
*
* \brief       Free allocated memory
*
* \details     Frees input and solution arrays which are allocated dynamically
*              at initialization.
*
* \param[in]   zptHandle            Problem instance
*
* \retval      void
*
******************************************************************************/

void Subset_Sum_Free (Subset_Sum_t * zptHandle)
{
    // Free allocated memory
    
    free(zptHandle->sauwInputSet);
    free(zptHandle->saucSolution);
}

// \}

/**************************************************************************//**
*
* \defgroup    Subset_Sum Internal       Private Functions
*
* \{
*
******************************************************************************/
/**************************************************************************//**
*
* \anchor      SS__Parse_Info
*
* \brief       Parse the info line at the top of an info file
*
* \details     Parses the info line and populates the given Subset Sum instance
*              accordingly.
*
* \param[in]   zptInst            Problem instance
* \param[in]   zpnInfo            Info string
*
* \retval      void
*
******************************************************************************/

static void SS__Parse_Info (Subset_Sum_t * zptInst, char * zpnInfo)
{
    // The format of the first line is [size] [target]\r\n so simply
    // scan the two expected values into the structure.

    sscanf(zpnInfo, "%d %ull\r\n", &zptInst->suwSize, &zptInst->sulTarget);
}

/**************************************************************************//**
*
* \anchor      SS__ParseInput
*
* \brief       Parse the given line
*
* \details     Parses the given line and adds the entry to the input set
*
* \param[in]   zptInst            Problem instance
* \param[in]   zuwIndex            File path
* \param[in]   zpnLine            File path
*
* \retval      void
*
******************************************************************************/

static void SS__Parse_Entry (Subset_Sum_t * zptInst, 
                                      uint32_t zuwIndex, char * zpnLine)
{
    // The format of the first line is [item]\r\n so simply
    // scan the expected values into the structure.
    
    uint32_t xuwtemp;

    sscanf(zpnLine, "%u\r\n", &xuwtemp);

    (zptInst->sauwInputSet)[zuwIndex] = xuwtemp;
}
/**************************************************************************//**
*
* \anchor      Subset_SumWriteData
*
* \brief       Write solution to given file handle.
*
* \details     Prints the solution of the given instance to the given outfile.
*              This can be an actual file handle or 0 for the console.
*
* \param[in]   zptHandle            Problem instance
* \param[in]   ziFd                 File descriptor
*
* \retval      void
*
******************************************************************************/

static void SS__Write (Subset_Sum_t * zptHandle, int ziFd)
{
    char xaucBuffer[80u];
    uint32_t xuwLoop;

    // File header

    sprintf(xaucBuffer, "Input: %s\n", zptHandle->sacName);
    write(ziFd, xaucBuffer, strlen(xaucBuffer));

    sprintf(xaucBuffer, "Target: %llu\n", zptHandle->sulTarget);
    write(ziFd, xaucBuffer, strlen(xaucBuffer));

    sprintf(xaucBuffer, "Size: %d\n", zptHandle->suwSize);
    write(ziFd, xaucBuffer, strlen(xaucBuffer));

    sprintf(xaucBuffer, "Initial: %llu\n", zptHandle->sulInitialSol);
    write(ziFd, xaucBuffer, strlen(xaucBuffer));    
    
    if (Subset_Sum_GetSum(zptHandle) != zptHandle->sulTarget)
    {
      sprintf(xaucBuffer, "Solved: NO, %d seconds, %llu, %.10f\n", zptHandle->suwTime, Subset_Sum_GetSum(zptHandle),
	      Subset_Sum_GetSum(zptHandle) / (float)zptHandle->sulTarget);
      write(ziFd, xaucBuffer, strlen(xaucBuffer));
    }
    else
    {
      sprintf(xaucBuffer, "Solved: YES, %d seconds, %llu, %.10f\n", zptHandle->suwTime, Subset_Sum_GetSum(zptHandle),
	      Subset_Sum_GetSum(zptHandle) / (float)zptHandle->sulTarget);
        write(ziFd, xaucBuffer, strlen(xaucBuffer));

        sprintf(xaucBuffer, "Solution:\n");
        write(ziFd, xaucBuffer, strlen(xaucBuffer));

        for(xuwLoop = 0u; xuwLoop < zptHandle->suwSize; xuwLoop++)
        {
            if(zptHandle->saucSolution[xuwLoop] == INCLUDED)
            {
                sprintf(xaucBuffer, "%d\n", zptHandle->sauwInputSet[xuwLoop]);
                write(ziFd, xaucBuffer, strlen(xaucBuffer));
            }
        }
    }
}

// \}

// \}
