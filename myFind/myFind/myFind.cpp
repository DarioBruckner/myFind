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
}
