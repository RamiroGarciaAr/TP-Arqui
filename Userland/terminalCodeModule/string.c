#include <string.h>
#include <stdint.h>

#define DELTA 'a' - 'A'
#define NULL ((void * )0)
#define TRUE 1
#define FALSE 0
#define AUX_BUFF_SIZE 50
#define TO_NUM 48
#define TO_NUM_UPPER 55
#define TO_NUM_LOWER 87
#define BASE_HEXA 16

/*
stringCompare: Returns -1 if str1 is smaller than str2, 1 otherwise, and 0 if they are equal.
*/
int stringCompare(const char *str1, const char *str2){
    unsigned char c1, c2;
	while (1) {
		c1 = *str1++;
		c2 = *str2++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}

/*
stringnCompare: Compares the first n chars from str1 and str2 and returns -1 if str1 is smaller than
                str2, 1 otherwise, and 0 if they are equal
*/
int stringnCompare(const char *str1, const char *str2, uint64_t count){
	unsigned char c1, c2;
	while (count) {
		c1 = *str1++;
		c2 = *str2++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

/*
toUpper: Transforms all characters of str from lower case to upper case
*/
void toUpper(char *str){
    for(int i=0; str[i] != '\0' ;i++)
        if(str[i] >= 'a' && str[i] <= 'z')
            str[i] -= DELTA;
}

/*
toLower: Transforms all characters of str from upper case to lower case
*/
void toLower(char *str){
    for(int i=0; str[i] != '\0' ;i++)
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] += DELTA;
}

/*
stringCopy: Copies content from src to dest and returns its location
*/
char *stringCopy(char *dest, int destSize,const char *source){
    char *temp = dest;
    while((*temp++ = *source++) != '\0' && destSize-- > 0);
    *temp = '\0';       // If destSize reached 0
    return dest;
}

/*
stringnCopy: Copies n chars from src to dest and returns its location
*/
char *stringnCopy(char *dest, int destSize, const char *src, int count){
    char *temp = dest;
    while(count && destSize){
        if((*temp = *src) != '\0')
            src++;
        temp++;
        count--;
        destSize--;
    }
    *temp = '\0';       // If destSize reached 0
    return dest;
}

/*
stringLength: Returns the length of the str (NULL included)
*/
uint64_t stringLength(const char *str){
    uint64_t length;
    for(length=0; str[length]!='\0' ;length++);
    return length;
}

/*
firstOcurrence: Returns the location of the first ocurrence of a character, or NULL if it's not found
*/
char *firstOcurrence(char *str, char c){
    char *tmp = str;
    for(int i=0; tmp[i] != '\0' ;i++)
        if(tmp[i] == c)
            return tmp;
    return NULL;
}

/*
stringToNum: Returns the number value of a string, or 0 if it's not a number
*/
long stringToNum(const char * str){
    double num = 0;
    double neg = 1;
    if(str[0] == '-'){
        neg*=-1.0;
        str++;
    }
    for(int i = 0; str[i] != 0 && str[i] != '\n'; i++) {
        num *= 10;
        if(!isCharNum(str[i]))
            return 0;
        num += charToNum(str[i]); 
    }
    return num*neg;
}

/*
charToNum: Returns the number value of a character, or 0 if it's not a number
*/
int charToNum(char c){
    if(isCharNum(c))
        return c - '0';
    return FALSE;
}

/*
isCharNum: Returns if a character is a number's ASCII value
*/
int isCharNum(char c){
    return c >= '0' && c <= '9';
}


/*
numToString: Transforms a number to a string. Returns the number of digits transformed.
*/
int numToString(int num, char *buffer, int bufferSize){
    int isNeg = 1;
    if(num < 0){
        isNeg = -1;
        buffer[0] = '-';
        buffer++;
    }
    num*=isNeg;

    return uintToBase(num, buffer, 10);
}

/*
decToHex: Transforms an integer number into its hexadecimal representation
*/
int decToHex(int num, char *buffer, int bufferSize){
    int isNeg = 1;
    if(num < 0){
        isNeg = -1;
        buffer[0] = '-';
        buffer++;
    }
    num*=isNeg;

    return uintToBase(num, buffer, 16);
}


/*
stringReverse: Reverses the given string.
*/
char *stringReverse(char *str){
    int i;
    int len = 0;
    char c;
    if (!str)
        return NULL;
    while(str[len++] != '\0')
    for(i = 0; i < (len/2); i++){
        c = str[i];
        str [i] = str[len - i - 1];
        str[len - i - 1] = c;
    }
    return str;
}


uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2){
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int isHex(char *number){
    if(*number++ == '0' && (*number == 'x' || *number == 'X')){
        number++;
        for(int i=0; number[i] != '\0' && number[i] != ' ' ;i++)
            if(!((number[i] >= '0' && number[i] <= '9') || (number[i] >= 'A' && number[i] <= 'F') 
                    || (number[i] >= 'a' && number[i] <= 'f')))
                return FALSE;
        return TRUE;
    }
    while(*number != '\0' && *number != 'h' && *number != 'H') number++;
    if(*number == '\0')
        return FALSE;
    return TRUE;
}

int stringHexToNum(const char *number){
    if(!isHex(number))
        return FALSE;
    char *start = number;

    if(*start == '0' && (*(start + 1) == 'X' || *(start+1) == 'x')) start++;
    while(*number != '\0' && *number != 'h' && *number != 'H') number++;
    
    number--;
    int toReturn = 0, base = 1;
    while(start != number){
        if(*number>= '0' && *number <= '9')
            toReturn += (*number - TO_NUM) * base;
        else if(*number >= 'A' && *number <= 'F')
            toReturn += (*number - TO_NUM_UPPER) * base;
        else if(*number >= 'a' && *number <= 'f')
            toReturn += (*number - TO_NUM_LOWER) * base;
        base *= 16;
        number--;
    }

    return toReturn;
}
