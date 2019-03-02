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

#ifndef _DOT_NET_HOST_H
#define _DOT_NET_HOST_H

#include <string>
#include <stdint.h>
#include <Windows.h>
#include "Utils.h"

#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

class DotNetHost
{
  public:
    void LaunchCLR();
    uint32_t LaunchNetAssembly(std::string assemblyPath, std::string typeName, std::string methodName, std::string argument);
    DotNetHost();

  private:
    ICLRMetaHost* m_pMetaHost;
    ICLRRuntimeInfo* m_pRuntimeInfo;
    ICLRRuntimeHost* m_pClrRuntimeHost;
    HRESULT m_hr;
};
#endif