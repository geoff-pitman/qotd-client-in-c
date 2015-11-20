/******************************************************************************
  Author: Geoffrey Pitman
  Created: 10/15/2015
  Due: 10/26/2015
  Course: CSC328 
  Professor: Dr. Frye
  Assignment: #5
  Filename: qotdcli.c
  Purpose: Create a client that will connect to a quote of the day server
            and print the quote of the day
  Compile: gcc qotdcli.c      :  on ACAD (csit)
  Command Line: argv[0] == executable file :  required
                argv[1] == hostname        :  required
				argv[2] == port #          :  optional - defaults to 17
                Ex:  ./client cygnus-x.net 17
                     ./client cygnus-x.net
 *****************************************************************************/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#define SIZE sizeof(struct sockaddr_in)
#define MAXBUF 512

// lookup_IP does a reverse lookup on hostname and sets ip to point at the 
//   ip address string
void lookup_IP(char* hostname, char* ip);

int main(int argc, char* argv[])
{
    struct sockaddr_in server;
    int sockfd, rb, i;
    char buf[MAXBUF], ip[128];
	
	// make sure hostname was passed via command line
	if(!argv[1])
	{ 
       printf("\n\nUsage: %s <hostname>(required) <port>(optional)\n\n", argv[0]);
       return -1;
    } // end if
	
    // reverse lookup hostname and set ip address 
    lookup_IP(argv[1], ip);  

    // assign server attributes
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
		
    if (argv[2])
        server.sin_port = htons(atoi(argv[2]));
    else 
        server.sin_port = htons(17);  

    // set up the transport end point
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("error: socket call failed\n");
        exit(-1);
    } // end if  
   
    // connect the socket to the server's address
    printf("\n\nConnecting to server...\n");
    if (connect(sockfd, (struct sockaddr *)&server, SIZE) == -1)
    {
        perror("error: connect call failed\n");
        exit(-1);
    } // end if
    printf("Connection established...awaiting response...\n");
    
    // receive message and print   
    if ((rb = recv(sockfd, buf, MAXBUF, 0)) == -1)
    {
        perror("error: receive call failed\n");
        exit(-1);
    } // end if
    buf[rb] = 0;
    printf("Received: %s\n", buf);

    // cleanup
    close(server); 

    return 0;
} // end main

// lookup_IP does a reverse lookup on hostname and sets ip to point at the 
//   ip address string
void lookup_IP(char* hostname, char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
	
    if ((he = gethostbyname(hostname)) == NULL)
    {
        herror("error: hostname could not be resolved");
        exit(-1);
    } // end if

    // set ip
    addr_list = (struct in_addr **) he->h_addr_list;
    strcpy(ip, inet_ntoa(*addr_list[0]));
} // end lookup_IP
