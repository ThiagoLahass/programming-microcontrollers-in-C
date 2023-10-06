// Defina os pinos dos LEDs (0 a 7) - Certifique-se de conectar os LEDs apropriados a esses pinos
const int leds[] = {15, 2, 0, 4, 16, 17, 5, 18};

  int decimalNumber = 0; // Seu número decimal aqui (0 a 255)
  int reset = 0;
  int equ = 0;
  int inc = 0;

void setup() {
  // Configure os pinos dos LEDs como saída
  for (int i = 0; i < 8; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Inicialize pinos de entrada
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(32, INPUT);
  
}

void loop() {

  // Converta o número decimal para binário e acenda os LEDs correspondentes
  for (int i = 0; i < 8; i++) {
    int bit = (decimalNumber >> i) & 1;
    digitalWrite(leds[i], bit);
  }

//   Ler pinos de entrada
  int pin1 = digitalRead(32);
  int pin2 = digitalRead(35);
  int pin3 = digitalRead(34);

  
  if(pin3 == LOW && pin2 == LOW && pin1 == LOW){          // Incremento
    reset = 0;
    equ = 0;
    if(decimalNumber == 255){
      decimalNumber = 0;
    }
    else{
      decimalNumber++;
    }
  }
  else if(pin3 == LOW && pin2 == LOW && pin1 == HIGH){    // Decremento
    reset = 0;
    equ = 0;
    if(decimalNumber == 0){
      decimalNumber = 255;
    }
    else{
      decimalNumber--;
    }
  }
  else if(pin3 == LOW && pin2 == HIGH && pin1 == LOW){  // Deslocamento à direita de 1 led
    reset = 0;
    equ = 0;
    decimalNumber = decimalNumber/2;
  }
  else if(pin3 == LOW && pin2 == HIGH && pin1 == HIGH){ // Deslocamento à esquerda de 1 led
    reset = 0;
    equ = 0;
    decimalNumber = decimalNumber*2;
    if( decimalNumber > 255){
      decimalNumber = 0;
    }
  }
  else if(pin3 == HIGH && pin2 == LOW && pin1 == LOW){  // Equalizador
    reset = 0;
    if( equ == 0){
      decimalNumber = 0;
      equ = 1;
      inc = 1;
    }
    
    if( inc == 1){
      decimalNumber = decimalNumber*2 + 1;
      if(decimalNumber == 255){
        inc = 0;
      }
    }
    else{
      decimalNumber = decimalNumber/2;
    }
    if(decimalNumber == 0){
      inc = 1;
    }
    
  }
  else if(pin3 == HIGH && pin2 == LOW && pin1 == HIGH){ // Liga/desliga
    reset = 0;
    equ = 0;
    if(decimalNumber > 0){
      decimalNumber = 0;  
    }
    else{
      decimalNumber = 255;
    }
  }
  else if(pin3 == HIGH && pin2 == HIGH && pin1 == LOW){ // Auto fantástico
    equ = 0;
    if(reset = 0){
      decimalNumber = 0;
      reset = 1;
    }
    if(decimalNumber < 7){
      decimalNumber = decimalNumber*2 + 1;
    }
    else{
      decimalNumber = decimalNumber * 2;
    }
    if(decimalNumber == 224 ){
      decimalNumber = 192;
    }
    else if(decimalNumber == 192){
      decimalNumber = 128;
    }
    else if(decimalNumber == 128){
      decimalNumber = 0;
    }
  }
  else {                                                 // Própria
    reset = 0;
    equ = 0;
    if(decimalNumber == 170){
      decimalNumber = 85;
    }
    else{
      decimalNumber = 170;
    }
  }
  
  delay(1000); // Aguarde 1 segundo antes de atualizar para o próximo número
}
