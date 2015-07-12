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


#include <stdio.h>
#include <TCHAR.H>

#include "getExePath.h"


#define BUFFER_LENGTH 512


int main(int argc, char** argv)
{
	TCHAR buf[BUFFER_LENGTH];
	int errorCode, i, which;

	for(i= 0; i < 4; i++)
	{
		switch(i)
		{
			case 0: which= GEP_EXE;    break;
			case 1: which= GEP_DIR;    break;
			case 2: which= GEP_PARENT; break;
			case 3: which= GEP_DRIVE;  break;
		};

		memset(buf, 0, BUFFER_LENGTH);
		errorCode= getExePath(which, buf, BUFFER_LENGTH);
		_tprintf(_T("errorCode <%d>\n"), errorCode);
		_tprintf(_T("result    <%s>\n\n"), buf);
	}

	printf("\n");
	return 0;
}