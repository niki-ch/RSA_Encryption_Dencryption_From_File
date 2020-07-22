// RSA_Encryption_Dencryption_From_File
// RSA Encryption and Dencryption of a text from file .txt

//Niki Chatzivasili

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //library for recognizing the function time()
#include <string.h> //library for recognizing the function strlen()
#include <unistd.h> //library for recognizing the function srand()
#include <gmp.h> //library for recognizing the file gmp.h

//declare functions
void generate_primeNum(mpz_t p, mpz_t q);
void check_primality(mpz_t p);
void generate_rsa_keys(mpz_t p, mpz_t q, mpz_t n, mpz_t e, mpz_t d);
void encryption(mpz_t c, mpz_t message, mpz_t n, mpz_t e);
void decryption(mpz_t m, mpz_t c, mpz_t d, mpz_t n);

gmp_randstate_t stat; //global variable - visible to the whole program

//function for generating two random prime numbers
void generate_primeNum(mpz_t p, mpz_t q){
    long sd = 0; //initialize variable sd to zero

    mpz_t seed; //declare integers

    //initialize variable p, q, seed to zero
    mpz_init(p);
    mpz_init(q);
    mpz_init(seed);

    do{
        //function for producing random numbers, that
        //stat: initialize stat with linear algorithm ALG_LC
        //GMP_RAND_ALG_LC: algorithm
        //120: size (in bits) of function (max size is 128)
        gmp_randinit(stat, GMP_RAND_ALG_LC, 120);

        srand(time(NULL)); //initialize generator of random number
        sd = rand(); //produce different random numbers every time

        mpz_set_ui(seed, sd); //set the value of seed equal with the value of sd (it has been already initialized)

        gmp_randseed(stat, seed); //set the value of seed in stat

        //produce random numbers in range of 0 to 511, that
        //p, q: variables for storing random numbers
        //stat: variable after the initialization in function gmp_randinit
        ///350: size (in bits) for the random variables
        mpz_urandomb(p, stat, 512);
        mpz_urandomb(q, stat, 512);

        printf("Generate two 512 bits random numbers \n");

        printf("First number p: ");
        mpz_out_str(stdout, 10, p); //print the first random integer in base of decimal form
        printf("\n");

        printf("Second number q: ");
        mpz_out_str(stdout, 10, q); //print the second random integer in base of decimal form
        printf("\n\n");

        printf("Checking the primality of number p.... \n");
        check_primality(p); //function that checks if the number p is prime
        printf("\n");

        printf("Checking the primality of number q.... \n");
        check_primality(q); //function that checks if the number q is prime
        printf("\n");
    }
    //if p=0 or p=0 or q=0, then executes again the above algorithm
    //mpz_cmp(): function for checking two numbers and if they are equal, returns 0
    while((mpz_cmp(p, q) == 0) || (mpz_cmp_ui(p, 0) == 0) || (mpz_cmp_ui(q, 0) == 0));
}

//function for checking if an integer is prime, that
//mpz_t p: variable which presents the integer for checking
void check_primality(mpz_t p){
    int prime_test;
    mpz_t m;

    mpz_init(m);

    //function for checking whether p is prime, uses the algorithm Miller-Rabin and returns,
    //return 0: if variable is composite
    //return 1: if variable is probably prime
    //return 2: if variable is prime
    prime_test = mpz_probab_prime_p(p, 10);

    //case variable is prime
    if (prime_test != 0){
        printf("The number is prime\n");
    }

    //case variable is not prime
    else{
        printf("The number is filePointer = fopen(“fileName.txt”, “w”)composite\n");
        mpz_nextprime(m, p); //function that founds the next prime which is greater than p

        printf("The first prime which is greater than this is: ");
        mpz_out_str(stdout, 10, m); //print the prime in decimal form
        printf("\n");

        mpz_set(p, m); //sets the value of variable m in variable p
    }
}

//function for calculating N, f(n), e, d and the keys (public, private)
void generate_rsa_keys(mpz_t p, mpz_t q, mpz_t n, mpz_t e, mpz_t d){
    long sd = 0;

    //declare variables for the conditions of e and initialization
    int first_condition = -1;
    int second_condition = -1;
    int third_condition = 0;

    //declare integers
    mpz_t seed;
    mpz_t f, p_1, q_1, gcd;

    //initialize variables to zero
    mpz_init(f);
    mpz_init(n);
    mpz_init(e);
    mpz_init(d);
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_init(gcd);
    mpz_init(seed);

    ///Calculation of N = p*q
    printf("Calculation of N: N = p * q = ");
    mpz_mul(n, p, q); //multiplies n = p*q
    mpz_out_str(stdout, 10, n); //print the result of n
    printf("\n\n");

    ///Calculation of f(n) = (p-1)*(q-1)

    mpz_sub_ui(p_1, p, 1); //calculates p-1
    mpz_sub_ui(q_1, q, 1); //calculates q-1

    printf("Calculation of f(n): f(n) = (p-1) * (q-1) = ");
    mpz_mul(f, p_1, q_1); //multiplies f(n) = (p-1)*(q-1)
    mpz_out_str(stdout, 10, f); //print the result of f(n)
    printf("\n\n");

    do{
        ///Produce random number for variable e

        gmp_randinit(stat, GMP_RAND_ALG_LC, 120); //function for producing random numbers

        srand(time(NULL)); //initialize generator of random number
        sd = rand(); //produce different random numbers every time

        mpz_set_ui(seed, sd); //set the value of seed equal with the value of sd (it has been already initialized)

        gmp_randseed(stat, seed); //set the value of seed in stat

        mpz_urandomb(e, stat, 32); //produce random number with size 32 bits

        ///Calculation of e, 1<e<f and gcd(f,e) = 1

        first_condition = mpz_cmp_ui(e, 1); //function that return positive value if e>1

        second_condition = mpz_cmp(f, e); //function that return positive value if f>e

        mpz_gcd(gcd, e, f); //function that calculate the gcd(e, f)
        third_condition = mpz_cmp_ui(gcd, 1); //function that return 0 if gcd=1
    }
    //if the three conditions are not applied then executes again the above algorithm
    while(first_condition < 0 || second_condition < 0 || third_condition != 0);

    printf("Random number e: ");
    mpz_out_str(stdout, 10, e); //prints the random integer in base of decimal form
    printf("\n\n");

    ///Calculation of d, d = e^(-1) mod f

     mpz_invert(d, e, f); //calculates d=e^(-1) mod f

    printf("Calculation of d: d = e^(-1) mod f = ");
    mpz_out_str(stdout, 10, d); //prints the first random integer in base of decimal form
    printf("\n\n");

    //print public key (n, e)
    printf("Public key: ");
    mpz_out_str(stdout, 0, n);
    mpz_out_str(stdout, 0, e);
    printf("\n\n");

    //print private key (d)
    printf("Private key: ");
    mpz_out_str(stdout, 0, d);
    printf("\n");
}

//function for the encryption of message
//c: encrypted message
//message: initial message in mpz_t
//n, e: public key
void encryption(mpz_t c, mpz_t message, mpz_t n, mpz_t e){
    mpz_init(c);  //initialize variable c

    mpz_powm(c, message, e, n); //calculates c = m^(e) mod n

    printf("Encrypted message: ");
    mpz_out_str(stdout, 0, c); //print the encrypted message

    printf("\n\n");
}

//function for the decryption of message
//m: decrypted message
//c: encrypted message
//d: private key
void decryption(mpz_t m, mpz_t c, mpz_t d, mpz_t n){
    mpz_init(m);  //initialize variable m

    mpz_powm(m, c, d, n); //calculates m = c^(d) mod n

    printf("Decrypted message: ");
    mpz_out_str(stdout, 0, m); //prints the decrypted message
    printf("\n");
}

int main(){
    int i; //variable - counter
    int length; //variable for the length of message
    char message[50]; //array for the message in char
    int messageInt[50]; //array for the message in integer

    char text[30] = ""; //array for the converted message in char
    char txt[30] = ""; //array for the whole message

    mpz_t msg, p, q, n, e, d, c, m; //declare variable

    //initialize variables to zero
    mpz_init(msg);
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(e);
    mpz_init(d);
    mpz_init(c);
    mpz_init(m);

    FILE *filePointer; //declare file pointer

    //open the existing file Data.txt with fopen()
    //"r": read mode
    filePointer  = fopen("Data.txt", "r");

    //case file does not exist
    if(filePointer == NULL){
        printf( "Data.txt file failed to open.");
        return 0; //exit
    }

    printf("Message from file: ");

    //prints the message from the file
    while(fgets(message, sizeof(message), filePointer) != NULL)
        printf ("%s", message);

    fclose(filePointer); //closing file

    printf("\n\n");

    length = strlen(message); //function that counts the number of characters from message and stops when there is null character

    printf("Message converted to integer: ");

    //print message - every character of message is converted to integer according to ASCII array
    for(i=0; i<(length-1); i++){
        messageInt[i] = message[i]; //store the array (char)message to array (int)message_int
        printf("%d", messageInt[i]);
    }
    printf("\n\n");

    //convert message[] to integer

    for(i=0; i<(length-1); i++){
        sprintf(text, "%d", messageInt[i]); //convert the array of integers - messageInt, to array of char - text

        //concatenates the char array text and the char array txt
        //the result is in txt
        strcat(txt, text);
    }

    mpz_set_str(msg, txt, 10); //convert the char array txt to mpz_t in base of decimal form

    printf("Message in initial form: ");
    //print message to the original form
    for(i=0; i<(length-1); i++){
        printf("%c", message[i]);
    }
    printf("\n\n");

    generate_primeNum(p, q); //generate two prime numbers

    generate_rsa_keys(p, q, n, e, d); //generates the keys for the rsa algorithm, public and private

    printf("\n");

    //print the initial message
    printf("Message: ");
    mpz_out_str(stdout, 0, msg);
    printf("\n\n");

    encryption(c, msg, n, e); //encrypt the message

    decryption(m, c, d, n); //decrypt the message

    //free space
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(m);
    mpz_clear(c);
    mpz_clear(msg);

} //end main

