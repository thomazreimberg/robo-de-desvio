#include <Servo.h>

Servo leftMotor;
Servo rightMotor;

const int serialPeriod = 250;       // limita a saída para o console para uma vez a cada quarto de segundo
unsigned long timeSerialDelay = 0;

const int loopPeriod = 20;          // define a frequência com que o sensor faz uma leitura para 20 ms, uma frequência de 50 Hz
unsigned long timeLoopDelay   = 0;

// designa as funções TRIG e ECHO aos pinos do Arduino. Ajuste os números se tiver feito conexões diferentes
const int ultrasonic2TrigPin = 9;
const int ultrasonic2EchoPin = 8;

int ultrasonic2Distance;
int ultrasonic2Duration;

// define os dois estados possíveis para o robô: para frente (DRIVE_FORWARD) ou para a esquerda (TURN_LEFT)
#define DRIVE_FORWARD     0
#define TURN_LEFT         1

int state = DRIVE_FORWARD; // 0 = para frente (PADRÃO), 1 = para a esquerda

void setup()
{
    Serial.begin(9600);
  
    // configurações dos pinos do sensor
    pinMode(ultrasonic2TrigPin, OUTPUT);
    pinMode(ultrasonic2EchoPin, INPUT);
    
    // designa os motores aos pinos Arduino
    leftMotor.attach(12);
    rightMotor.attach(13);
}


void loop()
{
    if(digitalRead(2) == HIGH) // detecta o botão
    {
        while(1)
        {
            leftMotor.write(90);
            rightMotor.write(90);
        }
    }

    debugOutput(); // imprime mensagens de depuração ao console serial
    
    if(millis() - timeLoopDelay >= loopPeriod)
    {
        readUltrasonicSensors(); // instrui o sensor a ler e armazenar as distâncias medidas
        
        stateMachine();
        
        timeLoopDelay = millis();
    }
}


void stateMachine()
{
    if(state == DRIVE_FORWARD) // se nenhum obstáculo for detectado
    {
        if(ultrasonic2Distance > 6 || ultrasonic2Distance < 0) // se não houver nada na frente do robô. A ultrasonicDistance (distância ultrassônica) será negativa para alguns sensores ultrassônicos se não houver obstáculo
        {
            // drive forward
            rightMotor.write(180);
            leftMotor.write(0);
        }
        else //  se houver um objeto à frente
        {
            state = TURN_LEFT;
        }
    }
    else if(state == TURN_LEFT) // se um obstáculo for detectado, vire para a esquerda
    {
        unsigned long timeToTurnLeft = 500; // leva cerca de 0,5 segundos para virar em 90 graus. Você pode precisar ajustar esse valor se as rodas tiverem um tamanho diferente do exemplo.
        
        unsigned long turnStartTime = millis(); // salva o tempo em que o giro começa

        while((millis()-turnStartTime) < timeToTurnLeft) // fica nesse loop até que timeToTurnLeft passe
        {
            // vira para a esquerda. Lembre-se de que, quando ambos estiverem em "180", ele vai virar
            rightMotor.write(180);
            leftMotor.write(180);
        }
        
        state = DRIVE_FORWARD;
    }
}


void readUltrasonicSensors()
{
    // para o ultrassônico 2. Você pode precisar alterar esses comandos se usar um sensor diferente.
    digitalWrite(ultrasonic2TrigPin, LOW);
    delayMicroseconds(2);                 
    digitalWrite(ultrasonic2TrigPin, HIGH);
    delayMicroseconds(10);                  
    digitalWrite(ultrasonic2TrigPin, LOW);
    
    ultrasonic2Duration = pulseIn(ultrasonic2EchoPin, HIGH);
    ultrasonic2Distance = (ultrasonic2Duration/2)/29;
}


    // o código a seguir serve para depurar erros no console.
void debugOutput()
{
        Serial.print("ultrasonic2Distance: ");
        Serial.print(ultrasonic2Distance);
        Serial.print("cm");
        Serial.println();
        

}
