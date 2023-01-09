/*
 * Copyright (c) 2023 Gerhard Schiller (gerhard.schiller@protonmail.com)
 * SPDX-License-Identifier: GPL-3.0-or-later
 * https://www.gnu.org/licenses/gpl-3.0.txt
 *
 * This code is based on parts of the file: "LwIP/LwIP_HTTP_Server_Raw/Src/httpd_cg_ssi.c"
 *
 *
 * For details see the copyright notice below.
 *
 * Note: this STMicroelectronics software component has no LICENSE file
 * in its root directory.
 *
 */


/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Raw/Src/httpd_cg_ssi.c
  * @author  MCD Application Team
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"


#include <string.h>
#include <stdlib.h>

// GPIOs for Leds
#define LED1	18
#define LED2	19
#define LED3	20
#define LED4	21


/* CGI handler for LED control */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
/* CGI handler for LED control with feedback*/
const char * LEDS_CGI_Handler2(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI LEDS_CGI={"/leds.cgi", LEDS_CGI_Handler};
/* Html request for "/leds2.cgi" will start LEDS_CGI_Handler2 */
const tCGI LEDS_CGI2={"/leds2.cgi", LEDS_CGI_Handler2};

/* Cgi call table, two CGI used */
tCGI CGI_TAB[2];

void LED_On(int led){
	printf("GPIO%d on\n", led);
	gpio_put(led, 1);
}

void LED_Off(int led){
	printf("GPIO%d off\n", led);
	gpio_put(led, 0);
}

const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i=0;

	/* First SSI handler iIndex = 0 (see CGI_TAB[] in cgi_init() )*/
	if (iIndex==0)
	{
		/* All leds off */
		LED_Off(LED1);
		LED_Off(LED2);
		LED_Off(LED3);
		LED_Off(LED4);

		/* Check cgi parameter : application GET /leds.cgi?led=2&led=4 */
		for (i=0; i<iNumParams; i++){
			/* check parameter "led" */
			if (strcmp(pcParam[i] , "led") == 0){
				/* switch led1 ON if 1 */
				if(strcmp(pcValue[i], "1") == 0)
					LED_On(LED1);

				/* switch led2 ON if 2 */
				else if(strcmp(pcValue[i], "2") == 0)
					LED_On(LED2);

				/* switch led3 ON if 3 */
				else if(strcmp(pcValue[i], "3") == 0)
					LED_On(LED3);

				/* switch led4 ON if 4 */
				else if(strcmp(pcValue[i], "4") == 0)
					LED_On(LED4);
			}
		}
	}
	/* uri to send after cgi call*/
	return "/cgi.html";
}

const char * LEDS_CGI_Handler2(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i=0;

	/* Second SSI handler iIndex = 1 (see CGI_TAB[] in cgi_init() )*/
	if (iIndex==1)
	{
		/* All leds off */
		LED_Off(LED1);
		LED_Off(LED2);
		LED_Off(LED3);
		LED_Off(LED4);

		/* Check cgi parameter : application GET /leds.cgi?led=2&led=4 */
		for (i=0; i<iNumParams; i++){
			/* check parameter "led" */
			if (strcmp(pcParam[i] , "led") == 0){
				/* switch led1 ON if 1 */
				if(strcmp(pcValue[i], "1") == 0)
					LED_On(LED1);

				/* switch led2 ON if 2 */
				else if(strcmp(pcValue[i], "2") == 0)
					LED_On(LED2);

				/* switch led3 ON if 3 */
				else if(strcmp(pcValue[i], "3") == 0)
					LED_On(LED3);

				/* switch led4 ON if 4 */
				else if(strcmp(pcValue[i], "4") == 0)
					LED_On(LED4);
			}
		}
	}
	/* uri to send after cgi call*/
	return "/ssi_cgi.shtml";
}

void cgi_init(void)
{
	/* configure CGI handler */
	CGI_TAB[0] = LEDS_CGI;
	CGI_TAB[1] = LEDS_CGI2;
	http_set_cgi_handlers(CGI_TAB, 2);

	for(int i = LED1; i <= LED4; i++){
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
		gpio_put(i, 0);
	}
}
