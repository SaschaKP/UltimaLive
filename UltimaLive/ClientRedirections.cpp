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

#include "ClientRedirections.h"
using namespace std;

UoLiveAppState* ClientRedirections::g_pInstance = NULL;
unsigned int* ClientRedirections::g_pNetworkObject = NULL;
bool ClientRedirections::g_hookedInstalled = false;
DotNetHost* ClientRedirections::g_pDotNetHost = NULL;

HANDLE ClientRedirections::g_UpdateStaticBlocksMutex = NULL;


HANDLE WINAPI ClientRedirections::OnCreateFileARunOnce(
  __in      LPCSTR lpFileName,
  __in      DWORD dwDesiredAccess,
  __in      DWORD dwShareMode,
  __in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  __in      DWORD dwCreationDisposition,
  __in      DWORD dwFlagsAndAttributes,
  __in_opt  HANDLE hTemplateFile
)
{
  ClientRedirections::InstallClientHooks();
#if defined DEBUG && defined DOTNET
  printf("Launching .NET");
  g_pDotNetHost = new DotNetHost();
  g_pDotNetHost->LaunchCLR();
  
  uint32_t result = g_pDotNetHost->LaunchNetAssembly("UltimaLiveObjectViewer.dll", "UltimaLiveObjectViewer.ObjectViewer", "ShowMainWindow", "asdf");
#endif
  return OnCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WINAPI ClientRedirections::OnCreateFileA(
  __in      LPCSTR lpFileName,
  __in      DWORD dwDesiredAccess,
  __in      DWORD dwShareMode,
  __in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  __in      DWORD dwCreationDisposition,
  __in      DWORD dwFlagsAndAttributes,
  __in_opt  HANDLE hTemplateFile
)
{
  if (g_hookedInstalled)
  {
    return g_pInstance->GetFileManager()->OnCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
  }

  return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

LPVOID WINAPI ClientRedirections::OnMapViewOfFile(
  __in  HANDLE hFileMappingObject,
  __in  DWORD dwDesiredAccess,
  __in  DWORD dwFileOffsetHigh,
  __in  DWORD dwFileOffsetLow,
  __in  SIZE_T dwNumberOfBytesToMap
)
{
  if (g_hookedInstalled)
  {
    BaseFileManager* pManager = g_pInstance->GetFileManager();
    return g_pInstance->GetFileManager()->OnMapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
  }

  return MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
}

int WINAPI ClientRedirections::OnMessageBox(
  _In_opt_  HWND hWnd,
  _In_opt_  LPCTSTR lpText,
  _In_opt_  LPCTSTR lpCaption,
  _In_      UINT uType
)
{
#ifdef DEBUG
  printf("========================================================================================================\n");
  printf("MessageBox: %s \n", lpCaption);
  printf("========================================================================================================\n");
  printf("%s\n", lpText);
  printf("========================================================================================================\n");
#endif

  return IDOK;
}


HANDLE WINAPI ClientRedirections::OnCreateFileMappingA(
  __in      HANDLE hFile,
  __in_opt  LPSECURITY_ATTRIBUTES lpAttributes,
  __in      DWORD flProtect,
  __in      DWORD dwMaximumSizeHigh,
  __in      DWORD dwMaximumSizeLow,
  __in_opt  LPCSTR lpName
)
{
  if (g_hookedInstalled)
  {
    return g_pInstance->GetFileManager()->OnCreateFileMappingA(hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
  }

  return CreateFileMappingA(hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
}


BOOL WINAPI ClientRedirections::OnCloseHandle(_In_  HANDLE hObject)
{
  if (g_hookedInstalled)
  {
    g_pInstance->GetFileManager()->OnCloseHandle(hObject);
  }

  return CloseHandle(hObject);
}

typedef int (__thiscall* TrueSendMethod)(void* This, unsigned char* pBuffer);
TrueSendMethod sendFunctionPtr;
int __fastcall ClientRedirections::OnSendPacket(void* This, void* edx, unsigned char* pBuffer)
{
  if (g_pInstance->GetNetworkManager()->OnSendPacket(pBuffer) == true)
  {
    int retValue = sendFunctionPtr(This, pBuffer);

    return retValue;
  }

  return 0;
}

typedef void (__thiscall* TrueRecvMethod)(void* This, unsigned char* pBuffer);
TrueRecvMethod recvFunctionPtr;
void __fastcall ClientRedirections::OnReceivePacket(void* This, void* edx, unsigned char* pBuffer)
{
  if (g_pInstance->GetNetworkManager()->OnReceivePacket(pBuffer) == true)
  {
    recvFunctionPtr(This, pBuffer);
  }
}

typedef void (__cdecl* TrueUpdateStaticBlocksMethod)();
TrueUpdateStaticBlocksMethod updateBlocksFunctionPtr;

void ClientRedirections::UpdateStaticBlocks()
{
  uint32_t dwWaitResult = WaitForSingleObject(g_UpdateStaticBlocksMutex, INFINITE); 
  updateBlocksFunctionPtr();
  ReleaseMutex(g_UpdateStaticBlocksMutex);
}

void ClientRedirections::OnUpdateStaticBlocks()
{
  uint32_t dwWaitResult = WaitForSingleObject(g_UpdateStaticBlocksMutex, INFINITE); 
  updateBlocksFunctionPtr();
  ReleaseMutex(g_UpdateStaticBlocksMutex);
}


void ClientRedirections::InstallClientHooks()
{
#ifdef DEBUG
  Utils::initializeConsole();
#endif

  g_UpdateStaticBlocksMutex = CreateMutex(NULL, false, NULL);

  g_hookedInstalled = true;

  //remap CreateFileA to avoid calling this multiple times
  LocalPeHelper32 ultimaHelper(Utils::GetCurrentModuleName());    
  ultimaHelper.Init();
  /*if(ultimaHelper.m_moduleName!="client.exe")
  {
	  g_hookedInstalled = false;
	  return;
  }*/

  if (!ultimaHelper.PatchImportedFunctionAddress("kernel32.dll", "CreateFileA", (DWORD)&ClientRedirections::OnCreateFileA))
  {
#ifdef DEBUG    
    printf("Failed to hook OnCreateFileA\n");
#endif
  }

  if (!ultimaHelper.PatchImportedFunctionAddress("user32.dll", "MessageBoxA", (DWORD)&ClientRedirections::OnMessageBox))
  {
#ifdef DEBUG    
    printf("Failed to hook MessageBox\n");
#endif
  }


  //search for client pointers
  updateBlocksFunctionPtr = (TrueUpdateStaticBlocksMethod) MasterControlUtils::GetUpdateBlocksFunctionPointer();
  sendFunctionPtr = (TrueSendMethod) MasterControlUtils::GetSendAddress();
  recvFunctionPtr = (TrueRecvMethod) MasterControlUtils::GetRecvAddress();
  g_pNetworkObject = MasterControlUtils::GetNetworkObjectAddress();
  unsigned char* pPlayerBasePointer = MasterControlUtils::GetPlayerBasePointer();

#ifdef DEBUG
  printf("Installing Client Hooks\n");
  printf("RECEIVE ADDRESS 0x%x\n", recvFunctionPtr);
  printf("SEND ADDRESS 0x%x\n", sendFunctionPtr);
  printf("PLAYER BASE POINTER: 0x%x\n", pPlayerBasePointer);
  printf("NETWORK ADDRESS POINTER: 0x%x\n", g_pNetworkObject);
  printf("UPDATE STATIC BLOCKS ADDRESS POINTER: 0x%x\n", updateBlocksFunctionPtr);
#endif

  if (sendFunctionPtr == NULL) 
  {
#ifdef DEBUG
    printf("!!!!!! Unable to find client send function!\n");
#endif
    g_hookedInstalled = false;
    return;
  }

  if (updateBlocksFunctionPtr == NULL) 
  {
#ifdef DEBUG
    printf("!!!!!! Unable to find client update blocks function!\n");
#endif
    g_hookedInstalled = false;
    return;
  }

  if (recvFunctionPtr == NULL) 
  {
#ifdef DEBUG
    printf("!!!!!! Unable to find client receive function!\n");
#endif
    g_hookedInstalled = false;
    return;
  }

  if (pPlayerBasePointer == NULL) 
  {
#ifdef DEBUG
    printf("!!!!!! Unable to find client player base pointer!\n");
#endif
    g_hookedInstalled = false;
    return;
  }

  if (g_pNetworkObject == NULL) 
  {
#ifdef DEBUG
    printf("!!!!!! Unable to find client network object!\n");
#endif
    g_hookedInstalled = false;
    return;
  }

  //if(ultimaHelper.


  g_pInstance = new UoLiveAppState();
  g_pInstance->Init();

  BOOL bHook = Mhook_SetHook((PVOID*)&sendFunctionPtr, 
			ClientRedirections::OnSendPacket);

  bHook = Mhook_SetHook((PVOID*)&updateBlocksFunctionPtr, 
			ClientRedirections::OnUpdateStaticBlocks);

  g_pInstance->setClientPlayerStructure(pPlayerBasePointer);

#ifdef DEBUG
  if (!bHook)
  {
    printf("Failed to hook!\n");
  }
#endif

  bHook = Mhook_SetHook((PVOID*)&recvFunctionPtr, 
			ClientRedirections::OnReceivePacket);

#ifdef DEBUG
  if (!bHook)
  {
    printf("Failed to hook!\n");
  }
#endif

  if (!ultimaHelper.PatchImportedFunctionAddress("kernel32.dll", "CreateFileMappingA", (DWORD)&ClientRedirections::OnCreateFileMappingA))
  {
#ifdef DEBUG
    printf("Failed to hook CreateFileMappingA\n");
#endif
  }
    
  if (!ultimaHelper.PatchImportedFunctionAddress("kernel32.dll", "CloseHandle", (DWORD)&ClientRedirections::OnCloseHandle))
  {
#ifdef DEBUG
    printf("Failed to hook CloseHandle\n");
#endif
  }

  if (!ultimaHelper.PatchImportedFunctionAddress("kernel32.dll", "MapViewOfFile", (DWORD)&ClientRedirections::OnMapViewOfFile))
  {
#ifdef DEBUG
    printf("Failed to hook CreateFileMappingA\n");
#endif
  }

  g_hookedInstalled = true;
}

void ClientRedirections::SendPacketToServer(unsigned char* pBuffer)
{
  sendFunctionPtr(reinterpret_cast<void*>(*g_pNetworkObject), pBuffer);
}

void ClientRedirections::SendPacketToClient(unsigned char* pBuffer)
{
  printf("Sending Packet to Client!\n");
  recvFunctionPtr(reinterpret_cast<void*>(*g_pNetworkObject), pBuffer);
}