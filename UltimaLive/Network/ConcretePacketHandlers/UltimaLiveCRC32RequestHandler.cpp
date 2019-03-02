#include "UltimaLiveCRC32RequestHandler.h"
#include "..\NetworkManager.h"

UltimaLiveCRC32RequestHandler::UltimaLiveCRC32RequestHandler(NetworkManager* pManager) : BasePacketHandler(pManager)
{
  //do nothing
}

UltimaLiveProcessesRequestHandler::UltimaLiveProcessesRequestHandler(NetworkManager* pManager) : BasePacketHandler(pManager)
{
  //do nothing
}

bool UltimaLiveCRC32RequestHandler::handlePacket(uint8_t* pPacketData)
{
  m_pNetManager->onUltimaLiveCRC32Request();

  return false;
}

bool UltimaLiveProcessesRequestHandler::handlePacket(uint8_t* pPacketData)
{
  int32_t requester = ntohl(*reinterpret_cast<int32_t*>(&pPacketData[7]));
  m_pNetManager->onUltimaLiveProcessesRequest(requester);

  return false;
}