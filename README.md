socketprogramming
=================

Socket programming
------------------
Readme:
-------
1. Client creates a UDP socket and binds to a known ip address and port and sends a message to the server with the md5 sum of the file which is to be stored or retrieved. Message type:
“ip_addreess_of_the_client:port:md5:store|retrieve”.
2. Server creates a UDP socket and binds to its ip address and waits for a message from the server or other client. Server receives a message from the client and calculates modulo(md5sum,number of servers in the network). If the destination node is not as same as the node id, it forwards the message from the client to the correct server.
3. If the operation is store, server sends a message to the client to inform the client that it has to send the files to this server. Then server opens a tcp connection and starts listening for the file. The client sends the file to the server.
4. If the operation is retrieve, server opens a tcp connection and and starts sending the file to the client. Client opens a tcp socket and starts listening to the requests.

Files:
------
1. FileMesh.cfg: configuration files for servers. 
2. Client.cpp: program to run on client side. 
3. Node.cpp :program to run on server side.
4. Input.txt :sample input file for client side.

Compiling Instructions:
-----------------------
<code><pre>
$ g++ Client.cpp –o Client
$ ./Client < Input.txt
For Server:
$ g++ Node.cpp –o Node
$ ./Node</pre>
</code>
