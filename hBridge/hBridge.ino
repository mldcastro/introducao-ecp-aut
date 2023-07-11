// pinos motor esquerda
const int ATIVAR_MOTOR_ESQUERDO_PIN = 4;        // L293 p1
const int MOTOR_ESQUERDO_MOVER_FRENTE_PIN = 5;  // L293 p2
const int MOTOR_ESQUERDO_MOVER_TRAS_PIN = 6;    // L293 p7
// pinos motor direita
const int ATIVAR_MOTOR_DIREITO_PIN = 12;        // L293 p9
const int MOTOR_DIREITO_MOVER_FRENTE_PIN = 10;  // L293 p15
const int MOTOR_DIREITO_MOVER_TRAS_PIN = 11;    // L293 p10

//pinos leds
const int pinLedEsquerdaMoverFrente = 2;
const int pinLedEsquerdaMoverTras = 3;
const int pinLedDireitaMoverFrente = 9;
const int pinLedDireitaMoverTras = 8;

// vetor
const int esquerda = 0;
const int direita = 1;
const int ATIVAR_MOTOR_PINS[2] = { ATIVAR_MOTOR_ESQUERDO_PIN, ATIVAR_MOTOR_DIREITO_PIN };
const int pinVetorMotorMoveFrente[2] = { MOTOR_ESQUERDO_MOVER_FRENTE_PIN, MOTOR_DIREITO_MOVER_FRENTE_PIN };
const int pinVetorMotorMoveTras[2] = { MOTOR_ESQUERDO_MOVER_TRAS_PIN, MOTOR_DIREITO_MOVER_TRAS_PIN };
const int pinLedMoveFrente[2] = { pinLedEsquerdaMoverFrente, pinLedDireitaMoverFrente };
const int pinLedMoveTras[2] = { pinLedEsquerdaMoverTras, pinLedDireitaMoverTras };

void setup() {
  // config pinos motores
  for (int i = 0; i < 2; i++) {
    pinMode(ATIVAR_MOTOR_PINS[i], OUTPUT);
    pinMode(pinVetorMotorMoveFrente[i], OUTPUT);
    pinMode(pinVetorMotorMoveTras[i], OUTPUT);
    digitalWrite(ATIVAR_MOTOR_PINS[i], HIGH);
  }
  Serial.begin(9600);
}

void loop() {
  moveFrente(255);
  delay(2000);
  frear();
  delay(2000);  // delay para dar tempo de frear

  moveTras(255);
  delay(2000);
  parar();
  delay(2000);

  giroHorarioEixo(255);
  delay(2000);
  parar();
  delay(2000);

  giroAntiHorarioEixo(255);
  delay(2000);
  parar();
  delay(2000);

  giroHorarioRoda(255);
  delay(2000);
  parar();
  delay(2000);

  giroAntiHorarioRoda(255);
  delay(2000);
  parar();
  delay(2000);
}
// Move roda i para frente, 0->esquerda, 1->direita
void moveRodaFrente(int roda, int velocidade) {
  digitalWrite(pinLedMoveFrente[roda], HIGH);
  digitalWrite(pinLedMoveTras[roda], LOW);
  analogWrite(pinVetorMotorMoveTras[roda], 0);
  analogWrite(pinVetorMotorMoveFrente[roda], velocidade);
}
// Move roda i para tras, 0->esquerda, 1->direita
void moveRodaTras(int roda, int velocidade) {
  digitalWrite(pinLedMoveFrente[roda], LOW);
  digitalWrite(pinLedMoveTras[roda], HIGH);
  analogWrite(pinVetorMotorMoveTras[roda], velocidade);
  analogWrite(pinVetorMotorMoveFrente[roda], 0);
}
// frea roda i, 0->esquerda, 1->direita
void freaRoda(int roda) {
  digitalWrite(pinLedMoveFrente[roda], HIGH);
  digitalWrite(pinLedMoveTras[roda], HIGH);
  digitalWrite(pinVetorMotorMoveFrente[roda], HIGH);
  digitalWrite(pinVetorMotorMoveTras[roda], HIGH);
}
// para roda i, 0->esquerda, 1->direita
void paraRoda(int roda) {
  digitalWrite(pinLedMoveFrente[roda], LOW);
  digitalWrite(pinLedMoveTras[roda], LOW);
  digitalWrite(pinVetorMotorMoveFrente[roda], LOW);
  digitalWrite(pinVetorMotorMoveTras[roda], LOW);
}
/*
  moveFrente - movimenta o robo para frente
    velocidade - velocidade de rotacao (0 a 255)
*/
void moveFrente(int velocidade) {
  Serial.print("Mover para frente\n");
  moveRodaFrente(esquerda, 255);
  moveRodaFrente(direita, 255);
}
/*
  moveTras - movimenta o robo para tras
    velocidade - velocidade de rotacao (0 a 255)
*/
void moveTras(int velocidade) {
  Serial.print("Mover para tras\n");
  moveRodaTras(esquerda, 255);
  moveRodaTras(direita, 255);
}
/*
  frear -  trava as rodas do robo
*/
void frear() {
  Serial.print("Frear\n");
  freaRoda(esquerda);
  freaRoda(direita);
}
/*
  parar - robo parado (neutro)
*/
void parar() {
  Serial.print("Parar\n");
  paraRoda(esquerda);
  paraRoda(direita);
}
/*
  Giro horario sobre eixo do robo
*/
void giroHorarioEixo(int velocidade) {
  Serial.print("Giro horario sobre eixo do robo\n");
  moveRodaFrente(esquerda, 255);
  moveRodaTras(direita, 255);
}
/*
  Giro ANTI horario sobre eixo do robo
*/
void giroAntiHorarioEixo(int velocidade) {
  Serial.print("Giro ANTI horario sobre eixo do robo\n");
  moveRodaFrente(direita, 255);
  moveRodaTras(esquerda, 255);
}
/*
  Giro horario sobre roda direita
*/
void giroHorarioRoda(int velocidade) {
  Serial.print("Giro horario sobre roda direita\n");
  moveRodaFrente(esquerda, 255);
  paraRoda(direita);
}
/*
  Giro ANTI horario sobre roda esquerda
*/
void giroAntiHorarioRoda(int velocidade) {
  Serial.print("Giro ANTI horario sobre roda esquerda\n");
  moveRodaFrente(direita, 255);
  paraRoda(esquerda);
}