#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    int pid1 = fork();

    if (pid1 == -1) {
        fprintf(stderr, "Fork failed!\n");
        return -1;
    }
    else if(pid1 == 0) {
        key_t key = ftok("task1.c", 65);
        int msgid = msgget(key, 0666 | IPC_CREAT);
        
        if (msgid == -1) {
            fprintf(stderr, "Message queue creation failed!\n");
            return -1;
        }

        struct msg_buffer ack_message;
        struct msg_buffer message;
        message.msg_type = 1;

        while (1) {
            printf("[%d]sender:Enter data to be sent:", getpid());
            fgets(message.msg_text, 100, stdin);
            
            message.msg_text[strlen(message.msg_text)-1] = '\0';
            
            if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
                fprintf(stderr, "[%d]sender:Message sending failed!\n", getpid());
                return -1;
            }

            if (strcmp(message.msg_text, "exit") == 0) {
                break;
            }

            if (msgrcv(msgid, &ack_message, sizeof(ack_message.msg_text), 2, 0) == -1) {
                fprintf(stderr, "[%d]sender:Message receiving failed!\n", getpid());
                return -1;
            }
        }

        msgctl(msgid, IPC_RMID, NULL);
    }
    else {
        int pid2 = fork();

        if (pid2 == -1) {
            fprintf(stderr, "Fork failed!\n");
            return -1;
        }
        else if(pid2 == 0) {
            sleep(1);
            key_t key = ftok("task1.c", 65);
            int msgid = msgget(key, 0666);

            if (msgid == -1) {
                fprintf(stderr, "Message queue access failed!\n");
                return -1;
            }

            struct msg_buffer message;
            struct msg_buffer ack_message;
            ack_message.msg_type = 2;

            while (1) {
                if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
                    fprintf(stderr, "[%d]receiver:Message receiving failed!\n", getpid());
                    return -1;
                }

                printf("[%d]receiver:Data received:%s\n", getpid(), message.msg_text);

                if (strcmp(message.msg_text, "exit") == 0) {
                    break;
                }

                if (msgsnd(msgid, &ack_message, sizeof(ack_message.msg_text), 0) == -1) {
                    fprintf(stderr, "[%d]receiver:Message sending failed!\n", getpid());
                    return -1;
                }
            }
        }

        wait(NULL);
        wait(NULL);
    }

    return 0;
}
