/*
    Reverse Fibonacci XOR Encryption
	Kyle Cole, Tinnekke Bebout, Suzana Herrera
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <inttypes.h>
#define MAX_SIZE 256

/*
 num
 variable to hold the fibonacci recursive subtraction sequence 
 num, when completely iterated through the fibonacci functions is also the public key
 
 p = variable to determine prime numbers for the random number generator
 min = sets the lower limit of the random number generator
 max = sets the upper limit of the random number generator 
 n = variable to hold the smallest number of the fibonacci sequence
 num2 = variable to hold the value of the private key
 
 pubKey = variable to hold the public key that is entered by the user
 pvtKey = variable to hold the private key entered by the user 
*/

unsigned long long num, n, num2, min, max, p;
unsigned char *pubKey, *pvtKey; 

//function that checks if the randomly-generated number (num) is prime
unsigned long long checkRandNum(unsigned long long p)
{
    unsigned long long i;
	
    if(p % 2 == 0 || p == 1) { return 0; }

    for(i = 3; i*i <= p; i += 2) 
	{
        if(p % i == 0) 
		{ 
			return 0; 
		}
			
    }
    
	//return 1 if p is prime
    return 1;

}

//return a uniform random value of a range of 0..n-1 inclusively
unsigned long long randPrime(unsigned long long min, unsigned long long max)
{
    unsigned long long randNum;

    while(checkRandNum((randNum = rand() % (max - min +1)) + min));

    //Set randNum = num, this will be used for the fibonacci sequence later
	num = randNum; 
    return num;
}


unsigned long long reverseFib(unsigned long long p) 
{ 
    //Corner cases 
    if (p == 0 || p == 1) 
        return p; 
  
    //Finds the greatest fibonacci number smaller than num
    unsigned long long f1 = 0, f2 = 1, f3 = 1; 
    while (f3 <= p)
	{ 
        f1 = f2; 
        f2 = f3; 
        f3 = f1 + f2; 
    } 
	
    return f2;
} 

//Prints a reverse fibonacci sequence of num by using the Greedy Algorithm
unsigned long long makePublicKey(unsigned long long num) 
{ 
    while (num> 0) 
	{ 
        // Find the greatest fibonacci number smaller than or equal to n
        n = reverseFib(num); 
		
        //Print the fibonacci number sequence in reverse order
        printf("%llu ",n);
		
        //Reduce num
        num = num - n; 
    } 
	
	//returns the smallest value of the fibonacci sequence
    return n;
} 

unsigned long long makePrivateKey(unsigned long long num2)
{
    num2 = pow(num, n+2);
	printf("\nPrivate key: %llu\n", num2);
    
    //Removes anything written to the private key file so that each iteration writes a fresh private key
	FILE *cipher2;
	cipher2 = fopen("privateKey.txt", "w+");
	fprintf(cipher2, "%llu ",num2);
	fclose(cipher2);
	
	return num2; 
	
}

void strip_KeyChar(char* stripChars);
void encrypt(FILE* inputFile, FILE* outputFile, char *pubKey, int index);

unsigned long long genKeys()
{
	//Removes anything written to this file, if anything
	FILE *removeCipher;
  
	removeCipher = fopen("publicKey.txt", "w+");
	fclose(removeCipher);
	
	unsigned long long min = 5, max = 257;
	
    srand(time(0));
	randPrime(min,max);
	
	FILE *cipher;
	cipher = fopen("publicKey.txt", "a+");
	fprintf(cipher, "%llu ",num);
	fclose(cipher);
			
	printf("Non-neighboring Fibonacci Representation of %llu is: ",num);
	makePublicKey(num);
	printf("\nPublic key: %llu", num);
	makePrivateKey(num2);
	
	printf("Done\n");
	
	printf("\nYour public key has been written to publicKey.txt\n");
	printf("Your private key has been written to privateKey.txt\n");
		
	return num, num2;
}
int getCiphers()
{
	char cipher1[90]; //array to hold the contents of the file being read
	FILE *public_Cipher; //Variable to hold the data of the file that will be read and printed to the screen
	
	printf("\nYour first cipher is: ");
	
	public_Cipher = fopen("XORcipher.txt", "r"); //reads the first ciphertext file 
	
	if(public_Cipher == NULL)
	{
		printf("That cipher file doesn't exist.");
	}
		
	while(!feof(public_Cipher))
	{
		fgets(cipher1, 90, public_Cipher); 
		puts(cipher1); 
	}
	
	fclose(public_Cipher);
	
	
	char cipher2[90];
	FILE *private_Cipher;
	
	printf("Your second cipher is: ");
	private_Cipher = fopen("XORprivateKey.txt", "r");
	
	if(private_Cipher == NULL)
	{
		printf("This cipher file doesn't exist.");
	}
		
	while(!feof(private_Cipher))
	{
		fgets(cipher2, 90, private_Cipher); 
		puts(cipher2); 
	}
	
	fclose(private_Cipher); 
	
	return 0;
}

unsigned long long main(int argc, char *argv[])
{
	//Check for valid number of arguments
	if (argc != 2)
	{
		printf("Invalid number of arguments. %d arguments were supplied.\n", argc);
		printf("Usage: %s inputFile\n", argv[0]); 
		exit(0);
	}
	
	genKeys();
	FILE* input;
	FILE* output;

	//Open input and output files
	input = fopen(argv[1], "r");
	output = fopen("XORcipher.txt", "w");
		
	//Check that the inputFile & outputFile exist
	if (input == NULL)
	{
		printf("Input file does not exist or could not be accessed.\n");
		exit(0);
	}
		
	if (output == NULL)
	{
		printf("Output file does not exist or could not be accessed.\n");
		exit(0);
	}
	
	//Sets pubKey to be stored with the size of MAX_SIZE
	pubKey = malloc(MAX_SIZE);

	//Prompt user to enter their public key
	printf("\nEnter your public key: ");

	//Read the characters of the public key 
	fgets(pubKey, MAX_SIZE, stdin);
	
	printf("Encrypting %s\n", argv[1]);

	//Strips new lines from the characters entered for the public key
	strip_KeyChar(pubKey);
	
	//calls the encrypt() function
	encrypt(input, output, pubKey, 0);
	
	//Release memory so that the key can be used by encrypt()
	free(pubKey);

	fclose(input);
	fclose(output);
	
	//Sets pvtKey memory storage the size of MAX_SIZE
	pvtKey = malloc(MAX_SIZE);
	
	printf("\nEnter your private key: ");
	fgets(pvtKey, MAX_SIZE, stdin);

	//Strip any newlines out of the private key characters the user enters
	strip_KeyChar(pvtKey);
	
	FILE *cipherFile, *privateKeyFile; 
	
	//opens the ciphertext file for reading, while the private key file will be open for writing
	cipherFile = fopen("XORcipher.txt", "rb");
	privateKeyFile = fopen("XORprivateKey.txt", "wb");
	//privateKeyFile is set to writing mode so that the file can be created
	
	int privateKey_Bytes;
	int privateKey_Index =0;
	
	while( (privateKey_Bytes = fgetc(cipherFile)) != EOF) 
	{
		//XOR the data and write it to a file
		fputc(privateKey_Bytes ^ pvtKey[privateKey_Index], privateKeyFile);

		//Increment key_count and start over if necessary
		privateKey_Index++;
		if(privateKey_Index == strlen(pvtKey))
		{
			privateKey_Index = 0;
		}
			
	}
	
	fclose(cipherFile);
	fclose(privateKeyFile);
	
	printf("\nPublic cipher data written to XORcipher.txt\n");
	printf("Private cipher data written to XORprivateKey.txt\n");
	
	//Call getCiphers() so the user knows that the cipher files were generated
	getCiphers();
	
	return 0;
}

void encrypt(FILE* inputFile, FILE* outputFile, char* pubKey, int index)
{
	index = 0; //Used to restart key if strlen(key) < strlen(encrypt)
	int publicKey_Bytes;
	
	while( (publicKey_Bytes = fgetc(inputFile)) != EOF) //Loop through each byte of file until EOF
	{
		//XOR the data and write it to a file
		fputc(publicKey_Bytes ^ pubKey[index], outputFile);

		//Increment key_count and start over if necessary
		index++;
		if(index == strlen(pubKey))
		{
			index = 0;
		}
			
	}
	
}

void strip_KeyChar(char* stripChars)
{
	//remove newlines
	if (stripChars[strlen(stripChars) - 1] == '\n')
	{
		stripChars[strlen(stripChars) - 1] = '\0';
	}
}