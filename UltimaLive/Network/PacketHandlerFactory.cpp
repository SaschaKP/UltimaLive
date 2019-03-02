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

#include "PacketHandlerFactory.h"
#include "ConcretePacketHandlers\ChangeMapHandler_7_0_29_2.h"
#include "ConcretePacketHandlers\LoginCompleteHandler_7_0_29_2.h"
#include "ConcretePacketHandlers\LogoutRequestHandler_7_0_29_2.h"
#include "ConcretePacketHandlers\LoginConfirmHandler_7_0_29_2.h"
#include "ConcretePacketHandlers\UltimaLiveRefreshClientViewHandler.h"
#include "ConcretePacketHandlers\UltimaLiveUpdateMapDefinitionsHandler.h"
#include "ConcretePacketHandlers\UltimaLiveUpdateStaticsHandler.h"
#include "ConcretePacketHandlers\UltimaLiveHashQueryHandler.h"
#include "ConcretePacketHandlers\UltimaLiveUpdateLandBlockHandler.h"
#include "ConcretePacketHandlers\UltimaLiveLoginCompleteHandler.h"
#include "ConcretePacketHandlers\UltimaLiveCRC32RequestHandler.h"
#include "ConcretePacketHandlers\ServerMobileStatusHandler_7_0_29_2.h"
//#include "ConcretePacketHandlers\ServerVersionRequestHandler.h"
#include "ConcretePacketHandlers\ClientCrashPacketHandler.h"
#include "ConcretePacketHandlers\MovementRequestHandler.h"
#include "ConcretePacketHandlers\AttackRequestHandler.h"

std::map<uint8_t, BasePacketHandler*> PacketHandlerFactory::GenerateClientPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager)
{
  std::map<uint8_t, BasePacketHandler*> handlers;
  //if (version.compare("7.0.29.2") == 0)
  {
    handlers[0x01] = new LogoutRequestHandler_7_0_29_2(pManager);
	handlers[0x02] = new MovementRequestHandler(pManager);
	handlers[0x05] = new AttackRequestHandler(pManager);
#ifdef DEBUG
    handlers[0xF4] = new ClientCrashPacketHandler(pManager);
#endif

  }

  return handlers;
}

std::map<uint8_t, BasePacketHandler*> PacketHandlerFactory::GenerateUltimaLiveServerPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager)
{
  std::map<uint8_t, BasePacketHandler*> handlers;
  //if (version.compare("7.0.29.2") == 0)
  {
    handlers[0x00] = new UltimaLiveUpdateStaticsHandler(pManager);
    handlers[0x01] = new UltimaLiveUpdateMapDefinitionsHandler(pManager);
    handlers[0x02] = new UltimaLiveLoginCompleteHandler(pManager);
    handlers[0x03] = new UltimaLiveRefreshClientViewHandler(pManager);
    handlers[0xF0] = new UltimaLiveCRC32RequestHandler(pManager);
	handlers[0xF1] = new UltimaLiveProcessesRequestHandler(pManager);
    handlers[0xFF] = new UltimaLiveHashQueryHandler(pManager);
  }

  return handlers;
}

std::map<uint8_t, BasePacketHandler*> PacketHandlerFactory::GenerateClientExtendedPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager)
{
  std::map<uint8_t, BasePacketHandler*> handlers;
  //if (version.compare("7.0.29.2") == 0)
  {
  }

  return handlers;
}

std::map<uint8_t, BasePacketHandler*> PacketHandlerFactory::GenerateServerPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager)
{
  std::map<uint8_t, BasePacketHandler*> handlers;
  //if (version.compare(std::string("7.0.29.2")) == 0 )
  {
    handlers[0x11] = new ServerMobileStatusHandler_7_0_29_2(pManager);
    handlers[0x1B] = new LoginConfirmHandler_7_0_29_2(pManager);
    handlers[0x40] = new UltimaLiveUpdateLandBlockHandler(pManager);
    handlers[0x55] = new LoginCompleteHandler_7_0_29_2(pManager);
    //handlers[0xBD] = new ServerVersionRequestHandler(pManager);
  }

  return handlers;
}

std::map<uint8_t, BasePacketHandler*> PacketHandlerFactory::GenerateServerExtendedPacketHandlers (uint32_t versionMajor, uint32_t versionMinor, NetworkManager* pManager)
{
  std::map<uint8_t, BasePacketHandler*> handlers;
  //if (version.compare("7.0.29.2") == 0)
  {
    handlers[0x08] = new ChangeMapHandler_7_0_29_2(pManager);   
  }

  return handlers;
}
