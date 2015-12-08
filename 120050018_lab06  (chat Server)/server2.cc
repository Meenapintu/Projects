/**  THIS IS A MULTI CLIENT SOCKET SERVER*/
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
#define TRUE   1
#define FALSE  0

using namespace std;

void error(string msg ,int t)						//error handling  function 
{
	perror(msg.c_str());
    exit(t);
}

struct socket_data							//struct to store socket info  ( ip and port no with socket (file descriptor)
{
	int sockfd;
	int port_no;
	string ip_addr;
};

socket_data find_me(int  fd,vector<socket_data> socks)			//retrun the other info  port no and ip address of a socket 
{
	for (int j=0;j<socks.size();++j)
	{
		if(socks[j].sockfd ==fd)
			return socks[j];
	}
}

string list_me(vector<socket_data> socks, int port)			//list  all cleint 's ip address and  port number in a string 
{
	string fin=" ";
	for(int j=0;j<socks.size();++j)
	{
		if(socks[j].port_no != port)
    		fin=fin+","+socks[j].ip_addr+" : "+to_string(socks[j].port_no);
    }
    return fin;
}

void remove_me(vector<int> &client_socket, int val)		//remove the file descriptor from client_socket vector  and close this socket 
{
	for(int i=0 ;i<client_socket.size();i++)
	{
		if(client_socket[i]==val)
		{
			close(client_socket[i]);client_socket.erase(client_socket.begin()+i);
		}
    }
}

void delete_me(vector<socket_data> &socks, int sockfd)      //delete entry of sockfd  ,given socket  from info entry vectro (socks)
{
	for(int i=0;i<socks.size();i++)
	{
		if(socks[i].sockfd == sockfd)
		{
			socks.erase(socks.begin()+i);
		}
	}
}

int main(int argc , char *argv[])
{
    int tf = TRUE;
    int sockfd , sd, newsockfd , event, i , n ,sockfd_temp;		// file descriptor ,read write (n) varfying  variable etc.
    std::vector<int> client_socket;					//vector of current client's socket 
    vector<socket_data> socks;						//info related to sockets 
    int max_sd;
	socklen_t  addrlen;
    struct sockaddr_in serv_addr;
    char inputbuf[1025];
    fd_set readfds;

    if (argc < 2) 
    {
    	error("ERROR, no port provided\n",1);
    }

    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == 0) 		//file descriptor creating 
    {
    	error("ERROR opening socket",1);
    }

    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&tf, sizeof(tf)) < 0 )		//manipulate  options for the socket reffered by sockfd 
    {
        error("ERROR in setsockopt",1);  
    }

    int PORT = atoi(argv[1]);
    if(PORT==0)error("ERROR ,Port number Error",1);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( PORT );

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
    {
        error("ERROR on binding",2);
    }

    if (listen(sockfd, 3) < 0)
    {
        error("listen",1);
    }

    addrlen = sizeof(serv_addr);

    while(TRUE) 
    {
        FD_ZERO(&readfds);								//nitializes the file descriptor set fdset to have zero bits for all file descriptor
        FD_SET(sockfd, &readfds);							//Sets the bit for the file descriptor fd in the file descriptor set fdset
        max_sd = sockfd;

        for ( i = 0 ; i < client_socket.size() ; i++) 
        {
        	sockfd_temp = client_socket[i];

            if(sockfd_temp > 0)
            	FD_SET(sockfd_temp , &readfds);
             
            if(sockfd_temp > max_sd)
                max_sd =sockfd_temp;
        }

        event = select( max_sd + 1 , &readfds , NULL , NULL , NULL);			//wait for an event  ,
    
        if ((event < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

        if (FD_ISSET(sockfd, &readfds)) 			//event on sockfd then true , it is an incoming connection
        {
            if ((newsockfd = accept(sockfd, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen))<0)	//accept connection
            {
                error("ERROR on accept",1);   
            }
            else
            {
				//printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , newsockfd , inet_ntoa(serv_addr.sin_addr) , ntohs(serv_addr.sin_port)); 
//some info related to new connection
                    client_socket.push_back(newsockfd);					//list all   new acceted connection to client_socket vector 
                    socket_data temp;
           			temp.sockfd = newsockfd;
            		temp.port_no =ntohs(serv_addr.sin_port);
            		temp.ip_addr =inet_ntoa(serv_addr.sin_addr);
            		socks.push_back(temp);						//now store the socket info to socks  vector 
                }
        }
		
		for (i = 0; i < client_socket.size(); i++)				//if there is IO operation on any  accepted  socket  
		{
        	sockfd_temp = client_socket[i];						// point socket of vector to in a tem sockfd  
        	if (FD_ISSET(sockfd_temp , &readfds))  
        	{
			 	bzero(inputbuf,256);
			    n = read(sockfd_temp,inputbuf,255);				//reding  

			    if (n < 0) error("ERROR reading from socket",1);

			    if(!(strlen(inputbuf)==0))					//check is client socket  closing  ,
			    {
			    	socket_data data_temp= find_me(sockfd_temp,socks);	//find  info of pointing socket in socks to reterive it 
					string replay;
					bool is_exit=false;				//this var  is true if socket  need to close 

					//cout<<data_temp.ip_addr<<" : "<<data_temp.port_no<<" Send message :"<<inputbuf<<endl;		//some info print ,who and cleint  send msg 

			    	if((inputbuf[0] == 'B' && inputbuf[1] == 'y' && inputbuf[2] == 'e' ))		//if msg is Bye then it's action  prepare check
			    	{
						replay="Goodbye ";is_exit=true; 
					}

					else
					{
						replay="OK "; is_exit=false;
					}
			    	int port = data_temp.port_no;								
			    	string str_port=  data_temp.ip_addr; 
					replay.append(str_port);
					replay.append(" : ");
			    	str_port=to_string(port);
					replay.append(str_port);

					if((inputbuf[0] == 'L') && (inputbuf[1] == 'i') && (inputbuf[2] == 's') && (inputbuf[3] == 't'))	//client msg filter what does he want 
					{
						replay.append(","+list_me(socks,port));		//send the list of all client's address  so listing them in  string
					}
					size_t tt=replay.size();				//get the size of replay  back msg 
				    n = write(sockfd_temp,replay.c_str(),tt);
				    if (n < 0) error("ERROR writing to socket",1);

				    if(is_exit)
				    {

				    	close(sockfd_temp);				//close socket 
				    	remove_me(client_socket,sockfd_temp);		//remove socket entry from socket's vector 
				    	delete_me(socks,sockfd_temp);			// delete  this .socket  info from info  vector socks 

				    }
			    
	        	}
        	}
        }
    } 
return 0;
} 
