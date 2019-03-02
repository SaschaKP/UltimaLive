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

#include "UltimaLiveUpdateStaticsHandler.h"
#include "..\NetworkManager.h"

UltimaLiveUpdateStaticsHandler::UltimaLiveUpdateStaticsHandler(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

bool UltimaLiveUpdateStaticsHandler::handlePacket(uint8_t* pPacketData)
{
  uint32_t blockNum = ntohl(*reinterpret_cast<uint32_t*>(&pPacketData[3]));
  uint32_t numStatics = ntohl(*reinterpret_cast<uint32_t*>(&pPacketData[7]));
  uint32_t totalBytes = numStatics * 7;
  uint8_t mapNumber = pPacketData[14];

  m_pNetManager->onStaticsUpdate(mapNumber, blockNum, &pPacketData[15], totalBytes);

  return false;
}
