/*
 * RCC_program.c
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	2.3 		Memory map
 *  	STM RM0368:	6.3.22 		RCC register map
 */


#ifndef		RCC_PRIVATE_H
#define		RCC_PRIVATE_H


typedef struct{
	uint32_t CR;				//	00
	uint32_t CFGR;			//	04
	uint32_t CIR;			//	08
	uint32_t APB2RSTR;		//	0C
	uint32_t APB1RSTR;		//	10
	uint32_t AHBENR;			//	14
	uint32_t APB2ENR;		//	18
	uint32_t APB1ENR;		//	1C
	uint32_t BDCR;			//	20
	uint32_t CSR;			//	24
}RCC_t;

#define RCC		((volatile RCC_t*)0x40021000)

/*	RCC_CR bits	*/
#define RCC_HSION								0
#define RCC_HSIRDY								1
#define RCC_HSITRIM_0							3
#define RCC_HSICAL_0							8
#define RCC_HSEON								16
#define RCC_HSERDY								17
#define RCC_HSEBYP								18
#define RCC_CSSON								19
#define RCC_PLLON								24
#define RCC_PLLRDY								25
#define RCC_PLL2ON								26
#define RCC_PLL2RDY								27
#define RCC_PLL3ON								28
#define RCC_PLL3RDY								29

/*	RCC_CFGR bits	*/
#define RCC_SW_0							0
#define RCC_SW_1							1
#define RCC_SWS_0							2
#define RCC_SWS_1							3
#define RCC_HPRE_0							4
#define RCC_PPRE1_0							8
#define RCC_PPRE2_0							11
#define RCC_ADCPRE_0						14
#define RCC_PLLSRC							16
#define RCC_PLLXTPRE						17
#define RCC_PLLMUL_0						18
#define RCC_OTGFSPRE						22
#define RCC_MCO_0							24

/*	RCC_CIR bits	*/
#define RCC_LSIRDYF							0
#define RCC_LSERDYF							1
#define RCC_HSIRDYF							2
#define RCC_HSERDYF							3
#define RCC_PLLRDYF							4
#define RCC_PLL2RDYF						5
#define RCC_PLL3RDYF						6
#define RCC_CSSF							7
#define RCC_LSIRDYIE						8
#define RCC_LSERDYIE						9
#define RCC_HSIRDYIE						10
#define RCC_HSERDYIE						11
#define RCC_PLLRDYIE						12
#define RCC_PLL2RDYIE						13
#define RCC_PLL3RDYIE						14
#define RCC_LSIRDYC							16
#define RCC_LSERDYC							17
#define RCC_HSIRDYC							18
#define RCC_HSERDYC							19
#define RCC_PLLRDYC							20
#define RCC_PLL2RDYC						21
#define RCC_PLL3RDYC						21
#define RCC_CSSC							23

#endif
