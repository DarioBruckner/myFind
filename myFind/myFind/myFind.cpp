#include <iostream>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
#include <unistd.h>


/* Hilfsfunktion */
void print_usage( char *programm_name ) {
    printf("Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n\n",programm_name);
    return;
}

void child(int argc, char **argv, int index, std::string path, bool rec, bool insensitive)
{
    pid_t pid = 1;

    DIR *dir; 
    struct dirent *diread;
    std::vector<char *> files;
    char real_path[256];
    strcpy(real_path, path.c_str());
    int counter = 0;
    std::string filename = "";
    
    if(rec){

    }else{
        if ((dir = opendir(real_path)) != nullptr) {
            while ((diread = readdir(dir)) != nullptr) {
                files.push_back(diread->d_name);
            }
            closedir (dir);
        } else {
            perror ("opendir");
        }


        for(auto file: files){
            filename = file; 
            if(insensitive){
                std::for_each(filename.begin(),filename.end(), [](char &c){
                    c = ::tolower(c);
                });
                std::string insenFile = argv[index];
                std::for_each(insenFile.begin(), insenFile.end(), [](char &c){
                    c = ::tolower(c);
                });
                if(filename == insenFile){
                    counter++;
                    strcpy(argv[index], filename.c_str());
                }

            }else{
                
                if(filename == argv[index]){
                    counter++;
                }   
            }
            
        }

        if(counter > 0){
            std::cout << pid << " : " << argv[index] << " : " << path << "/" << argv[index] << std::endl;
        }else{
            std::cout << "File(s) not found" << std::endl;
        }
    }
   
    //Suchlogik einbauen
    // printf("%s\n",argv[index]);
}

void test(){

    std::string path =  "/home/dario/Documents/TestFolder";
    std::string filename = "pyp.py";

    
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
