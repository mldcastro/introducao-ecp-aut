/* Tarefa 4
  Prof. Renato Ventura Bayan Henriques
  Contador binario com sensor ultrassônico
  Grupo:
  Kelvin Schaun Brose
  Maria Clara da Silva Brandi Escobar
  Matheus Luís de Castro
  Vanessa da Fontoura
  Vitor Dal Bó Abella */

// Constantes
const int NUMBER_OF_LEDS = 5;
const int LEDS_PINS[NUMBER_OF_LEDS] = { 5, 6, 9, 10, 11 };
const int TRIGGER_PIN = 7;
const int ECHO_PIN = 8;

// Variáveis de estado
int ledState = LOW;

// Variáveis
unsigned int distance;


static void sendTriggerPulse(int triggerPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

unsigned int measureDistance(int triggerPin, int echoPin) {
  sendTriggerPulse(triggerPin);

  // Velocidade do som em centímetros por microsegundos
  const int SPEED_OF_SOUND_CM_US = 0.034;

  unsigned int pulseDuration = pulseIn(echoPin, HIGH);
  unsigned int distance = (pulseDuration / 2) * SPEED_OF_SOUND_CM_US;

  return distance;
}


void binaryDistance(unsigned int distance) {
  int divisor;
  int remainder;

  for (int i = 0; i < NUMBER_OF_LEDS; i++) {  // for que atualiza todos os leds neste loop
    /* Aqui estamos fazendo i+1 left-shifts no número
      1, isto é, estamos elevando 2 na potência i+1. */
    divisor = 1 << (i + 1);
    remainder = distance % divisor;        //calcula o resto do numero do loop que estamos pelo divisor atual(em qual led estamos)
    ledState = remainder / (1 << i);       //divide o resto pelo valor atribuido ao led. (0=LOW e 1=HIGH)
    digitalWrite(LEDS_PINS[i], ledState);  //atualiza o led "i" com seu estado "ledState"
  }
  delay(500);
  powerOffLeds();
}


void powerOffLeds() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    digitalWrite(LEDS_PINS[i], LOW);
  }
  delay(500);
}


void setup() {
  Serial.begin(9600);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    pinMode(LEDS_PINS[i], OUTPUT);
  }
}


void loop() {
    distance = measureDistance(TRIGGER_PIN, ECHO_PIN);
    Serial.println(distance);
    binaryDistance(distance);
    delay(10);
}
