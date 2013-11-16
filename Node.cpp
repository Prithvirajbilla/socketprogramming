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
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <iterator>
#define BUFSIZE 2048
using namespace std;
#include "md5.cpp"
//for splitting string into two or more
vector<string>* split(const string &s, char delim, vector<string>* elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems->push_back(item);
    }
    return elems;
}
int main(int argc, char const *argv[])
{
	// read the file
	vector<pair<string,string> > v;
	//filename = FileMesh.cfg
	string line;
	ifstream cfgfile ("FileMesh.cfg");
 	char buf[BUFSIZE]; /* receive buffer */
	if(cfgfile.is_open())
	{
	    while (getline(cfgfile,line))
	    {
	    	size_t found = line.find(" ");
	    	//inserting the string into vector
	      	v.push_back(pair<string,string>(line.substr(0,found),line.substr(found+1,line.length()-found)));
	    }
	    cfgfile.close();
	}
	else
	{
		cout<<"Unable to open the File\n";
		return 0;
	}
	int node_id;
	cout<<"Enter the Node Id"<<endl;
	cin>>node_id;

	//create a socket
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	// domain=IP, type = datagram and protocol =0

	if(s < 0)
	{
		cout<<"Error creating a socket."<<endl;
		return 0;
	}
	else
	{
		cout<<"Socket Created"<<endl;
	}
		// Binding to the socket
	struct sockaddr_in myaddr;
	//client address
	string ip_port = v[node_id].first;

	size_t del = ip_port.find(":");
	//the ip_addr of the machine
	string ip_addr = ip_port.substr(0,del);
	//the port of the machine
	int port = atoi((ip_port.substr(del+1,ip_port.length()-del)).c_str());

	//initailsing struct the sockaddr_in
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	myaddr.sin_port = htons(port);

	if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("bind failed"); 
		return 0;
	}
	else
	{
		cout<<"Bind Intialised."<<endl;
	}
//
	 struct sockaddr_in remaddr; /* remote address */
	 socklen_t addrlen = sizeof(remaddr); /* length of addresses */
	 int recvlen; 
 for (;;) 
 { 
 	printf("waiting on port %d\n", port);
 	recvlen = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
  	if (recvlen > 0)
  	{ 
  		buf[recvlen] = 0;
  		printf("%s\n", buf);
	  	int i=0;
	  	string sip="";
	  	string sport="";
	  	string smd5="";
	  	string stype="";
	  	while(buf[i] != ':')
	  	{
	  		sip.push_back(buf[i]);i++;//clients ip
	  	}i++;
	  	while(buf[i] != ':')
	  	{
	  		sport.push_back(buf[i]);i++;//clients port
	  	}i++;
	  	while(buf[i] != ':')
	  	{
	  		smd5.push_back(buf[i]);i++;//md5sum valuse
	  	}i++;int l=i;
	  	while(buf[i] != 0)
	  	{
	  		stype.push_back(buf[i]);i++;//store(or)retreive
	  	}
	  	//cout<<sip<<" "<<sport<<" "<<smd5<<" "<<stype<<endl;
	  	int dest_id = md5(smd5,v.size());
	  	//cout<<"destination id"<<dest_id<<" "<<node_id<<endl;
	  	if(dest_id == node_id)//if the present node is the destined node
	  	{	
	  		system(("mkdir -p "+ v[node_id].second).c_str());//create folder 0
	  		int sock = socket(AF_INET, SOCK_DGRAM, 0);
	  		struct sockaddr_in client_addr;
	 		memset((char *)&client_addr, 0, sizeof(client_addr));
	  		client_addr.sin_family = AF_INET;
			client_addr.sin_addr.s_addr = inet_addr(sip.c_str());
			int sporti = atoi(sport.c_str());
			client_addr.sin_port = htons(sporti);//creating udp socket
			cout<<"Destination server is one"<<endl;
			if(connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr))<0)
	    	{
	      		printf("\n Error : Connect Failed \n");
	      		return 1;
	   		 }
	   		 if(buf[l]=='s')
	   		 {
			   		string mmm = "send files:"+v[node_id].first;//through the udp socket created signaling client to send message

			  		if (sendto(sock, mmm.c_str(), mmm.length(), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) 
				  	{ 
						perror("sendto failed");
						return 0; 
				    }
				    close(s);
				    //creating a TCP SOCKET

				    int tcp_socket = socket(AF_INET,SOCK_STREAM,0);
				    if(tcp_socket < 0)
				    {
				    	perror("Cannot create TCP SOCKET\n");
				    	return 0;
				    }
					if (bind(tcp_socket, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
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
						perror("Listen failed\n");
						return 0;
					}
					/**loop for accepting and writing to a file **/
					//create file and open a file
					ofstream outputfile(smd5.c_str());
					int rqst;
					struct sockaddr_in client_addr1; 
					char textBuffer[100000];
					socklen_t slen;
					int nbytes;
					
					while((rqst = accept(tcp_socket,(struct sockaddr *)&client_addr1,&slen)) <0)
					{
							if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) 
							{ 
								perror("accept failed");
								exit(1);
							} 
					}
					cout<<"Request accepted"<<endl;
					ofstream dfile((v[node_id].second+"/"+smd5).c_str(), ios::out | ios::binary);
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
				else
				{
					//creating a TCP SOCKET for retreiving
					cout<<"Destination server is one"<<endl;
				    int tcp_socket = socket(AF_INET,SOCK_STREAM,0);
				    struct sockaddr_in csa;
			          memset((char *)&csa, 0, sizeof(csa));
			          csa.sin_family = AF_INET;
			          csa.sin_addr.s_addr = inet_addr(sip.c_str());
			          int sporti = atoi(sport.c_str());
			          csa.sin_port = htons(sporti);

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
			          file.open((v[node_id].second+"/"+smd5).c_str(),ios::in|ios::binary|ios::ate);
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
	  	else if(dest_id != node_id)
	  	{
	  		cout<<"Destination Server is different from the present server"<<endl;
	  		string dest_ip_port = v[dest_id].first;

			size_t deli = dest_ip_port.find(":");
			//the ip_addr of the machine
			string dest_ip_addr = dest_ip_port.substr(0,deli);
			//the port of the machine
			int dest_port = atoi((dest_ip_port.substr(deli+1,dest_ip_port.length()-deli)).c_str());

	  		int sock = socket(AF_INET,SOCK_DGRAM,0);
	  		struct sockaddr_in dest_addr;
	 		memset((char *)&dest_addr, 0, sizeof(dest_addr));
	  		dest_addr.sin_family = AF_INET;
			dest_addr.sin_addr.s_addr = inet_addr(sip.c_str());
			dest_addr.sin_port = htons(dest_port);
			if(connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr))<0)
	    	{
	      		printf("\n Error : Connect Failed \n");
	      		return 1;
	   		 }
	  		if (sendto(sock, buf, recvlen, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) 
		  	{ 
				perror("sendto failed");
				return 0; 
		    }
		    //closing the udp socket

	  	}
	 }
  }

  return 0;

}