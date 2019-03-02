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

#ifndef _PE_HELPER_H
#define _PE_HELPER_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdint.h>

#include <list>
#include <Winternl.h>
#include <Psapi.h>
#include <TlHelp32.h>

#pragma comment(lib, "Psapi.lib")
#define DPSAPI_VERSION (1)

/*     
 *  +-------------------------+      
 *  | Import Descriptor       |      
 *  +-------------------------+      
 *  | Characteristics (RVA)   | ---> Hint Name Table     
 *  | TimeDateStamp           |      
 *  | Forwarder Chain         | ---> 0xFFFFFFFF 
 *  | Imported DLL Name (RVA) | ---> Kernel32.dll
 *  | First Thunk (RVA)       | ---> First Thunk Table (Import Address Table)
 *  +-------------------------+
 *
 *
 * There are two parallel arrays of pointers to IMAGE_IMPORT_BY_NAME.  The first array 
 * (pointed to by Characteristics) is left alone, and never modified. It is sometimes
 * called the hint-name table.
 *
 * The second array (pointed to by FirstThunk) is overwritten by the PE loader. The 
 * loader iterates through each pointer in the array and finds the address of the 
 * function that each IMAGE_IMPORT_BY_NAME structure refers to. The loader then
 * overwrites the pointer with the found function's address.
 *
 *  +----------------------+    +----------------------+
 *  | Hint Name Table      |    | Import Address Table |
 *  +----------------------+    +----------------------+
 *  | Hint    | Name       |    | 0x... RVA            |
 *  +----------------------+    +----------------------+
 *  | Hint    | Name       |    | 0x... RVA            |
 *  +----------------------+    +----------------------+
 *  | Hint    | Name       |    | 0x... RVA            |
 *  +----------------------+    +----------------------+
 *  |         NULL         |    |         NULL         |
 *  +----------------------+    +----------------------+
 *
 *  Ord Name
 *   10 BeginPaint
 *   35 CharUpperA
 */

class LocalPeHelper32
{
  public:
 	  static std::list<MODULEENTRY32> GetLocalModuleList();
    static void HexPrint(char* buffer, int size);
    static void HexPrint(unsigned char* buffer, int size);
    static DWORD SetMemoryWritable(DWORD startingAddress);

    LocalPeHelper32(std::string moduleName);
    bool Init();

    bool PatchImportedFunctionAddress(std::string dllName, std::string functionName, DWORD newAddress);
    DWORD getImportedFunctionAddress(std::string dllName, std::string functionName);
    DWORD getExportedFunctionAddress(std::string dllName, std::string functionName);

    std::list<std::string> getImportedFunctionNames(std::string dllName);
    std::list<std::string> getExportedFunctionNames();

private:
    IMAGE_IMPORT_DESCRIPTOR* matchImportDescriptor(std::string dllName);
    IMAGE_EXPORT_DIRECTORY* matchExportDirectory(std::string dllName);
    
    bool m_initialized;
    BYTE* m_pBaseAddress;
    std::string m_moduleName;
    IMAGE_DOS_HEADER* m_pDosHeader;
    IMAGE_NT_HEADERS* m_pNtHeader;
    IMAGE_EXPORT_DIRECTORY* m_pExportDirectory;
    IMAGE_IMPORT_DESCRIPTOR* m_pImportDescriptor;
};

#endif //_PE_HELPER_H
