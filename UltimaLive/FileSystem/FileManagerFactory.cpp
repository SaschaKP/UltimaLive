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

#include "FileManagerFactory.h"
#include "ConcreteFileManagers\FileManager_7_0_29_2.h"
#include "ConcreteFileManagers\FileManager.h"

BaseFileManager* FileManagerFactory::CreateFileManager(uint32_t versionMajor, uint32_t versionMinor)
{
  //printf("Version: 0x%x, 0x%x\n", versionMajor, versionMinor);
  int a = (int)(versionMajor >> 16);
  int b = (int)(versionMajor & 0xFFFF);
  int c = (int)(versionMinor >> 16);
  int d = (int)(versionMinor & 0xFFFF);

#ifdef DEBUG
  printf("version = %i, %i, %i, %i = 0x%x, 0x%x\n", a, b, c, d, versionMajor, versionMinor);
#endif

  BaseFileManager* pFileManager = NULL;

  //7.0.24.2 0007 0000 0018 0002

  if (versionMajor < 0x70000 || versionMinor < 0x180002)
  {
    pFileManager = new FileManager();

#ifdef DEBUG
    printf("Using older client FileManager\n");
#endif
  }
  else
  {
    pFileManager = new FileManager_7_0_29_2();

#ifdef DEBUG
    printf("Using FileManager 7.0.29.2\n");
#endif
  }

  return pFileManager;
}