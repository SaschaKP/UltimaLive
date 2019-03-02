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

#include "ServerMobileStatusHandler_7_0_29_2.h"
#include "..\NetworkManager.h"

ServerMobileStatusHandler_7_0_29_2::ServerMobileStatusHandler_7_0_29_2(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

bool ServerMobileStatusHandler_7_0_29_2::handlePacket(uint8_t* pPacketData)
{
  m_pNetManager->onServerMobileUpdate();
  return true;
}
