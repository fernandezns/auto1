/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 typedef union{
        uint8_t u8[4];
        int8_t  i8[4];

        uint16_t u16[2];
        int16_t  i16[2];

        uint32_t u32;
        int32_t  i32;

        float    f;
}_sWork;

typedef union{
    struct{
        uint8_t b0:1;
        uint8_t b1:1;
        uint8_t b2:1;
        uint8_t b3:1;
        uint8_t b4:1;
        uint8_t b5:1;
        uint8_t b6:1;
        uint8_t b7:1;
    }bit;
    uint8_t byte;
}flag;

flag flag1;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define race flag1.bit.b0

#define UART_PC		2
#define UART_ESP	1
#define LONG_CWJAP_MEGA	40
#define LONG_CWJAP_MICROS	35
#define LONG_ANS_CWJAP_MEGA	74
#define LONG_ANS_CWJAP_MICROS 70
const char CWJAP_MEGA[]={'A','T','+','C','W','J','A','P','=','"','M','E','G','A','C','A','B','L','E','-','7','0','"',',','"','6','0','2','A','D','0','7','4','F','7','7','0','"','\r','\n'};
const char CWJAP_MICROS[]={'A','T','+','C','W','J','A','P','=','"','M','I','C','R','O','S','"',',','"','m','i','c','r','o','s','1','2','3','4','5','6','7','"','\r','\n'};
const char CIFSR[]={"AT+CIFSR\r\n"};
const char CIPMUX[]={"AT+CIPMUX=0\r\n"};
const char CIPSTART[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
					   '"',',','"','1','9','2','.','1','6','8','.','0','.','1','3','"',',',
					   '3','0','0','1','6',',','3','0','1','6','\r','\n'};
const char CIPSEND[]={"AT+CIPSEND="};
const char CIPCLOSE[]={"AT+CIPCLOSE\r\n"};
const char CWQAP[]={"AT+CWQAP\r\n"};
const char CWMODE[]={"AT+CWMODE=3\r\n"};
const char ANS_CWQAP[]={"AT+CWQAP\r\n\r\nOK\r\n"};
const char ANS_CWMODE[]={"AT+CWMODE=3\r\n\r\nOK\r\n"};

const char ANS_CWJAP_MEGA[]={'A','T','+','C','W','J','A','P','=','"','M','E','G','A','C','A','B','L','E','-','7','0','"',',','"','6','0','2','A','D','0','7','4','F','7','7','0','"',
						'\r','\n','W','I','F','I',' ','C','O','N','N','E','C','T','E','D','\r','\n','W','I','F','I',
						' ','G','O','T','I','P','\r','\n','\r','\n','O','K','\r','\n'};

const char ANS_CWJAP_MICROS[]={'A','T','+','C','W','J','A','P','=','"','M','I','C','R','O','S','"',',','"','m','i','c','r','o','s','1','2','3','4','5','6','7',
						'"','\r','\n','W','I','F','I',' ','C','O','N','N','E','C','T','E','D','\r','\n','W','I','F','I',
						' ','G','O','T','I','P','\r','\n','\r','\n','O','K','\r','\n'};

const char ANS_CIPMUX[]={"AT+CIPMUX=0\r\n\r\nOK\r\n"};
const char ANS_CIPSTART[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
						   '"',',','"','1','9','2','.','1','6','8','.','0','.','1','3','"',',','3','0','0','1','6',',','3','0','1','6','\r',
						   '\n','C','O','N','N','E','C','T','\r','\n','\r','\n','O','K','\r','\n'};//59

const char ANS_CIPSEND[]={};
const char AUTOMATIC_WIFI_CONNECTED[]={"WIFI CONNECTED\r\nWIFI GOT IP\r\n"};
const char WIFI_DISCONNECT[]={"WIFI DISCONNECT\r\n"};

const char CIFSR_STAIP[]={"+CIFSR:STAIP,"};
const char OK[]={"\r\nOK\r\n"};

//const char CIPSEND_4BYTES[]={'A','T','+','C','I','P','S','E','N','D','=','8','\r','\n','\r','\n','O','K','\r','\n','>'};
const char CIPSEND1[]={'A','T','+','C','I','P','S','E','N','D','='};
const char CIPSEND2[]={'\r','\n','\r','\n','O','K','\r','\n','>'};
const char CIPSEND3[]={"Recv"};
const char CIPSEND4[]={" bytes\r\n\r\nSEND OK\r\n"};//25

const char ALIVE[]={'U','N','E','R', 0x02 ,':',0xF0};

const char IPD[]={'\r','\n','+','I','P','D',','};
const char UNER[]={'U','N','E','R'};

const char ACK_D0[]={'U','N','E','R',0x03,':',0xD0,0x0D,0xDC};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t ADCData[32][8];

volatile uint8_t bufferTxESP[256];
volatile uint8_t bufferRxESP[256];
volatile uint8_t bufferDebug[256];
volatile uint8_t indexR_TxESP=0;
volatile uint8_t indexW_TxESP=0;
volatile uint8_t indexR_RxESP=0;
volatile uint8_t indexW_RxESP=0;
volatile uint8_t indexR_Debug=0;
volatile uint8_t indexW_Debug=0;
uint8_t AT=0;
volatile uint16_t timeOut=0;
volatile uint16_t timeOut2=0;
volatile uint16_t timeOut3=0;
volatile uint16_t timeOut4=0;
uint8_t duty=0;
uint8_t readyToSend=0;
uint8_t espReadyToRecieve=0;
uint8_t decodeCIPSEND=0;
uint8_t decodeCIFSR=0;
char espIP[15];
uint8_t lengthIP=0;

uint8_t cmdUNERprotocol;
uint8_t bytesUNERprotocol;
uint8_t decodeIPD;
uint8_t cantBytes;
uint8_t cks;
uint8_t bytesToSend;
uint8_t comando=0xF0;
volatile uint8_t indexADC=0;
uint8_t bytesToSend_aux=0;

_sWork PWM_motor1,PWM_motor2,jobTime,valueADC;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void uartTX(uint8_t uart);

void initESP();
void DecodeAnsESP();
void udpCom(uint8_t cmd);
void DecodeCommands();
void readADC();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	indexADC++;
	if(indexADC == 32)
		indexADC=0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(&(*huart)==&huart2){
		//indexW_PC++;
		//HAL_UART_Receive_IT(&huart2, &bufferPC[indexW_PC], 1);
	}

	if(&(*huart)==&huart1){
		indexW_RxESP++;
		HAL_UART_Receive_IT(&huart1, &bufferRxESP[indexW_RxESP], 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM4){	//TIMER 4:	500us
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADCData[indexADC],8);
		if(timeOut>0)
			timeOut--;
		if(timeOut2>0)
			timeOut2--;
		if(timeOut3>0)
			timeOut3--;
		if(timeOut4>0)
			timeOut4--;
	}
}

void readADC(){
//	if(indexADC>0)
//		valueADC.u16[0]=ADCData[indexADC-1][0];
}

void uartTX(uint8_t uart){

	if(uart == UART_PC ){
		if((huart2.Instance->SR & UART_FLAG_TXE)==UART_FLAG_TXE){
			huart2.Instance->DR=bufferDebug[indexR_Debug];
			indexR_Debug++;
		}
	}

	if(uart == UART_ESP ){
		if((huart1.Instance->SR & UART_FLAG_TXE)==UART_FLAG_TXE){
			huart1.Instance->DR=bufferTxESP[indexR_TxESP];
			indexR_TxESP++;
		}
	}
}

void  initESP(){

	if(readyToSend){
		switch(AT){
			case 0:
				memcpy(&bufferTxESP[indexW_TxESP],CWMODE,13);
				indexW_TxESP+=13;
				timeOut2=200;
				readyToSend=0;
				break;
			case 1:
				timeOut2=20000;
				readyToSend=0;
				break;
			case 2:
				memcpy(&bufferTxESP[indexW_TxESP],CWJAP_MEGA,LONG_CWJAP_MEGA);
				indexW_TxESP+=LONG_CWJAP_MEGA;
				timeOut2=2000;
				readyToSend=0;
				break;
			case 3:
				memcpy(&bufferTxESP[indexW_TxESP],CIPMUX,13);
				indexW_TxESP+=13;
				timeOut2=200;
				readyToSend=0;
				break;
			case 4:
				memcpy(&bufferTxESP[indexW_TxESP],CIFSR,10);
				indexW_TxESP+=10;
				timeOut2=4000;
				readyToSend=0;
				break;
			case 5:
				memcpy(&bufferTxESP[indexW_TxESP],CIPSTART,45);
				indexW_TxESP+=45;
				timeOut2=2000;
				readyToSend=0;
				break;
			case 6:
				duty++;
				break;
		}
	}
}

void DecodeAnsESP(){
	static uint8_t i=0,k=2;

	bufferDebug[indexW_Debug]=bufferRxESP[indexR_RxESP];
	indexW_Debug++;

	switch(AT){
		case 0:
			if(bufferRxESP[indexR_RxESP]==ANS_CWMODE[i]){
				i++;
				if(i==19){
					AT++;
					i=0;
					readyToSend=1;
				}
			}else{
				if(i>0){
					indexR_RxESP=indexW_RxESP;
					readyToSend=1;
					i=0;
				}
			}
			indexR_RxESP++;
			break;

		case 1:
			if(bufferRxESP[indexR_RxESP]==AUTOMATIC_WIFI_CONNECTED[i]){
				i++;
				if(i==29){
					AT=3;
					i=0;
					readyToSend=1;
				}
			}
			else{
				if(bufferRxESP[indexR_RxESP]==WIFI_DISCONNECT[i]){
					i++;
					if(i==17){
						AT=0;
						i=0;
						readyToSend=1;
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);//Reset ESP8266
						timeOut=2000;
					}
				}else{
					AT=2;
					i=0;
					readyToSend=1;
				}
			}
			indexR_RxESP++;
			break;
		case 2:
			if(bufferRxESP[indexR_RxESP]==ANS_CWJAP_MEGA[i]){
				i++;

				if(i==LONG_ANS_CWJAP_MEGA){
					AT++;
					i=0;
					readyToSend=1;
				}
			}
			else{
				if(i>0){
					indexR_RxESP=indexW_RxESP;
					readyToSend=1;
					i=0;
					break;
				}
			}
			indexR_RxESP++;
			break;
		case 3:
			if(bufferRxESP[indexR_RxESP]==ANS_CIPMUX[i]){
				i++;
				if(i==19){
					AT++;
					i=0;
					readyToSend=1;
				}
			}else{
				if(i>0){
					indexR_RxESP=indexW_RxESP;
					readyToSend=1;
					i=0;
					break;
				}
			}
			indexR_RxESP++;
			break;
		case 4:
			switch(decodeCIFSR){
				case 0:
					if(bufferRxESP[indexR_RxESP]==CIFSR[i]){
						i++;

						if(i==10){
							decodeCIFSR++;
							i=0;
						}
					}else{
						if(i>0){
							indexR_RxESP=indexW_RxESP;
							readyToSend=1;
							i=0;
							break;
						}
					}
					indexR_RxESP++;
					break;
				case 1:
					if(bufferRxESP[indexR_RxESP]==CIFSR_STAIP[i]){
						i++;
						if(i==13){
							decodeCIFSR++;
							i=0;
						}
					}
					indexR_RxESP++;
					break;
				case 2:
					espIP[i]=bufferRxESP[indexR_RxESP];
					i++;
					lengthIP++;
					indexR_RxESP++;
					if((bufferRxESP[indexR_RxESP]=='"')&&(lengthIP>1)){
						i=0;
						decodeCIFSR++;
					}
					break;
				case 3:
					if(bufferRxESP[indexR_RxESP]==OK[i]){
						i++;
						if(i==6){
							AT++;
							i=0;
							readyToSend=1;
							decodeCIFSR=0;
						}
					}
					indexR_RxESP++;
					break;
			}

			break;
		case 5:
			if(bufferRxESP[indexR_RxESP]==ANS_CIPSTART[i]){
				i++;
				if(i==60){
					AT++;
					i=0;
					readyToSend=1;
				}
			}else{
				if(i>0){
					indexR_RxESP=indexW_RxESP;
					readyToSend=1;
					i=0;
					break;
				}
			}
			indexR_RxESP++;
			break;
		case 6:
			switch(decodeCIPSEND){
					case 0:
						if(bufferRxESP[indexR_RxESP]==CIPSEND1[i]){
							i++;
							if(i==11){
								i=0;
								decodeCIPSEND++;
							}
						}
						else{
							if(i>0){
								indexR_RxESP=indexW_RxESP;
								readyToSend=1;
								i=0;
								break;
							}
						}
						indexR_RxESP++;
						break;
					case 1:
						if((bufferRxESP[indexR_RxESP]==bytesToSend+'0')&&((bytesToSend<10))){
							decodeCIPSEND=3;
						}else{

							if(bufferRxESP[indexR_RxESP]==bytesToSend/10+'0'){
								decodeCIPSEND++;
								bytesToSend_aux=bytesToSend/10;
								bytesToSend_aux*=10;
							}

						}
						indexR_RxESP++;
						break;
					case 2:
						if(bufferRxESP[indexR_RxESP]==bytesToSend-bytesToSend_aux+'0'){
							decodeCIPSEND++;
						}
						indexR_RxESP++;
						break;
					case 3:
						if(bufferRxESP[indexR_RxESP]==CIPSEND2[i]){
							i++;
							if(i==9){
								i=0;
								decodeCIPSEND++;
								espReadyToRecieve=1;
								readyToSend=1;
							}
						}
						indexR_RxESP++;
						break;
					case 4:
						if(bufferRxESP[indexR_RxESP]==CIPSEND3[i]){
							i++;
							if(i==4){
								i=0;
								decodeCIPSEND++;
							}
						}
						indexR_RxESP++;
						break;
					case 5:
						if((bufferRxESP[indexR_RxESP]==bytesToSend+'0')&&((bytesToSend<10))){
							decodeCIPSEND=7;
						}else{

							if(bufferRxESP[indexR_RxESP]==bytesToSend/10+'0'){
								decodeCIPSEND++;
							}

						}
						indexR_RxESP++;
						break;
					case 6:
						if(bufferRxESP[indexR_RxESP]==bytesToSend-bytesToSend_aux+'0'){
							decodeCIPSEND++;
						}
						indexR_RxESP++;
						break;
					case 7:
						if(bufferRxESP[indexR_RxESP]==CIPSEND4[i]){
							i++;
							if(i==19){
								i=0;
								decodeCIPSEND=0;
								readyToSend=1;
								AT++;
							}
						}

						indexR_RxESP++;
						break;
				}

			break;
		case 7:
			timeOut2=2;
			switch(decodeIPD){
				case 0:
					if(bufferRxESP[indexR_RxESP]==IPD[i]){
						i++;

						if(i==7){
							i=0;
							decodeIPD++;
						}
					}
					else{
						if(i>0){
							indexR_RxESP=indexW_RxESP;
							i=0;
							break;
						}
					}
					indexR_RxESP++;
					break;
				case 1:
					if(bufferRxESP[indexR_RxESP]==':'){
						decodeIPD++;
					}
					indexR_RxESP++;
					break;
				case 2:
					if(bufferRxESP[indexR_RxESP]==UNER[i]){
						i++;
						if(i==4){
							i=0;
							decodeIPD++;
							cks='U'^'N'^'E'^'R';
						}
					}else{
						if(i>0){
							indexR_RxESP=indexW_RxESP;
							i=0;
							decodeIPD=0;
							break;
						}
					}
					indexR_RxESP++;
					break;
				case 3:
					bytesUNERprotocol=bufferRxESP[indexR_RxESP];
					decodeIPD++;
					cks^=bufferRxESP[indexR_RxESP];
					indexR_RxESP++;
					break;
				case 4:
					if(bufferRxESP[indexR_RxESP]==':'){
						decodeIPD++;
						cks^=bufferRxESP[indexR_RxESP];

					}else{
						indexR_RxESP=indexW_RxESP;
						i=0;
						decodeIPD=0;
						break;
					}
					indexR_RxESP++;
					break;
				case 5:
					cmdUNERprotocol=bufferRxESP[indexR_RxESP];
					decodeIPD++;
					cks^=bufferRxESP[indexR_RxESP];
					indexR_RxESP++;
					i=indexR_RxESP;
					break;
				case 6:
					if(k<bytesUNERprotocol){
						cks^=bufferRxESP[i];
						i++;
						k++;
					}else{
						if(cks==bufferRxESP[i]){
							DecodeCommands();
							indexR_RxESP++;
							k=2;
							i=0;
							decodeIPD=0;
						}else{
							indexR_RxESP=indexW_RxESP;
							i=0;
							decodeIPD=0;
							k=2;
						}
					}
					break;
			}
			break;
	}
}

void udpCom(uint8_t cmd){
	if(readyToSend){
		switch(cmd){
			case 0xF0:
				if(!espReadyToRecieve){
					AT=6;
					memcpy(&bufferTxESP[indexW_TxESP],CIPSEND,11);
					indexW_TxESP+=11;
					memcpy(&bufferTxESP[indexW_TxESP],"8\r\n",3);
					indexW_TxESP+=3;
					bytesToSend=8;
					timeOut2=400;
					readyToSend=0;
				}
				else{
					memcpy(&bufferTxESP[indexW_TxESP],ALIVE,7);
					indexW_TxESP+=7;
					bufferTxESP[indexW_TxESP]='U'^'N'^'E'^'R'^0x02^':'^0xF0;
					indexW_TxESP++;
					espReadyToRecieve=0;
					duty++;
					timeOut=400;
				}
				break;
			case 0xD0:
				if(!espReadyToRecieve){
					AT=6;
					memcpy(&bufferTxESP[indexW_TxESP],CIPSEND,11);
					indexW_TxESP+=11;
					memcpy(&bufferTxESP[indexW_TxESP],"9\r\n",3);
					indexW_TxESP+=3;
					bytesToSend=9;
					timeOut2=400;
					readyToSend=0;
				}
				else{
					memcpy(&bufferTxESP[indexW_TxESP],ACK_D0,9);
					indexW_TxESP+=9;
					bufferTxESP[indexW_TxESP]='U'^'N'^'E'^'R'^0x03^':'^0xD0^0x0D;
					indexW_TxESP++;
					espReadyToRecieve=0;
					duty++;
				}
				break;
			case 0xA1:
				if(!espReadyToRecieve){
					AT=6;
					memcpy(&bufferTxESP[indexW_TxESP],CIPSEND,11);
					indexW_TxESP+=11;
					memcpy(&bufferTxESP[indexW_TxESP],"10\r\n",4);
					indexW_TxESP+=4;
					bytesToSend=10;
					timeOut2=200;
					readyToSend=0;
				}
				else{
					memcpy(&bufferTxESP[indexW_TxESP],UNER,4);
					indexW_TxESP+=4;
					bufferTxESP[indexW_TxESP]=0x04;
					indexW_TxESP++;
					bufferTxESP[indexW_TxESP]=':';
					indexW_TxESP++;
					bufferTxESP[indexW_TxESP]=0xA1;
					indexW_TxESP++;
					bufferTxESP[indexW_TxESP]=valueADC.u8[0];
					indexW_TxESP++;
					bufferTxESP[indexW_TxESP]=valueADC.u8[1];
					indexW_TxESP++;
					bufferTxESP[indexW_TxESP]='U'^'N'^'E'^'R'^0x04^':'^0xA1^valueADC.u8[0]^valueADC.u8[1];
					indexW_TxESP++;
					espReadyToRecieve=0;
					readyToSend=0;
					timeOut=2000;
					timeOut2=200;
				}
				break;
		}
	}
}

void DecodeCommands(){
	switch(cmdUNERprotocol){
//		case 0xF0:
//			duty=2;
//			readyToSend=1;
//			AT=6;
//			indexR_RxESP=indexW_RxESP;
//			break;

		case 0xD0://COMANDO EN HEX 55 4E 45 52 0E 3A D0 C8 00 00 00 C8 00 00 00 88 13 00 00 73 (5 SEG)
			//55 4E 45 52 0E 3A D0 C8 00 00 00 C8 00 00 00 E8 03 00 00 03 (1 SEG)
			//55 4E 45 52 0E 3A D0 C8 00 00 00 C8 00 00 00 D0 07 00 00 3F (2 SEG)
			PWM_motor1.u8[0]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			PWM_motor1.u8[1]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			PWM_motor1.u8[2]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			PWM_motor1.u8[3]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;

			PWM_motor2.u8[0]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			PWM_motor2.u8[1]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			PWM_motor2.u8[2]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			PWM_motor2.u8[3]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;

			jobTime.u8[0]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			jobTime.u8[1]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			jobTime.u8[2]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;
			jobTime.u8[3]=bufferRxESP[indexR_RxESP];
			indexR_RxESP++;

			timeOut4=jobTime.u32/100;

			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,PWM_motor1.u32);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,PWM_motor2.u32);
			race=1;
			duty=2;
			comando=0xD0;
			readyToSend=1;
			break;

	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim3);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Start_IT(&htim4);
  //HAL_UART_Receive_IT(&huart2, &bufferPC[indexW_PC], 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);//ESP8266 Pin Enable
  timeOut2=6000;
  timeOut3=2000;
  timeOut=6000;
  readyToSend=1;
  race=0;
  duty=0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(!timeOut3){
		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  timeOut3=2000;
		  readADC();

	  }
	  switch(duty){
	  	  case 0:
	  		  if(!timeOut){
	  			 initESP();
	  			 HAL_UART_Receive_IT(&huart1, &bufferRxESP[indexW_RxESP], 1);
	  		  	  duty++;
	  		  }
	  		  break;
	  	  case 1:
	  		  initESP();
	  		  break;
	  	  case 2:
	  		  udpCom(comando);
	  		  break;
	  	  case 3:
	  		  if(!timeOut)
	  			  udpCom(0xA1);
	  		  break;
	  }

	  if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==GPIO_PIN_RESET)&&(!timeOut)){
		  HAL_UART_AbortReceive_IT(&huart1);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);//ESP8266 Pin Enable
		  timeOut=30;
		  AT=1;
		  duty=0;
	  }




	  if(indexR_TxESP!=indexW_TxESP){
		  uartTX(UART_ESP);
	  }

	  if(indexR_Debug!=indexW_Debug){
		  uartTX(UART_PC);
	  }

	  if((indexR_RxESP!=indexW_RxESP)&&(!timeOut2)){
		  DecodeAnsESP();
	  }

	  if((!timeOut4)&&(race)){
		race=0;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 8;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 99;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 210;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 42;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
