 
/*  Program to demonstrate the working of ‘SELECTIVE REPEAT PROTOCOL’.
* This module acts as a client which establishes a connection with the server, sends the  
   windowsize, accepts the frames and then sends acknowledgement for each packet wihin    
   the given frame.
* The connection used is UDP and the window size is taken from the user(should be<=40)
* It uses two structures viz. ‘frame’ for accepting the frames send by the server and ‘ack’ for 
   sending the acknowledgement.
* Here the acknowledgement for each packet is accepted from the user. The user  can enter -1 
   for negative acknowledgement or any other integer for positive acknowledgement.
* NOTE: This module can be compiled using the command ‘c++ selectiverepeat_server.cpp –
                o b’ and executed using the command ‘./b’
 -> Always compile and execute the server module first.
*/

                                                             //inculsion.
#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>  //needed for bzero
#include <unistd.h>  //needed for close
#include <time.h>
#include <stdlib.h>
 
#define cls() printf("\n")
                                                 //structure definition for accepting the packets.
struct frame
{
 int packet[10000];
};
                         //structure definition for constructing the acknowledgement frame
struct ack
{
 int acknowledge[10000];
};
 
int main()
{
 //seed random generator
 srand (time(NULL));
 //generator random number between 1 and 1000
 int rnum;

 int clientsocket;
 sockaddr_in serveraddr;
 socklen_t len;
 hostent * server;
 frame f1;
 int windowsize,totalpackets,totalframes,i=0,j=0,framesreceived=0,k,l,m,repacket[10000];
 ack acknowledgement;
 char req[50];
  clientsocket=socket(AF_INET,SOCK_DGRAM,0);
  bzero((char*)&serveraddr,sizeof(serveraddr));
 serveraddr.sin_family=AF_INET;
 serveraddr.sin_port=htons(5018);
 server=gethostbyname("127.0.0.1");
 bcopy((char*)server->h_addr,(char*)&serveraddr.sin_addr.s_addr,sizeof(server->h_addr));
                                                 //establishing the connection.
 printf("\nSending request to the client.\n");
 sendto(clientsocket,"HI I AM CLIENT.",sizeof("HI I AM CLIENT."),0,(sockaddr*)&serveraddr,sizeof(serveraddr));
  printf("\nWaiting for reply.\n");
 recvfrom(clientsocket,req,sizeof(req),0,(sockaddr*)&serveraddr,&len);
 printf("\nThe server has send:\t%s\n",req);
                                                 //accepting window size from the user.
 printf("\nEnter the window size:\t");
 scanf("%d",&windowsize);
                                                //sending the window size.
 printf("\n\nSending the window size.\n");
 sendto(clientsocket,(char*)&windowsize,sizeof(windowsize),0,(sockaddr*)&serveraddr,sizeof(serveraddr));
 cls();
                                                 //collecting details from server.
 printf("\nWaiting for the server response.\n");
 recvfrom(clientsocket,(char*)&totalpackets,sizeof(totalpackets),0,(sockaddr*)&serveraddr,&len);
 printf("\nThe total packets are:\t%d\n",totalpackets);
 sendto(clientsocket,"RECEIVED.",sizeof("RECEIVED."),0,(sockaddr*)&serveraddr,sizeof(serveraddr));
  recvfrom(clientsocket,(char*)&totalframes,sizeof(totalframes),0,(sockaddr*)&serveraddr,&len);
 printf("\nThe total frames/windows are:\t%d\n",totalframes);
 sendto(clientsocket,"RECEIVED.",sizeof("RECEIVED."),0,(sockaddr*)&serveraddr,sizeof(serveraddr));
                                                 //starting the process.
 printf("\nStarting the process of receiving.\n");
 j=0;
 l=0;
 while(l<totalpackets)
 {                                                //initialising the receive buffer.
   printf("\nInitialising the receive buffer.\n");
   printf("\nThe expected frame is %d with packets:  ",framesreceived);
     for(m=0;m<j;m++)
   {                                    //readjusting for packets with negative acknowledgement.
    printf("%d  ",repacket[m]);
   }
   while(j<windowsize && i<totalpackets)
   {
    printf("%d  ",i); 
    i++;
    j++; 
   }
 
   printf("\n\nWaiting for the frame.\n");
                                                //accepting the frame.  
   recvfrom(clientsocket,(char*)&f1,sizeof(f1),0,(sockaddr*)&serveraddr,&len);
   printf("\nReceived frame %d\n\nEnter -1 to send negative acknowledgement for the following packets.\n",framesreceived);
                                                 //constructing the acknowledgement frame.
   j=0;
   m=0;
   k=l;
   while(m<windowsize && k<totalpackets)
   {
    printf("\nPacket: %d\n",f1.packet[m]); 
                                                //accepting acknowledgement from the user.
    //scanf("%d",&acknowledgement.acknowledge[m]);
     rnum = rand() % 1000 + 1;
     acknowledgement.acknowledge[m]=(int)rnum;
     printf("acknowledgement is: %d\n",acknowledgement.acknowledge[m]); 
     if(acknowledgement.acknowledge[m]==1)
    {
     repacket[j]=f1.packet[m];
     j++;
    }
    else
    {
     l++;
    }
    m++;
    k++;
   }
   framesreceived++;
                                                   //sending acknowledgement to the server.
 sendto(clientsocket,(char*)&acknowledgement,sizeof(acknowledgement),0,(sockaddr*)&serveraddr,sizeof(serveraddr));
  cls();
 }
  printf("\nAll frames received successfully.\n\nClosing connection with the server.\n");
 close(clientsocket);
}
