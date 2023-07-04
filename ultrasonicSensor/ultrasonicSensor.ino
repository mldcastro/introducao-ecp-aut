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
const int MIN_DISTANCE_CM = 2;   // distância mínima em cm
const int MAX_DISTANCE_CM = 31;  // distância máxima em cm

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
  const float SPEED_OF_SOUND_CM_US = 0.034;

  unsigned int pulseDuration = pulseIn(echoPin, HIGH);
  unsigned int distance = (pulseDuration / 2.0) * SPEED_OF_SOUND_CM_US;

  return distance;
}


void binaryDistance(unsigned int distance) {
  int divisor;
  int remainder;

  // Este loop atualiza o valor de cada led para cada distância medida
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    // Aqui estamos fazendo i+1 left-shifts no número
    // 1, isto é, estamos elevando 2 na potência i+1.
    divisor = 1 << (i + 1);

    // calcula o resto do numero do loop que estamos pelo divisor atual(em qual led estamos)
    remainder = distance % divisor;

    // divide o resto pelo valor atribuido ao led. (0=LOW e 1=HIGH)
    ledState = remainder / (1 << i);
  
    digitalWrite(LEDS_PINS[i], ledState);
  }

  delay(100);
}


void ledPulse() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    for (int fadeValue = 0; fadeValue <= 225; fadeValue += 5) {
      analogWrite(LEDS_PINS[i], fadeValue);
      delay(10);
    }
  }

  delay(50);

  for (int i = NUMBER_OF_LEDS - 1; i >= 0; i--) {
    for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
      analogWrite(LEDS_PINS[i], fadeValue);
      delay(10);
    }
  }

  delay(50);
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

  if (distance >= MIN_DISTANCE_CM && distance <= MAX_DISTANCE_CM) {
    binaryDistance(distance);
  } else {
    ledPulse();
  }

  delay(10);
}
