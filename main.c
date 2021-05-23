// Asadullah Chaudhry, East Esitmez, David Farma
// CP2: Semester opdracht
// Elektronica-ICT
// PBEAI-A

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define __DEBUG

#define BMPINPUTFILE "test.bmp"
#define MAXCHAR 1000

//Simplified declaration of used types
typedef uint32_t DWORD; 
typedef int32_t  LONG; 
typedef uint16_t WORD;


char secretMessage[] = "Secret message*";

#pragma pack(push, 1) //Disable padding
	typedef struct teInverteren //Struct to temporarily save 1 byte value
	{
		char tempByte;
	}RGBSAVE;
	#pragma pack(pop)


//Struct to save infoheader
#pragma pack(push, 1) //Disable padding
typedef struct INFOHeader
{
	DWORD infoGrootte;		//Struct size (byte)
	LONG infoBreedte;		//Bitmap width (pixel)
	LONG infoHoogte;		//Bitmap height (pixel)
	WORD kleurPlane;		//Amount of color planes (1)
	WORD bitPerPixel;		//Amount of bits per pixel
	DWORD compressieType;	//Compression type
	DWORD bestandGrootte;	//Image size (byte)
	LONG xResolutie;		//X axis pixels per meter
	LONG yResolutie;		//Y axis pixels per meter
	DWORD bmpKleuren;		//Amount colors used in bitmap
	DWORD bmpKleurenImp;	//Amount important colors
}INFOHEADER;
#pragma pack(pop)
	
//Struct to save fileheader
#pragma pack(push, 1) //Disable padding
typedef struct BMPHeader
{
	WORD bestandType; 		//File type
	DWORD bestandGrootte; 	//File size (byte)
	WORD reserved1; 		//Reserved (0)
	WORD reserved2;			//Reserved (0)
	DWORD headerOffset;		//Offset fileheader to actual bitmap (bit)
}HEADER;
#pragma pack(pop)


void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

int main(int argc, char *argv[ ])
{
	printf("arguments: %i, %s \r\n", argc, argv[1]);
	
	if (strncmp(argv[1], "--help", 6) == 0)
	{
		printf("USAGE: Je moet gcc main.c -o test\r\n");
	}
	else if (strncmp(argv[1], "-c", 2) == 0 )
	{
		if (argc == 8 && strncmp(argv[2], "-s", 2) == 0 && strncmp(argv[4], "-i", 2) == 0 && strncmp(argv[6], "-o", 2) == 0)
		{
				FILE *filePointerIn = NULL;
	
	filePointerIn = fopen(argv[5], "rb"); //Open input file as read-only (binary)
	if(filePointerIn == NULL)
	{
			printf("Kan bestand niet openen\n");
			exit(EXIT_FAILURE);
			return 0;
	}
	
	FILE *filePointerOut = NULL;
	
	filePointerOut = fopen(argv[7], "w"); //Open/create output file as writable (binary)
	if(filePointerOut == NULL)
	{
		printf("Kan bestand niet aanmaken\n");
		exit(EXIT_FAILURE);
		return 0;
	}
	
	HEADER headFile;
	INFOHEADER infoHead;
	
	//Next 4 lines copy header information
	fread(&headFile, sizeof(headFile), 1, filePointerIn);
	fwrite(&headFile, sizeof(headFile), 1, filePointerOut);
	fread(&infoHead, sizeof(infoHead), 1, filePointerIn);
	fwrite(&infoHead, sizeof(infoHead), 1, filePointerOut);
	
	RGBSAVE saveVariable;
	
	unsigned char testClause = 1; //Variable to check error at end of file to stop loop
	long counter = 0;
	char counterByte = 0;

	
	char letter = secretMessage[counter];
	while(testClause == 1)
	{

		
		char bitmask = 0;
		testClause = fread(&saveVariable.tempByte, 1, 1, filePointerIn); //Read value
		if(strlen(secretMessage) > counter)
		{
			
			bitmask = 0x01 & (letter >> 7 - counterByte);
			saveVariable.tempByte = saveVariable.tempByte | bitmask;
			//printf("%x \r\n", saveVariable.tempByte);
			bin(saveVariable.tempByte);
			printf("letter: %c \r\n", letter);
		}
		fwrite(&saveVariable.tempByte, 1, 1, filePointerOut); //Write value
		
		//saveVariable.tempByte = ~saveVariable.tempByte; //Invert value
		counterByte++;
		if (counterByte > 7)
		{
			counterByte = 0;
			if (counter < strlen(secretMessage))
			{
				counter++;
			}
			letter = secretMessage[counter];
		}
	}
	
	//Next 2 lines end transmission
	fclose(filePointerIn);
	fclose(filePointerOut);
    //free(inputPixels);
		}
		else { printf("Use --help argument for usage\r\n"); }
	}
	else if(strncmp(argv[1], "-d", 2) == 0)
	{
		if (argc == 6 && strncmp(argv[2], "-i", 2) == 0 && strncmp(argv[4], "-o", 2) == 0)
		{
			FILE *filePointerIn = NULL;
	
	filePointerIn = fopen(argv[3], "rb"); //Open input file as read-only (binary)
	if(filePointerIn == NULL)
	{
			printf("Kan bestand niet openen\n");
			exit(EXIT_FAILURE);
			return 0;
	}
	
	FILE *filePointerOut = NULL;
	
	filePointerOut = fopen(argv[5], "w"); //Open/create output file as writable (binary)
	if(filePointerOut == NULL)
	{
		printf("Kan bestand niet aanmaken\n");
		exit(EXIT_FAILURE);
		return 0;
	}
	
	HEADER headFile;
	INFOHEADER infoHead;
	
	//Next 4 lines copy header information
	fread(&headFile, sizeof(headFile), 1, filePointerIn);
	//fwrite(&headFile, sizeof(headFile), 1, filePointerOut);
	//fread(&infoHead, sizeof(infoHead), 1, filePointerIn);
	fwrite(&infoHead, sizeof(infoHead), 1, filePointerOut);
	
	RGBSAVE saveVariable;
	
	unsigned char testClause = 1; //Variable to check error at end of file to stop loop
	long counter = 0;
	int counterIndex = 0;
	char msgEnd = 0;
	char decodedText[1024] = ""; //Max message
	char bitmask = 0x01;
	char tempChar = 0;
	while(testClause == 1 && msgEnd == 0)
	{
		//hier doet het letter per letter in temchar schrijven tot dat er ster komt dan eindingt het bericht
		testClause = fread(&saveVariable.tempByte, 1, 1, filePointerIn); //Read value
		tempChar = tempChar << 1;
		tempChar = tempChar | (saveVariable.tempByte & bitmask);
		if (counter == 7)
		{
			counter = 0;
			if (counterIndex > 4) {
				decodedText[counterIndex - 5] = tempChar;
				printf("MSG: %c\r\n", tempChar);
			}
			counterIndex++;
			if (tempChar == '*')
			{
				msgEnd += 1;
			}
			tempChar = 0;
		}
		else
		{
		counter++;
		}
	}
	
	memset(filePointerOut, 0, 1024);//maakt de outfile leeg zodat er geen random char inzitten
	filePointerOut = fopen(argv[5], "w");//outfile opnieuw opnenen 
	printf("text: %s\r\n", decodedText);//printen van secret message in de console
	int results = fputs(decodedText, filePointerOut);//output van secret message in txt file
	//print to text file
	fclose(filePointerIn);// open files terug sluiten
	fclose(filePointerOut);
		}
		else { printf("Use --help argument for usage\r\n"); }
	}
	else { printf("Use --help argument for usage\r\n"); }
	
	return 0;
}

//compile fast daddy
