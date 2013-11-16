#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <utility>
#include <string.h>
#include <string>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sendfile.h>
using namespace std;
int main()
{
	string ip;
	string port;
	cout<<"Enter ur IP"<<endl;//clients port and ip
	cin>>ip;
	cout<<"Enter ur Port"<<endl;
	cin>>port;
	int sock = 0;
	char sendBuff[1025]; 
	struct sockaddr_in serv_addr;//creating udp socket
	if((sock = socket(AF_INET, SOCK_DGRAM, 0))< 0)
    {
      perror("\n Error : Could not create socket \n");
      return 1;
    }
    //Connect to a server
    serv_addr.sin_family = AF_INET;
  	serv_addr.sin_port = htons(5001);
  	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
  	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
      printf("\n Error : Connect Failed \n");
      return 1;
    }
    string my_message;
    my_message.append(ip);
    my_message.append(":");
    my_message.append(port);//sending clients info to server
    string operation;
    cout<<"Operation you want to perform"<<endl;
    cin>>operation;
    if(operation.compare("store") == 0)//checking if its retreive /store
    {
      cout<<"Give me the file path"<<endl;
      string filename;
      cin>>filename;
      //calculates md5
      string command = "md5sum ";
      command.append(filename);
      command.append(" | awk '{print$(1)}' >1");
      system(command.c_str());
      string line;
    ifstream myfile ("1");
    if (myfile.is_open())
    {
      getline(myfile,line);
    
      myfile.close();
    }
      cout<<line<<endl;
      my_message.append(":");
      my_message.append(line);//md5sum
      my_message.append(":store");
    if (sendto(sock, my_message.c_str(), my_message.length(), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
      perror("sendto failed");
      return 0; 
     }
     int receiveLen;
     char buf[1024];
     int s = socket(AF_INET,SOCK_DGRAM,0);//creating udp socket for
     struct sockaddr_in correct_serv_addr; /* remote address */
     socklen_t correct_serv_addr_len = sizeof(correct_serv_addr); /* length of addresses */
     struct sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    int porti = atoi(port.c_str());
    myaddr.sin_port = htons(porti);//binding client to its address
    if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
    {
      perror("bind failed"); 
      return 0;
    }
    else
    {
      cout<<"Bind Intialised."<<endl;
    }
    for(;;)
    {  
      cout<<"Waiting for a message from server"<<endl;
      receiveLen = recvfrom(s,buf,1024,0,(struct sockaddr *)&correct_serv_addr, &correct_serv_addr_len);
      if(receiveLen > 0)
      {
          buf[receiveLen] = 0;
          printf("%s\n", buf);int i=0;//send files is received here
          while(buf[i]!=':'){
            i++;
          }
          string cip="";i++;
           while(buf[i]!=':'){
            cip.push_back(buf[i]);
            i++;
          }                           //servers ip and port
          i++;
          string cport="";
          cport.push_back(buf[i]);i++;
          cport.push_back(buf[i]);i++;
          cport.push_back(buf[i]);i++;
          cport.push_back(buf[i]);i++;
          int tcp_socket = socket(AF_INET,SOCK_STREAM,0);//tcp socket for file transfer
           struct sockaddr_in csa;
          memset((char *)&csa, 0, sizeof(csa));
          csa.sin_family = AF_INET;
          csa.sin_addr.s_addr = inet_addr(cip.c_str());
          int cporti = atoi(cport.c_str());
          csa.sin_port = htons(cporti);
          while(true)
          {
            if(connect(tcp_socket,(struct sockaddr *)&csa,sizeof(csa)) < 0)
            {
              perror("connection failed to destination server");
            }
            else
            {
              break;
            }
          }
           /**loop for reading file **/
            //read file and send its info
          ifstream file;
          int l,bsent;
          string mes;
          file.open(filename.c_str(),ios::in|ios::binary|ios::ate);
          if(file.is_open())
          {
                int size = file.tellg();     //retrieve get pointer position
                file.seekg (0, ios::beg);     //position get pointer at the begining of the file
                char* buffer = new char [size];     //initialize the buffer
                file.read (buffer, size);     //read file to buffer
                if((bsent = send(tcp_socket,buffer,size,0)) < 0) 
                {
                     printf("sending failed\n");
                     return 0;
                }
              file.close();                        
            }
        }
    }
  }
  else if(operation.compare("retreive") == 0)
  {
      cout<<"Give me the file md5 sum"<<endl;//getting md5sum from user
      string filename;
      cin>>filename;
      my_message.append(":");
      my_message.append(filename);
      my_message.append(":retrieve");
      if (sendto(sock, my_message.c_str(), my_message.length(), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
      { 
        perror("sendto failed");
        return 0; 
       }
       //tcp socket to retreiving
      int tcp_socket=socket(AF_INET,SOCK_STREAM,0);
       struct sockaddr_in myaddr;
       int rqst;
      memset((char *)&myaddr, 0, sizeof(myaddr));
      myaddr.sin_family = AF_INET;
      myaddr.sin_addr.s_addr = inet_addr(ip.c_str());
      int porti = atoi(port.c_str());
      myaddr.sin_port = htons(porti);
      struct sockaddr_in client_addr1;
      socklen_t slen;
      int nbytes;
      char textBuffer[100000];
      if(tcp_socket < 0)
      {
          perror("Cannot create TCP SOCKET\n");
          return 0;
      }
      if (bind(tcp_socket, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)//binding the client to its ip
      {
        perror("bind failed"); 
        return 0;
      }
      else
      {
        cout<<"Bind Intialised."<<endl;
      }
      if(listen(tcp_socket,5) < 0)
      {
        perror("Listen failed\n");//client in listening mode
        return 0;
      }
      /**loop for accepting and writing to a file **/
      while((rqst = accept(tcp_socket,(struct sockaddr *)&client_addr1,&slen)) <0)
      {
          if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) 
          { 
            perror("accept failed");
            exit(1);
          } 
      }
      /**loop for accepting and writing to a file **/
          //create file and open a file
      ofstream dfile(filename.c_str(),ios::out | ios::binary);
      for(;;)
      {
        nbytes = recv(rqst,textBuffer,100000,0);
        dfile.write(textBuffer,nbytes);
        if(nbytes <= 0)
        {
          break;
        }
      }
      dfile.close();

  }
    return 0;
}


