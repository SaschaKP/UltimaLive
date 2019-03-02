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

#include "UltimaLiveUpdateMapDefinitionsHandler.h"
#include "..\NetworkManager.h"

UltimaLiveUpdateMapDefinitionsHandler::UltimaLiveUpdateMapDefinitionsHandler(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

bool UltimaLiveUpdateMapDefinitionsHandler::handlePacket(uint8_t* pPacketData)
{
  uint32_t count = ntohl(*reinterpret_cast<uint32_t*>(&pPacketData[7]));
  int newNumMaps = (count * 7) / 9;
  std::vector<MapDefinition> definitions;
  definitions.reserve(count);

  for (int i = 0; i < newNumMaps; i++)
  {
    int offset = 15 + (i * 9);
    uint8_t mapNumber = (uint8_t)pPacketData[offset]; //iteration byte 000         -  map file index number
    uint16_t width     = ntohs(*reinterpret_cast<uint16_t*>(&pPacketData[offset + 1]));   //iteration byte 001 to 002  -  map width
    uint16_t height    = ntohs(*reinterpret_cast<uint16_t*>(&pPacketData[offset + 3]));  //iteration byte 003 to 004  -  map height
    uint16_t wrapX     = ntohs(*reinterpret_cast<uint16_t*>(&pPacketData[offset + 5]));   //iteration byte 005 to 006  -  wrap around dimension X
    uint16_t wrapY     = ntohs(*reinterpret_cast<uint16_t*>(&pPacketData[offset + 7]));   //iteration byte 007 to 008  -  wrap around dimension Y

    if ((width > 0) && (height > 0) && (wrapX > 0) && (wrapY > 0))
    {
      MapDefinition def;
      def.mapNumber = mapNumber;
      def.mapWidthInTiles = width;
      def.mapHeightInTiles = height;
      def.mapWrapWidthInTiles = wrapX;
      def.mapWrapHeightInTiles = wrapY;
      definitions.push_back(def);
    }
#ifdef DEBUG
    else
    {
      printf("Received bad definition for map %i\n", i);
    }
#endif
  }

  m_pNetManager->onMapDefinitionUpdate(definitions);
  return false;
}
