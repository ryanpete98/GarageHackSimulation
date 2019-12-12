#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "led.h"

int i;

int lfsr8Bit(int randNum){
	int start=rand()%256+1;
        int msb=0;  //most significant bit
        int j;
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<256;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>2) & 1;
                tap3=(start>>3) & 1;
                tap4=(start>>4) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<7;
                start=start>>1;
                start=start+msb;
		counter++;
        LED_SetGroupValue(start);
        for(j=0; j<100000; j++) {
            
        }
		if(randNum==start){
			return counter*8;
                }
        }
}
int lfsr9Bit(int randNum){
	int start=rand()%512+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<512;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>2) & 1;
                tap3=(start>>3) & 1;
                tap4=(start>>4) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<8;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*9;
                }
        }
}
int lfsr10Bit(int randNum){
	int start=rand()%1024+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<1024;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>2) & 1;
                tap3=(start>>3) & 1;
                tap4=(start>>4) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<9;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*10;
                }
        }
}
int lfsr11Bit(int randNum){
	int start=rand()%2048+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<2048;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>1) & 1;
                tap3=(start>>2) & 1;
                tap4=(start>>4) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<10;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*11;
                }
        }
}
int lfsr12Bit(int randNum){
	int start=rand()%4096+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<4096;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>1) & 1;
                tap3=(start>>4) & 1;
                tap4=(start>>6) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<11;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*12;
                }
        }
}
int lfsr13Bit(int randNum){
	int start=rand()%8192+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<8192;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>1) & 1;
                tap3=(start>>3) & 1;
                tap4=(start>>4) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<12;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*13;
                }
        }
}
int lfsr14Bit(int randNum){
	int start=rand()%16384+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<8192;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>1) & 1;
                tap3=(start>>3) & 1;
                tap4=(start>>5) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<13;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*14;
                }
        }
}
int lfsr15Bit(int randNum){
	int start=rand()%32768+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<32768;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>1) & 1;
                tap3=(start>>2) & 1;
                tap4=(start>>4) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<14;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*15;
                }
        }
}
int lfsr16Bit(int randNum){
	int start=rand()%65536+1;
        int msb=0;  //most significant bit
        int tap1,tap2,tap3,tap4;
	int counter=0;
        for(i=0;i<65536;i++){
                printf("%d\n",start);
                tap1=start & 1;
                tap2=(start>>2) & 1;
                tap3=(start>>3) & 1;
                tap4=(start>>5) & 1;
                msb=tap1^ tap2 ^ tap3 ^ tap4;
                msb=msb<<15;
                start=start>>1;
                start=start+msb;
		counter++;
		if(randNum==start){
			return counter*16;
                }
        }
}
