/* Copyright (C) 2013 Ian Karlinsey
 * 
 * UltimeLive is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * UltimaLive is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with UltimaLive.  If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef _CLIENT_REDIRECTIONS_H
#define _CLIENT_REDIRECTIONS_H

#include <windows.h>
#include <string>
#include <iostream>
#include "UoLiveAppState.h"
#include "LocalPeHelper32.hpp"
#include "mhook.h"
#include <stdint.h>
#include "MasterControlUtils.h"
#include "DotNetHost.h"

class UoLiveAppState;

class ClientRedirections
{
  public:
    static HANDLE WINAPI OnCreateFileARunOnce(__in LPCSTR lpFileName, __in DWORD dwDesiredAccess, __in DWORD dwShareMode,
      __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes, __in DWORD dwCreationDisposition, __in DWORD dwFlagsAndAttributes, __in_opt HANDLE hTemplateFile);

    static void SendPacketToServer(unsigned char* pBuffer);
    static void SendPacketToClient(unsigned char* pBuffer);
    static void UpdateStaticBlocks();

  private:
    static DotNetHost* g_pDotNetHost;

    static UoLiveAppState* g_pInstance;

    static HANDLE WINAPI OnCreateFileA(__in LPCSTR lpFileName, __in DWORD dwDesiredAccess, __in DWORD dwShareMode,
      __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes, __in DWORD dwCreationDisposition, __in DWORD dwFlagsAndAttributes, __in_opt HANDLE hTemplateFile);

    static LPVOID WINAPI OnMapViewOfFile(__in HANDLE hFileMappingObject, __in DWORD dwDesiredAccess, __in DWORD dwFileOffsetHigh, __in DWORD dwFileOffsetLow,
      __in SIZE_T dwNumberOfBytesToMap);

    static HANDLE WINAPI OnCreateFileMappingA(__in HANDLE hFile, __in_opt LPSECURITY_ATTRIBUTES lpAttributes, __in DWORD flProtect,
      __in DWORD dwMaximumSizeHigh, __in DWORD dwMaximumSizeLow, __in_opt LPCSTR lpName);

    static int WINAPI OnMessageBox(_In_opt_  HWND hWnd, _In_opt_  LPCTSTR lpText, _In_opt_  LPCTSTR lpCaption, _In_ UINT uType);

    static BOOL WINAPI OnCloseHandle(_In_  HANDLE hObject);

    static void __fastcall OnReceivePacket(void* This, void* edx, unsigned char *pBuffer);
    static int __fastcall OnSendPacket(void* This, void* edx, unsigned char* pBuffer);

    static HANDLE g_UpdateStaticBlocksMutex;
    static void OnUpdateStaticBlocks();

    static void InstallClientHooks();

    static unsigned int* g_pNetworkObject;
    static bool g_hookedInstalled;
};
#endif