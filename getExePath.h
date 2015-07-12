/*
* Copyright 2015 Justin Adams. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted provided
* that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
* the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
* and the following disclaimer in the documentation and/or other materials provided with the
* distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
* THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef  GETEXEPATH_H
#define  GETEXEPATH_H  12345


#include <Windows.h>
#include <stdio.h>
#include <memory.h>

#define  TEMP_BUF_LEN       1024

#define  GEP_EXE              10
#define  GEP_DIR              20
#define  GEP_PARENT           30
#define  GEP_DRIVE            35

#define  GEP_PATH_TOO_LONG    40
#define  GEP_NO_DRIVE         45
#define  GEP_UNKNOWN_WHICH    60
#define  GEP_SUCCESS          80
#define  GEP_NULL_PTR         90
#define  GEP_DIR_NOT_FOUND   100

/*
Finds path of the currently running executable, similar to /proc/self/exe in Linux.
Writes result into buf. buf should already be allocated.
Results do not contain trailing backslash.
getWhich = GEP_EXE:    full executbale path
getWhich = GEP_DIR:    full path of directory containing excutable
getWhich = GEP_PARENT: full path of parent of directory containing executable
getWhich = GEP_DRIVE:  drive letter (includes colon character)
*/
int getExePath(int getWhich, TCHAR* buf, size_t bufLen)
{
	TCHAR tc[TEMP_BUF_LEN];
	TCHAR curr;
	unsigned int slashCount;
	DWORD errorCode, gmfnReturn, lastIndex;

	if(buf == NULL)
	{
		fprintf(stderr, "getExePath(): null pointer.\n");
		return GEP_NULL_PTR;
	}

	gmfnReturn= GetModuleFileName(NULL, (LPTSTR) tc, TEMP_BUF_LEN);
	errorCode= GetLastError();

	// If GetModuleFileName does not succeed.
	if(gmfnReturn == 0)
	{
		// Some unknown reason.
		if(errorCode == 0)
		{
			errorCode= GetLastError();
			fprintf(stderr, "getExePath(): GetModuleFileName() not successful, error code was %d.\n", errorCode);
			return errorCode;
		}

		// If path does not fit into tc.
		if(errorCode == ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf(stderr, "getExePath(): Temp buffer length (%d) is too short.\n", TEMP_BUF_LEN);
			return GEP_PATH_TOO_LONG;
		}
	}

	switch(getWhich)
	{
		case GEP_EXE:
			lastIndex= gmfnReturn - 1;
			break;

		case GEP_DIR:
		case GEP_PARENT:
			for(lastIndex= gmfnReturn - 1, slashCount= 0; (lastIndex > 1) && (slashCount < 3); lastIndex--)
			{
				curr= tc[lastIndex];
				if(((char) curr) == '\\')
				{
					slashCount++;

					if((slashCount == 1 && getWhich == GEP_DIR) || (slashCount == 2 && getWhich == GEP_PARENT))
					{
						if(lastIndex < 2)
						{
							fprintf(stderr, "getExePath(): Could not find directory.\n");
							return GEP_DIR_NOT_FOUND;
						}

						lastIndex--;
						break;
					}
				}
			}
			if((getWhich == GEP_DIR && slashCount != 1) || (getWhich == GEP_PARENT && slashCount != 2) || (slashCount > 2))
			{
				fprintf(stderr, "getExePath(): Could not find directory.\n");
				return GEP_DIR_NOT_FOUND;
			}
			break;

		case GEP_DRIVE:
			if(((char) tc[1]) == ':')
				lastIndex= 1;
			else
			{
				fprintf(stderr, "getExePath(): Could not find drive letter.\n");
				return GEP_NO_DRIVE;
			}
			break;

		default:
			fprintf(stderr, "getExePath(): Uknown 'which' code (%d).\n", getWhich);
			return GEP_UNKNOWN_WHICH;
	};

	if(lastIndex > bufLen - 2)
	{
		fprintf(stderr, "getExePath(): Buffer length (%d) is not enough to hold result of (%d).\n", bufLen, lastIndex);
		return GEP_PATH_TOO_LONG;
	}

	memset(buf, 0, bufLen * sizeof(TCHAR));
	memcpy(buf, tc, (lastIndex + 1) * sizeof(TCHAR));

	return GEP_SUCCESS;
}


#endif