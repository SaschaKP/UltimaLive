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

#include "LoginHandler.h"
#include "Network\NetworkManager.h"

LoginHandler::LoginHandler(NetworkManager* pManager)
  : m_pNetManager(pManager),
  m_needToSendCachedLoginPacket(false),
  m_pCachedLoginPacket(NULL),
  m_firstHashQuery(true),
  m_firstMobileUpdateFromServer(true)
{
  //do nothing
}

void LoginHandler::init()
{
  m_pNetManager->subscribeToLogout(std::bind(&LoginHandler::onLogoutRequest, this));
  m_pNetManager->subscribeToLoginConfirm(std::bind(&LoginHandler::onLoginConfirm, this, std::placeholders::_1));
  m_pNetManager->subscribeToLoginComplete(std::bind(&LoginHandler::onLoginComplete, this));
  m_pNetManager->subscribeToOnBeforeMapChange(std::bind(&LoginHandler::onBeforeMapChange, this, std::placeholders::_1));
  m_pNetManager->subscribeToServerMobileUpdate(std::bind(&LoginHandler::onServerMobileUpdate, this));
  m_pNetManager->subscribeToMapDefinitionUpdate(std::bind(&LoginHandler::onUpdateMapDefinitions, this, std::placeholders::_1));
}

void LoginHandler::onUpdateMapDefinitions(std::vector<MapDefinition> definitions)
{
  m_firstMobileUpdateFromServer = true;
}

void LoginHandler::onServerMobileUpdate()
{
  if (m_firstMobileUpdateFromServer)
  {
#ifdef DEBUG
  printf("~~~~~~~~~~~~~~~~~~~~~~ First Mobile Update from the server\n");
#endif
    m_firstMobileUpdateFromServer = false;
    m_needToSendCachedLoginPacket = false;
    m_pNetManager->onRefreshClient();
  }
}

void LoginHandler::onBeforeMapChange(uint8_t& rMap)
{

#ifdef DEBUG
  printf("~~~~~~~~~~~~~~~~~~~~~~Login Handler received on before change map packet\n");
#endif

  if (m_pCachedLoginPacket != NULL && m_needToSendCachedLoginPacket)
  {
#ifdef DEBUG
    printf("~~~~~~~~~~~~~~~~~~~~~~ SENDING cached login packet\n");
#endif
    m_pNetManager->sendPacketToClient(m_pCachedLoginPacket);
	  m_needToSendCachedLoginPacket = false;
    m_pNetManager->onRefreshClient();
  }
}

void LoginHandler::onLoginConfirm(uint8_t* pPacketData)
{
#ifdef DEBUG
  printf("~~~~~~~~~~~~~~~~~~~~~~LOGIN HANDLER RECEIVED LOGIN CONFIRM\n");
#endif
  if (m_pCachedLoginPacket == NULL)
  {
    m_pCachedLoginPacket = new uint8_t[37];
  }

  memcpy(m_pCachedLoginPacket, pPacketData, 37);
  //m_needToSendCachedLoginPacket = false;
}



uint8_t* BuildUnicodeMessagePacket(uint32_t serial, uint8_t messageMode, uint16_t hue, char* message)
{
      // send welcome message
  uint8_t aUnicodeMessage[48] = { 
    /* 00          Packet ID   */    0xAE,                     
    /* 01 02       Packet Size */    0x00, 0x00,               
    /* 03 06       Serial      */    0xFF, 0xFF, 0xFF, 0xFF,   
    /* 07 08       Graphic     */    0xFF, 0xFF,               
    /* 09          Mode        */    0x03,                     
    /* 10 11       Text Color  */    0x05, 0xb2,               
    /* 12 13       Font        */    0x00, 0x03,               
    /* 14 17       Language    */    0x45, 0x4E, 0x55, 0x00,   
    /* 18 48       Name        */    0x53, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 18 48       Name        */    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


    int messageLen = strlen(message);
    uint16_t packetLength = 50 + (messageLen * 2); // 48 + message + 2 (null terminator)
    uint8_t* pUnicodePacket = new uint8_t[packetLength];

    memset(pUnicodePacket, 0x00, packetLength);
    memcpy(pUnicodePacket, aUnicodeMessage, 48);

    *reinterpret_cast<uint16_t*>(&pUnicodePacket[1]) = packetLength;
    *reinterpret_cast<uint32_t*>(&pUnicodePacket[3]) = serial;
    pUnicodePacket[9] = messageMode;
    *reinterpret_cast<uint16_t*>(&pUnicodePacket[10]) = hue;

    for (int i = 0; i < messageLen; ++i)
    {
      pUnicodePacket[49 + (2 * i)] = message[i];
    }

    return pUnicodePacket;
}


void LoginHandler::onLoginComplete()
{
#ifdef DEBUG
  printf("~~~~~~~~~~~~~~~~~~~~~~LOGIN HANDLER RECEIVED LOGIN COMPLETE\n");
#endif
  m_needToSendCachedLoginPacket = true;
  m_firstMobileUpdateFromServer = true;


  //send welcome message
  /*char welcomeMessageBuff[20];
  uint16_t minor = Utils::getModuleMinorVersionLower();
  uint16_t major = Utils::getModuleMinorVersionUpper();

  sprintf(welcomeMessageBuff, "ULTIMALIVE v.%u.%u", major, minor);

  uint8_t* pPacket = BuildUnicodeMessagePacket(0xFFFFFFFF, 0x03, 0xb205, welcomeMessageBuff);
  m_pManager->sendPacketToClient(pPacket);
  delete pPacket;*/

  //uint16_t len = *reinterpret_cast<uint16_t*>(&pPacket[1]);

/*#ifdef DEBUG
  printf("%s\n", welcomeMessageBuff);
#endif*/
}

void LoginHandler::onLogoutRequest()
{
#ifdef DEBUG
  printf("~~~~~~~~~~~~~~~~~~~~~~LoginHandler received logout request!\n");
#endif
  m_needToSendCachedLoginPacket = false;
  m_firstHashQuery = true;

  if (m_pCachedLoginPacket != NULL)
  {
    delete m_pCachedLoginPacket;
    m_pCachedLoginPacket = NULL;
  }
}


