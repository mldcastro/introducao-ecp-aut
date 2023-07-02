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
// especificando pinos
const int PUSH_BUTTON_PIN = 2;
const int LEDS_PINS[4] = {3, 4, 5, 6};
const int NUMBER_OF_LEDS = 4;
//variaveis de estado
int pin = 0;
//int buttonState = LOW;
bool isInStandby = true;
bool isInCrescent = true;

//inicializador de funçoes
void standByMode();
void powerOffLeds();
void binaryCounter();
void blink();
//variaveis vlateis da funcao binaryCounter
int count = 0;
int ledState = LOW;
int i = 0;
int resto;
int divisor;

volatile byte state = LOW;

//funcao para apagar os leds
void powerOffLeds()
{
  for (i = 0; i < NUMBER_OF_LEDS; i++) {
    digitalWrite(LEDS_PINS[i], LOW);
  }
  state = LOW;
  delay(500);
}
//função do contador binario
void binaryCounter()
{
  for ( count = 0; count < 16; count++)
  {
    for (i = 0; i < NUMBER_OF_LEDS; i++)
    { //for que atualiza todos os leds neste loop
      /* Aqui estamos fazendo i+1 left-shifts no número
        1, isto é, estamos elevando 2 na potência i+1. */
      divisor = 1 << (i + 1);
      resto = count % divisor;    //calcula o resto do numero do loop que estamos pelo divisor atual(em qual led estamos)
      ledState = resto / (1 << i);  //divide o resto pelo valor atribuido ao led. (0=LOW e 1=HIGH)
      digitalWrite(LEDS_PINS[i], ledState);//atualiza o led "i" com seu estado "ledState"
    }
    delay(500);
  }
  powerOffLeds();
}

//função do modo standby (nivel lógico baixo)
void standByMode() {

  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    digitalWrite(LEDS_PINS[i], HIGH);
    delay(200);           //delay para acender o proximo led
  }
  for (int i = NUMBER_OF_LEDS - 1; i >= 0; i--)
  {
    digitalWrite(LEDS_PINS[i], LOW);
    delay(200);
  }
}
void blink() {
  state = !state;
  for (volatile int i = 3; i >= 0; i--)
  {
    digitalWrite(LEDS_PINS[i], LOW);
  }
}

//inicializar variaveis e pinos
void setup() {
  pinMode(PUSH_BUTTON_PIN, INPUT);
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    pinMode(LEDS_PINS[i], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), blink, RISING);
}

//loop principal
void loop()
{
  if (!state) {
    standByMode();
  }
  else {
    binaryCounter();
  }
}
