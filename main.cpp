//
//  main.c
//  Wake on LAN
//
//  Created by Markus Kwaśnicki on 17.12.11.
//  Copyright (c) 2011 Kwasniccy. All rights reserved.
//

#include <stdio.h>
#include "wol.h"

//调用方法：wakeonlan 192.168.1.255 00-11-85-6C-A3-90
int main (int argc, const char * argv[])
{
    return wakeOnLan(argv[1], argv[2]);
}

