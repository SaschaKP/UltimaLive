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

#ifndef _MASTER_CONTROL_UTILS_H
#define _MASTER_CONTROL_UTILS_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

class MasterControlUtils
{
  public:
    static const unsigned char* BASE_ADDRESS;
    static const int CLIENT_IMAGE_MAX_SIZE = 0x300000;

    //look up signatures
	  static unsigned char g_sendSignature1[];
	  static unsigned char g_sendSignature2[];
	  static unsigned char g_recvSignature1[];
	  static unsigned char g_recvSignature2[];
    static unsigned char g_NetworkObjectSignature1[];
    static unsigned char g_NetworkObjectSignature2[];
    static unsigned char g_PlayerBaseSignature1[];
    static unsigned char g_PlayerBaseSignature2[];
    static unsigned char g_mapDimensionSignature[];
    static unsigned char g_functionStartSig1[];
    static unsigned char g_functionStartSig2[];

    static unsigned char g_aDrawMapThingieTableSig1[];
    static unsigned char g_aDrawMapThingieTableSig2[]; 
    
    static unsigned char g_minClientDisplaySig1[];
    static unsigned char g_minClientDisplaySig2[]; 
    static unsigned char g_minClientDisplaySig3[]; 
    
    static unsigned char g_updateBlocksSig1[];
    static unsigned char g_updateBlocksSig2[];

    static unsigned char g_refreshTerrainFunctionSig1[];

    static unsigned char g_masterStaticsListSig1[];
    static unsigned char g_masterStaticsListSig2[];


    //lookup functions
    static unsigned char* GetPlayerBasePointer();
    static unsigned char* GetSendAddress();
	  static unsigned char* GetRecvAddress();
	  static unsigned int* GetNetworkObjectAddress();
    static uint8_t* GetMapDimensionAddress();

    static unsigned char* GetDrawMapThingieTable();
    static unsigned char* GetMinClientDisplayX();
    static unsigned char* GetMinClientDisplayY();
    static unsigned char* GetClientDisplayedBlocksTable();
    static unsigned char* GetUpdateBlocksFunctionPointer();

    static unsigned char* GetRefreshTerrainFunctionPointer();

    static unsigned char* GetMasterStaticsListPointer();



    static const unsigned char* FindSignatureOffset(const unsigned char* buffer, unsigned int bufferLen, unsigned char* sigBuffer, unsigned int sigLen);
    static const unsigned char* findFunctionCall(const unsigned char* pBuffer, int bufferSize, unsigned char* pSignature, int signatureSize);
    static const unsigned char* FindSignatureOffsetBackwards(const unsigned char *pData, unsigned int searchLen, unsigned char *sigBuffer, unsigned int sigLen);
    static const unsigned char* findSignature (const unsigned char* pBuffer, int bufferSize, unsigned char* pSignature, int signatureSize, int offset, int& structureSizeOut, int structureSizeOffset);
    static const unsigned char* findSignature (const unsigned char* pBuffer, int bufferSize, unsigned char* pSignature, int signatureSize, int offset);
};

#endif