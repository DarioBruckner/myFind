#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>
#include <string.h>

int main(){

    DIR *dir; 
    struct dirent *diread;
    std::vector<char *> files;
    char real_path[256];
    std::string path =  "/home/dario/Documents/TestFolder";
    std::string filename = "pyp.py";

    strcpy(real_path, path.c_str());

    if ((dir = opendir(real_path)) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            files.push_back(diread->d_name);
        }
        closedir (dir);
    } else {
        perror ("opendir");
        return EXIT_FAILURE;
    }

    //for (auto file : files) std::cout << file << "| ";
    //    std::cout << std::endl;
    for(auto file: files){
        if(file == filename){
            std::cout << "File found" << std::endl;
        }
    }


    return EXIT_SUCCESS;
#include <unistd.h>
#include <string>

/* Hilfsfunktion */
void print_usage( char *programm_name ) {
    printf("Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n\n",programm_name);
    return;
}

void child(int argc, char **argv, int index, std::string path, bool rec, bool insensitive)
{
    pid_t pid;
    // Suchlogik einbauen
    printf("%s\n",argv[index]);
}

int main(int argc, char*argv[] )
{
    int c;
    // Command und Path
    int param_count = 2;
    char *programm_name;
    pid_t pid;
    bool rec = false;
    bool insensitive = false;
    programm_name = argv[0];
    while ((c = getopt(argc, argv, "Ri")) != EOF){
        switch (c)
        {
        case '?':
            print_usage(programm_name);
            exit(1);
            break;
        case 'R':
            rec = true;
            printf("R\n");
            param_count++;
            /* code */
            break;
        case 'i':
            insensitive = true;
            printf("i\n");
            param_count++;
            break;
        default:
            print_usage(programm_name);
            break;
        }
    }
    
    /* Geht die angegebenen Files durch und forkt für jedes File */
    if(param_count < argc){
        for(int i = param_count; i < argc; i++){
        pid = fork();
        switch (pid)
	    {
            case -1:
                printf("Child konnte nicht gestartet werden.");
                exit(EXIT_FAILURE);
                break;
            case 0:
                printf("child\n");
                child(argc, argv, i, argv[param_count-1], rec, insensitive);
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
	    }
    }
    }else{
        print_usage(programm_name);
    }
    
}
