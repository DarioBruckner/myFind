#include <iostream>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
#include <unistd.h>
#include "myqueue.h"
#include <sys/wait.h>

/* Hilfsfunktion */
void print_usage(char *programm_name)
{
    printf("Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n\n", programm_name);
    return;
}

std::vector<std::string> searchLogic(std::vector<std::string> message, char **argv, int index, std::string path, bool rec, bool insensitive, pid_t pid)
{

    DIR *dir;
    struct dirent *diread;
    std::vector<char *> files;
    char real_path[256];
    strcpy(real_path, path.c_str());
    int counter = 0;
    std::string filename = "";

    if ((dir = opendir(real_path)) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {

            std::string name = diread->d_name;
            if (rec && (diread->d_type == DT_DIR) && name != "." && name != "..")
            {
                std::string newpath = path + "/" + name;
                message = searchLogic(message, argv, index, newpath, rec, insensitive, pid);
            }

            files.push_back(diread->d_name);
        }
        closedir(dir);
    }
    else
    {
        perror("opendir");
    }

    for (auto file : files)
    {
        filename = file;
        if (insensitive)
        {
            std::for_each(filename.begin(), filename.end(), [](char &c)
                          { c = ::tolower(c); });
            std::string insenFile = argv[index];
            std::for_each(insenFile.begin(), insenFile.end(), [](char &c)
                          { c = ::tolower(c); });
            if (filename == insenFile)
            {
                counter++;
                strcpy(argv[index], filename.c_str());
            }
        }
        else
        {

            if (filename == argv[index])
            {
                counter++;
            }
        }
    }

    if (counter > 0)
    {

        std::string temp = argv[index];

        //printf("%d \n", pid);
        std::string filefound = std::to_string(pid) + " : " + temp + " : " + path + "/" + argv[index];
        message.push_back(filefound);
    }

    return message;
}

/* searches for a file in a given path */
void child(int argc, char **argv, int index, std::string path, bool rec, bool insensitive)
{
    pid_t pid = getpid(); // get process id of child
    message_t msg;        // Buffer for message
    msg.mType = 1;        // Set message type
    int msgid = -1;       // Message Queue ID

    /* Open message queue */
    if ((msgid = msgget(KEY, PERM)) == -1)
    {
        /* error handling */
        fprintf(stderr, "%s: Can't access message queue\n", argv[0]);
    }

    std::vector<std::string> messages;
    messages = searchLogic(messages, argv, index, path, rec, insensitive, pid); // call searchlogic

    if (messages.size() > 0)
    {
        for (std::string mes : messages)
        {
            char mess[265];
            strcpy(mess, mes.c_str());
            /* send message */
            strncpy(msg.mText, mess, MAX_DATA);
            if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
            {
                /* error handling */
                fprintf(stderr, "%s: Can't send message\n", argv[0]);
            }
        }
    }
    else
    {
        std::string nothing = std::to_string(pid) + " : "+ argv[index] + " : File(s) not found";
        char mess[255];
        strcpy(mess, nothing.c_str());
        strncpy(msg.mText, mess, MAX_DATA);
        if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
        {
            /* error handling */
            fprintf(stderr, "%s: Can't send message\n", argv[0]);
        }
    }
}

int main(int argc, char *argv[])
{
    int c;
    pid_t pid;
    message_t msg;       // Buffer for the message
    int msgid = -1;      // Message queue id
    int param_count = 2; // Parameter counter for calculating file number
    char *programm_name;
    programm_name = argv[0];
    bool rec = false;         // To check if recursive search is toggled
    bool insensitive = false; // To check if case insensitive search is toggled

    /* checks the command for flags */
    while ((c = getopt(argc, argv, "Ri")) != EOF)
    {
        switch (c)
        {
        case '?':
            print_usage(programm_name);
            exit(1);
            break;
        case 'R':
            rec = true;
            param_count++;
            break;
        case 'i':
            insensitive = true;
            param_count++;
            break;
        default:
            print_usage(programm_name);
            break;
        }
    }
    /* create new message queue */
    if ((msgid = msgget(KEY, PERM | IPC_CREAT | IPC_EXCL)) == -1)
    {
        /* error handling */
        fprintf(stderr, "%s: Error creating message queue\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* checks the parameters, for each listed file a child will be forked */
    if (param_count < argc)
    {
        for (int i = param_count; i < argc; i++)
        {
            pid = fork();
            switch (pid)
            {
            case -1:
                printf("Child konnte nicht gestartet werden.");
                exit(EXIT_FAILURE);
                break;
            case 0:
                child(argc, argv, i, argv[param_count - 1], rec, insensitive); // call child method to search for file
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
            }
        }
    }
    else
    {
        /* print usage if no files are listed in the command */
        print_usage(programm_name);
    }

    /* wait for childs to be closed */
    wait(NULL);
    int msg_status;
    do
    {
        msg_status = msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 0, IPC_NOWAIT);
        if (msg_status >= 0)
        {
            printf("%s\n", msg.mText);
        }
    } while (msg_status >= 0);

    /* close the message queue */
    msgctl(msgid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}
