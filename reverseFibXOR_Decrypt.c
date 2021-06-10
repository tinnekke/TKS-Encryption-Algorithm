/*
    Reverse Fibonacci XOR Decryption
	Kyle Cole, Tinnekke Bebout, Suzana Herrera
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 256

/*

 pubKey = variable to hold the public key that is entered by the user
 pvtKey = variable to hold the private key entered by the user 
 privateKeyFile = file that has the ciphertext created by the private key
 cipheredTwiceFile = file that will hold the XOR result of privateKeyFile ^ pvtKey
 decryptFile = file that will contain final decrypted text, should match the original message
 
*/

unsigned char *pubKey, *pvtKey; 
void strip_KeyChars(char* stripChars);

int main(int argc, char* argv[])
{
	//Check for valid number of arguments
	if (argc != 2)
	{
		printf("Invalid number of arguments. %d arguments were supplied.\n", argc);
		printf("Usage: program.exe | private cipher file");
		exit(0);
	}
	
	FILE*privateKeyFile, *cipheredTwiceFile; 
	
	privateKeyFile = fopen(argv[1], "rb");    //XORprivateKey.txt 
	cipheredTwiceFile = fopen("XORcipherTwice.txt", "wb"); //XORcipherTwice.txt
	
	
	
	//Check if files exist
	if (privateKeyFile == NULL)
	{
		printf("Input file doesn't exist or cannot be read.\n");
		exit(0);
	}
	
	//Sets pvtKey to be stored with the size of MAX_SIZE
	pvtKey = malloc(MAX_SIZE);
	
	//Prompt user to enter their private key to decrypt the private cipher file
	printf("Enter the private key associated with %s: ",argv[1]);

	fgets(pvtKey, MAX_SIZE, stdin);
	
	//Strips new lines from the characters entered for the private key
	strip_KeyChars(pvtKey);
		
	int privateKeyDecrypt_Bytes;
	int pvtKey_Index=0;
	
	//XORs the private cipher file with the private key
	while( (privateKeyDecrypt_Bytes = fgetc(privateKeyFile)) != EOF) 
	{
		//XOR the private key with the private cipher file
		fputc(privateKeyDecrypt_Bytes ^ pvtKey[pvtKey_Index], cipheredTwiceFile);

		pvtKey_Index++;
		if(pvtKey_Index == strlen(pvtKey))
		{
			pvtKey_Index = 0;
		}
			
	}
	
	printf("Your private key has been used to decrypt the private cipher.\n"); 
	printf("Your third cipher has been sent to XORcipherTwice.txt\n");
	
	fclose(cipheredTwiceFile);
	fclose(privateKeyFile);
	
	//Sets pubKey memory storage the size of MAX_SIZE
	pubKey = malloc(MAX_SIZE);
	printf("\nEnter your public key to decrypt the third cipher file: ");

	fgets(pubKey, MAX_SIZE, stdin);

	//Strip any newlines out of the public key characters the user enters
	strip_KeyChars(pubKey);
	
	FILE*cipherVersion3, *decryptFile; 
	
	//opens the third cipher file produced by private key ^ private cipher file
	cipherVersion3 = fopen("XORcipherTwice.txt","rb");
	
	//Check that the third cipher text to decrypt exists
	if (cipherVersion3 == NULL)
	{
		printf("Cannot decrypt your third cipher. The third cipher either doesn't exist or cannot be read.\n");
		exit(0);
	}
	decryptFile = fopen("decryptedFile.txt", "wb");
	
	int publicKeyDecrypt_Bytes;
	int pubKey_Index =0;
	
	while( (publicKeyDecrypt_Bytes = fgetc(cipherVersion3)) != EOF) 
	{
		//XOR the public key with the third cipher file
		fputc(publicKeyDecrypt_Bytes ^ pubKey[pubKey_Index], decryptFile);

		pubKey_Index++;
		if(pubKey_Index == strlen(pubKey))
		{
			pubKey_Index = 0;
		}
			
	}
	
	printf("Your public key has been used to decrypt the third cipher.\n"); 
	printf("\nYour decrypted text has been written to the file: decryptedFile.txt"); 
	printf("\nIf your message is not correctly decrypted, make sure you don't have an extra space when entering keys.");
	
	fclose(cipherVersion3);
	fclose(decryptFile);

	//Release memory
	free(pubKey);	
	
	return 0;

}

void strip_KeyChars(char *stripChars)
{
	//remove newlines
	if (stripChars[strlen(stripChars) - 1] == '\n')
	{
		stripChars[strlen(stripChars) - 1] = '\0';
	}
}