#ifndef __LCD_H_
#define __LCD_H_
#include "main.h"

void TM1637_SendByte(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t data) ;  // 发送一个字节到TM1637
void TM1637_Start(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) ;  // 启动信号
void TM1637_Stop(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) ;   // 停止信号
void TM1637_Init(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN);// 初始化TM1637
void TM1637_Display(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t* digits, uint8_t length);// 显示数字（带小数点）
void TM1637_Display1(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t* digits, uint8_t length);// 显示数字（不带小数点）
void display_float(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN,float num);
void display_angle(float num);
#endif
