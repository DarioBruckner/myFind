// myFind.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unistd.h>
/* Hilfsfunktion */
void print_usage( char *programm_name ) {
    printf("Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n\n",programm_name);
    return;
}

int main(int argc, char*argv[] )
{
    int c;
    char *programm_name;
    programm_name = argv[0];

    while ((c = getopt(argc, argv, "Ri:")) != EOF){
        switch (c)
        {
        case '?':
            print_usage(programm_name);
            exit(1);
            break;
        case 'R':
            /* code */
            break;
        case 'i':
            break;
        default:
            break;
        }
    }
    std::cout << "Hey World!\n";
}
