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

#include "DotNetHost.h"

DotNetHost::DotNetHost()
  : m_pMetaHost(NULL),
    m_pRuntimeInfo(NULL),
    m_pClrRuntimeHost(NULL),
    m_hr(NULL)
{
  //do nothing
}

void DotNetHost::LaunchCLR()
{
  m_hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&m_pMetaHost));
  m_hr = m_pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&m_pRuntimeInfo));
  m_hr = m_pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_PPV_ARGS(&m_pClrRuntimeHost));
  m_hr = m_pClrRuntimeHost->Start();
}

uint32_t DotNetHost::LaunchNetAssembly(std::string assemblyPath, std::string typeName, std::string methodName, std::string argument)
{
  DWORD returnValue = 0;

  m_hr = m_pClrRuntimeHost->ExecuteInDefaultAppDomain(Utils::s2ws(assemblyPath.c_str()).c_str(), Utils::s2ws(typeName.c_str()).c_str(), Utils::s2ws(methodName.c_str()).c_str(), Utils::s2ws(argument.c_str()).c_str(), &returnValue);

  return returnValue;
}
