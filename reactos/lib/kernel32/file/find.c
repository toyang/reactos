/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            lib/kernel32/file/find.c
 * PURPOSE:         Find functions
 * PROGRAMMER:      Ariadne ( ariadne@xs4all.nl)
 * UPDATE HISTORY:
 *                  Created 01/11/98
 */

/* INCLUDES *****************************************************************/

#include <windows.h>
#include <wstring.h>
#include <ddk/ntddk.h>

/* TYPES ********************************************************************/

typedef struct _KERNEL32_FIND_FILE_DATA
{
   HANDLE DirectoryHandle;
   FILE_DIRECTORY_INFORMATION FileInfo;
} KERNEL32_FIND_FILE_DATA, *PKERNEL32_FIND_FILE_DATA;

/* FUNCTIONS *****************************************************************/

HANDLE FindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
   WCHAR lpFileNameW[MAX_PATH];
   ULONG i;
   
   i = 0;
   while (lpFileName[i]!=0)
     {
        lpFileNameW[i] = lpFileName[i];
	i++;
     }
   
   return(FindFirstFileW(lpFileNameW,lpFindFileData));
}

WINBOOL FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
   return(FindNextFileW(hFindFile, lpFindFileData));
}

BOOL FindClose(HANDLE hFindFile)
{
   PKERNEL32_FIND_FILE_DATA IData;
   
   IData = (PKERNEL32_FIND_FILE_DATA)hFindFile;
   NtClose(IData->DirectoryHandle);
   HeapFree(GetProcessHeap(), 0, IData);
   return(TRUE);
}

HANDLE STDCALL FindFirstFileW(LPCWSTR lpFileName, 
			      LPWIN32_FIND_DATA lpFindFileData)
{
   WCHAR CurrentDirectory[MAX_PATH];
   WCHAR Pattern[MAX_PATH];
   WCHAR Directory[MAX_PATH];
   PWSTR End;
   PKERNEL32_FIND_FILE_DATA IData;
   OBJECT_ATTRIBUTES ObjectAttributes;
   UNICODE_STRING DirectoryNameStr;
   IO_STATUS_BLOCK IoStatusBlock;
   UNICODE_STRING PatternStr;
   
   dprintf("FindFirstFileW(lpFileName %w, lpFindFileData %x)\n",
	   lpFileName, lpFindFileData);
   
   GetCurrentDirectoryW(MAX_PATH, CurrentDirectory);
   Directory[0] = '\\';
   Directory[1] = '?';
   Directory[2] = '?';
   Directory[3] = '\\';
   Directory[4] = 0;
   wcscat(Directory, CurrentDirectory);
   wcscat(Directory, lpFileName);
   End = wcschr(Directory, '\\');
   *End = 0;
   
   wcscpy(Pattern, End+1);
   
   dprintf("Directory %w End %w\n",Directory,End);
   
   IData = HeapAlloc(GetProcessHeap(), 
		     HEAP_ZERO_MEMORY, 
		     sizeof(KERNEL32_FIND_FILE_DATA));
   
   RtlInitUnicodeString(&DirectoryNameStr, Directory);
   InitializeObjectAttributes(&ObjectAttributes,
			      &DirectoryNameStr,
			      0,
			      NULL,
			      NULL);
   
   if (ZwOpenFile(&IData->DirectoryHandle,
		  FILE_TRAVERSE,		 
		  &ObjectAttributes,
		  &IoStatusBlock,
		  0,
		  OPEN_EXISTING)!=STATUS_SUCCESS)
     {
	return(NULL);
     }
   
   RtlInitUnicodeString(&PatternStr, Pattern);
   
   NtQueryDirectoryFile(IData->DirectoryHandle,
			NULL,
			NULL,
			NULL,
			&IoStatusBlock,
			(PVOID)&IData->FileInfo,
			sizeof(IData->FileInfo),
			FileDirectoryInformation,
			TRUE,
			&PatternStr,
			FALSE);
   
   return(IData);
}

WINBOOL STDCALL FindNextFileW(HANDLE hFindFile,
			      LPWIN32_FIND_DATA lpFindFileData)
{
}
