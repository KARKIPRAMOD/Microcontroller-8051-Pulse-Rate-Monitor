// Program for LCD Interfacing with 8051 Microcontroller (AT89S52) 


#include<reg51.h>

#define display_port P2    //Data pins connected to port 2 on microcontroller

sbit rs = P3^4;  //RS pin connected to pin 2 of port 3

sbit rw = P3^5;  // RW pin connected to pin 3 of port 3

sbit e =  P3^6;  //E pin connected to pin 4 of port 3


sbit rd_adc=P3^0; //Read(RD) pin of ADC0804

sbit wr_adc=P3^1; //Write(WR) pin of ADC0804

sbit intr_adc=P3^2; //Interrupt(INTR) pin of ADC0804

unsigned int BPM =0;
unsigned int Threshold = 524;   //Minimum Digital reading for Pulse calculation

void msdelay(unsigned int time)  // Function for creating delay in milliseconds.

{

    unsigned i,j ;

    for(i=0;i<time;i++)    

    for(j=0;j<1275;j++);

}

void lcd_cmd(unsigned char command)  //Function to send command instruction to LCD

{

    display_port = command;

    rs= 0;

    rw=0;

    e=1;

    msdelay(1);

    e=0;

}


void lcd_data(unsigned char disp_data)  //Function to send display data to LCD

{

    display_port = disp_data;

    rs= 1;

    rw=0;

    e=1;

    msdelay(1);

    e=0;

}


 void lcd_init()    //Function to prepare the LCD  and get it ready

{

    lcd_cmd(0x38);  // for using 2 lines and 5X7 matrix of LCD

    msdelay(10);

    lcd_cmd(0x0F);  // turn display ON, cursor blinking

    msdelay(10);

    lcd_cmd(0x01);  //clear screen

    msdelay(10);

    lcd_cmd(0x81);  // bring cursor to position 1 of line 1

    msdelay(10);

}

void main()

{

    unsigned char a[15]="HEALTH KIT";
		unsigned char b[25]="PLACE YOUR FINGER ";	//string of 14 characters with a null terminator.

	
	  int l=0;
	  P1=0xFF; //make Port 1 as input port
 
    P2=0x00; //make Port 0 as output port

   

    lcd_init();

    while(a[l] != '\0') // searching the null terminator in the sentence

    {

        lcd_data(a[l]);
        l++;
        msdelay(30);

    }
		 while(1)    //repeat forever

		{
		lcd_cmd(0xC1); // display on second line 

		wr_adc=0; //send LOW to HIGH pulse on WR pin

		msdelay(1);

		wr_adc=1;

		while(intr_adc==1); //wait for End of Conversion

		rd_adc=0; //make RD = 0 to read the data from ADC0804
		BPM = P1;
		if(BPM>=Threshold)
		{
			BPM = P1*(1024/5);
			x1=(BPM / 100) + 0x30;
			lcd_data(x1); // Display hundreds digit
			lcd_data(((BPM / 10) % 10) + 0x30); // Display tens digit
			lcd_data((BPM % 10) + 0x30); // Display ones digit
			
			msdelay(200);  //interval between every cycles
	
			rd_adc=1;   //make RD = 1 for the next cycle
		}
		else{
			int l=0;
			lcd_init();
			while(b[l] != '\0') // searching the null terminator in the sentence

				{

					lcd_data(b[l]);
					l++;
					msdelay(30);

				}
			}
		}
}