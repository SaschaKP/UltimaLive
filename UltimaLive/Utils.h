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

#ifndef _UTILS_H
#define _UTILS_H

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "resource.h"
#include "ProgressBarDialog.h"
//#include "SplashScreen.h"

class Utils
{
  public:
    static const int MAX_PATH_LENGTH = 2048;
    static std::string GetCurrentModuleName();
	static char* GetCurrentModuleNameC();
    static std::string GetCurrentModuleFullPath();
    static std::string GetCurrentPathWithoutFilename();
    static void initializeConsole();
    static bool IsElevated();
    static std::string getFilenameFromPath(std::string path);
    static std::string getBaseFilenameWithoutExtension(std::string filename);
    static std::string strToLower(std::string str);

    static int getModuleMinorVersionLower();
    static int getModuleMinorVersionUpper();

    static void displaySplashScreen();

    static std::wstring s2ws(const std::string& s);
};

#endif