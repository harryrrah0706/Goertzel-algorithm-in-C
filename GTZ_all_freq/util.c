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

#define threshold 1000

UInt32 time1, time2;

char digit;

int freq1, freq2;
int mag1, mag2;

int sample, gtz_out[8];

short coef[8] = {0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE}; // goertzel coefficients



void task1_dtmfDetect(void)
{


	int i, a1=0, a2=0, f1=0, f2=0, dig, len;
	char user_input[1];
	char input_char;
	int index=0;
	int lower_sum,upper_sum=0;
	int upper_max_reg,upper_max_index,lower_max_reg,lower_max_index=0;
	char detected_char;

	int freq[8]={697,770,852,941,1209,1336,1477,1633};

	// 2d array used to store all digits, characters and symbols, also used to determined the user input at the end using indexes.
	char valid_char[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};


	while (1) {



		/*	 1. write a code that keep reading the value of "digit" from the keyboard and set the magnitude for mag1 = mag2= 32768.0 and the frequencies freq1 and freq2 according the digit value as:
			 in case when the wrong digit is entered, set: the mag1 = 00000.0, mag2 = 00000.0, freq1 = 000 and freq2 = 0000




						 '0' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1336
						 '1' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1209,
						 '2' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1336,
						 '3' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1477,
						 '4' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1209,
						 '5' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1336,
						 '6' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1477,
						 '7' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1209,
						 '8' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1336,
						 '9' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1477,
						 '*' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1209,
						 '#' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1477,
						 'A' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 697, freq2 = 1633,
						 'B' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 770, freq2 = 1633,
						 'C' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 852, freq2 = 1633,
						 'D' : mag1 = 32768.0, mag2 = 32768.0, freq1 = 941, freq2 = 1633,
						otherwise : mag1 = 00000.0, mag2 = 00000.0, freq1 = 000, freq2 = 0000  */

		// a infinite loop that keeps on repeating if multiple input is detected.
		// only breaks when the input is a single character.
		do{
			System_printf("------------------- Goertzel Algorithm (all freq) -------------------");
			System_printf("\n>>> List of valid characters -> [0,1,2,3,4,5,6,7,8,9,A,B,C,D,*,#]");
			System_printf("\n>>> Please type in a character and press Enter: ");
			System_flush();
			scanf("%s", user_input);

			len=strlen(user_input);

			if (len==1){		// checks whether the number of input is 1.
				System_printf("\n>>> Analyzing signal...");
				System_printf("\n>>> Calculating Goertzel value for all frequencies...");
				System_flush();
				break;
			}
			else{		// if multiple input is detected then asks the user to input again.
				System_printf("\n*** Error: Multiple character input detected, please enter only ONE character at a time.");
				System_printf("\n>>> Hint: In order to detect a series of character, input them with spaces separated in between.");
				System_printf("\n    e.g. -> 1 0 2 4 A 9 C B.\n\n\n");
				System_flush();
			}
		}while(1);

		input_char=user_input[0];

		mag1 = 32768.0;
		mag2 = 32768.0;

		// use of switch to assign value to freq1 and freq2 based on user input.
		switch(input_char){
		case 'A':
			freq1 = 697;
			freq2 = 1633;
			break;
		case 'B':
			freq1 = 770;
			freq2 = 1633;
			break;
		case 'C':
			freq1 = 852;
			freq2 = 1633;
			break;
		case 'D':
			freq1 = 941;
			freq2 = 1633;
			break;
		case '0':
			freq1 = 941;
			freq2 = 1336;
			break;
		case '1':
			freq1 = 697;
			freq2 = 1209;
			break;
		case '2':
			freq1 = 697;
			freq2 = 1336;
			break;
		case '3':
			freq1 = 697;
			freq2 = 1477;
			break;
		case '4':
			freq1 = 770;
			freq2 = 1209;
			break;
		case '5':
			freq1 = 770;
			freq2 = 1336;
			break;
		case '6':
			freq1 = 770;
			freq2 = 1477;
			break;
		case '7':
			freq1 = 852;
			freq2 = 1209;
			break;
		case '8':
			freq1 = 852;
			freq2 = 1336;
			break;
		case '9':
			freq1 = 852;
			freq2 = 1477;
			break;
		case '#':
			freq1 = 941;
			freq2 = 1477;
			break;
		case '*':
			freq1 = 941;
			freq2 = 1209;
			break;

		// if the user inputs something that is not 0 to 9, A to D or * or #, then mag1, mag2, freq1 and freq2 will all be assigned 0.
		default:
			mag1 = 00000.0;
			mag2 = 00000.0;
			freq1 = 000;
			freq2 = 0000;
		}


		Task_sleep(500);

		// 2. Read the GTZ for each frequency and take a decision on which key has been pressed and print it.
	    a1=0, a2=0, f1=0, f2=0;


	    /* checks the goertzel value of all frequencies.
		System_printf("\n\nGoertzel value of 697Hz is: %d", gtz_out[0]);
		System_printf("\nGoertzel value of 770Hz is: %d", gtz_out[1]);
		System_printf("\nGoertzel value of 852Hz is: %d", gtz_out[2]);
		System_printf("\nGoertzel value of 941Hz is: %d", gtz_out[3]);
		System_printf("\nGoertzel value of 1209Hz is: %d", gtz_out[4]);
		System_printf("\nGoertzel value of 1336Hz is: %d", gtz_out[5]);
		System_printf("\nGoertzel value of 1477Hz is: %d", gtz_out[6]);
		System_printf("\nGoertzel value of 1633Hz is: %d\n", gtz_out[7]);
		System_flush();
		*/


	    // checks whether the goertzel value of lower and upper band are zero or not
		lower_sum = gtz_out[0]+gtz_out[1]+gtz_out[2]+gtz_out[3];
		upper_sum = gtz_out[4]+gtz_out[5]+gtz_out[6]+gtz_out[7];

	    // if all value in lower or upper band is zero, then the user has typed in something outside the valid list.
		// then error message and hint pops up.
		if (lower_sum==0 || upper_sum==0){
		    a1=0, a2=0, f1=0, f2=0;
		    System_printf("\n\n*** Error: Invalid character detected, please try again.");
		    System_printf("\n>>> Hint: This may be because you:");
		    System_printf("\n   - entered a character that is not included within the list.");
		    System_printf("\n   - entered a lower case 'a', 'b', 'c' or 'd', they should be in upper case form.\n\n\n");
		    System_flush();
		}
		// otherwise uses two for loops to determine the largest goertzel value in the lower and upper band.
		else{
			System_printf("\n>>> Calculating largest Goertzel value in lower band...");
			System_flush();
		    for (index=0;index<4;index++){
		    	if (gtz_out[index] > lower_max_reg){
		    		lower_max_index=index;
		    		lower_max_reg=gtz_out[index];
		    	}
		    }

			System_printf("\n>>> Calculating largest Goertzel value in upper band...");
			System_flush();
		    for (index=4;index<8;index++){
		    	if (gtz_out[index] > upper_max_reg){
		    		upper_max_index=index;
		    		upper_max_reg=gtz_out[index];
		    	}
		    }
		    //System_printf("\nLower band index: %d",lower_max_index);
		    //System_printf("\nUpper band index: %d",upper_max_index);


		    // Assigning all the values to its corresponding variables.
			System_printf("\n>>> Generating result...");
			System_flush();
		    a1=freq[lower_max_index];
		    a2=freq[upper_max_index];
		    mag1=gtz_out[lower_max_index];
		    mag2=gtz_out[upper_max_index];
		    detected_char=valid_char[lower_max_index][upper_max_index-4]; // the upper_max_index needs to be subtracted by 4 because of the limit of 2-d array.

		    // printing the result of the algorithm.
		    System_printf("\n>>> Process complete.");
		    System_printf("\n-------------------- Result --------------------");
		    System_printf("\n>>> Detected character is -> %c", detected_char);
		    System_printf("\n>>> Information:");
		    System_printf("\n   - Lower band frequency -> %dHz", a1);
		    System_printf("\n   - Upper band frequency -> %dHz", a2);
		    System_printf("\n   - Lower band Goertzel value -> %d", mag1);
		    System_printf("\n   - Upper band Goertzel value -> %d\n\n\n", mag2);
		    System_flush();
		}

		// resets the indexes and registers.
	    lower_max_index = lower_max_reg = upper_max_index = upper_max_reg = 0;


	}
}

