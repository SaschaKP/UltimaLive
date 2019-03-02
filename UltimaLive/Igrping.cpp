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

#include "Igrping.h"
using namespace std;

typedef bool (__cdecl *SendPingMessageSignature)(char*, int, char*, char*, int);

//mimic methods exported by the original Igrping.DLL
extern "C"
__declspec(dllexport) CIgr_pinger& __thiscall CIgr_pinger::operator=(CIgr_pinger const &rOther)
{
#ifdef DEBUG
  printf("IGRPING = OPERATOR CALLED\n");
#endif
  return (CIgr_pinger&)rOther;
}

//mimic methods exported by the original Igrping.DLL
extern "C"
__declspec(dllexport) 
CIgr_pinger::CIgr_pinger()
{
#ifdef DEBUG
  printf("IGRPING CONSTRUCTOR CALLED\n");
#endif
  //do nothing
}

//mimic methods exported by the original Igrping.DLL
__declspec(dllexport)
bool __cdecl SendPingMessage(char* a, int b, char* c, char* d, int e)
{
#ifdef DEBUG
  printf("IGRPING SEND PING MESSAGE\n");
#endif

	HMODULE hLib = 0L;
	SendPingMessageSignature hProc = 0L;
    bool ret = false;

	hLib = LoadLibrary("igrping_.dll");

	if (hLib != 0L)
	{
		hProc = (SendPingMessageSignature) GetProcAddress(hLib,
		"SendPingMessage");

		if (hProc != 0L)
		{
			ret = hProc(a, b, c, d, e);
		}
		FreeLibrary(hLib);
	}

	return ret;
}

bool CIgr_pinger::g_firstRun = true;

/* The processing in this method should not be done here because a deadlock can be 
 * caused by Lock Order Inversion.
 *
 * See: http://download.microsoft.com/download/a/f/7/af7777e5-7dcd-4800-8a0a-b18336565f5b/dll_bestprac.doc
 *
 * The reason that this risk is being taken is to avoid having the need for a dedicated loading program
 * that would load the exe suspended, spin it up, insert the proper hooks and resume it. The strategy to inject
 * UOLive code into the client uses redirection.  This strategy could easily be changed if a dedicated 
 * loader were used.
 *
 * DLL redirection was chosen so that other client assistance programs could continue to work despite
 * UOLive.
 *
 * The purpose of this function is to hook the kernel32.dll CreateFileA which is used to open all the client files.
 * All other initialization will be called during the first call to the hooked CreateFileA method to avoid unnecesary 
 * risk in this function.  
 */
BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
	if (CIgr_pinger::g_firstRun == true)
	{
    LocalPeHelper32 ultimaHelper(Utils::GetCurrentModuleName());

    ultimaHelper.Init();
    list<string> importedFunctionNames = ultimaHelper.getImportedFunctionNames("kernel32.dll");

    if (importedFunctionNames.size() > 0)
    {
      CIgr_pinger::g_firstRun = false;
      ultimaHelper.PatchImportedFunctionAddress("kernel32.dll", "CreateFileA", (DWORD)&ClientRedirections::OnCreateFileARunOnce);
    }
	}

	return TRUE;
}
