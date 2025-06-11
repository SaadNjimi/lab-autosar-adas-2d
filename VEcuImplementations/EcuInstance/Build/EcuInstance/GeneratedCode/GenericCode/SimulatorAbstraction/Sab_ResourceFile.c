/**************************************************************************************************\
 *** Sab_ResourceFile.c
 ***
 *** Implementation the GetResourceFullPathName() for VEOS using the Windows API.
 ***
 *** Copyright 2021 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_RESOURCE_FILE_C
#define SAB_RESOURCE_FILE_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Windows.h"


/*------------------------------------------------------------------------------------------------*\
  PARAMETRIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetResourceLocationFullPathName
 ***
 *** DESCRIPTION:
 ***     Returns the root directory on the target platform where the resource files of the V-ECU are stored. 
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     
 ***
 *** RETURNS:
 ***     char*   The root directory where the resource files of the V-ECU application process are located.
 \**************************************************************************************************/
char* Sab_GetResourceLocationFullPathName(void)
{
    wchar_t resourcePathW[MAX_PATH];
    static char resourcePath[MAX_PATH];

    if (GetCurrentDirectoryW(MAX_PATH, resourcePathW) == 0)
    {
        return NULL;        
    }

    if (WideCharToMultiByte(CP_UTF8, 0, resourcePathW, -1, resourcePath, MAX_PATH, NULL, NULL) == 0)
    {
        return NULL;
    }

    return resourcePath;
}


#endif /* SAB_RESOURCE_FILE_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
