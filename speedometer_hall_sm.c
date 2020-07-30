/**
* \version 1.0
* \author bazhen.levkovets
* \date 2019 
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary
* \copyright	Ukraine
*************************************************************************************
*/

/*
**************************************************************************
*							INCLUDE FILES
**************************************************************************
*/

#include "speedometer_hall_sm.h"

/*
**************************************************************************
*							LOCAL DEFINES
**************************************************************************
*/


/*
**************************************************************************
*							LOCAL CONSTANTS
**************************************************************************
*/


/*
**************************************************************************
*						    LOCAL DATA TYPES
**************************************************************************
*/


/*
**************************************************************************
*							  LOCAL TABLES
**************************************************************************
*/

/*
**************************************************************************
*								 MACRO'S
**************************************************************************
*/


/*
**************************************************************************
*						 LOCAL GLOBAL VARIABLES
**************************************************************************
*/

typedef struct	{
		UART_HandleTypeDef * uart_debug	;
	} Debug_struct 						;

	Debug_struct 			Debug_ch				= { 0 }	;

	#define	DEBUG_STRING_SIZE		300

	int counter = 0;

	 tm1637_struct h1_tm1637 =
	  {
		 .clk_pin  = GPIO_PIN_14,
		 .clk_port = GPIOB,
		 .dio_pin  = GPIO_PIN_15,
		 .dio_port = GPIOB
	  };

	 volatile uint8_t IRQ_flag = 0;
/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void Speedometer_init (void) {
	Debug_init(&huart1);

	char debugString[DEBUG_STRING_SIZE];
	sprintf(debugString," START\r\n AutoSpeedoMeter by Hall\r\n plus ind TM1637") ;
	Debug_print( debugString ) ;

	int		soft_version_arr_int[3] = {0} ;
	soft_version_arr_int[0] 	= ((SOFT_VERSION) / 100) %10 ;
	soft_version_arr_int[1] 	= ((SOFT_VERSION) /  10) %10 ;
	soft_version_arr_int[2] 	= ((SOFT_VERSION)      ) %10 ;
	sprintf (	debugString						,
			" 2020-June-30 v%d.%d.%d. \r\n"	,
			soft_version_arr_int[0]			,
			soft_version_arr_int[1]			,
			soft_version_arr_int[2]			) ;
	Debug_print( debugString ) ;

	tm1637_Init(&h1_tm1637);
	tm1637_Set_Brightness(&h1_tm1637, bright_45percent);
	tm1637_Display_Decimal(&h1_tm1637, 8765, double_dot);
	HAL_Delay(1000);

	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_Base_Start_IT(&htim4);
}
//***********************************************************

void Speedometer_main (void) {
	if ( IRQ_flag == 1 ) {
		char debugString[DEBUG_STRING_SIZE] ;
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) ;
		sprintf(debugString,"%04d\r\n", counter++) ;
		Debug_print( debugString ) ;

		tm1637_Display_Decimal(&h1_tm1637, counter, no_double_dot) ;
		IRQ_flag = 0 ;
	}
	//HAL_Delay(1000);
}
//***********************************************************

void Debug_print( char * _string ) {
	size_t len_szt = strlen( _string );
	if (len_szt > DEBUG_STRING_SIZE) {
		len_szt = DEBUG_STRING_SIZE ;
	}
	HAL_UART_Transmit(	Debug_ch.uart_debug		,
						(uint8_t * ) _string	,
						len_szt					,
						100						) ;
}
//***********************************************************

void Debug_init( UART_HandleTypeDef * _huart ) {
	Debug_ch.uart_debug = _huart ;
}
//*********************************************************
/*
**************************************************************************
*                           LOCAL FUNCTIONS
**************************************************************************
*/
