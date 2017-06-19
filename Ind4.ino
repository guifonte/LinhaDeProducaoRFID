/* Linha de produção com sensor RFID

    Projeto Final de EA076 ministrado na Universidade Estadual de Campinas

    Resumo:
    Implementação de uma esteira com um sensor RFID que identifica tags NFC e escolhe
    se o objeto deve ou não continuar na linha de produção, sendo dependendo do caso
    jogado para fora da linha. Esse projeto simula uma linha de produção de industria
    4.0 com possibilidade de tracking individual de cada produto.

    Materiais utilizados:
    3 Motores
    1 Módulo Leitor Rfid Mfrc522
    2 Tags Mifare UltraLight
    1 Tag Mifare One
    
    Descrição de funcionamento:
    A esteira, que utiliza duas esteiras roda até que um objeto com uma tag passe pelo sensor.
    Ao passar pelo sensor, se da um feedback no monitor serial do arduino a respeito da tag,
    dizendo o tipo e o número da tag. A esteira para e o programa verifica se esse objeto deve
    continuar na linha ou deve ser jogado para fora. Caso deva continuar, a esteira volta a 
    funcionar. Caso deva ser jogado para fora, a esteira anda um pouco para alinhar o objeto
    à pá do terceiro motor. O terceiro motor é ligado girando a pá e jogando o objeto para 
    fora da linha, e é ligada novamente em sentido contrário para retornar a pá para a posição
    inicial. A esteira volta a funcionar e aguardar o próximo objeto
        
    Guilherme Nishino Fontebasso - RA 135973
    João Paulo de Tassis Machado - RA 136261
    18.06.2017
*/

//Bibliotecas utilizadas pelo módulo RFID
#include <AddicoreRFID.h>
#include <SPI.h>

#define MAX_LEN 16 //Tamanho máximo para o array
#define uchar unsigned char
#define uint  unsigned int
uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; // cria um objeto AddicoreRFID para controlar o módulo RFID

// --------------------------------------------------------------------------- RFID
const int chipSelectPin = 10;
const int NRSTPD = 9;

// --------------------------------------------------------------------------- Motores
int motor_alto[] = {2,4}; //Pinos do motor de cima
int motores_esteira[] = {6, 7}; //Pinos dos motores da esteira
int enable_alto = 3; //PWM de controle de tracao do motor esquerdo
int enable_esteira = 5; //PWM de controle de tracao dos motores da esteira

//---------------------------------------------------------------------------- Funcoes

void move_esteira(){ //Liga os dois motores da esteira e certifica que o motor que empurra as caixas está desligado
  analogWrite(enable_esteira, 185);
  analogWrite(enable_alto, 0);
  digitalWrite(motores_esteira[0], HIGH);
  digitalWrite(motores_esteira[1],LOW);
}

void para_esteira(){ //Desliga o motor da esteira e certifica que o motor que empurra as caixas está desligado
  analogWrite(enable_esteira,0);
  analogWrite(enable_alto, 0);
}

void chuta_item(){ //Desliga o motor da esteira, liga o motor da que empurra as caixas em um sentido, espera um tempo e retorna ele para o outro sentido
  analogWrite(enable_esteira, 0);
  analogWrite(enable_alto, 255);
  digitalWrite(motor_alto[0],HIGH);
  digitalWrite(motor_alto[1],LOW);
  delay(1000);
  digitalWrite(motor_alto[0],LOW);
  digitalWrite(motor_alto[1],HIGH);
}

void setup() {
  Serial.begin(9600);
  //Setup das portas do motor
  pinMode(enable_alto, OUTPUT);
  pinMode(enable_esteira, OUTPUT);
  int i;
  for(i = 0; i < 2; i++){
  pinMode(motor_alto[i], OUTPUT);
  pinMode(motores_esteira[i], OUTPUT);
  }

  SPI.begin();  
  pinMode(chipSelectPin,OUTPUT);            // Setup do pin 10 como OUTPUT para conectar a o pin enable do RFID
  digitalWrite(chipSelectPin, LOW);         // Ativa o leitor RFID
  pinMode(NRSTPD,OUTPUT);                   // Setup do pin 9, Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  
}

void loop() {
  move_esteira();
  {
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;  //Selection operation block address 0 to 63
  String mynum = "";

  str[1] = 0x4400;
  //Encontra as tags e retorna o seu tipo
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
  if (status == MI_OK)
  {
      Serial.println("Tag RFID foi detectada");
      Serial.print("Tipo da Tag:\t\t");
      uint tagType = str[0] << 8;
      tagType = tagType + str[1];
      switch (tagType) { //Escreve o tipo da Tag RFID
        case 0x4400:
          Serial.println("Mifare UltraLight");
          break;
        case 0x400:
          Serial.println("Mifare One (S50)");
          break;
        case 0x200:
          Serial.println("Mifare One (S70)");
          break;
        case 0x800:
          Serial.println("Mifare Pro (X)");
          break;
        case 0x4403:
          Serial.println("Mifare DESFire");
          break;
        default:
          Serial.println("Unknown");
          break;
      }
  }

  //Retorna o número serial da tag
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
      checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
      Serial.print("O numero da tag consiste em:\t");
      //Serial.print(str[0]); //número do tipo da Tag
      Serial.print(checksum1);
  
      if(str[0] == 136)                      //Tag UltraLight - É empurrada para fora da linha
      {
          Serial.println("\nPara o retrabalho, por favor!\n");
          para_esteira();
          delay(2000);
          move_esteira();           
          delay(200);
          para_esteira();
          delay(1000);
          chuta_item();
          delay(3000);
          move_esteira();
      }
      if(str[0] == 61)                      //Tag One - Passa reto e continua
      {
          Serial.println("\nPode seguir reto!\n");
          para_esteira();
          delay(3000);
          move_esteira();
      }
      Serial.println();
      delay(1000);
  }
    
        myRFID.AddicoreRFID_Halt();      //Comanda o RFID para ficar em hibernação            

  }
}
