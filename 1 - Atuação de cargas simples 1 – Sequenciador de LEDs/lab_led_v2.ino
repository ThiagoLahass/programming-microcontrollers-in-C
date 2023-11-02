/*
  SISTEMAS EMBARCADOS I
  Experiencia 06 - Sequenciador de Leds
  Objetivo: acionamento de cargas simples
*/

// ==============================================================
// VARIAVEIS E PINOS
// ==============================================================
#define NUM_LEDS 8
#define delayVal 200

int leds[] = {6, 7, 8, 9, 10, 11, 12, 13};    // Arduino
//int leds[] = {15, 2, 0, 4, 16, 17, 5, 18};  // ESP32
int chaves[] = {4, 3, 2};  // Arduino
//int chaves[] = {34, 35, 32};  //ESP32
int estado = 0;
int estados[] = {0, 0, 0};

/*0 a 7
000 Contador ascendente - Incremento
001 Contador descendente - Decremento 
010 Deslocamento a direita de 1 led
011 Deslocamento a esquerda de 1 led
100 Equalizador - 0x1,0x3,0x7,0xF,.....0xFF
101 Liga/desliga
110 knight Rider
111 Própria */
int ledStates[] = {0, 0, 0, 0, 0, 0, 0, 0};

// ==============================================================
// FUNCOES
// ==============================================================
// Setup leds com leds
void LedStream(int *p);
//Setup chaves
void SwitchStream(int *s);
// Le os valores das chaves
int readChaves(int *s);
// Seta o valor dos leds de acordo com um vetor
void setLeds(int *p, int *s);
// Desliga todos leds
void desligaLeds(int *l);

// Funcoes por estado:
void incremento(int *l);
void decremento(int *l);
void shiftDir(int *l);
void shiftEsq(int *l);
void equalizador(int *l);
void onOff(int *l);
void autoF(int *l);
void prop(int *l);

void setup()
{
  Serial.begin(9600);
  LedStream(leds);
  SwitchStream(chaves);
}

// ==============================================================
// LOOP PRINCIPAL
// ==============================================================
void loop()
{
  estado = readChaves(chaves);
  switch(estado){
    case 0:
      Serial.println("Estado 0 - Incremento");
      incremento(leds);
      break;
    case 1:
      Serial.println("Estado 1 - Decremento");
      decremento(leds);
      break;
    case 2:
      Serial.println("Estado 2 - Shift para Direita");
      shiftDir(leds);
      break;
    case 3:
      Serial.println("Estado 3 - Shift para Esquerda");
      shiftEsq(leds);
      break;
    case 4:
      Serial.println("Estado 4 - Equalizador");
      equalizador(leds);
      break;
    case 5:
      Serial.println("Estado 5 - On/Off");
      onOff(leds);
      break;
    case 6:
      Serial.println("Estado 6 - Auto Fantastico");
      autoF(leds);
      break;
    case 7:
      Serial.println("Estado 7 - Funcao Propria");
      prop(leds);
      break;
  }
  
  delay(500);
}

// ==============================================================
// FUNCOES AUXILIARES
// ==============================================================
void LedStream(int *p){
  for(int i = 0; i < 8; i++){
    pinMode(*p, OUTPUT);
    digitalWrite(*p, LOW);
    p++;
  }
} 

// **************************************************************
void SwitchStream(int *s){
  for(int i = 0; i < 3; i++){
    pinMode(*s, INPUT);
    s++;
  }
}

// **************************************************************
int readChaves(int *sw){
  //estados[] = {0, 0, 0};
  estados[0] = digitalRead(*sw);
  estados[1] = digitalRead(*(sw+1));
  estados[2] = digitalRead(*(sw+2));
  
  if (estados[0] == 0 && estados[1] == 0 && estados[2] == 0){
    return 0;
  } else if (estados[0] == 0 && estados[1] == 0 && estados[2] == 1){
    return 1;
  } else if (estados[0] == 0 && estados[1] == 1 && estados[2] == 0){
    return 2;
  } else if (estados[0] == 0 && estados[1] == 1 && estados[2] == 1){
    return 3;
  } else if (estados[0] == 1 && estados[1] == 0 && estados[2] == 0){
    return 4;
  } else if (estados[0] == 1 && estados[1] == 0 && estados[2] == 1){
    return 5;
  } else if (estados[0] == 1 && estados[1] == 1 && estados[2] == 0){
    return 6;
  } else if (estados[0] == 1 && estados[1] == 1 && estados[2] == 1){
    return 7;
  }
}


// **************************************************************
// Parametros - LEDs e Estado
void setLeds(int *p, int *s){
  for(int i = 0; i < NUM_LEDS; i++){
    digitalWrite(*p, *s);
    p++;
    s++;
  }
}

// **************************************************************
void desligaLeds(int *l){
  for(int i = 0; i < NUM_LEDS; i++){
    digitalWrite(*l, LOW);
    l++;
  }
}

// **************************************************************
void int2vec(int counter, int *st){
  int mask = 1;
  for(int i = 0; i < NUM_LEDS; i++){
	st[i] = counter & (mask<<i);
  }
}


// ==============================================================
// FUNCOES DE ORDEM DOS LEDS
// ==============================================================
// 0 - INCREMENTO

void incremento(int *l){
  // Estado inicial dos leds:
  int st[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // Variaveis da funcao
  int counter = 0;
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 0){
      break;
    } else {
        int2vec(counter,st);
        setLeds(l, st);
      	delay(delayVal);
        counter++;
      } 	  
  } 
}

// **************************************************************
// 1 - DECREMENTO
void decremento(int *l){
  // Estado inicial dos leds:
  int st[] = {1, 1, 1, 1, 1, 1, 1, 1};
  // Variaveis da funcao
  int counter = 0xff;
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 1){
      break;
    } else {
        int2vec(counter,st);
        setLeds(l, st);
      	delay(delayVal);
        counter--;
    } 	 
  }
}

// **************************************************************
// 2 - SHIFT DIR
void shiftDir(int *l){
  // Estado inicial dos leds:
  int st[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // Variaveis da funcao
  int var = 0x80; 
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 2){
      break;
    } else {
      if (var != 0){
        int2vec(var,st);
        setLeds(l, st);
      	delay(delayVal);
        var = var>>1;
      } else{
        var = 0x80;
      }
    } 
  }
}

// **************************************************************
// 3 - SHIFT ESQ
void shiftEsq(int *l){
  // Estado inicial dos leds:
  int st[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // Variaveis da funcao
  int var = 0x01; 
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 3){
      break;
    } else {
      if (var != 0x100){
        int2vec(var,st);
        setLeds(l, st);
      	delay(delayVal);
        var = var<<1;
      } else {
        var = 0x01;
      }
    } 
  }
}

// **************************************************************
// 4 - EQUALIZADOR
void equalizador(int *l){
  // Estado inicial dos leds:
  int st[] = {0, 0, 0, 0, 0, 0, 0, 0};
  setLeds(l, st);
  
  // Variaveis da funcao
  int max[] = {5, 3, 7, 4, 4, 5, 7, 6};
  int *pt_leds = l;  // ponteiro pra led
  int pt_max = 0; // valor no vetor
  int inc = 1; // 1: inc, 0: dec
  
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 4){
      break;
    } else {
      // Se eh um incremento
      if (inc) {
        // Se atingiu o ultimo led:
        if (pt_leds == l+max[pt_max]) {
            // comeca a decrementar
          inc = 0;
        } else {
            // Senao eh o ultimo
            // Liga o led apontado e passa pro prox
          digitalWrite(*pt_leds, HIGH);
          pt_leds++;
        }
      }
      // Se eh decremento
      if (!inc) {
        // Se atingiu o ultimo led:
        if (pt_leds == l-1) {
          // comeca a incrementar
            inc = 1;
            // Passa prox max valor:
            // - Se estourou o vetor:
            if (pt_max >= 7){
              pt_max = 0;
            } else { // Senao
              pt_max++;
            }
        } else {
            // Senao eh o ultimo
            // Desliga o led apontado e passa pro ant
          digitalWrite(*pt_leds, LOW);
          pt_leds--;
        }
      }
    }
    delay(delayVal);
  }
}

// **************************************************************
// 5 - ON/OFF
void onOff(int *l){
  // Estado inicial dos leds:
  int st[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // Variaveis da funcao
  int state = 0;
  
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 5){
      break;
    } else {
      // Altera entre os estados on e off
      int2vec(state,st);
      setLeds(l, st);
      delay(delayVal);
      state = ~state;
    }
  }
}

// **************************************************************
// 6 - Knight Rider
void autoF(int *l){
  // Estado inicial dos leds:
  int off[] = {0, 0, 0, 0, 0, 0, 0, 0};
  
  // Variaveis da funcao
  int *pt_led_i = l;    // ponteiro para o led inicial
  int *pt_led_f = pt_led_i+4; // ponteiro para o led final
  int inc = 1;
  
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 6){
      break;
    } else {
      setLeds(l, off);
      for (int* i = pt_led_i; i < pt_led_f; i++){
        digitalWrite(*i, HIGH);
      }
      
      if (pt_led_f == l+NUM_LEDS){
        inc = 0;
      } else if (pt_led_i == l){
        inc = 1;
      }
        
      if (inc) {
        pt_led_i++;
        pt_led_f++;
      } else {
        pt_led_i--;
        pt_led_f--;
      }
      
      delay(delayVal);
    }
  }
} 

// **************************************************************
// 7 - PROPRIA: PISCA-PISCA ALTERNADO
void prop(int *l){
  // Estado inicial dos leds:
  int off[] = {0, 0, 0, 0, 0, 0, 0, 0};
  setLeds(l, off);
  
  // Variaveis da funcao
  int par[] = {1, 0, 1, 0, 1, 0, 1, 0};
  int impar[] = {0, 1, 0, 1, 0, 1, 0, 1};
  int state = 0;
  
  while(1){
    // Se mudou a chave acionada:
    estado = readChaves(chaves);
    if (estado != 7){
      break;
    } else {
      // Altera entre os estados par e impar
      if (state == 0){
        setLeds(l, par);
        state = 1;
      } else {
        setLeds(l, impar);
        state = 0;
      }
      delay(delayVal);
    }
  }
}