/*
 * Author: Waleed Ahmad
 */
#include<stdio.h>
#include<thread.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>
#include <list.h>
#define TERMINATE_CODE 404

typedef struct {
    char msg[1024];
    struct tm current_time; 
} MessageBuffer;

MessageBuffer sendBufferStruct;
MessageBuffer rcvBufferStruct;
LIST* sendBufferList;
LIST* rcvBufferList;

PID PrintBufferToPort;
PID consoleToPrintBuffer;
PID portToReceiveBuffer;
PID receiveBufferToConsole;
PID serverThreadPid;
int endproc = 0;
int transmitPort;
int recievePort;
char machineNameReciever[1080];


/*Function Declarations*/
PROCESS readConsoleEntriesAndSendToServer();

PROCESS readBufferAndTransmit();

PROCESS listenPortAndFeedBuffer();

PROCESS readBufferAndPrint();

PROCESS server();

void parseCmdInput(int argc, char **argv,
                   char *machineNameTransmitter,
                   char *machineNameReciever,
                   int *transmitPort,
                   int *recievePort);

// Function to get the current time as a struct tm
struct tm* get_current_time_struct() {
    // Get the current time as seconds since the Epoch
    time_t now_tt = time(NULL);
    if (now_tt == (time_t)-1) {
        perror("time");
        return NULL;
    }

    // Convert the time_t to a struct tm using localtime
    struct tm *tm = localtime(&now_tt);
    if (tm == NULL) {
        perror("localtime");
    }

    return tm;
}

// Function to convert a struct tm into a human-readable string.
// The buffer to hold the string and its size are passed as arguments.
void time_to_string(struct tm* tm, char* buf, size_t buf_size) {
    if (!strftime(buf, buf_size, "%c %Z", tm)) {
        snprintf(buf, buf_size, "strftime failed");
    }
}

/*Function Definitions*/
PROCESS readConsoleEntriesAndSendToServer() {
    printf("Chat Initiated\n");
    MessageBuffer buffer;
    int len;
    int structLen;
    while (1) {
        fgets(buffer.msg, sizeof(buffer.msg), stdin);
        
        struct tm* tmp = get_current_time_struct();
        if(tmp){
            buffer.current_time = *tmp;

        }

        len = strlen(buffer.msg);

        // Strip newline characters from the end
        while (len > 0 && 
        (buffer.msg[len - 1] == '\n' || 
        buffer.msg[len - 1] == '\r')) {
            buffer.msg[len - 1] = '\0'; // Replace with null char
            len--; // Decrease the length
        }

        if (len > 0 && strcmp(buffer.msg, "!xx") != 0) {
            
            structLen = sizeof(buffer);
            Send(serverThreadPid, &buffer, &structLen);
        }

        if (strcmp(buffer.msg, "!xx") == 0 || endproc == 1) {
            break;
        }
    }
    struct tm* tmp = get_current_time_struct();
    strcpy(buffer.msg, "!xx");
    buffer.current_time = *tmp;
    len = sizeof(buffer);
    Send(serverThreadPid, &buffer, &len);
    endproc =1;

}

PROCESS readBufferAndTransmit() {
    int len;
    PID sender;
    int success = 0;
    int returnCode = TERMINATE_CODE;
    struct hostent *server;

    struct sockaddr_in servaddr = {0};
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server = gethostbyname(machineNameReciever);
    if (server == NULL) {
    // Print an error and exit if the name could not be resolved.
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
}

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(transmitPort);
    memcpy((char *)&servaddr.sin_addr.s_addr, 
		    (char *)server->h_addr, server->h_length);
    
    MessageBuffer* buffer;

    while (1) {
        
        buffer = (MessageBuffer *) Receive(&sender, &len);
        Reply(sender, &success, sizeof(success));

        int len = sendto(sockfd, buffer, sizeof(*buffer), 0,
                         (const struct sockaddr *) &servaddr, 
			 sizeof(servaddr));

        if (len < 0) {
            perror("sendto failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if (strcmp(buffer->msg, "!xx") == 0) {
            break;
        }

    }
    endproc = 1;
    close(sockfd);

}

PROCESS listenPortAndFeedBuffer() {
    int sockfd;

    struct sockaddr_in servaddr = {0};

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(recievePort);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int rc = bind(sockfd, (const struct sockaddr *) &servaddr, 
		    sizeof(servaddr));

    if (rc < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int n;

    MessageBuffer buffer;
    while (1) {
        int reply;
        socklen_t len = 0;
        n = -1;
        
        if(endproc == 1)
        {
            break;
        }

        n = recvfrom(sockfd, &buffer, sizeof(MessageBuffer), 
			MSG_DONTWAIT, 0, &len);
        if (strcmp(buffer.msg, "!xx") == 0) {
            close(sockfd);
            endproc = 1;
            break;
        }
        if (n > 0) {
            //strcpy(rcvBufferStruct.msg, buffer.msg);
            n = sizeof(buffer);
            Send(serverThreadPid, &buffer, &n);
        }
        if(reply == TERMINATE_CODE)
        {
            break;
        }
        
    }
    close(sockfd);


}

PROCESS readBufferAndPrint() {
    int len;
    PID sender;
    int success = 0;
    MessageBuffer *buffer;
    char lastMsg[1024] = {0};  // will compare timestamp once implemented
    char curTime[1024] = {0};
    
    while (1) {

        buffer = (MessageBuffer *) Receive(&sender, &len);
        if (!buffer) {
            break; // Handle error
        }
        
        
        Reply(sender, &success, sizeof(success));


        if ((strcmp(lastMsg, buffer->msg) != 0) && (strcmp(buffer->msg, 
					"!xx") != 0)) {
            printf("Client @: %d:%d:%d\n", buffer->current_time.tm_hour,
            buffer->current_time.tm_min, buffer->current_time.tm_sec);
            printf("\t%s\n", buffer->msg);
            strncpy(lastMsg, buffer->msg, sizeof(lastMsg) - 1);  
            lastMsg[sizeof(lastMsg) - 1] = '\0';     
        }
        if (strcmp(buffer->msg, "!xx") == 0) {
            break;
        }
        if(endproc ==1)
        {
            break;
        }
    }

}

PROCESS server(){
   
    sendBufferList = ListCreate();
    rcvBufferList = ListCreate();
    PID sender;
    int len;
    MessageBuffer *item;
    int status;
    int success = 0;
    char prevMsg[1024] = {0};
    char prevMsgSend[1024] = {0};
    while (1)
    {
        MessageBuffer* buffer;
        buffer = (MessageBuffer *) Receive(&sender, &len);
        
        if (sender == consoleToPrintBuffer){
            
            status = ListAppend(sendBufferList, (MessageBuffer *) buffer);
            Reply(sender, &success, sizeof(success));
            strcpy(prevMsg, buffer->msg);

            
            int count = ListCount(sendBufferList);
            if(count >=1)
            {
                ListFirst(sendBufferList);
                len = sizeof(buffer);
                item = (MessageBuffer *) ListRemove(sendBufferList);
                Send(PrintBufferToPort, item, &len);
                
            }

            
        }
        else if (sender == portToReceiveBuffer){
            
                status = ListAppend(rcvBufferList, (MessageBuffer *) buffer);
                if(strcmp(buffer->msg, "!xx") == 0)
                {
                    success = TERMINATE_CODE;
                    Reply(sender, &success, sizeof(success));
                }
                Reply(sender, &success, sizeof(success));
                strcpy(prevMsgSend, buffer->msg);
            
            int countSend = ListCount(rcvBufferList);
            if(countSend >=1)
            {
                ListFirst(rcvBufferList);
                len = sizeof(buffer);
                item = (MessageBuffer *) ListRemove(rcvBufferList);
                Send(receiveBufferToConsole, item, &len);
            }

        }
            
         if(strcmp(buffer->msg, "!xx") == 0)
         {
            break;
         }   
    }
    strcpy(item->msg, "!xx");
    len = sizeof(item);
    Send(receiveBufferToConsole, item, &len);
    Send(PrintBufferToPort, item, &len);
    endproc = 1;
        
}
    


void parseCmdInput(int argc, char **argv,
                   char *machineNameTransmitter,
                   char *machineNameReciever,
                   int *transmitPort,
                   int *recievePort) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <transmit_port> \
			<reciever_machine_name> \
			<receive_port>\n", argv[0]);
        exit(-1);
    }

    *transmitPort = atoi(argv[1]);

    strncpy(machineNameReciever, argv[2], 255);  
    machineNameReciever[255] = '\0'; // Ensure null termination

    *recievePort = atoi(argv[3]);

}

int mainp(int argc, char *argv[]) {
    char machineNameTransmitter[1080];
    


    // Make stdin non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return 1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        return 1;
    }

    gethostname(machineNameTransmitter, sizeof(machineNameTransmitter));
    memmove(machineNameTransmitter, machineNameTransmitter + 5, 
		    strlen(machineNameTransmitter) - 4);


	parseCmdInput(argc, argv, 
					machineNameTransmitter, 
					machineNameReciever,
					&transmitPort, 
					&recievePort);


    serverThreadPid = Create(server, 160000, "server", NULL, HIGH, USR);

    consoleToPrintBuffer = Create(readConsoleEntriesAndSendToServer,
                                  160000, 
				  "readConsoleEntriesAndSendToServer", 
				  NULL, HIGH, USR);
    PrintBufferToPort = Create(readBufferAndTransmit, 160000,
                               "readBufferAndTransmit", NULL, HIGH, USR);
    portToReceiveBuffer = Create(listenPortAndFeedBuffer, 160000,
                                 "listenPortAndFeedBuffer", NULL, HIGH, USR);
    receiveBufferToConsole = Create(readBufferAndPrint, 160000,
                                    "readBufferAndPrint", NULL, HIGH, USR);


}
