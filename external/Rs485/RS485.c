/*
 * RS485.c
 *
 *  Created on: Nov 18, 2018
 *      Author: win7pro
 */
#include "RS485.h"

extern void USART2_IRQHandler(void);

extern UART_HandleTypeDef huart2;

extern void ModBus_ReceiveByte(uint8_t receivedByte);

uint32_t gonderilen = 0;
uint32_t gelen = 0;

/******************************************************************************
* Function    : MX_USART2_UART_Init
* Description : Printer
*******************************************************************************/
void MX_RS485_UART_Init(uint32_t IntEnable)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();


	/*Configure GPIO pin : DE Pin */
	GPIO_InitStruct.Pin = RS485_DE;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RS485_GPIO_Port, &GPIO_InitStruct);

	/**USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX
	*/
	GPIO_InitStruct.Pin       = RS485_RX|RS485_TX;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(RS485_GPIO_Port, &GPIO_InitStruct);

	if(IntEnable)
		USART2->CR1 |= (USART_CR1_UE |            //  USART
										USART_CR1_TE |            //
										USART_CR1_RE |
										USART_CR1_TCIE |          // Transmit Complete int enable
										USART_CR1_RXNEIE);        //

	huart2.Instance          = USART2;
	huart2.Init.BaudRate     = 9600;
	huart2.Init.WordLength   = UART_WORDLENGTH_8B;
	huart2.Init.StopBits     = UART_STOPBITS_1;
	huart2.Init.Parity       = UART_PARITY_NONE;
	huart2.Init.Mode         = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
	 //_Error_Handler(__FILE__, __LINE__);
	}
	if(IntEnable)
	{  /* USART2 interrupt Init */
	 HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
	 HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}

//*****************************************************************************
// Function    : USART2_IRQHandler
// Description : handles USART 2 RX TX interrupt request.
//*****************************************************************************
void USART2_IRQHandler(void)
{
	if((USART2->SR & USART_SR_RXNE) && !(USART2->SR & (USART_SR_PE |    //USART_SR_RXNE: rx data ready bit
                                                    USART_SR_FE |
                                                    USART_SR_NE |
                                                    USART_SR_ORE)))
	{
		gelen++;
		//ModBus_ReceiveByte(USART2->DR);
		USART2->SR &= ~USART_SR_RXNE;
	}
	if((USART2->SR & USART_SR_TC) && !(USART2->SR & (USART_SR_PE |			//USART_SR_TC: transmit completed bit
                                                    USART_SR_FE |
                                                    USART_SR_NE |
                                                    USART_SR_ORE)))
	{
		gonderilen++;
		USART2->CR1 &= ~USART_CR1_TCIE;   // Disable TC int
		DE_Low();
		USART2->SR &= ~USART_SR_TC;       // Clear TC Flag
	}
}

void DE_High(void)
{
	HAL_GPIO_WritePin(RS485_GPIO_Port,RS485_DE, GPIO_PIN_SET);
}

void DE_Low(void)
{
	HAL_GPIO_WritePin(RS485_GPIO_Port,RS485_DE, GPIO_PIN_RESET);
}
/******************************************************************************
* Function    : RS485_SendByte
* Description :
*******************************************************************************/
void RS485_SendByte(uint8_t C)
{
   while(!(USART2->SR & USART_SR_TXE));
   USART2->DR = C;
}


/******************************************************************************
* Function   : UART3_Write_Text
* Description :
*******************************************************************************/
void RS485_SendBuffer(uint8_t *Buff, uint8_t LenBuff)
{
  uint8_t i;
	DE_High();
	for(i=0;i<LenBuff;i++)
	{
		if ( i == (LenBuff-1) )
			USART2->CR1 |= USART_CR1_TCIE;  //Tx transmit completed interrupt enable
		else
			USART2->CR1 &= ~USART_CR1_TCIE;
		
		RS485_SendByte(Buff[i]);
	}
}
