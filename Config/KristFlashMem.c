/* To install pigpio:
 * wget https://github.com/joan2937/pigpio/archive/master.zip
 * unzip master.zip
 * cd pigpio-master
 * make
 * sudo make install
 * To compile:
 * gcc -o flash_mem flash_mem.c -lpigpio -lrt -lpthread
 */

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h> //gpio access for Rasperry Pi

void send_command(unsigned char);
int read_byte(unsigned int);
void read_string(unsigned int, unsigned int, char **);
void write_char(unsigned char, unsigned int);
void write_string(unsigned char *, unsigned int);
void write_enable();
void write_disable();
void chip_erase();
void power_down();
void release_power_down();
int read_status_register();

#define MOSI 10
#define MISO 9
#define SCLK 11
#define CE 8

int main(int argc, char *argv[]){
	char str[256] = {0};
	char *strptr = str;

	if(gpioInitialise() < 0){
		fprintf(stderr, "pigpio initialisation failed\n");
		return 1;
	}
	
	gpioSetMode(MOSI, PI_OUTPUT); //set MOSI as output
	gpioSetMode(MISO, PI_INPUT); //set MISO as input
	gpioSetMode(SCLK, PI_OUTPUT); // set SCLK as output
	gpioSetMode(CE, PI_OUTPUT); //set CE as output
	
	gpioWrite(CE, 1);
	char word[75];
    FILE *in_file;

    in_file = fopen("testing.txt", "r");

    if (in_file == NULL)
        printf("Can't open file for reading.\n");
    else
    {
        while (fscanf(in_file, "%s", word) != EOF)
        {
            //send the words to memory whatever is the way you do it
        }
        fclose(in_file);
    }
	
	//printf("str1: %s\n", str);
	//write_enable();
	//send_command(0x60);
	//read_status_register();
	//write_char('a', 0x1F0000);
	//write_string("advil", 0x1F0000);
	read_string(0x1F0000, 5, &strptr);
	printf("str2: %s\n", str);
	//read_byte(0x1F0000);
	gpioTerminate();
	
	return 0;
}

void write_enable(){ //must be executed before writing data
	send_command(0x06);
}

void write_disable(){
	send_command(0x04);
}

void chip_erase(){
	send_command(0xC7);
}

void power_down(){
	send_command(0xB9);
}

void release_power_down(){
	send_command(0xAB);
}

void send_command(unsigned char cmd){ //writes a single byte command
	int i;
	
	gpioWrite(CE, 0); //activate CE
	sleep(.01); //might need delay
	
	for(i=0;i<8;i++){
		if(cmd & 0x80){
			gpioWrite(MOSI, 1);
		}
		else{
			gpioWrite(MOSI, 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		cmd = cmd<<1;
	}
	//printf("\n");
	gpioWrite(MOSI, 1);
	gpioWrite(CE, 1); //deactivate CE
}

int read_byte(unsigned int address){
	int i;
	unsigned int cmd = 0x03, bit, data = 0;
	
	gpioWrite(CE, 0); //activate CE
	sleep(.01); //might need delay
	
	for(i=0;i<8;i++){ //send read command 0x03
		if(cmd & 0x80){
			gpioWrite(MOSI, 1);
		}
		else{
			gpioWrite(MOSI, 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		cmd = cmd<<1;
	}
	
	for(i=0;i<24;i++){ //send 24-bit address
		if(address & 0x800000){
			gpioWrite(MOSI, 1);
		}
		else{
			gpioWrite(MOSI, 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		address = address<<1;
	}
	
	gpioWrite(MOSI, 1);
	gpioWrite(SCLK, 0);
	sleep(.1);
	
	for(i=0;i<8;i++){ //read data
		bit = gpioRead(MISO);
		//printf("%d", bit);
		if(bit == 1){
			data += 1<<(7-i);
		}
		gpioWrite(SCLK, 1);
		sleep(.1);
		gpioWrite(SCLK, 0);
		sleep(.1);
	}
	gpioWrite(SCLK, 1);
	gpioWrite(CE, 1); //deactivate CE
	printf("%x\n", data);
	return data;
}

void read_string(unsigned int address, unsigned int num_bytes, char **str){
	int i, j;
	unsigned int cmd = 0x03, bit, data;
	
	if(num_bytes > 256 || num_bytes == 0)
		return;
	
	gpioWrite(CE, 0); //activate CE
	sleep(.01); //might need delay
	
	for(i=0;i<8;i++){ //send read command 0x03
		if(cmd & 0x80){
			gpioWrite(MOSI, 1);
		}
		else{
			gpioWrite(MOSI, 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		cmd = cmd<<1;
	}
	
	for(i=0;i<24;i++){ //send 24-bit address
		if(address & 0x800000){
			gpioWrite(MOSI, 1);
		}
		else{
			gpioWrite(MOSI, 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		address = address<<1;
	}
	
	gpioWrite(MOSI, 1);
	gpioWrite(SCLK, 0);
	sleep(.1);
	
	for(i=0;i<num_bytes;i++){
		data=0;
		for(j=0;j<8;j++){ //read data
			bit = gpioRead(MISO);
			//printf("%d", bit);
			if(bit == 1){
				data += 1<<(7-j);
			}
			gpioWrite(SCLK, 1);
			sleep(.1);
			gpioWrite(SCLK, 0);
			sleep(.1);
		}
		*((*str)+i) = data; //idk why this works but it does
		//printf("%c\n", data);
	}
	gpioWrite(SCLK, 1);
	gpioWrite(CE, 1); //deactivate CE
	//printf("%s\n", *str);
}

void write_char(unsigned char ch, unsigned int address){ //writes a single byte command
	int i;
	unsigned int cmd = 0x02;
	
	gpioWrite(CE, 0); //activate CE
	sleep(.01); //might need delay
	
	for(i=0;i<8;i++){ // send write command
		if(cmd & 0x80){
			gpioWrite(MOSI, 1);
			//printf("%d", 1);
		}
		else{
			gpioWrite(MOSI, 0);
			//printf("%d", 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		cmd = cmd<<1;
	}
	
	for(i=0;i<24;i++){ //send 24-bit address
		if(address & 0x800000){
			gpioWrite(MOSI, 1);
			//printf("%d", 1);
		}
		else{
			gpioWrite(MOSI, 0);
			//printf("%d", 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		address = address<<1;
	}
	
	for(i=0;i<8;i++){
		if(ch & 0x80){
			gpioWrite(MOSI, 1);
			//printf("%d", 1);
		}
		else{
			gpioWrite(MOSI, 0);
			//printf("%d", 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		ch = ch<<1;
	}
	//printf("\n");
	gpioWrite(MOSI, 1);
	gpioWrite(CE, 1); //deactivate CE
}

void write_string(unsigned char *str, unsigned int address){ //writes a single byte command
	int i, ch_index=0;
	unsigned int cmd = 0x02;
	unsigned char c;
	
	gpioWrite(CE, 0); //activate CE
	sleep(.01); //might need delay
	
	for(i=0;i<8;i++){ // send write command
		if(cmd & 0x80){
			gpioWrite(MOSI, 1);
			//printf("%d", 1);
		}
		else{
			gpioWrite(MOSI, 0);
			//printf("%d", 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		cmd = cmd<<1;
	}
	
	for(i=0;i<24;i++){ //send 24-bit address
		if(address & 0x800000){
			gpioWrite(MOSI, 1);
			//printf("%d", 1);
		}
		else{
			gpioWrite(MOSI, 0);
			//printf("%d", 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		address = address<<1;
	}
	
	while((c=str[ch_index]) != 0){
		//printf("%c", c);
		for(i=0;i<8;i++){
			if(c & 0x80){
				gpioWrite(MOSI, 1);
				//printf("%d", 1);
			}
			else{
				gpioWrite(MOSI, 0);
				//printf("%d", 0);
			}
			gpioWrite(SCLK, 0);
			sleep(.1);
			gpioWrite(SCLK, 1);
			sleep(.1);
			c = c<<1;
		}
		ch_index++;
		//printf("\n");
	}
	//printf("\n");
	gpioWrite(MOSI, 1);
	gpioWrite(CE, 1); //deactivate CE
}

int read_status_register(){
	int i;
	unsigned int cmd = 0x05, bit, data = 0;
	
	gpioWrite(CE, 0); //activate CE
	sleep(.01); //might need delay
	
	for(i=0;i<8;i++){ //send read command 0x03
		if(cmd & 0x80){
			gpioWrite(MOSI, 1);
		}
		else{
			gpioWrite(MOSI, 0);
		}
		gpioWrite(SCLK, 0);
		sleep(.1);
		gpioWrite(SCLK, 1);
		sleep(.1);
		cmd = cmd<<1;
	}
	
	gpioWrite(MOSI, 1);
	gpioWrite(SCLK, 0);
	sleep(.1);
	
	for(i=0;i<8;i++){ //read data
		bit = gpioRead(MISO);
		printf("%d", bit);
		if(bit == 1){
			data += 1<<(7-i);
		}
		gpioWrite(SCLK, 1);
		sleep(.1);
		gpioWrite(SCLK, 0);
		sleep(.1);
	}
	gpioWrite(SCLK, 1);
	gpioWrite(CE, 1); //deactivate CE
	printf("\n");
	return data;
}
