#ifndef __LCD_H_
#define __LCD_H_
#include "main.h"

void TM1637_SendByte(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t data) ;  // ����һ���ֽڵ�TM1637
void TM1637_Start(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) ;  // �����ź�
void TM1637_Stop(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) ;   // ֹͣ�ź�
void TM1637_Init(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN);// ��ʼ��TM1637
void TM1637_Display(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t* digits, uint8_t length);// ��ʾ���֣���С���㣩
void TM1637_Display1(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t* digits, uint8_t length);// ��ʾ���֣�����С���㣩
void display_float(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN,float num);
void display_angle(float num);
#endif
