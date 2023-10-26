
/*
stringCompare: Returns -1 if str1 is smaller than str2, 1 otherwise, and 0 if they are equal.
*/
int stringCompare(const char *str1, const char *str2);

/*
stringnCompare: Compares the first n chars from str1 and str2 and returns -1 if str1 is smaller than
                str2, 1 otherwise, and 0 if they are equal
*/
int stringnCompare(const char *str1, const char *str2, uint64_t count);

/*
toUpper: Transforms all characters of str from lower case to upper case
*/
void toUpper(char *str);

/*
toLower: Transforms all characters of str from upper case to lower case
*/
void toLower(char *str);

/*
stringCopy: Copies content from src to dest and returns its location
*/
char *stringCopy(char *dest, int destSize, const char *source);

/*
stringnCopy: Copies n chars from src to dest and returns its location
*/
char *stringnCopy(char *dest, int destSize, const char *src, int count);

/*
stringLength: Returns the length of the str (NULL included)
*/
uint64_t stringLength(const char *str);

/*
firstOcurrence: Returns the location of the first ocurrence of a character, or NULL if it's not found
*/
char *firstOcurrence(char *str, char c);

/*
stringToNum: Returns the number value of a string, or 0 if it's not a number
*/
long stringToNum(const char * str);

/*
charToNum: Returns the number value of a character, or 0 if it's not a number
*/
int charToNum(char c);

/*
isCharNum: Returns if a character is a number's ASCII value
*/
int isCharNum(char c);

/*
numToString: Transforms a number to a string. Returns the number of digits transformed
*/
int numToString(int num, char *buffer, int bufferSize);

/*
decToHex: Transforms an integer number into its hexadecimal representation
*/
int decToHex(int num, char *buffer, int bufferSize);


uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

/*
stringReverse: Reverses the given string.
*/
char *stringReverse(char *str);

int isHex(char *number);

int stringHexToNum(const char *number);

#endif // _STRING_H_