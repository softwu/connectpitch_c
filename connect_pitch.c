///////////////////////////////////////////////////////////////////////////////
// connect_pitch.c
// by Sheng Kris Chen
///////////////////////////////////////////////////////////////////////////////
// This program is to compute a straight line, an exponential curve, 
// or a logarithmic curve of arbitrary steps and range of pitches in frequency 
// with a user-defined bpm, pitch duration, and amplitude.
///////////////////////////////////////////////////////////////////////////////
// to compile and run: gcc -o connect_pitch connect_pitch.c && ./connect_pitch
///////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define title "===================================================\nConnecting Two Pitches with A Desired Line or Curve\n===================================================\n"

void print_personal()
{
	printf("\n===================================================\n");
	printf("\tAUTHOR: \tSheng Kris Chen\n");
	printf("\tCONTACT:\teskeisee@gmail.com\n");
	printf("===================================================\n");
}

// 3 prototype of functions of different lines to choose: straight, exponential, and logarithmic
void straight(float, float, int, float, float, int);
void expon(float, float, int, float, float, int);
void logar(float, float, int, float, float, int);
            
int main()
{
	// Define "length" for the number of steps between the two pitches,
	// "amp" for the amplitude from 0-1, 
	// "bpm" for beats per minute,
	// "start" for the starting pitch,
	// "end" for the final pitch,
	// "dur" for duration of each pitch.
	// Initialize "key" with 1 in order to enter the while loop
	
    int   length, bpm, key = 1; 
    float start, end, dur, amp;
	
	printf("\n\n\n%s", title);
	print_personal();
	
	// The first while loop is to prompt the user to make a decision to either run it again or exit
	while(key != '0')
	{

    printf("\nEnter a number (float) for the frequency (Hz) of the starting pitch (Human hearing range: 20-18000): ");
    scanf("%f",&start);

    printf("\nEnter a number (float) for the frequency (Hz) of the final pitch: ");
    scanf("%f",&end);
    
    printf("\nEnter a number (integer) for steps between the two pitches: ");
    scanf("%d",&length);
    
    printf("\nEnter a number (float) for duration (seconds) of each step: ");
    scanf("%f",&dur);
    
    printf("\nEnter a number (float between 0 - 1) for the amplitude: ");
    scanf("%f",&amp);
    
    printf("\nEnter a number (integer) for beats per minute (bpm): ");
    scanf("%d",&bpm);
    
   	
   		// The second while loop here is used to prompt the user 
   		// to choose other curves without changing the previous values.
		while(key != '0')
		{
    	// Choose a desired line for computing
    	printf("\nEnter a number (integer) to choose a curve between the two pitches:");
    	printf("\n1. A Straight Line\n2. An Exponential Curve\n3. A Logarithmic Curve\n");
    	scanf("%d",&key);

    	// Use switch to call the function outside the main
    	switch(key){
    	case 1:
    	straight(start, end, length, dur, amp, bpm);
    	break;
    
    	case 2:
    	expon(start, end, length, dur, amp, bpm);
    	break;
    
    	case 3:
    	logar(start, end, length, dur, amp, bpm);
    	break;
    	}
    	printf("Press ENTER key to CHOOSE another curve or 0 to EXIT\n");
    	getchar();
    	key = getchar();
    	}

	printf("Press ENTER key to RUN the program again or 0 to EXIT\n");
	getchar();
	key = getchar();
	
	}
	return 0;
}

// Straight line function
void straight(float start, float end, int length, float dur, float amp, int bpm)
{
	int x, key=1;
	float y, a;
	char score[5000]; score[0] = '\0';
	char temp[5000]; temp[0] = '\0';
	char csdFileName[256];
	char string[256];
	
	// Clear the console before beginning
	system("clear"); 
    printf("\n\n==============================================\n\n");
    
    printf("\t\nHere is a STRAIGHT LINE of pitches with %d steps going from %fHz to %fHz:\n", length, start, end);

    printf("\t# \tFrequency \n");
    
    // The for loop is to print out the list of pitches as well as the Csound note list.
 	for (x = 1; x <= length; x++)
    {
          a = (float) (x-1) / (length-1);
          y = start + a * (end-start);
          printf ("\t%d \t%f \n", x, y);
          sprintf(temp, "i1 %d %f %f \n", x, dur, y);
          strcat(score, temp);
    }
  	printf("\n\n==============================================\n\n");


	// Choose a name to make the Csound file
  	printf("\nEnter a name for your new csd file (excluding \".csd\"): ");
	scanf("%s", csdFileName);	
  	strcat(csdFileName, ".csd");
  
  
  	FILE *csdFilePointer = fopen(csdFileName, "w");


	if(csdFilePointer)

	{
	

  		// Print out the Csound file
  		fprintf(csdFilePointer, "<CsoundSynthesizer>\n");
  		fprintf(csdFilePointer, "<CsInstruments>\n");
  		fprintf(csdFilePointer, "sr = 44100\n");
  		fprintf(csdFilePointer, "ksmps = 4410\n");
  		fprintf(csdFilePointer, "nchnls = 1\n");
  		fprintf(csdFilePointer, "0dbfs = 1\n");
  		fprintf(csdFilePointer, "instr 1\n");
  		fprintf(csdFilePointer, "kenv linen %f, .1, p3, .1\n", amp); // The value stored in "amp" is being passed onto the string
 	 	fprintf(csdFilePointer, "aout oscil kenv, p4, 1\n");
  		fprintf(csdFilePointer, "out aout\n");
  		fprintf(csdFilePointer, "endin\n");
  		fprintf(csdFilePointer, "</CsInstruments>\n");
  		fprintf(csdFilePointer, "<CsScore>\n");
  		fprintf(csdFilePointer, "f 1 0 16384 10 1\n");
  		fprintf(csdFilePointer, "t 0 %d\n", bpm); // The value stored in "bpm" is being passed onto the string
  		fprintf(csdFilePointer, score); // String "score" is being passed
  		fprintf(csdFilePointer, "</CsScore>\n");
  		fprintf(csdFilePointer, "</CsoundSynthesizer>\n");


	}
	else 

	printf("\nAn error occured while trying to create or open the file %s\n", csdFileName);
		
	fclose(csdFilePointer);
	
	// Print instructions to the user for rendering the Csound file
	printf("\nA new file \"%s\" has been created in this directory and is ready to run!\n", csdFileName);


	sprintf(string, "csound -odac -d -O null %s\n", csdFileName);
  	printf("\nPress ENTER key to run \"%s\"\n", csdFileName);
	getchar();
	getchar();
	
		// This while loop prompts the user to render the file again
		while(key != '0')
		{
		system(string); 
	
		printf("\n==============================================\n");
		printf("\nThat was a STRAIGHT LINE from %fHz to %fHz", start, end);
		printf("\n%d steps at a rate of %f second(s) per note", length, dur);
		printf("\nBPM = %d and AMP = %f", bpm, amp);
		printf("\n==============================================\n\n");
		printf("Press ENTER key to run the Csound file again or 0 to EXIT\n");
		key = getchar();
		}
}

// Exponential curve function
void expon(float start, float end, int length, float dur, float amp, int bpm)
{
    int   x, key=1;
    float y, a;
	char score[5000]; score[0] = '\0';
	char temp[5000]; temp[0] = '\0';
	char csdFileName[256];
	char string[256];
	
	system("clear"); /* clear the console before beginning */
    printf("\n\n==============================================\n\n");

    printf("\t\nHere is an EXPONENTIAL CURVE of pitches with %d steps going from %f to %f:\n", length, start, end);
    printf("\t# \tFrequency \n");
    
 for (x = 1; x <= length; x++)
    {
          a = (float)(x-1) * (x-1) / ((length-1) * (length-1));
          y = start + a * (end-start);
          printf ("\t%d \t%f \n", x, y);
          sprintf(temp, "i1 %d %f %f \n", x, dur, y);
          strcat(score, temp);          
    }
  printf("\n\n==============================================\n\n");


	// Choose a name to make the Csound file
  	printf("\nEnter a name for your new csd file (excluding \".csd\"): ");

  	scanf("%s", csdFileName);
  	strcat(csdFileName, ".csd");
  
  
  	FILE *csdFilePointer = fopen(csdFileName, "w");


	if(csdFilePointer)

	{
	

  		// Print out the Csound file
  		fprintf(csdFilePointer, "<CsoundSynthesizer>\n");
  		fprintf(csdFilePointer, "<CsInstruments>\n");
  		fprintf(csdFilePointer, "sr = 44100\n");
  		fprintf(csdFilePointer, "ksmps = 128\n");
  		fprintf(csdFilePointer, "nchnls = 1\n");
  		fprintf(csdFilePointer, "0dbfs = 1\n");
  		fprintf(csdFilePointer, "instr 1\n");
  		fprintf(csdFilePointer, "kenv linen %f, .1, p3, .1\n", amp);
 	 	fprintf(csdFilePointer, "aout oscil kenv, p4, 1\n");
  		fprintf(csdFilePointer, "out aout\n");
  		fprintf(csdFilePointer, "endin\n");
  		fprintf(csdFilePointer, "</CsInstruments>\n");
  		fprintf(csdFilePointer, "<CsScore>\n");
  		fprintf(csdFilePointer, "f 1 0 16384 10 1\n");
  		fprintf(csdFilePointer, "t 0 %d\n", bpm);
  		fprintf(csdFilePointer, score);
  		fprintf(csdFilePointer, "</CsScore>\n");
  		fprintf(csdFilePointer, "</CsoundSynthesizer>\n");


	}
	else 

	printf("\nAn error occured while trying to create or open the file %s\n", csdFileName);
		
	fclose(csdFilePointer);
	
	// Print instructions to the user for rendering the Csound file
	printf("\nA new file \"%s\" has been created in this directory and is ready to run!\n", csdFileName);


	sprintf(string, "csound -odac -d -O null %s\n", csdFileName);
  	printf("\nPress ENTER key to run \"%s\"\n", csdFileName);
	getchar();
	getchar();
	
		// This while loop prompts the user to render the file again
		while(key != '0')
		{
		system(string); 
	
		printf("\n==============================================\n");
		printf("\nThat was a EXPONENTIAL CURVE from %fHz to %fHz", start, end);
		printf("\n%d steps at a rate of %f second(s) per note", length, dur);
		printf("\nBPM = %d and AMP = %f", bpm, amp);
		printf("\n==============================================\n\n");
		printf("Press ENTER key to run the Csound file again or 0 to EXIT\n");
		key = getchar();
		}
}

// Logarithmic curve function 
void logar(float start, float end, int length, float dur, float amp, int bpm)
{
    int   x, b, key=1;
    float y, a;
	char score[5000]; score[0] = '\0';
	char temp[5000]; temp[0] = '\0';
	char csdFileName[256];
	char string[256];
	
	system("clear"); /* clear the console before beginning */
    printf("\n\n==============================================\n\n");

    printf("\t\nHere is a LOGARITHMIC CURVE of pitches with %d steps going from %f to %f:\n", length, start, end);
    printf("\t# \tFrequency \n");
    
 for (x = 1; x <= length; x++)
    {
          a = x;
          b = length;        
          y = start + log(a)/log(b) * (end-start);
          printf ("\t%d \t%f \n", x, y);
          sprintf(temp, "i1 %d %f %f \n", x, dur, y);
          strcat(score, temp);    
    }
  printf("\n\n==============================================\n\n");


	// Choose a name to make the Csound file
  	printf("\nEnter a name for your new csd file (excluding \".csd\"): ");

  	scanf("%s", csdFileName);
  	strcat(csdFileName, ".csd");
  
  
  	FILE *csdFilePointer = fopen(csdFileName, "w");


	if(csdFilePointer)

	{
	

  		// Print out the Csound file
  		fprintf(csdFilePointer, "<CsoundSynthesizer>\n");
  		fprintf(csdFilePointer, "<CsInstruments>\n");
  		fprintf(csdFilePointer, "sr = 44100\n");
  		fprintf(csdFilePointer, "ksmps = 128\n");
  		fprintf(csdFilePointer, "nchnls = 1\n");
  		fprintf(csdFilePointer, "0dbfs = 1\n");
  		fprintf(csdFilePointer, "instr 1\n");
  		fprintf(csdFilePointer, "kenv linen %f, .1, p3, .1\n", amp);
 	 	fprintf(csdFilePointer, "aout oscil kenv, p4, 1\n");
  		fprintf(csdFilePointer, "out aout\n");
  		fprintf(csdFilePointer, "endin\n");
  		fprintf(csdFilePointer, "</CsInstruments>\n");
  		fprintf(csdFilePointer, "<CsScore>\n");
  		fprintf(csdFilePointer, "f 1 0 16384 10 1\n");
  		fprintf(csdFilePointer, "t 0 %d\n", bpm);
  		fprintf(csdFilePointer, score);
  		fprintf(csdFilePointer, "</CsScore>\n");
  		fprintf(csdFilePointer, "</CsoundSynthesizer>\n");


	}
	else 

	printf("\nAn error occured while trying to create or open the file %s\n", csdFileName);
		
	fclose(csdFilePointer);
	
	// Print instructions to the user for rendering the Csound file
	printf("\nA new file \"%s\" has been created in this directory and is ready to run!\n", csdFileName);


	sprintf(string, "csound -odac -d -O null %s\n", csdFileName);
  	printf("\nPress ENTER key to run \"%s\"\n", csdFileName);
	getchar();
	getchar();
	
		// This while loop prompts the user to render the file again
		while(key != '0')
		{
		system(string); 
	
		printf("\n==============================================\n");
		printf("\nThat was a LOGARITHMIC CURVE from %fHz to %fHz", start, end);
		printf("\n%d steps at a rate of %f second(s) per note", length, dur);
		printf("\nBPM = %d and AMP = %f", bpm, amp);
		printf("\n==============================================\n\n");
		printf("Press ENTER key to run the Csound file again or 0 to EXIT\n");
		key = getchar();
		}
}