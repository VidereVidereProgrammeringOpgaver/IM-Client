// Table 25.4			Echo  client program using TCP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
//#include <arpa/innet.h> // inet -> innet !error!
#include <arpa/inet.h>
#include <sys/wait.h>

int main (int argc, char* argv[])		// Three arguments to be checked later
{
	// Declare and define
	int s;						// Socket descriptor
	int  n;						// Number of bytes in each recv call
	char* servName;				// Server name
	int servPort;					// Server port number
	char* string;					// String to be echoed
	int len;						// Length of string to be echoed
	char buffer [256 + 1];			// Buffer
	char* ptr = buffer;    		// Pointer to move along the buffer
	struct sockaddr_in serverAddr;	// Server socket address
	int maxLen = 256; // !ADD!

	// Check and set arguments
	//if (argc != 3)
	if (argc != 4)
	{
		printf ("Error: three arguments are needed!");
		exit (1);
	}
	//servName = arg[1]; // !blad!
	servName = argv[1];
	//servPort = atoi(arg [2]); // !blad!
	servPort = atoi(argv[2]);
	//string = arg [3]; // !blad!
	string = argv[3];

	struct sockaddr_in servAddr; // !ADD!


	// Create socket
    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    /**/
    // Create remote (server) socket address
    memset(&servAddr, '0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(servPort);

    if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // Connect  to the server
    if( connect(s, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
		perror ("Error: connection failed!");
		exit (1);
    }

	printf("connect\n");
	printf("---------\n");
	int dataSend=0;

	int t=18;
	while(t>0)
	{
		t--;

		printf("\n\n-------------------------------\n");

		// Data transfer section
		dataSend = send (s, string, strlen(string), 0);
		//dataSend = send (s, "123", 4, 0);
		if(dataSend==-1)
		{
			printf("dataSend==-1 \n");
			perror("send");
		}


		if(( n = recv( s, buffer, sizeof( buffer ), 0 ) ) <= 0 )
		{
            if( n == 0 )
            {
              printf( "selectserver: socket %d hung up\n", s );
              break;
            }
            else
            {
            	perror("recv");
            }
		}
		else
		{
			printf("Recv %d byte \n", n);
		}


		buffer [n] = '\0';

		printf ("Echoed string received: %s \n", buffer);


		printf ("\n");
		sleep(10); // wait 10 seconds
	}


	// Close socket
	close (s);

	// Stop program
	exit (0);

} // End of echo client program
