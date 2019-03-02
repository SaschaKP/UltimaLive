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

#ifndef _PACKET_HANDLER_FACTORY_H
#define _PACKET_HANDLER_FACTORY_H

#include <map>
#include <stdint.h>
#include "BasePacketHandler.h"

class PacketHandlerFactory
{
  public:
    static std::map<uint8_t, BasePacketHandler*> GenerateClientPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager);
    static std::map<uint8_t, BasePacketHandler*> GenerateClientExtendedPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager);
    static std::map<uint8_t, BasePacketHandler*> GenerateServerPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager);
    static std::map<uint8_t, BasePacketHandler*> GenerateServerExtendedPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager);
    static std::map<uint8_t, BasePacketHandler*> GenerateUltimaLiveServerPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager);
};
#endif