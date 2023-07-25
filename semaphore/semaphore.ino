/* Tarefa 3 - Introdução a engenharia de controle e automação
  Prof. Renato Ventura Bayan Henriques
  Contador binario com botão e com modo de standby com leds acendendo
  em ordem crescente e apagando em modo decrescente
  Grupo:
  Kelvin Schaun Brose
  Maria Clara da Silva Brandi Escobar
  Matheus Luís de Castro
  Vanessa da Fontoura
  Vitor Dal Bó Abella
*/

class CarSemaphore {
  const int GREEN_LIGHT_PIN;
  const int RED_LIGHT_PIN;
  const int YELLOW_LIGHT_PIN;

public:
  CarSemaphore(int greenLightPin, int redLightPin, int yellowLightPin)
    : GREEN_LIGHT_PIN(greenLightPin), RED_LIGHT_PIN(redLightPin), YELLOW_LIGHT_PIN(yellowLightPin) {}

  void setupPins(void) {
    pinMode(GREEN_LIGHT_PIN, OUTPUT);
    pinMode(RED_LIGHT_PIN, OUTPUT);
    pinMode(YELLOW_LIGHT_PIN, OUTPUT);
  }

  void open(void) {
    digitalWrite(GREEN_LIGHT_PIN, HIGH);
    digitalWrite(RED_LIGHT_PIN, LOW);
    digitalWrite(YELLOW_LIGHT_PIN, LOW);
  }

  void close(void) {
    digitalWrite(GREEN_LIGHT_PIN, LOW);
    digitalWrite(RED_LIGHT_PIN, HIGH);
    digitalWrite(YELLOW_LIGHT_PIN, LOW);
  }

  void attention(void) {
    digitalWrite(GREEN_LIGHT_PIN, LOW);
    digitalWrite(RED_LIGHT_PIN, LOW);
    digitalWrite(YELLOW_LIGHT_PIN, HIGH);
  }
};


class PedestrianSemaphore {
  const int GREEN_LIGHT_PIN;
  const int RED_LIGHT_PIN;

public:
  PedestrianSemaphore(int greenLightPin, int redLightPin)
    : GREEN_LIGHT_PIN(greenLightPin), RED_LIGHT_PIN(redLightPin) {}

  void setupPins(void) {
    pinMode(GREEN_LIGHT_PIN, OUTPUT);
    pinMode(RED_LIGHT_PIN, OUTPUT);
  }

  void open(void) {
    analogWrite(GREEN_LIGHT_PIN, HIGH);
    analogWrite(RED_LIGHT_PIN, LOW);
  }

  void close(void) {
    analogWrite(GREEN_LIGHT_PIN, LOW);
    analogWrite(RED_LIGHT_PIN, HIGH);
  }
};

const int NUMBER_OF_SEMAPHORES = 4;

//variaveis de estado
int pin = 0;

//int buttonState = LOW;
bool isInStandby = true;

//inicializador de funçoes
void standByMode();
void binaryCounter();
void blink();

//variaveis volateis da funcao binaryCounter
int count = 0;
int ledState = LOW;
int i = 0;
int resto;
int divisor;
int position = 0;

volatile byte state = LOW;


PedestrianSemaphore pedestrianSouthEast = PedestrianSemaphore(A10, A11);
PedestrianSemaphore pedestrianSouthWest = PedestrianSemaphore(A8, A9);
PedestrianSemaphore pedestrianNorthWest = PedestrianSemaphore(A14, A15);
PedestrianSemaphore pedestrianNorthEast = PedestrianSemaphore(A12, A13);

PedestrianSemaphore pedestrianSemaphores[NUMBER_OF_SEMAPHORES] = {
  pedestrianSouthEast,
  pedestrianSouthWest,
  pedestrianNorthWest,
  pedestrianNorthEast,
};

CarSemaphore carSouthNorth = CarSemaphore(35, 39, 37);
CarSemaphore carWestEast = CarSemaphore(41, 45, 43);
CarSemaphore carNorthSouth = CarSemaphore(23, 27, 25);
CarSemaphore carEastWest = CarSemaphore(29, 33, 31);

CarSemaphore carSemaphores[NUMBER_OF_SEMAPHORES] = {
  carSouthNorth,
  carWestEast,
  carNorthSouth,
  carEastWest,
};


//inicializar variaveis e pinos
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    pedestrianSemaphores[i].setupPins();
    pedestrianSemaphores[i].close();
    carSemaphores[i].setupPins();
    carSemaphores[i].close();
  }

  // pinMode(PUSH_BUTTON_PIN, INPUT);
  // attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), sinalN, RISING);  //x4
  // attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), sinalS, RISING);
  // attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), sinalO, RISING);
  // attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), sinalL, RISING);
}

//loop principal
void loop() {
  tecla = Serial.read();
  red();
  if (!state) {
    if (tecla == 'n')
      carroN();
    else if (tecla == 's')
      carroS();
    else if (tecla == 'o')
      carroO();
    else if (tecla == 'l')
      carroL();
    else
      standByMode(position);
  } else {
    pedestre();
  }
}



void standByMode(int position) {
  while (position < NUMBER_OF_SEMAPHORES) {
    carSemaphores[position].open();
    delay(1500);
    carSemaphores[position].attention();
    delay(1000);
    carSemaphores[position].close();

    position++;
  }
  position = 0;
}

void sinalN() {
  state = !state;
  // local = 'n';
  //for (volatile int i = 3; i >= 0; i--) { //apagar todos os leds
  //  digitalWrite(LEDS_PINS[i][i], LOW);
  //}

  for (volatile int i = 0; i < NUMBER_OF_LEDS + 1; i++) {  // todos leds semaforo

    for (volatile int j = 0; j < NUMBER_OF_LEDS; j++) {
      digitalWrite(LEDS_PINS[i][0], HIGH);
    }
  }
}

void pedestre() {
  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    //for (int j = 0; j < NUMBER_OF_LED-1; j++) {
    digitalWrite(LEDS_PINS_P[i][1], HIGH);
    //}
  }
  // if (local)
  //pisca
  for (int i = 0; i < 10; i++) {
    //for (int j = 0; j < NUMBER_OF_LED-1; j++) {
    digitalWrite(LEDS_PINS_P[i][1], HIGH);
    delay(100);
    digitalWrite(LEDS_PINS_P[i][1], LOW);
    delay(100);
    //}
  }
}

//função do contador binario
void binaryCounter() {
  for (count = 0; count < 16; count++) {
    for (i = 0; i < NUMBER_OF_LEDS; i++) {  //for que atualiza todos os leds neste loop
      /* Aqui estamos fazendo i+1 left-shifts no número
    	1, isto é, estamos elevando 2 na potência i+1. */
      divisor = 1 << (i + 1);
      resto = count % divisor;                  //calcula o resto do numero do loop que estamos pelo divisor atual(em qual led estamos)
      ledState = resto / (1 << i);              //divide o resto pelo valor atribuido ao led. (0=LOW e 1=HIGH)
      digitalWrite(LEDS_PINS[i][i], ledState);  //atualiza o led "i" com seu estado "ledState"
    }
    delay(500);
  }
  // powerOffLeds();
}
