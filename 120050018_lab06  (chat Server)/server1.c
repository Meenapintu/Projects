/*  THIS IS A SINGLE CLIENT SOCKET SERVER  */
#include <iostream> 
#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <vector>

using namespace std;

void error(string msg,int t)
{
    perror(msg.c_str());
    exit(t);
}   //error  action function

int main(int argc, char *argv[])
{   int sockfd, newsockfd, portno;							//sockfd and newsockfd are file descriptors  ,portno for port 
    socklen_t addrlen;                                                                 //store the size of  the address of client  , require for accpet  ststem call
    char inputbuf[256];									//read buffer  to read  data from socket conn
    struct sockaddr_in serv_addr;  							//it  contain a n  internet address  (netinet/in.h)
    int n;										//for read write  status check    ,<0 if unsucess 
    if (argc < 2) 									//is   port varifying 
    {   
        error("ERROR, no port provided\n",1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);						//creating new socket   

    if (sockfd < 0) 
        error("ERROR opening socket",1);

    bzero((char *) &serv_addr, sizeof(serv_addr));					//set s all values in  a buffer to zero ,buffer pointer ,and size  are input 
    portno = atoi(argv[1]);								//convering argv[1] to string if   it is string portno =0 else int 
    if(portno==0)error("ERROR ,Port number Error",1);
    serv_addr.sin_family = AF_INET;								//serv addr   family define
    serv_addr.sin_addr.s_addr = INADDR_ANY;							//address (IP)
    serv_addr.sin_port = htons(portno);								//port no 

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 		//binding  the socket with  port and ip
         error("ERROR on binding",2);

    listen(sockfd,1);									//listening socket  for  connection
    addrlen = sizeof(serv_addr);
    sockfd = accept(sockfd, (struct sockaddr *) &serv_addr, &addrlen);			//accepting connection  ,it retrun a file descriptor 
    if (newsockfd < 0) 
        error("ERROR on accept",1);
    while(1)
    {
        bzero(inputbuf,256);
        n = read(sockfd,inputbuf,255);						//read   sockefd file descriptor 

        if (n < 0) error("ERROR reading from socket",1);
        if(!(strlen(inputbuf)==0)){
        string replay;
        bool is_exit=false;
        //cout<<inet_ntoa(serv_addr.sin_addr)<<" : "<<ntohs(serv_addr.sin_port)<<" Send message :"<<inputbuf<<endl; 	//printing some  info ,ip port ,and msg 
        if((inputbuf[0] == 'B' && inputbuf[1] == 'y' && inputbuf[2] == 'e' ))						
        {
            replay="Goodbye ";is_exit=true; 
        }
        else 
        {
            replay="OK "; is_exit=false;
        }
        int port = ntohs(serv_addr.sin_port);                              
        string str_port=  inet_ntoa(serv_addr.sin_addr);        
        replay.append(str_port);
        replay.append(" : ");
        str_port=to_string(port);
        replay.append(str_port);
        size_t tt=replay.size();
        n = write(sockfd,replay.c_str(),tt);						//writing to client   , (server msg sending to client)
        if(is_exit)
        {
            close(sockfd);break;							//closing   file descriptor 
        }
        if (n < 0) error("ERROR writing to socket",1);
	   }
	}
return 0; 
}


//This is a single  client support  socket server  
