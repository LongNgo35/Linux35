#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main()
{
    int msgq_id, msgq_rcv = 0;
    struct msgbuf handler_buffer = {};
    // create key
    key_t key = ftok("key.txt", 66);
    if (key == -1)
    {
        perror("Cannot generate new key\n");
        return -1;
    }
    
    // create message queue
    msgq_id = msgget(key, 0666 | IPC_CREAT);
    if (msgq_id == -1)
    {
        perror("Cannot create new message queue\n");
        return -1;
    }
    
    handler_buffer.mtype = 1;
    // write to message queue
    msgq_rcv = msgrcv(msgq_id, &handler_buffer, sizeof(handler_buffer.mtext), 1, 0);

    printf("The message was sent: %s\n", handler_buffer.mtext);
    sleep(20);
    return 0;
    
}