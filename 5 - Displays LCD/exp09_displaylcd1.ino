#include <string.h>

// Definindo os pinos do CI HD44780 (Interface LCD)
/*#define RS 33
#define E 25
const int DATA[] = {13, 14, 27, 26}; */
#define RS 6
#define E 7
const int DATA[] = {5, 4, 3, 2};
#define DATA_SIZE 4

// Commands: [- - - RS D7 D6 D5 D4]
#define FUNCTION_SET 	0x20 	// 0010 0000 - 4 bits
#define DISPLAY_CONTROL 0x0C 	// 0000 1100
#define CLEAR_DISPLAY 	0x01 	// 0000 0001
#define RETURN_HOME 	0x02 	// 0000 0010
#define ENTRY_MODE_SET 	0x06 	// 0000 0110

// States
#define LCD_DISPLAYON 		0x04
#define LCD_DISPLAYOFF 		0x00
#define DISPLAY_FUNCTION 	0x08
#define ENTER 				0xA8 //1010 1000
#define DISPLAY_SHIFT_LEFT 	0x18 //0001 1000
#define DISPLAY_SHIFT_RIGHT 0x1C //0001 1100

void pulseEnable();
void initLCD();
void write4bits(int value);
void write8bits(int value);
void writeData(char* value);

void setup() {
  // put your setup code here, to run once:
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);
  
  for (int i = 0; i < DATA_SIZE; i++){
    pinMode(DATA[i], OUTPUT);
  }
  
  Serial.begin(9600);
  //Serial.begin(115200); //ESP32
  initLCD();
  
  // Always good to clear and return home
  write8bits(CLEAR_DISPLAY);
  write8bits(RETURN_HOME);
  delay(2); // 1.52ms delay needed for the Return Home command
  writeData("Thiago Lahass");
  write8bits(ENTER);
  writeData("Eng Computacao");
}

void loop() {
  for(int i = 0; i < 16; i++){
    write8bits(DISPLAY_SHIFT_LEFT);
  }
  
  for(int i = 0; i < 32; i++){
    write8bits(DISPLAY_SHIFT_RIGHT);
  	delay(100);
  }
  
  for(int i = 0; i < 16; i++){
    write8bits(DISPLAY_SHIFT_LEFT);
  }
}

/* ------------------------------------------------------
	Pulses the Enable pin to send data to the display
*/
void pulseEnable(){
  // Making sure the pin is LOW at first
  digitalWrite(E, LOW);
  delayMicroseconds(1);
  
  // Pulse the Enable pin
  digitalWrite(E, HIGH);
  delayMicroseconds(1);
  digitalWrite(E, LOW);
  delayMicroseconds(100);
}

/* ------------------------------------------------------
	Initializes the display as in Figure 24 of the 
    HD44780U datasheet requests
*/
void initLCD(){
  // Waiting at first
  delay(40);
  
  //Serial.println("Function set 4 bits 0b0010.");
  digitalWrite(RS, LOW);
  
  // Function set the interface with 4 bits
  write4bits(FUNCTION_SET >> 4);
  delayMicroseconds(4500); // A little more than 4.1 ms
  
  // Now, we set:
  // - Number of lines in the display (2 lines: 16x2)
  // - Size of the pixel matrix (5x8)
  // RS remains 0 (only is 1 when writing)
  //Serial.println("Function set 4 bits 0b0010 1000.");
  write8bits(FUNCTION_SET | DISPLAY_FUNCTION);
  
  // Display OFF
  //Serial.println("Display ON/OFF control 0b0000 1100.");
  write8bits(DISPLAY_CONTROL); 
  
  // Entry mode set
  //Serial.println("Entry mode set 0b0000 0110.");
  write8bits(ENTRY_MODE_SET); 
  
  // Clearing and returning home
  write8bits(CLEAR_DISPLAY);
  write8bits(RETURN_HOME);
  delay(2); // 1.52ms delay needed for the Return Home command
  
  // Now you're free to use the display
}

/* ------------------------------------------------------
	Actually sends the commands to the display
*/
void write4bits(int value){
  for(int i = 0; i < 4; i++){
    // Only the value corresponding to the bit of interest
  	digitalWrite(DATA[i], (value>>(3-i)) & 0x1);
  }
  pulseEnable();
}

/* ------------------------------------------------------
	Writes first half of data, than second half
*/
void write8bits(int value){
  // Sends first half of the data (upper part):
  write4bits(value>>4);
  // Sends last half of the data (lower part):
  write4bits(value);
}

void writeData(const char* value){
  char c;
  for (int i = 0; i < strlen(value); i++){
    c = value[i];
    
    digitalWrite(RS, HIGH);
    write8bits(c);
    digitalWrite(RS, LOW);
  }
}