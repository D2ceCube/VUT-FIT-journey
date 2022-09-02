#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define FALSE (1==0)
#define TRUE (1==1)
#define MAX_LENGHT 102

/*********************************************
 *		Assatulla Dias               *
 *		    1BIT                     *
 *	  1. Projekt - Prace s textem        *
 * This is my first such serious programming.* 
 * I haven't done this before. 80% of my     *
 * work was troubleshooting :p               *
 *                                           *
 *                                           *
 *********************************************
 */



// Cons of the function atoi() is that if I input for example: 
// ./programm 1a 1a 
// It will still accept these values and the program will work.
// When, logically, it shouldn't
// So this function checks, if the input arguments are number without characters
int anotherAtoi(char *str) {
	int res = 0;

	for(int i = 0; str[i] != '\0'; ++i) {
		if(str[i] > '9' && str[i] < '0') {
			return -1;
		}
		res = res*10+str[i] - '0';
	}
	return res;
}


// When we start the programm. It needs important arguments LEVEL and PARAM
// So this function checks if it is true or not. Otherwise, it will notify where the error is
// 
_Bool isItNumber(char *arg1, char *arg2) {
	signed int levelCheck = anotherAtoi(arg1);
	signed int paramCheck = anotherAtoi(arg2);
	if ( (levelCheck >=  1 && levelCheck <= 4 ) && (paramCheck > 0 && paramCheck <= MAX_LENGHT)) 
	{
		return TRUE;
	}

	else if (levelCheck > 4 || levelCheck < 1) {
		fprintf(stderr,"\nLEVEL argument must be in interval 1-4!\n");
		return FALSE;
	}
	else {
		fprintf(stderr,"\nPARAM argument must be without -\n");
		return FALSE;
	}

}

//Common function that gets lenght of string
int giveStringLen(char *strn) {
	int i = 0;
	while(strn[i] != '\0') {
		i++;
	}
	return i;
}

// Function security1 checks, 
// if the password has 1 Big alpha-character AND 1 low alpha-character
_Bool security1 (char *hesla, int lenght) {
	_Bool bigPismenoFlag = FALSE; // If the password has 1 big character, then the flag turns into TRUE
	_Bool lowPismenoFlag = FALSE; // same thing but with low character
	for(int i = 0; i < lenght;i++) {
		if((hesla[i] >= 'A') && (hesla[i] <= 'Z')) {
			bigPismenoFlag = TRUE;
		}
		
		else if ((hesla[i] >= 'a') && (hesla[i] <= 'z'))
		{
			lowPismenoFlag = TRUE;
		}
	}
		
	if ((bigPismenoFlag && lowPismenoFlag) == TRUE) {
		return TRUE;
	}
	return FALSE;
}

// Function security2 checks, if the password has contains characters from at least X groups. 
_Bool security2 (char *hesla, int indexParam, int lenght) {
	_Bool malePismenoFlag = FALSE; // low-alpha-character Flag PARAM 1
	_Bool velkePismenoFlag = FALSE; // big-alpha-character Flag PARAM 2
	_Bool cisloFlag = FALSE; // if it contains number PARAM 3 
	_Bool specialFlag = FALSE; // special characters such as @ # ! etc. PARAM 4

	// A loop that checks each character for the aforementioned groups
	for(int i = 0; i < lenght;i++) {
			if(indexParam ==  1){ 
				if(hesla[i] >= 'a' && hesla[i] <= 'z')
					malePismenoFlag = TRUE;
			}
			if(indexParam == 2){
				// The 1 and 2 groups of LEVEL 2 is similar to 1 LEVEL.
				// I already have this function, so I decided that is better to
				// again use it than writing 10-15 similar line code again. 
				if(security1(hesla,lenght)) {
                                        malePismenoFlag = TRUE;
                                        velkePismenoFlag = TRUE;
                                }
			}
			if(indexParam ==  3){ 
				if(security1(hesla,lenght)) {
					malePismenoFlag = TRUE;
					velkePismenoFlag = TRUE;
				}
				if(hesla[i] >= '0' && hesla[i] <= '9')
					cisloFlag = TRUE;
			}
			if(indexParam >=  4){ 
				if((hesla[i] >= 32 && hesla[i] <= 47) ||
				  (hesla[i] >= 58 && hesla[i] <= 64) ||
				  (hesla[i] >= 91 && hesla[i] <= 96) ||
				  (hesla[i] >= 123 && hesla[i] <= 126)	)
					specialFlag = TRUE;
				if(security1(hesla,lenght)) {
					malePismenoFlag = TRUE;
					velkePismenoFlag = TRUE;
				}
				if(hesla[i] >= '0' && hesla[i] <= '9' )
					cisloFlag = TRUE;
			}
			else
				continue;
		
	}
	// This switch() returns TRUE or FALSE depending on if the password passed 
	// all verification steps successfully
	if(indexParam == 1) {
		if(malePismenoFlag == TRUE)
		{
			return TRUE;
		}
		else 	return FALSE;
	}	
	if(indexParam == 2) {
		if((velkePismenoFlag && malePismenoFlag) == TRUE) {
			return TRUE;
		}
		else    return FALSE;
	}	
	if(indexParam == 3) {
		if((cisloFlag && velkePismenoFlag && malePismenoFlag) == TRUE) {
			return TRUE;
		}
		else	return FALSE;
	}	
	if(indexParam >= 4) {
		if((malePismenoFlag && specialFlag && cisloFlag && velkePismenoFlag) == TRUE) {
			return TRUE;
		}
		else	return FALSE;	
 	}
	else 
		return FALSE;
}

_Bool security3 (char *hesla, int indexParam, int lenght) {
	int equalCounter = 1;
	// Function checks, if the current symbol [i] is similar to the next symbol [i+1]
	// if TRUE -> then equalCounter++. Its integer because the lenght that can be allowed 
	// depends on indexParam
	//
	for(int i = 0; i < lenght; i++) {
		if(hesla[i] == hesla[i+1])
			equalCounter++;
	}
	if(equalCounter < indexParam)
		return TRUE;

	return FALSE;

}
 
_Bool security4 (char *hesla, int indexParam, int lenght) {
	int equalCounter = 1;   // that var is similar to the var from the fun. security3
	int allIsEqual = 1;     // that var i need to check if the all symbols of subtring are similar

	for(int i = 0; i < lenght; i++) {
		for(int j = lenght - 1; j > i; j--) {
			if(hesla[i] == hesla[j]) {
				for(int subString = 1; subString < indexParam; subString++ ) {
					
					if(hesla[i+subString] == hesla[j+subString])
					{
						allIsEqual++;
					}
					// If only ONE symbol is diffrent, then the counter(allIsEqual) 
					// 				   get his inital value
					else {
						allIsEqual = 1;
						break;
					}
				}
				
			}
			if(allIsEqual >= indexParam) break;
			else continue;
		}
		// if i find at least 2 same substring in 
		if(allIsEqual >= indexParam) {
			equalCounter++;
			break;
		}	
	}
	if(equalCounter < 2)
		return TRUE;
	else
		return FALSE;

}

void checkSecurity(char *arg1,char *arg2, _Bool statFlag) {
	
	// This char array needs to temporary save the password
	char hesla[MAX_LENGHT];
	// Since I'm sure that here I will definitely have a number WITHOUT characters, 
	// I decided to use the atoi () function
	int securityLevel = atoi(arg1);
	int indexParam = atoi(arg2);

	// Those vars are for Statistics
	// I know that if there is no "--stats" argument, 
	// then these variables are simply wasting memory 
	// and the optimization will drop, 
	// but I do not know how to solve this yet.
	
	
	
	int minPassLenght = MAX_LENGHT; // Since that max symbols are 100 (+1 '\0'), 
					// then for a start I make it to the maximum
	double summary = 0; 		// Summary of password symbols
	int hesloCounter = 0; 		// That var i need in the end of this function
   	int ascii_lenght = 95; 
	int asciiTable[ascii_lenght];   // this array need to count various character
	int diff = 32; 			// This variable is the starting value, which, starting with "SPACE", 
					// fills the ASCII table in this cycle
	//Filling the array so that in the future 
	//it could distinguish on the uniqueness of the symbol
	for(int i = 0; i < ascii_lenght; i++) {
       		asciiTable[i] = diff++;
	}
	int variousCharCounter = 0;
	
	

	while(fgets(hesla,MAX_LENGHT, stdin) != NULL) {
		int len = giveStringLen(hesla);
	
		summary += len-1; // Len -1 because of '\0'
		hesloCounter++; 
		
		if(minPassLenght > len) {
			minPassLenght = len;
		}
		// Here I am checking for statistics how many different characters were used. 
		// And so that there are no duplicates, after the 1st match, I equate this character to 0
		for(int i = 0; i < len; i++) {
			for(int j = 0; j < ascii_lenght; j++) {
				if(hesla[i] == asciiTable[j]) {
					variousCharCounter++;
					asciiTable[j] = 0;
				}
			} 
		}
			
		
		if (len == 101) {
			continue;
			
		}
		else {
			switch(securityLevel) {
				case 1:	
					if(security1(hesla, len)) {
						printf("%s",hesla); 
					}
					else continue; 
					break;  
				case 2:
					if(security1(hesla, len)) {
						if(security2(hesla, indexParam, len)) {
							printf("%s",hesla);  
						}
						else continue; 
					}
					else continue;
					break; 
				case 3:
					if(security1(hesla,len)){
						if(security2(hesla,indexParam,len)) {
							if(security3(hesla,indexParam,len)){
								printf("%s",hesla);
							}
							else continue;
						}
						else continue;
					}
					else continue;
					break;
				case 4:
					if(security1(hesla,len)){
						if(security2(hesla,indexParam,len)) {
							if(security3(hesla,indexParam,len)){
								if(security4(hesla,indexParam,len)){
									printf("%s",hesla);
								}
								else continue;
							}
							else continue;
						}
						else continue;
					}
					else continue;
					break;
				default:
					break;
			}
		}
	}

	
	if (statFlag == TRUE) {
		double average = summary / hesloCounter;
		printf("Statistika:\n");
		printf("Ruznych znaku: %d\n", variousCharCounter);
		printf("Minimalni delka: %d\n", minPassLenght-1);
		printf("Prumerna delka: %0.1f\n", average);
	}
}
// The third non-important argument can be anything
// So this functions checks, if the third argument is "--stats"
_Bool doesItHaveStat(char *argv) {
	int len = giveStringLen(argv);
	int equalFlag = 0;
	char stat[] = "--stats";
	// Here the loop checks each character for a match.
	for(int i = 0; i < len; i++) {
		if(argv[i] == stat[i]) {
			equalFlag++;
		}
	}

	if (equalFlag == len) {
		return TRUE;
	}
	else 
	{
		fprintf(stderr, "\nERROR: Don't know argument '%s'\n",argv);	
		return FALSE;
	}
}

int main(int argc, char *argv[]) {
	// This variable is needed in the future. 
	// Depending on whether the --stats argument was specified, 
	// this flag will change its meaning.
	_Bool statFlag = FALSE;

	// Firstly I wanted to use switch(x), but 
	// I wanted to split when the arguments are too much or less.
	// So I decided to use If else expressions
	if( argc == 3 ) {	
		if (isItNumber(argv[1],argv[2])) {	       
			checkSecurity(argv[1],argv[2],statFlag);			
		}
		else return 1;
		
	}
	else if (argc == 4) {
		if(isItNumber(argv[1],argv[2]) && doesItHaveStat(argv[3])) {
			statFlag = TRUE;
			checkSecurity(argv[1],argv[2],statFlag);
		}
		else return 1;
	}
	else if ( argc > 4) {
		fprintf(stderr,"\n\nToo many arguments.\n");
		return 1;
	}
	else {
		fprintf(stderr,"\n\nTwo arguments expected.\n");
		return 1;
	}
	return 0;
}
