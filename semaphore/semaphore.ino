/* Tarefa 6 - Introdução a engenharia de controle e automação
  Prof. Renato Ventura Bayan Henriques
  Semáforo de 4 tempos
  Grupo:
  Kelvin Schaun Brose
  Maria Clara da Silva Brandi Escobar
  Matheus Luís de Castro
  Vanessa da Fontoura
  Vitor Dal Bó Abella
*/

#include <string.h>  // para a função tolower

// inicializador de funçoes
void carSemaphoresLoop(volatile byte *position);
void closeAllCarSemaphores();
int userInputToCarSemaphorePosition();

class CarSemaphore {
  const byte GREEN_LIGHT_PIN;
  const byte RED_LIGHT_PIN;
  const byte YELLOW_LIGHT_PIN;

public:
  CarSemaphore(byte greenLightPin, byte redLightPin, byte yellowLightPin)
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
  const byte GREEN_LIGHT_PIN;
  const byte RED_LIGHT_PIN;
  const byte PUSH_BUTTON_PIN;

public:
  PedestrianSemaphore(byte greenLightPin, byte redLightPin, byte pushButtonPin)
    : GREEN_LIGHT_PIN(greenLightPin),
      RED_LIGHT_PIN(redLightPin),
      PUSH_BUTTON_PIN(pushButtonPin) {}

  bool wasPushButtonPressed = false;

  void setupPins(void) {
    pinMode(GREEN_LIGHT_PIN, OUTPUT);
    pinMode(RED_LIGHT_PIN, OUTPUT);

    pinMode(PUSH_BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), closeAllCarSemaphores, RISING);
  }

  void open(void) {
    analogWrite(RED_LIGHT_PIN, LOW);
    analogWrite(GREEN_LIGHT_PIN, 255);
  }

  void openPulsing(void) {
    if (wasPushButtonPressed) {
      for (int i = 0; i < 3; i++) {
        for (int i = 0; i <= 255; i += 15) {
          analogWrite(GREEN_LIGHT_PIN, i);
          delay(100);
        }

        for (int i = 255; i >= 0; i -= 15) {
          analogWrite(GREEN_LIGHT_PIN, i);
          delay(100);
        }
      }
    }

    wasPushButtonPressed = false;
  }

  void close(void) {
    analogWrite(GREEN_LIGHT_PIN, LOW);
    analogWrite(RED_LIGHT_PIN, 255);
  }

  void updatePushButtonState(void) {
    // Se um semáforo teve o botão apertado, então a variável de estado wasPushButtonPressed
    // é setada para true, assim sabemos qual semáforo devemos manter piscando quando o pedestre
    // aperta o botão
    if (digitalRead(PUSH_BUTTON_PIN)) {
      wasPushButtonPressed = true;
    }
  }
};


const byte NUMBER_OF_SEMAPHORES = 4;

volatile byte position = 0;  // Esta variável vai controlar qual semáforo de carro abrir
volatile bool isPedestrianPushingButton = false;  // Esta variável controla o attachInterrupt


PedestrianSemaphore pedestrianSouthEast = PedestrianSemaphore(A10, A11, 3);
PedestrianSemaphore pedestrianSouthWest = PedestrianSemaphore(A8, A9, 2);
PedestrianSemaphore pedestrianNorthWest = PedestrianSemaphore(A14, A15, 20);
PedestrianSemaphore pedestrianNorthEast = PedestrianSemaphore(A12, A13, 21);

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


// inicializar variaveis e pinos
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    pedestrianSemaphores[i].setupPins();
    pedestrianSemaphores[i].close();

    carSemaphores[i].setupPins();
    carSemaphores[i].close();
  }
}

// loop principal
void loop() {
  if (!isPedestrianPushingButton) {
    carSemaphoresLoop(&position);
  } else {
    openAllPedestrianSemaphores();
  }
}


void carSemaphoresLoop(volatile byte *position) {
  int carComingAt;

  for (*position; *position < NUMBER_OF_SEMAPHORES && !isPedestrianPushingButton; (*position)++) {
    /* Este loop itera sobre cada semáforo de carro e checa para ver se o usuário entrou com algum
    input. Se o usuário entrou com algum input, a variável *position é atualizada com o valor do semáforo
    escolhido pelo usuário.
    Se um pedestre aperta o botão para atravessar a rua, o loop termina a iteração atual e vai para a rotina
    do semáforo dos pedestres. */
    carComingAt = userInputToCarSemaphorePosition();

    if (carComingAt != -1) {  // se o input é válido
      *position = carComingAt;
    }

    carSemaphores[*position].open();
    delay(1500);
    carSemaphores[*position].attention();
    delay(1000);
    carSemaphores[*position].close();
  }

  if (*position == NUMBER_OF_SEMAPHORES) {
    *position = 0;
  }
}


void closeAllCarSemaphores() {
  // Se um pedestre aperta o botão para atravessar a rua, então mudamos a variável de estado que controla
  // o attachInterrupt para falso
  isPedestrianPushingButton = !isPedestrianPushingButton;

  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    // Este loop itera sobre todos os semáforos de pedestres e atualiza a variável de estado
    // wasPushButtonPressed para verdadeiro para o semáforo que teve o botão apertado
    pedestrianSemaphores[i].updatePushButtonState();
  }
}


void openAllPedestrianSemaphores() {
  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    // Este loop abre todos os semáforos de pedestres
    pedestrianSemaphores[i].open();
  }

  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    // Este loop itera novamente sobre os semáforos de pedestre e checa
    // qual foi o semáforo que teve o botão apertado. Se o semáforo teve o botão
    // apertado, então esse semáforo vai ter o led verde pulsando 3 vezes.
    if (pedestrianSemaphores[i].wasPushButtonPressed) {
      pedestrianSemaphores[i].openPulsing();
    }
  }

  for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++) {
    // Este loop fecha todos os semáforos de pedestres
    pedestrianSemaphores[i].close();
  }

  // Após o fim do intervalo dos semáforos dos pedestres, mudamos a variável de estado que controla
  // o attachInterrupt para falso novamente
  isPedestrianPushingButton = !isPedestrianPushingButton;
}

int userInputToCarSemaphorePosition() {
  char userInput;
  int semaphorePosition = -1;

  userInput = tolower(Serial.read());

  if (userInput == 's') {
    semaphorePosition = 0;
  } else if (userInput == 'o') {
    semaphorePosition = 1;
  } else if (userInput == 'n') {
    semaphorePosition = 2;
  } else if (userInput == 'l') {
    semaphorePosition = 3;
  }

  if (semaphorePosition != -1) {
    Serial.print("Entered input is: ");
    Serial.println(userInput);
  }

  return semaphorePosition;
}
