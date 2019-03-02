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
#ifdef DEBUG

#include "ClientCrashPacketHandler.h"
#include "..\NetworkManager.h"

ClientCrashPacketHandler::ClientCrashPacketHandler(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

/* Handles the map change packet from the server 
 *
 * Packet Definition:
 *   byte 00            Packet ID: 0xBF (Extended Command)
 *   byte 01 - 02       Packet Size 
 *   byte 03 - 04       Extended Command ID: 0x08
 *   byte 05            Map ID
 */
bool ClientCrashPacketHandler::handlePacket(uint8_t* pPacketData)
{
  printf("Client crash!!!\n");
  return true;
}

#endif