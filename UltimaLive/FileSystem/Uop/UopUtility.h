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


#ifndef _UOP_UTILITIES_H
#define _UOP_UTILITIES_H
#include <stdint.h>
#include <string>
#include <map>
#include <list>
#include <sstream>
#include <fstream>
#include "UopStructs.h"
#include "..\..\Utils.h"
#include <algorithm>

class ProgressBarDialog;

class UopUtility
{
  public:
    static uint64_t HashFileName(std::string s);
    static std::map<uint32_t, uint64_t>* getMapHashes(int count, std::string pattern);
    static uint32_t getUopMapSizeInBytes(std::string filename);
    static void convertUopMapToMul(std::string uopSourceFilename, std::string uopDestFilename, ProgressBarDialog* pProgress);
};

#endif
