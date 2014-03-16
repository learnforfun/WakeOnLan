//
//  wol.c
//  Wake on LAN
//
//  Created by Markus Kwa?nicki on 17.12.11.
//  Copyright (c) 2011 Kwasniccy. All rights reserved.
//

/* 
 * How to found from: 
 * ? http://www.jibble.org/wake-on-lan/
 * ? http://www.osxentwicklerforum.de/index.php?page=Thread&postID=103923 
 * 
 * Additional notes:
 * ? The ?Magic Packet“ needs to be sent as UDP packet to the networks broadcast address on port 7 or 9.
 * ? Errors should be evaluated via ?errno“!
 */

#include "wol.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <WinSock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#pragma  comment(lib,"ws2_32.lib")

int wakeOnLan(const char *ipBroadcast, const char *mac2wake)
{
    /* Create magic packet */
	unsigned char magicPacket[102];
	unsigned char macAddress[6];
    
	//初始化
	WSADATA WSAData;
	if( WSAStartup( MAKEWORD( 1, 1 ), &WSAData ) ){
		printf( "initializationing error!\n" );
		WSACleanup( );
		exit( 0 );
	}

    /* The first 6 bytes of the magic packet are filled with 0xFF. This is probably the source broadcast MAC address? */
	for (int i = 0; i < 6; i++) {
		magicPacket[i] = 0xFF;
	}
    
    /* The next 6 bytes are the MAC address of the target computer. */
    char *iterator = (char *)mac2wake;  // Reading from const char only, storing iterators position
    for (int i = 0; i < 6; i++) {
        long macPart = strtol(iterator, &iterator, 16);
        iterator++;
        macAddress[i] = static_cast<unsigned char>(macPart);
    }
    
    /* Each subsequent set of 6 bytes is also filled with the MAC address of the target computer, until the packet is full at 102 octets. */
    for (int i = 6; i < 102; i++) {
        for (int j = 0; j < 6; j++) {
            magicPacket[i++] = macAddress[j];
        }
        i--;
    }

	/* Sending UDP magic packet */
	int socketDescriptor;
	if ((socketDescriptor = socket(AF_INET, SOCK_DGRAM, 17)) < 0) {
		perror("socket");
		return 1;
	}

	int isBroadcast = 1;
	setsockopt(socketDescriptor, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&isBroadcast), sizeof(isBroadcast));
	
	//设置广播地址
	struct sockaddr_in socketAddress;
	memset(&socketAddress, 0, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(9);
	inet_pton(AF_INET, ipBroadcast, &socketAddress.sin_addr);

	sendto(socketDescriptor, reinterpret_cast<char*>(magicPacket), 102, 0, (struct sockaddr *)&socketAddress, sizeof(socketAddress));
	
	closesocket(socketDescriptor);
    // --
	WSACleanup( );
	return 0;
}

