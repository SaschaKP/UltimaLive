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

#include "Utils.h"
using namespace std;

std::wstring Utils::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

std::string Utils::GetCurrentModuleName()
{
  char fullPath[MAX_PATH_LENGTH]; 
  int moduleNameLength = GetModuleFileName (NULL, fullPath, MAX_PATH_LENGTH);

  char* pBaseName = NULL;
  for (int i = 0; i < moduleNameLength - 1; ++i)
  {
    if (fullPath[i] == '\\' || fullPath[i] == '/')
    {
      pBaseName = &fullPath[i] + 1;
    }
  }

  return std::string(pBaseName);
}

char* Utils::GetCurrentModuleNameC()
{
  char fullPath[MAX_PATH_LENGTH]; 
  int moduleNameLength = GetModuleFileName (NULL, fullPath, MAX_PATH_LENGTH);

  char* pBaseName = NULL;
  for (int i = 0; i < moduleNameLength - 1; ++i)
  {
    if (fullPath[i] == '\\' || fullPath[i] == '/')
    {
      pBaseName = &fullPath[i] + 1;
    }
  }

  return pBaseName;
}


std::string Utils::GetCurrentPathWithoutFilename()
{
  char buffer[MAX_PATH];
  GetModuleFileName( NULL, buffer, MAX_PATH );
  string::size_type pos = string( buffer ).find_last_of( "\\/" );
  return string( buffer ).substr( 0, pos);
}


std::string Utils::GetCurrentModuleFullPath()
{
  char fullPath[MAX_PATH_LENGTH]; 
  int moduleNameLength = GetModuleFileName (NULL, fullPath, MAX_PATH_LENGTH);

  return std::string(fullPath);
}

VS_FIXEDFILEINFO* getFileInfo()
{
  //get file info
  char fileName[_MAX_PATH];
  DWORD size = GetModuleFileName(GetModuleHandle("igrping.dll"), fileName, _MAX_PATH);
  printf("%s\n", fileName);
  fileName[size] = NULL;
  DWORD handle = 0;
  size = GetFileVersionInfoSize(fileName, &handle);
  BYTE* versionInfo = new BYTE[size];
  if (!GetFileVersionInfo(fileName, handle, size, versionInfo))
  {
#ifdef DEBUG
    printf("Failed to get version information\n");
#endif
      delete[] versionInfo;
      return NULL;
  }
  uint32_t len = 0;
  VS_FIXEDFILEINFO* pVersionFileInfo = NULL;
  VerQueryValue(versionInfo, "\\\\", (void**)&pVersionFileInfo, &len);
  delete[] versionInfo;

  return pVersionFileInfo;
}

int Utils::getModuleMinorVersionLower()
{
  int returnValue = 0;
  VS_FIXEDFILEINFO* pVersionFileInfo = getFileInfo();

  if (pVersionFileInfo != NULL)
  {
    returnValue = LOWORD(pVersionFileInfo->dwFileVersionLS);
  }

  return returnValue;
}

int Utils::getModuleMinorVersionUpper()
{
  int returnValue = 0;
  VS_FIXEDFILEINFO* pVersionFileInfo = getFileInfo();

  if (pVersionFileInfo != NULL)
  {
    returnValue = HIWORD(pVersionFileInfo->dwFileVersionLS);
  }

  return returnValue;
}



//taken from http://stackoverflow.com/questions/8046097/how-to-check-if-a-process-has-the-admin-rights
bool Utils::IsElevated( ) 
{
    bool fRet = false;
    HANDLE hToken = NULL;
    if( OpenProcessToken( GetCurrentProcess( ), TOKEN_QUERY, &hToken )) 
    {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize )) 
        {
            fRet = Elevation.TokenIsElevated;
        }
    }

    if( hToken ) 
    {
      CloseHandle( hToken );
    }

    return fRet;
}

void Utils::initializeConsole()
{
  if (AllocConsole())
  {
    AttachConsole(GetCurrentProcessId());

    SetConsoleTitle("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);  
  
	  long lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	  FILE* fp = _fdopen( hConHandle, "w" );

	  *stdout = *fp;
	  setvbuf( stdout, NULL, _IONBF, 0 );
	  ios::sync_with_stdio();
    freopen("CONOUT$", "w", stdout);
  }
}

std::string Utils::strToLower(std::string str)
{
  for (int i = 0; i < str.length(); ++i)
  {
    str[i] = tolower(str[i]);
  }

  return str;
}


std::string Utils::getBaseFilenameWithoutExtension(std::string filename)
{
  std::string filenameToReturn(filename);

  int index = -1;

  for (int i = filename.length() - 1; i > 0; --i)
  {
    if (filename[i] == '.')
    {
      index = i;
      break;
    }
  }

  if (index >= 0)
  {
    filenameToReturn = filename.substr(0, index);
  }

  return filenameToReturn;
}

std::string Utils::getFilenameFromPath(std::string path)
{
  std::string filenameToReturn;

  int startIdx = -1;


  for (int i = path.length() -1; i > 0; --i)
  {
    if (path[i] == '\\')
    {
      startIdx = i + 1;
      break;
    }
  }

  if (startIdx >= 0)
  {
    filenameToReturn = path.substr(startIdx, path.length() - 1);
  }

  return filenameToReturn;
}