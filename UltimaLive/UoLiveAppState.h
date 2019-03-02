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

#ifndef _UO_LIVE_APP_STATE_H
#define _UO_LIVE_APP_STATE_H

#include <stdint.h>
#include <string>
#include <map>
#include <Windows.h>
#include "Utils.h"
#include "ClientRedirections.h"
#include "Network\NetworkManager.h"
#include "FileSystem\BaseFileManager.h"
#include "FileSystem\FileManagerFactory.h"
#include "Maps\Atlas.h"
#include "LoginHandler.h"
#include "MasterControlUtils.h"

class PlayerLocation
{
  public:
    uint16_t X;
    uint16_t Y;
    uint8_t Z;
    uint8_t Facing;
};

class UoLiveAppState
{
  public:
    UoLiveAppState();
    void Init();
    BaseFileManager* GetFileManager();
    NetworkManager* GetNetworkManager();
    Atlas* GetAtlas();

    void setPlayerLocation(PlayerLocation location);
    PlayerLocation getPlayerLocation();

    void setClientPlayerStructure(uint8_t* pPlayer);
    uint8_t* m_pMapDimensions;

  private:
    BaseFileManager* m_pFileManager;
    NetworkManager* m_pNetworkManager;
    uint32_t m_versionMajor;
    uint32_t m_versionMinor;
    Atlas* m_pAtlas;
    PlayerLocation m_playerLocation;
    std::map<uint8_t, uint8_t> m_movementRequests;
    uint8_t* m_pClientPlayerStructure;
    LoginHandler* m_pLoginHandler;
};

#endif