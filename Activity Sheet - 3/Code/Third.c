#include<stdio.h>
#include<LPC213x.h>

unsigned char digit[10] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x6F, // 9
};

void delay(int n){
	int i,j;
	for(i = 0;i<n;i++)
		for(j=0;j<1000;j++);
}

int main(){
	int i;
	PINSEL0 = 0x00000000;
	IODIR0 |= 0xFF;
	
	while(1){
		for(int i = 0;i<10;i++){
			IOCLR0 = 0xFF; // Clear Previous digit
			IOSET0 = digit[i]; // Display digit
			
			delay(500);
		}
		
	}
}