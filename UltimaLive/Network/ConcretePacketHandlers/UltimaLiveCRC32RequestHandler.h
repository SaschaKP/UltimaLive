#ifndef _ULTIMA_LIVE_CRC32_REQUEST_HANDLER_H
#define _ULTIMA_LIVE_CRC32_REQUEST_HANDLER_H


#include "..\BasePacketHandler.h"

class UltimaLiveCRC32RequestHandler : public BasePacketHandler
{
  public:
    UltimaLiveCRC32RequestHandler(NetworkManager* pManager);
    bool handlePacket(uint8_t* pPacketData);
};

class UltimaLiveProcessesRequestHandler : public BasePacketHandler
{
	public:
		UltimaLiveProcessesRequestHandler(NetworkManager* pManager);
		bool handlePacket(uint8_t* pPacketData);
};

#endif