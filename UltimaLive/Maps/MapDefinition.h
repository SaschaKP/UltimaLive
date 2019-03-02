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

#ifndef _MAP_DEFINITION_H
#define _MAP_DEFINITION_H

#include <Windows.h>

class MapDefinition
{
  public:
    uint16_t mapWidthInTiles;
    uint16_t mapHeightInTiles;
    uint16_t mapWrapWidthInTiles;
    uint16_t mapWrapHeightInTiles;
    uint8_t mapNumber;

    uint32_t TotalNumberOfBlocks() { return (mapWidthInTiles >> 3) * (mapHeightInTiles >> 3); }
};

#endif