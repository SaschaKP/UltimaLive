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


#ifndef _MAP_FILE_SET_H
#define _MAP_FILE_SET_H

#include <string>
#include <windows.h>
#include <stdint.h>

class MapFileSet
{
  public:
    MapFileSet(uint32_t indexIn, std::string mapFilenameIn, std::string staticsFilenameIn, std::string staidxFilenameIn);

    uint32_t index;

    std::string mapFilename;
    std::string mapFilePath;
    HANDLE mapHandle;

    std::string staticsFilename;
    std::string staticsFilePath;
    HANDLE staticsHandle;

    std::string staidxFilename;
    std::string staidxFilePath;
    HANDLE staidxHandle;
};

#endif