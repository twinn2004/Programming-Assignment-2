// Taylor Winn
// Assingment #2
// Febuary 12th 2024

#include <stdio.h>
#include <stdlib.h> // including nessecary libraries
#include <string.h>

int main(int argc, char *argv[]){ // declaring main program to return as type int

    void RecursivePermute(char **string, int k, int people, char **cornholders, int counter, int NoPairs, char **pairs, int *count);    // prototype for the recursive funtion taking 8 parameters
    void ExchangeCharacter(char **string, int i, int j);    // prototype for the function that exchanges the strings within the list
    int isValid(char **string, char **cornholders, int people, int counter);    // prototype for the first function to check if the list is valid with the given parameters
    int cantPair(char **string, char **pairs, int NoPairs, int people); // prototype for the second function to check if the list arrangment is valid

    int people = 0; // number of names within the list
    int NoPairs = 0;    // number of pairs who cannot be seated next to each other
    int possible = 0;   // just a test variable

    FILE * file;    // creating a pointer that points to the file directory
    file = fopen(argv[1], "r"); // assinging the file pointer to the file inputted in the command line
    if(file == NULL){      // making sure the file was opened and isnt blank
        printf("\nnothing found in file\n");
    } else {
        fscanf(file, "%d %d", &people, &NoPairs);       // starting the process (receiving and storing input)
    }

    char *Main[people]; // pointer to a character array where the strings will be stored in order to pass to functions
    
    int index = people; 
    char **cornholders = malloc(index * sizeof(char*)); // dynamically allocated character array that will store the names who hold popcorn
    int counter = 0;

    int i = 0;
    while(i < people && !feof(file)){  // makes sure we receive all of the names

        char string[100];   // temp string
        int number; 
        fscanf(file, "%s %d", string, &number); 
        if(number == 1){    // if the number next to the name is one, they have popcorn and therefor need to be appended to the cornholders list
            int length = strlen(string);
            cornholders[counter] = malloc((length-1) * sizeof(char));   // dynamically allocating space for the string since we don't know the size 
            strcpy(cornholders[counter], string);
            counter += 1;
        }
        Main[i] = malloc(100 * sizeof(char));   // more dynamic allocation (this time for the index)
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
    char *pairs[NoPairs*2];     // since a pair is a set of two the size of the array would just be the number of pairs multiplied by 2
    while(i < NoPairs && !feof(file)){  // since the pairs are the final lines of input,  we can use file end of file to check for end of input
        pairs[index] = malloc(100 * sizeof(char));
        pairs[index + 1] = malloc(100 * sizeof(char));  // allocating memory for both the initial input and its pair to follow
        fscanf(file, "%s %s", pairs[index], pairs[index+1]);
        index += 2; // incrimenting by two since we are assinging the memory by pair
        count += 1; 
    }

    int k = 0;
    int track = 0;
    RecursivePermute(Main, k, people, cornholders, counter, NoPairs, pairs, &track);    // calling the recursive permutaion 

    free(cornholders[1]);   //freeing the dynamically allocated memory of both the array and the strings within the array to avoid memory leak
    for(int b = 0; b < NoPairs*2; b++){
        free(pairs[b]);
    }
    for(int j = 0; j < counter; j++){   
        free(Main[j]);  // more memory freeing to avoid memory leak
    }
    printf("\n\n%d possible combinations", track);  // printing output since its just a single int

return 0;   //return the main function
}


int isValid(char **string, char **cornholders, int people, int counter){    // first paramteter, making sure that everyone have access to popcorn
    
    int arry[people];
    for(int a = 0; a < people; a++){
        for(int b = 0; b < counter; b++){
            if(strcmp(string[a], cornholders[b]) == 0){ // simple implementaion is a nested for loop, looping through the array of names until we find one who has corn
                arry[a] = 1;    // adding 1 (cornholding) to the appropriate infex within the temp array just to make things simpler
                break;
            } else {
                arry[a] = 0;
            }
        }
    }

    int negtest = 0;

    for(int i = 0; i < people; i++){
        if(arry[i] < 1){
            if(i == 0 && arry[i + 1] < 1){
                negtest = negtest - 1;
            } else if (i == (people-1) && arry[i-1] < 1){   // looping through to make sure that everyone has access
                negtest = negtest - 1;  // if someone dosent have popocorn, we check their left and right (+ and -) to make sure that one of them has it. if not, the whole list is invalid, so return a negative number
            } else {
                if(arry[i+1] < 1 && arry[i-1] < 1){
                    negtest = negtest - 1;  // if mot, return a positive number and the list is passed to the next validating function
                }
            }
        }
    }
return negtest;
}


int cantPair(char **string, char **pairs, int NoPairs, int people){

    for(int a = 0; a < NoPairs * 2; a += 2){
        for(int b = 0; b < people - 1; b++){    // same thing as previous, nested loop will cross compare indecies. 
            if((strcmp(pairs[a], string[b]) == 0 && strcmp(pairs[a+1], string[b+1]) == 0) || strcmp(pairs[a], string[b+1]) == 0 && strcmp(pairs[a+1], string[b]) == 0){ // all notes are in other file
                return -1;
            }   // if someone is found next to their pair, the whole list is invalid. thus, a negative number is returned to indicate the list must be discarded
        }
    }
    return 0;   // if no pairs are found, pass 0 and the list is deemed valid

return 0;
}

void ExchangeCharacter(char **string, int i, int j){    
    char *temp = string[i];
    string[i] = string[j];      // all notes are in other file
    string[j] = temp;
}

void RecursivePermute(char **string, int k, int people, char **cornholders, int counter, int NoPairs, char **pairs, int *count){
    int j = 0;
    if(k == people){
        int tester = isValid(string, cornholders, people, counter); // calling test functions to check list for 2 parameters
        int negtester = cantPair(string, pairs, NoPairs, people);
        if(tester == 0 && negtester == 0){  
            (*count) += 1;  // instead of printing the list if it is valid, this time we record the count using a pointer to an int variable (i couldnt just use an int variable beacuse the resursive function would re-set its value)
        }
    } else {
        for(j = k; j < people; j++){
            ExchangeCharacter(string, k, j);
            RecursivePermute(string, k+1, people, cornholders, counter, NoPairs, pairs, count);
            ExchangeCharacter(string, j, k);
        }
    }
}  