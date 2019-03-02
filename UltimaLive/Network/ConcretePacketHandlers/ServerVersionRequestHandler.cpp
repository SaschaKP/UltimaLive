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

//#include "ServerVersionRequestHandler.h"
//#include "..\NetworkManager.h"
/*
ServerVersionRequestHandler::ServerVersionRequestHandler(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

bool ServerVersionRequestHandler::handlePacket(uint8_t* pPacketData)
{
  uint16_t major = Utils::getModuleMinorVersionUpper();
  uint16_t minor = Utils::getModuleMinorVersionLower();

  //build packet
  uint8_t* pResponse = new uint8_t[21];
  pResponse[0] = 0x3F;                                               //byte 000              -  cmd
  *reinterpret_cast<uint16_t*>(pResponse + 1) = 21;                  //byte 001 through 002  -  packet size
  *reinterpret_cast<uint32_t*>(pResponse + 3) = 0x00000000;          //byte 003 through 006  -  no blocks associated with this packet
  *reinterpret_cast<uint32_t*>(pResponse + 7) = htonl(1);            //byte 007 through 010  -  number of statics in the packet (8 for a query response)          
  pResponse[11] = 0x00;                                              //byte 011              -  UltimaLive sequence number
  pResponse[12] = 0x00;                                              //byte 012              -  UltimaLive sequence number
  pResponse[13] = 0xFE;                                              //byte 013              -  UltimaLive command (0xFE is a send UltimaLive Version Number)
  pResponse[14] = 0;                                                 //byte 014              -  No Map Associated with this packet
  *reinterpret_cast<uint16_t*>(pResponse + 15) = htons(major);                  //byte 015 through 016  -  Major Version Number  
  *reinterpret_cast<uint16_t*>(pResponse + 17) = htons(minor);                 //byte 017 through 018  -  Minor Version Number
  pResponse[19] = 0;
  pResponse[20] = 0;

  #ifdef DEBUG
    printf("Sending UltimaLive Version Number %u.%u\n", major, minor);
  #endif

  m_pManager->sendPacketToServer(pResponse);
  return true;
}
*/