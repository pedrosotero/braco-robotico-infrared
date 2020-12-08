/*
 Braço robótico controlado por infra-vermelho

 Este programa controla o funcionamento de um braço robótico
 usando 4 servo motores e um receptor infra-vermelho.

 Criado pelo Grupo 5 de Introdução à Engenharia de Computação
 2020.1, composto pelos alunos: Augusto Anastacio, Beatriz
 Rocha, J. Victor Oliveira, Luiz Souza e Pedro Sotero.

 Novembro de 2020.
 
*/

#include <Servo.h>
#include <IRremote.h>

#define GARRA_MENOS 0xE0E008F7
#define GARRA_MAIS 0xE0E048B7
#define BASE_DIR 0xE0E046B9
#define BASE_ESQ 0xE0E0A659
#define ALT_MAIS 0xE0E006F9
#define ALT_MENOS 0xE0E08679
#define PROF_MAIS 0xE0E0E01F
#define PROF_MENOS 0xE0E0D02F
#define REPETICAO 0xFFFFFFFF

#define VEL1 0xE0E020DF
#define VEL2 0xE0E0A05F
#define VEL3 0xE0E0609F
#define VEL4 0xE0E010EF
#define VEL5 0xE0E0906F
#define VEL6 0xE0E050AF

#define RESET 0xE0E040BF
#define SOLTAR 0xE0E0807F
#define TEMPO 20

IRrecv receptor(6); // Definicao do receptor IR e criacao de uma variavel para armazenar a tecla
decode_results tecla;

Servo servo_base, servo_altura, servo_profund, servo_garra; // Definicao dos servos
int posBase = 50, posAltura = 80, posProfund = 100, posGarra = 90, velocidade = 3;
unsigned long anterior;

void setup()
{
    Serial.begin(9600); // Inicia o monitor serial para acompanhamento dos angulos dos motores em tempo real
  
    // Ligacao de cada servo com seu respectivo pino
    servo_base.attach(5);
    servo_altura.attach(4);
    servo_profund.attach(3);
    servo_garra.attach(2);
  
    receptor.enableIRIn(); // Inicia o receptor
}

void loop()
{
  if(receptor.decode(&tecla)){

    if(tecla.value == REPETICAO){ //Caso segure o botao
      tecla.value = anterior; //Faz com que a tecla atual, que seria 0xFFFFFFFF, receba o valor da anterior, que é uma tecla válida
    }

    if(tecla.value == VEL1){ //Muda a velocidade dos motores
      velocidade = 1;
    }else if(tecla.value == VEL2){
      velocidade = 2;
    }else if(tecla.value == VEL3){
      velocidade = 3;
    }else if(tecla.value == VEL4){
      velocidade = 4;
    }else if(tecla.value == VEL5){
      velocidade = 5;
    }else if(tecla.value == VEL6){
      velocidade = 6;
    }

    if(tecla.value == SOLTAR){  //Levanta, gira pra esquerda e abre a garra
      for( ; posAltura < 76; posAltura += 2){
        servo_altura.write(posAltura);
        delay(TEMPO);
        }

        for( ; posProfund > 55; posProfund -= 2){
        servo_profund.write(posProfund);
        delay(TEMPO);
        }

        for( ; posAltura < 134; posAltura += 2){
        servo_altura.write(posAltura);
        delay(TEMPO);
        }

        for( ; posBase < 170; posBase += 2){
        servo_base.write(posBase);
        delay(TEMPO);
        }

        posGarra = 90;
        servo_garra.write(posGarra);
      }

    if(tecla.value == RESET){   //Volta para a posicao inicial suavemente
      if(posAltura < 80){
        for( ; posAltura < 80; posAltura += 2){
        servo_altura.write(posAltura);
        delay(TEMPO);
        }
      }else if(posAltura > 80){
        for( ; posAltura > 80; posAltura -= 2){
        servo_altura.write(posAltura);
        delay(TEMPO);
        }
      }

      if(posProfund > 100){
        for( ; posProfund > 100; posProfund -= 2){
        servo_profund.write(posProfund);
        delay(TEMPO);
        }
      }else if(posProfund < 100){
        for( ; posProfund < 100; posProfund += 2){
        servo_profund.write(posProfund);
        delay(TEMPO);
        }
      }

      if(posBase < 50){
        for( ; posBase < 50; posBase += 2){
        servo_base.write(posBase);
        delay(TEMPO);
        }
      }else if(posBase > 50){
        for( ; posBase > 50; posBase -= 2){
        servo_base.write(posBase);
        delay(TEMPO);
        }
      }

        posGarra = 90;
        servo_garra.write(posGarra);
    }
    
    /*
    Cada desvio condicional aumenta ou diminui o angulo do motor
    relacionado com a tecla apertada, de acordo com a velocidade
    atual do programa e escreve esse angulo no monitor serial.

    Além disso, para resolver o problema da repeticao, ao final
    do desvio, a variavel anterior recebe o valor da tecla atual,
    assim sendo possivel sua utilizacao caso o controle apresente
    um código de repetição.
    */

    if(tecla.value == BASE_DIR){    
      posBase -= velocidade;
      
      if(posBase < 10){posBase = 10;}
      
      Serial.print("Base: ");
      Serial.println(posBase);
      anterior = tecla.value;
    }
    
    if(tecla.value == BASE_ESQ){
      posBase += velocidade;
      
      if(posBase > 170){posBase = 170;}  
      
      Serial.print("Base: ");
      Serial.println(posBase);
      anterior = tecla.value;
    }
    
    if(tecla.value == ALT_MAIS){
      posAltura += velocidade;
      
      if(posAltura > 136){posAltura = 136;}
      
      Serial.print("Altura: ");
      Serial.println(posAltura);
      anterior = tecla.value;
    }
    
    if(tecla.value == ALT_MENOS){
      posAltura -= velocidade;
      
      if(posAltura < 42){posAltura = 42;}
      
      Serial.print("Altura: ");
      Serial.println(posAltura);
      anterior = tecla.value;
    }
    
    if(tecla.value == PROF_MAIS){
      posProfund += velocidade;
      
      if(posProfund > 160){posProfund = 160;}
  
      Serial.print("Profundidade: ");
      Serial.println(posProfund);
      anterior = tecla.value;
    }
    
    if(tecla.value == PROF_MENOS){
      posProfund -= velocidade;

      if(posProfund < 50){posProfund = 50;}
      
      Serial.print("Profundidade: ");
      Serial.println(posProfund);
      anterior = tecla.value;
    }
    
    if(tecla.value == GARRA_MENOS){
      posGarra += 4;
      
      if(posGarra > 110){posGarra = 110;}
      
      Serial.print("Garra: ");
      Serial.println(posGarra);
      anterior = tecla.value;
    }
    
    if(tecla.value == GARRA_MAIS){
      posGarra -= 4;
      
      if(posGarra < 90){posGarra = 90;}
      
      Serial.print("Garra: ");
      Serial.println(posGarra);
      anterior = tecla.value;
    }
    
    // Escreve em cada motor o angulo atual
    servo_base.write(posBase);
    servo_altura.write(posAltura);
    servo_profund.write(posProfund);
    servo_garra.write(posGarra);
    
    Serial.println(tecla.value, HEX);
    
    receptor.resume(); // Libera o receptor para captar uma nova tecla
  }  
}
