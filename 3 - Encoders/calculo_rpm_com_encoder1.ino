// C++ code
//
#define PINENCODER 2
#define DELTAPULSO 250

volatile unsigned long pulsos;				//volatile pois o valor dela pode ser alterado em mais de um lugar
unsigned long verificaPulso;
float velocidadeRPM;
unsigned long dT, ultimaVerificacao;

void incrementaPulsos(){
  pulsos++; 
}

void setup(){
  Serial.begin(9600);						//9600 bps
  pinMode(PINENCODER, INPUT);
  pulsos = 0L;
  attachInterrupt(digitalPinToInterrupt(PINENCODER), incrementaPulsos, FALLING);		// PINO 2 corresponde a interrução 0, borda de descida
  verificaPulso = ultimaVerificacao = millis();
}	


void loop(){
  if (millis() >= verificaPulso){
    dT = millis() - ultimaVerificacao;
    noInterrupts();
    // Calcular velocidade em RPM
    // fazendo medições foi verificado que o encoder da 324 pulsos por volta
    velocidadeRPM = 60 * (((float)pulsos / 324) * 1000) / dT;
    pulsos = 0L;							//Zerar contador de pulsos
    interrupts();
    ultimaVerificacao = millis();
    verificaPulso = ultimaVerificacao + DELTAPULSO;
    Serial.print(velocidadeRPM);
    Serial.println(" RPM");
  }
}