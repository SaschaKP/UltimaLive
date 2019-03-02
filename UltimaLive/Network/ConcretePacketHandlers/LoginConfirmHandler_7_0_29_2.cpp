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

#include "LoginConfirmHandler_7_0_29_2.h"
#include "..\NetworkManager.h"

LoginConfirmHandler_7_0_29_2::LoginConfirmHandler_7_0_29_2(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

/*
  byte 00         cmd
  byte 01 - 04    serial
  byte 05 - 08    00 00 00 00
  byte 09 - 10    Body
  byte 11 - 12    X
  byte 13 - 14    Y
  byte 15 - 16    Z
  byte 17         dir
  byte 18         00
  byte 19 - 22    FF FF FF FF
  byte 23 - 26    00 00 00 00
  byte 27 - 28    width
  byte 29 - 30    height
  byte 31 - 36    00 00 00 00 00 00
*/
bool LoginConfirmHandler_7_0_29_2::handlePacket(uint8_t* pPacketData)
{
  m_pNetManager->onLoginConfirm(pPacketData);
  return true;
}
