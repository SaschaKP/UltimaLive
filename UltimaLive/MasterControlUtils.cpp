/* Copyright (C) 2012 Matthew Geyer
 * Copyright (C) 2013 Ian Karlinsey
 * 
 * This file is part of MasterControl.
 * 
 * MasterControl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * MasterControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MasterControl.  If not, see <http://www.gnu.org/licenses/>. */

#include "MasterControlUtils.h"

const unsigned char* MasterControlUtils::BASE_ADDRESS = reinterpret_cast<const unsigned char*>(0x400000);

unsigned char MasterControlUtils::g_sendSignature1[] = { 0x8D, 0x8B, 0x94, 0x00, 0x00, 0x00, 0xE8, 0xCC, 0xCC, 0xCC, 0xCC, 0x55, 0x8D, 0x8B, 0xBC, 0x00, 0x00, 0x00 };
unsigned char MasterControlUtils::g_sendSignature2[] = { 0x0F, 0xB7, 0xD8, 0x0F, 0xB6, 0x06, 0x83, 0xC4, 0x04, 0x53, 0x50, 0x8D, 0x4F, 0x6C };

unsigned char MasterControlUtils::g_recvSignature1[] = { 0x53, 0x56, 0x57, 0x8B, 0xF9, 0x8B, 0x0D, 0xCC, 0xCC, 0xCC, 0xCC, 0x33, 0xD2 };
unsigned char MasterControlUtils::g_recvSignature2[] = { 0xA1, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xC7, 0x05, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x8B, 0x38, 0x8B, 0xCC, 0xBE };

unsigned char MasterControlUtils::g_NetworkObjectSignature1[] = { 0xA1, 0xCC, 0xCC, 0xCC, 0xCC, 0x8B, 0x0D, 0xCC, 0xCC, 0xCC, 0xCC, 0x8B, 0x16 };
unsigned char MasterControlUtils::g_NetworkObjectSignature2[] = { 0xC7, 0x06, 0xCC, 0xCC, 0xCC, 0xCC, 0x89, 0x35, 0xCC, 0xCC, 0xCC, 0xCC, 0x8B, 0x4C, 0x24, 0x0C };

unsigned char MasterControlUtils::g_PlayerBaseSignature1[] = { 0x66, 0x89, 0x71, 0x24, 0x8B, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
unsigned char MasterControlUtils::g_PlayerBaseSignature2[] = { 0x66, 0x89, 0x71, 0x28, 0x8B, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
unsigned char MasterControlUtils::g_mapDimensionSignature[] = { 0x00, 0x1c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00 };

unsigned char MasterControlUtils::g_functionStartSig1[] = { 0x55, 0x8B, 0xEC };
unsigned char MasterControlUtils::g_functionStartSig2[] = { 0x90, 0x90, 0x6A };

unsigned char MasterControlUtils::g_aDrawMapThingieTableSig1[] = { 0x83, 0xE1, 0x3F, 0x83, 0xE2, 0x3F, 0xC1, 0xE2, 0x06 }; 
unsigned char MasterControlUtils::g_aDrawMapThingieTableSig2[] = { 0x83, 0xE1, 0x3F, 0xC1, 0xE1, 0x06, 0x83, 0xE2, 0x3F }; 

unsigned char MasterControlUtils::g_minClientDisplaySig1[] = { 0xB8, 0xF1, 0xD8, 0xFF, 0xFF, 0x89, 0x3D, 0xCC, 0xCC, 0xCC, 0xCC, 0xA3, 0xCC, 0xCC, 0xCC, 0xCC, 0xA3 };
unsigned char MasterControlUtils::g_minClientDisplaySig2[] = { 0xB8, 0xF1, 0xD8, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xA3, 0xCC, 0xCC, 0xCC, 0xCC, 0xA3 };
unsigned char MasterControlUtils::g_minClientDisplaySig3[] = { 0xB8, 0xF1, 0xD8, 0xFF, 0xFF, 0xA3, 0xCC, 0xCC, 0xCC, 0xCC, 0xA3 };

unsigned char MasterControlUtils::g_updateBlocksSig1[] = { 0xEE, 0xC1, 0xCC, 0x03, 0xC1, 0xCC, 0x03 }; //works up through 7.0.1.1
unsigned char MasterControlUtils::g_updateBlocksSig2[] = { 0x2B, 0xD8, 0xC1, 0xCC, 0x03, 0xC1, 0xCC, 0x03 }; //works up through 7.0.1.1

unsigned char MasterControlUtils::g_refreshTerrainFunctionSig1[] = { 0x75, 0xCC, 0x83, 0xCC, 0xFF, 0x74, 0xCC, 0xE8 };

unsigned char MasterControlUtils::g_masterStaticsListSig1[] = { 0x33, 0xf6, 0x68, 0x00, 0x00, 0xCC, 0x00, 0x89 }; //2.0.8n to 7.0.8.2
unsigned char MasterControlUtils::g_masterStaticsListSig2[] = { 0x68, 0x40, 0xF9, 0x2F, 0x00, 0x89, 0x35 };

unsigned char* MasterControlUtils::GetRefreshTerrainFunctionPointer()
{
  unsigned char* retValue = 0;

  const unsigned char* func1SigAddr = FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_refreshTerrainFunctionSig1, sizeof(g_refreshTerrainFunctionSig1));

  if (func1SigAddr != 0)
  {
    int func1Offset = *reinterpret_cast<int*>(const_cast<unsigned char*>(func1SigAddr + 8));
    retValue = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(func1SigAddr + func1Offset + 12));
  }

  return retValue;
}

const unsigned char* MasterControlUtils::FindSignatureOffsetBackwards(const unsigned char *pData, unsigned int searchLen, unsigned char *sigBuffer, unsigned int sigLen)
{
  unsigned char* buffer = (unsigned char *)(pData - searchLen - 1);

	BOOL found = FALSE;
	for (unsigned int x = searchLen; x > 0; --x)
	{
		for(unsigned int y = 0; y < sigLen; ++y)
		{
			if (sigBuffer[y] == 0xCC || buffer[x + y] == sigBuffer[y])
      {
				found = TRUE;
      }
			else
			{
				found = FALSE;
				break;
			}
		}
		if (found)
		{
			return const_cast<unsigned char *>(buffer + x);
		}
	}

	return 0;
}

const unsigned char* MasterControlUtils::findFunctionCall(const unsigned char* pBuffer, int bufferSize, unsigned char* pSignature, int signatureSize)
{
  unsigned char* pAddress = 0;

  unsigned char* pSigAddress = const_cast<unsigned char*>(MasterControlUtils::FindSignatureOffset(reinterpret_cast<const unsigned char*>(pBuffer), bufferSize, pSignature, signatureSize));

  if (pSigAddress != 0)
  {
    pAddress = const_cast<unsigned char *>(MasterControlUtils::FindSignatureOffsetBackwards(pSigAddress, 100, g_functionStartSig1, sizeof(g_functionStartSig1)));
    if (pAddress == 0)
    {
      pAddress = const_cast<unsigned char*>(MasterControlUtils::FindSignatureOffsetBackwards(pSigAddress, 100, g_functionStartSig2, sizeof(g_functionStartSig2)));
      if (pAddress != 0)
      {
        pAddress +=2;
      }
    }
  }

  return pAddress;
}

const unsigned char* MasterControlUtils::findSignature (const unsigned char* pBuffer, int bufferSize, unsigned char* pSignature, int signatureSize, int offset, int& structureSizeOut, int structureSizeOffset)
{
  unsigned char* retValue = 0;

  unsigned char* pAddress = const_cast<unsigned char *>(FindSignatureOffset(reinterpret_cast<const unsigned char*>(pBuffer), bufferSize, pSignature, signatureSize));

  if (pAddress != 0)
  {
    retValue = reinterpret_cast<unsigned char*>(*reinterpret_cast<unsigned int*>(pAddress + offset));
    structureSizeOut = *reinterpret_cast<int *>(pAddress + structureSizeOffset);
  }

  return retValue;
}

const unsigned char* MasterControlUtils::findSignature (const unsigned char* pBuffer, int bufferSize, unsigned char* pSignature, int signatureSize, int offset)
{
  unsigned char* retValue = 0;

  unsigned char* pAddress = const_cast<unsigned char *>(FindSignatureOffset(reinterpret_cast<const unsigned char*>(pBuffer), bufferSize, pSignature, signatureSize));

  if (pAddress != 0)
  {
    retValue = reinterpret_cast<unsigned char*>(*reinterpret_cast<unsigned int*>(pAddress + offset));
  }

  return retValue;
}

unsigned char* MasterControlUtils::GetPlayerBasePointer()
{
  const unsigned char* pOffset = FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_PlayerBaseSignature1, sizeof(g_PlayerBaseSignature1));
 
  if(pOffset==0)
  {
    pOffset = FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_PlayerBaseSignature2, sizeof(g_PlayerBaseSignature2));
  }

  unsigned char* retPointer = (unsigned char*)*(uint32_t*)(pOffset + 6);

#ifdef DEBUG
  printf("PLAYER BASE POINTER 0x%x\n", retPointer);
#endif

  return retPointer;
}

unsigned char* MasterControlUtils::GetSendAddress()
{
  unsigned char* pReturnAddress = NULL;

  const unsigned char* pOffset =  FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_sendSignature1, sizeof(g_sendSignature1));

	if (pOffset != 0)
	{
		pReturnAddress = (unsigned char*)(pOffset - 0x22);
	}
	else
	{
    pOffset = FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_sendSignature2, sizeof(g_sendSignature2));
		if (pOffset != 0)
		{
      pReturnAddress = (unsigned char*)(pOffset - 0xF);
		}
	}

#ifdef DEBUG
  printf("SEND FUNCTION ADDRESS 0x%x\n", pReturnAddress);
#endif

	return pReturnAddress;
}

unsigned char* MasterControlUtils::GetRecvAddress()
{
  unsigned char* pReturnAddress = NULL;
  const unsigned char* pOffset =  FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_recvSignature1, sizeof(g_recvSignature1));

	if (pOffset != 0)
	{
		pReturnAddress = (unsigned char*)pOffset;
	}
	else
	{
    pOffset =  FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_recvSignature2, sizeof(g_recvSignature2));

    if (pOffset != 0)
		{
			pReturnAddress = (unsigned char*)pOffset;
		}
	}

#ifdef DEBUG
  printf("RECV FUNCTION ADDRESS 0x%x\n", pReturnAddress);
#endif

	return pReturnAddress;
}

unsigned int* MasterControlUtils::GetNetworkObjectAddress()
{
  const unsigned char* pOffset =  FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_NetworkObjectSignature1, sizeof(g_NetworkObjectSignature1));


  unsigned int* pNetworkObject = NULL;

  if (pOffset == 0)
	{
    pOffset =  FindSignatureOffset(BASE_ADDRESS, CLIENT_IMAGE_MAX_SIZE, g_NetworkObjectSignature2, sizeof(g_NetworkObjectSignature2));

		if (pOffset != 0)
		{
      pNetworkObject = reinterpret_cast<uint32_t*>(const_cast<unsigned char*>(pOffset) + 8);
      pNetworkObject = reinterpret_cast<uint32_t*>(*pNetworkObject);
		}
	}
	else
	{
    pNetworkObject = reinterpret_cast<uint32_t*>(const_cast<unsigned char*>(pOffset) + 7);
    pNetworkObject = reinterpret_cast<uint32_t*>(*pNetworkObject);
	}

#ifdef DEBUG
  printf("NETWORK OBJECT ADDRESS 0x%x\n", pNetworkObject);
#endif

	return pNetworkObject;
}

uint8_t* MasterControlUtils::GetMapDimensionAddress()
{
  return const_cast<uint8_t*>(FindSignatureOffset(BASE_ADDRESS, 0x300000, g_mapDimensionSignature, sizeof(g_mapDimensionSignature)));
}

/*********************************************************************************************************************************************
 * Search memory for specified byte signature.  Note that 0xCC is a wildcard byte, it matches anything.  A more proper way to do this is
 * one pointer to a signature and another pointer to wildcard flags, but this is simpler and works well.
 * Returns a pointer to the beginning of the signature match.  Returns 0 on no match.
 *********************************************************************************************************************************************/
const unsigned char* MasterControlUtils::FindSignatureOffset(const unsigned char *buffer, unsigned int bufferLen, unsigned char *sigBuffer, unsigned int sigLen)
{
	BOOL found = FALSE;
	for (unsigned int x = 0; x < bufferLen - sigLen; x++)
	{
		for(unsigned int y = 0; y < sigLen; y++)
		{
			if (sigBuffer[y] == 0xCC || buffer[x + y] == sigBuffer[y])
				found = TRUE;
			else
			{
				found = FALSE;
				break;
			}
		}
		if (found)
		{
			return const_cast<unsigned char *>(buffer + x);
		}
	}

	return 0;
}


unsigned char* MasterControlUtils::GetDrawMapThingieTable()
{

  unsigned char* drawMapThingieTableAddress = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_aDrawMapThingieTableSig1, sizeof(g_aDrawMapThingieTableSig1), 14));
  if (drawMapThingieTableAddress == 0)
  {
    drawMapThingieTableAddress = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_aDrawMapThingieTableSig2, sizeof(g_aDrawMapThingieTableSig2), 14));
  }

  return drawMapThingieTableAddress;
}

unsigned char* MasterControlUtils::GetMasterStaticsListPointer()
{
  unsigned char* masterStaticListAddress = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_masterStaticsListSig1, sizeof(g_masterStaticsListSig1), 9));
  if (masterStaticListAddress == 0)
  {
    masterStaticListAddress = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_masterStaticsListSig2, sizeof(g_masterStaticsListSig2), 7));
  }
  
  return masterStaticListAddress;
}


unsigned char* MasterControlUtils::GetMinClientDisplayX()
{
  unsigned char* minClientDisplayX = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig1, sizeof(g_minClientDisplaySig1), 12));

  if (minClientDisplayX == 0)
  {
    minClientDisplayX = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig2, sizeof(g_minClientDisplaySig2), 11));

    if (minClientDisplayX == 0)
    {
      minClientDisplayX = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig3, sizeof(g_minClientDisplaySig3), 6));
    }
  }
   
  return minClientDisplayX;
}

unsigned char* MasterControlUtils::GetMinClientDisplayY()
{
  unsigned char* minClientDisplayY = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig1, sizeof(g_minClientDisplaySig1), 17));

  if (minClientDisplayY == 0)
  {
    minClientDisplayY = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig2, sizeof(g_minClientDisplaySig2), 16));

    if (minClientDisplayY == 0)
    {
      minClientDisplayY = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig3, sizeof(g_minClientDisplaySig3), 11));
    }
  }

  return minClientDisplayY;
}

unsigned char* MasterControlUtils::GetClientDisplayedBlocksTable()
{
  unsigned char* minClientDisplayX = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig1, sizeof(g_minClientDisplaySig1), 12));

  if (minClientDisplayX == 0)
  {
    minClientDisplayX = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig2, sizeof(g_minClientDisplaySig2), 11));

    if (minClientDisplayX == 0)
    {
      minClientDisplayX = const_cast<unsigned char*>(MasterControlUtils::findSignature(BASE_ADDRESS, 0x300000, g_minClientDisplaySig3, sizeof(g_minClientDisplaySig3), 6));
    }
  }

  if (minClientDisplayX != 0)
  {
    minClientDisplayX -= (36 * 4);

  }

  return minClientDisplayX;
}

unsigned char* MasterControlUtils::GetUpdateBlocksFunctionPointer()
{
  unsigned char* updateBlock = const_cast<unsigned char*>(findFunctionCall(BASE_ADDRESS, 0x300000, g_updateBlocksSig1, sizeof(g_updateBlocksSig1)));

  if (updateBlock == 0)
  {
    updateBlock = const_cast<unsigned char*>(findFunctionCall(BASE_ADDRESS, 0x300000, g_updateBlocksSig2, sizeof(g_updateBlocksSig2)));
  }

  return updateBlock;
}

