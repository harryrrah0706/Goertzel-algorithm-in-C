/*
 *  ======== util.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

UInt32 time1, time2;

char digit;

int freq1, freq2;
int mag1, mag2;

int sample, gtz_out[8];

short coef[8] = {0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE}; // Goertzel coefficients


void task0_dtmfGen(void)
{



// This task is used to read the symbol from the keyboard and set freq1 and freq2

	System_printf("\n>>>I am in Task 0.\n");
	System_flush();


	Task_sleep(500); // to allow the GTZ to be calculated.


}

void task1_dtmfDetect(void)
{
	int i, a1=0, a2=0, f1=0, f2=0, dig;
	static int num = 0;		// used to indicate the number of attempts, used in line 60.

	System_printf("\n--------------- Goertzel Algorithm (one freq) ---------------");
	System_printf("\n>>> I am in Task 1 for the first time, please wait...\n");
	System_flush();

	while (1) {

		num++;		//incrementing num by 1 each loop.

		Task_sleep(2000);  /* 0.25 second for 125us tick */

		// output the value of the calculated goertzel value with the inputted frequency.
		System_printf("\n>>> -%d- Goertzel value [%dHz]: %d.", num, freq1, gtz_out[0]);
		System_flush();

		// output error message if goertzel value is 0
		if (gtz_out[0]==0){
			System_printf("\n>>> Frequency [%dHz] does NOT match.",freq1);
			System_flush();
		}

		System_printf("\n>>> I am leaving Task 1, please wait for a minute or so to get the next Goertzel value...\n");
		System_flush();

		a1=0, a2=0, f1=0, f2=0;
		     }

}

