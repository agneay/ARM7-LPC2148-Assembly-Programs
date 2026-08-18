#include<LPC213x.h>

void delay(int n){
	int i;
	for(i = 0;i<n*1000;i++);
}

int main(){
	PINSEL0 = 0x00000000;
	IODIR0 |= (1<<16);
	IODIR0 &= ~(1<<8);
	
	while(1){
		if(IOPIN0 & (1<<8)){
			// turn LED on
			IOSET0 = (1<<16);
			delay(10);
		}else{
			IOCLR0 = (1<<16);
			delay(10);
		}
	}
}