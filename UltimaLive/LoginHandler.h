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

#ifndef _LOGIN_HANDLER_H
#define _LOGIN_HANDLER_H

#include <stdint.h>
#include <string>
#include <vector>
#include "Utils.h"
#include "LocalPeHelper32.hpp"

class NetworkManager;
class MapDefinition;

class LoginHandler
{
  public:
    LoginHandler(NetworkManager* pManager);
    void init();

  private:
    void onLogoutRequest();
    void onLoginConfirm(uint8_t* pPacketData);
    void onLoginComplete();
    void onBeforeMapChange(uint8_t& rMap);
    void onHashQuery(uint32_t blockNumber, uint8_t mapNumber, uint16_t sequence);
    void onServerMobileUpdate();
    void onUpdateMapDefinitions(std::vector<MapDefinition> definitions);

    NetworkManager* m_pNetManager;
    bool m_needToSendCachedLoginPacket;
    bool m_firstHashQuery;
    uint8_t* m_pCachedLoginPacket;
    bool m_firstMobileUpdateFromServer;
};

#endif
