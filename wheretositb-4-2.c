// Taylor Winn
// Assingment #2
// Febuary 12th 2024

#include <stdio.h>
#include <stdlib.h> // including nessecary libraries
#include <string.h>

#define len 200 // constant int for later implementation in the recursive function

int main(int argc, char *argv[]){     // declaring main program to return as type int

    char** RecursivePermute(char **string, int k, int people, char **cornholders, int counter, int NoPairs, char **pairs, int *output); // prototype for the recursive funtion taking 8 parameters
    void ExchangeCharacter(char **string, int i, int j);    // prototype for the function that exchanges the strings within the list
    int isValid(char **string, char **cornholders, int people, int counter);    // prototype for the first function to check if the list is valid with the given parameters
    int cantPair(char **string, char **pairs, int NoPairs, int people); // prototype for the second function to check if the list arrangment is valid

    int people = 0; // number of names within the list
    int NoPairs = 0;    // number of pairs who cannot be seated next to each other
    int possible = 0;   // just a test variable 

    FILE *file = NULL;
    file = fopen(argv[1], "r");
    if(file == NULL){       // notes in other file, same process
        printf("cannot open file");
        return 1;
    } 

    fscanf(file, "%d %d", &people, &NoPairs);

    char *Main[people]; // pointer to a character array where the strings will be stored in order to pass to functions
    
    int index = people; 
    char **cornholders = malloc(index * sizeof(char*)); // dynamically allocated character array that will store the names who hold popcorn
    int counter = 0;

    int i = 0;
    while(i < people && !feof(file)){  // makes sure we receive all of the names

        char string[100];
        int number;
        fscanf(file, "%s %d", string, &number);
        if(number == 1){
            int length = strlen(string);
            cornholders[counter] = malloc((length-1) * sizeof(char));   // notes in other file, same process
            strcpy(cornholders[counter], string);
            counter += 1;
        }
        Main[i] = malloc(100 * sizeof(char));
        strcpy(Main[i], string);
        i++;
    }


    cornholders = realloc(cornholders, counter * sizeof(*cornholders)); // reallocating the memory for the number of people we need to avoid memory leek
    for(int i = 0; i > (counter - 1) && i < (index - 1); i++){
        free(cornholders[i]);   // freeing excess indecies
    }


    i = 0;
    int count = 0;
    index = 0;
    char *pairs[NoPairs*2];
    while(i < NoPairs && !feof(file)){
        pairs[index] = malloc(100 * sizeof(char));
        pairs[index + 1] = malloc(100 * sizeof(char));      // notes in other file, same process
        fscanf(file, "%s %s", pairs[index], pairs[index+1]);
        index += 2; 
        count += 1;
    }

    int k = 0;
    int output = 0;
    char **re = RecursivePermute(Main, k, people, cornholders, counter, NoPairs, pairs, &output);   // pointer to a pointer pointing to an array of string which will be the return value containing the correctly ordered list
    if(re != NULL){ // makes sure that nothing is executed if the list is empty
        for(int i = 0; i < people; i++){    // printing each string within the list
            printf("\n%s", re[i]);
        }
    }
    
    free(cornholders[1]);   // freeing all the memory that is no longer needed to avoid memory leek
    for(int b = 0; b < NoPairs*2; b++){
        free(pairs[b]);
    }
    for(int j = 0; j < counter; j++){
        free(Main[j]);
    }

    fclose(file);

return 0;   // returning the main function
}


int isValid(char **string, char **cornholders, int people, int counter){    // first function to check the validity of the (tested) list
    
    int arry[people];   // temp array to store a 1 if the person holds popcorn, 0 if not
    for(int a = 0; a < people; a++){
        for(int b = 0; b < counter; b++){   // nested for loop to loop through both the names as well as the names with corn to check if they hold or not
            if(strcmp(string[a], cornholders[b]) == 0){
                arry[a] = 1;    // assingning one if they have corn
                break;
            } else {
                arry[a] = 0;    // 0 if not
            }
        }
    }

    int negtest = 0;    // return variable

    for(int i = 0; i < people; i++){
        if(arry[i] < 1){    // if the person doesnt hold corn, the person next to them on either the right (i+1) or left (i-1) needs to
            if(i == 0 && arry[i + 1] < 1){  // checking the right side if there is no value on the left
                negtest = negtest - 1;
            } else if (i == (people-1) && arry[i-1] < 1){   // checking the left if theres nothing on the right
                negtest = negtest - 1;
            } else {
                if(arry[i+1] < 1 && arry[i-1] < 1){ // checking both sides if in between, there only needs to be one on the right side or the left, not both, so || is appropriate
                    negtest = negtest - 1;
                }
            }
        }
    }
return negtest; // return the test variable
}


int cantPair(char **string, char **pairs, int NoPairs, int people){ // second confirming function, checking to see if the person is in a pair, and if theyre next to their pair if so

    for(int a = 0; a < NoPairs * 2; a += 2){    // another nested for loop serving the same purpose
        for(int b = 0; b < people - 1; b++){
            if((strcmp(pairs[a], string[b]) == 0 && strcmp(pairs[a+1], string[b+1]) == 0) || strcmp(pairs[a], string[b+1]) == 0 && strcmp(pairs[a+1], string[b]) == 0){ // all of the test cases, returning -1 if ANY pair is found as that invalidates the list
                return -1;
            }
        }
    }
return 0;
}

void ExchangeCharacter(char **string, int i, int j){    // function for the modules that switches the order of elements within the array, i modified it to take in a pointer to the char point array 
    char *temp = string[i];
    string[i] = string[j];  // switches the two elements 
    string[j] = temp;
}

char** RecursivePermute(char **string, int k, int people, char **cornholders, int counter, int NoPairs, char **pairs, int *output){ // the other function from the modules, heavily modified to take in all the nessecary variables
    static char* lexigraph[len];    // static character pointer array that will be assinged to the final list and passed back to the main function
    static int tru = 0; // test case
    
    int j = 0;
    if(k == people){    // checks to see if we have reached the end of the list (people = number of names within list)
        int tester = isValid(string, cornholders, people, counter); // passing in the two functions that are checking if the input is valid given the special parameters
        int negtester = cantPair(string, pairs, NoPairs, people);
        if(tester == 0 && negtester == 0){  // if both functions are valid, they will return 0. if not, they return negative numbers
            if(!tru || strcmp(string[0], lexigraph[0]) < 0){    // checking if we have reached the correct (lexicographical) ordering of the first valid list
                for(int a = 0; a < people; a++){
                    lexigraph[a] = string[a];   // if so, we need to assign the values from each index of the test string to the answer string
                }
            } tru = 1;  // change value to break loop
        }
    } else {    // if not, we need to keep calling the functions to switch the order and execute the recursive function
        for(j = k; j < people; j++){
            ExchangeCharacter(string, k, j);
            RecursivePermute(string, k+1, people, cornholders, counter, NoPairs, pairs, output);
            ExchangeCharacter(string, j, k);
        }
    }

    if(tru){
        return lexigraph;   // returning the answer string
    } else {
        return NULL;    // if nothing is in list, return null
    }
}   