#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr
#include <unistd.h>
#include <arpa/inet.h>
struct Location {
   char name[40];  
   double lon;
   double lat;
  
};
 
// Function designed for chat between client and server.
char* getNthLineFromFile (int lineNumber){


 const char filename[] = "Locations.txt";
FILE *file = fopen(filename, "r");
int count = 0;
if ( file != NULL )
{
    char line[256]; /* or other suitable maximum line size */
   char* buffer;
    while (fgets(line, sizeof line, file) != NULL) /* read a line */
    {
        if (count == lineNumber -1)
        {
            buffer =(char*)malloc(sizeof(char) * strlen(line));
	    strcpy(buffer, line);
	    return buffer;
        }
        else
        {
            count++;
        }
    }
    fclose(file);
}
else
{
    //file doesn't exist
}
	
}

struct Location createLocation (char* line){


	struct Location location;
	char buff[100];
	strcpy(buff, line);
	int init_size = strlen(buff);
	char delim[] = " ";

	char *ptr = strtok(buff, delim);
	strcpy(location.name, ptr);
	
	double lon;
	ptr = strtok(NULL, delim);
	sscanf(ptr, "%lf", &lon);
	location.lon = lon;

	double lat;
	ptr = strtok(NULL, delim);
	sscanf(ptr, "%lf", &lat);
	location.lat = lat;


return location;
	



}

void func(int sockfd)
{
    char buff[MAX];
    int n;
    char option;

    // infinite loop for chat
    for (;;)
    {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
	
	if (strcmp (buff, "a\n") == 0){
		bzero(buff, MAX);
		strcpy(buff, "Prima optiune a fost aleasa\n Alegeti locatia din lista pentru care doriti prognoza meteo: ");
		option = 'a';
		
	}
	else if (strcmp (buff, "b\n") == 0){
		printf ("Otiunea b a fost aleasa\n Alegeti locatia din lista pentru care doriti raportul detaliat: ");
		option = 'b';
	}

	else{
	//receives a number rerpesenting the desired location	
	// then will get desired location from from file
	}



        printf("From client: %s\t To client : ", buff);
        //bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        //while ((buff[n++] = getchar()) != '\n')
          //  ;

        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.t
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);

 //printf ("%s", getNthLineFromFile(1));
 //struct Location location = createLocation("salut 45353.43 432.42342");
//printf ("%lf", location.lat);
}

