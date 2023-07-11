struct Engine {
  const int ENABLE_PIN;
  const int MOVE_FORWARD_PIN;
  const int MOVE_BACKWARD_PIN;
  const int LED_FORWARD_PIN;
  const int LED_BACKWARD_PIN;
};

struct Engine leftEngine = { 4, 5, 6, 2, 3 };
struct Engine rightEngine = { 12, 10, 11, 9, 8 };

void setup() {
  pinMode(leftEngine.ENABLE_PIN, OUTPUT);
  pinMode(leftEngine.MOVE_FORWARD_PIN, OUTPUT);
  pinMode(leftEngine.MOVE_BACKWARD_PIN, OUTPUT);
  pinMode(leftEngine.LED_FORWARD_PIN, OUTPUT);
  pinMode(leftEngine.LED_BACKWARD_PIN, OUTPUT);

  pinMode(rightEngine.ENABLE_PIN, OUTPUT);
  pinMode(rightEngine.MOVE_FORWARD_PIN, OUTPUT);
  pinMode(rightEngine.MOVE_BACKWARD_PIN, OUTPUT);
  pinMode(rightEngine.LED_FORWARD_PIN, OUTPUT);
  pinMode(rightEngine.LED_BACKWARD_PIN, OUTPUT);

  digitalWrite(leftEngine.ENABLE_PIN, HIGH);
  digitalWrite(rightEngine.ENABLE_PIN, HIGH);

  Serial.begin(9600);
}


void loop() {
  moveForward(255);
  brake();

  moveBackward(255);
  stop();

  clockwiseRotationAroundAxis(255);
  stop();

  counterclockwiseRotationAroundAxis(255);
  stop();

  clockwiseRotationAroundWheel(255);
  stop();

  counterclockwiseRotationAroundWheel(255);
  stop();
}


void moveWheelForward(struct Engine engine, int speed) {
  digitalWrite(engine.LED_FORWARD_PIN, HIGH);
  digitalWrite(engine.LED_BACKWARD_PIN, LOW);

  analogWrite(engine.MOVE_BACKWARD_PIN, 0);
  analogWrite(engine.MOVE_FORWARD_PIN, speed);
}


void moveWheelBackward(struct Engine engine, int speed) {
  digitalWrite(engine.LED_FORWARD_PIN, LOW);
  digitalWrite(engine.LED_BACKWARD_PIN, HIGH);

  analogWrite(engine.MOVE_BACKWARD_PIN, speed);
  analogWrite(engine.MOVE_FORWARD_PIN, 0);
}


void brakeWheel(struct Engine engine) {
  digitalWrite(engine.LED_FORWARD_PIN, HIGH);
  digitalWrite(engine.LED_BACKWARD_PIN, HIGH);
  digitalWrite(engine.MOVE_FORWARD_PIN, HIGH);
  digitalWrite(engine.MOVE_BACKWARD_PIN, HIGH);
}


void stopWheel(struct Engine engine) {
  digitalWrite(engine.LED_FORWARD_PIN, LOW);
  digitalWrite(engine.LED_BACKWARD_PIN, LOW);
  digitalWrite(engine.MOVE_FORWARD_PIN, LOW);
  digitalWrite(engine.MOVE_BACKWARD_PIN, LOW);
}


/*
  moveForward - movimenta o robo para frente
    speed - velocidade de rotacao (0 a 255)
*/
void moveForward(int speed) {
  Serial.print("Mover para frente\n");
  moveWheelForward(leftEngine, speed);
  moveWheelForward(rightEngine, speed);

  delay(2000);
}


/*
  moveBackward - movimenta o robo para tras
    speed - velocidade de rotacao (0 a 255)
*/
void moveBackward(int speed) {
  Serial.print("Mover para tras\n");
  moveWheelBackward(leftEngine, speed);
  moveWheelBackward(rightEngine, speed);

  delay(2000);
}


/*
  brake -  trava as rodas do robo
*/
void brake() {
  Serial.print("Frear\n");
  brakeWheel(leftEngine);
  brakeWheel(rightEngine);

  delay(2000);  // delay para dar tempo de frear
}


/*
  stop - robo parado (neutro)
*/
void stop() {
  Serial.print("Parar\n");
  stopWheel(leftEngine);
  stopWheel(rightEngine);

  delay(2000);
}


/*
  Giro horario sobre eixo do robo
*/
void clockwiseRotationAroundAxis(int speed) {
  Serial.print("Giro horario sobre eixo do robo\n");
  moveWheelForward(leftEngine, speed);
  moveWheelBackward(rightEngine, speed);

  delay(2000);
}

/*
  Giro ANTI horario sobre eixo do robo
*/
void counterclockwiseRotationAroundAxis(int speed) {
  Serial.print("Giro ANTI horario sobre eixo do robo\n");
  moveWheelForward(rightEngine, speed);
  moveWheelBackward(leftEngine, speed);

  delay(2000);
}


/*
  Giro horario sobre roda direita
*/
void clockwiseRotationAroundWheel(int speed) {
  Serial.print("Giro horario sobre roda direita\n");
  moveWheelForward(leftEngine, speed);
  stopWheel(rightEngine);

  delay(2000);
}


/*
  Giro ANTI horario sobre roda esquerda
*/
void counterclockwiseRotationAroundWheel(int speed) {
  Serial.print("Giro ANTI horario sobre roda esquerda\n");
  moveWheelForward(rightEngine, speed);
  stopWheel(leftEngine);

  delay(2000);
}