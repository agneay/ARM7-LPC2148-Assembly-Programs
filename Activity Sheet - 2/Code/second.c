#include<stdio.h>
#include<LPC213x.h>

void delay(int n){
	int i;
	for(i = 0;i<n*10000;i++);
}

int main(){
	PINSEL0 = 0x00000000;
	IODIR0 |= (1<<8);
	while(1){
		IOSET0 = (1<<8);
		delay(10);
		IOCLR0 = (1<<8);
		delay(10);
	}
}