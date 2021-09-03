#include <iostream>
#include <cstdlib>
#include <vector>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>
#define MAX_LEN 512

using namespace std;

bool exit_flag = false;
thread t_send, t_recv;
int client_socket;


void send_message(int client_socket);
void recv_message(int client_socket);
int eraseText(int count);
void catch_exit(int signal); // user want to exit 

int main()
{
	char buffer[MAX_LEN],name1[MAX_LEN];
	const char* s = " ";
	char *command, *ip, *port, *name;
	int port1;
	
	if((client_socket=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket: ");
		exit(1);
	}
	//initial the info for connect
	cout << "Input ------>connect ip  port  UserName:" << endl;
	//fgets函式從stdin流中讀取N-1個字元放入buffer中
	fgets(buffer,MAX_LEN,stdin);
	//fgets函式讀取的最後一個字元為\n，此處將其替換為'\0'
	buffer[strlen(buffer) - 1] = '\0';
	command = strtok(buffer,s);
	ip = strtok(NULL,s);
	port = strtok(NULL,s);
	name = strtok(NULL,s);
	port1 = atoi(port);
///////////////////////////////////////////////////////
	if(strcmp(command,"connect") == 0)
	{
		struct sockaddr_in client;
		bzero(&client, sizeof(client));
		client.sin_family = AF_INET; //AF_INET代表TCP／IP協議
		client.sin_port = htons(port1); // Port no. of server
		client.sin_addr.s_addr = inet_addr(ip); // Provide IP address of server

		bzero(&client.sin_zero,0);

		if((connect(client_socket,(struct sockaddr *) &client,sizeof(client))) < 0)
		{
			perror("connect: ");
			exit(1);
		}
	
		signal(SIGINT, catch_exit);
		send(client_socket,name,sizeof(name),0);//send input name to server side
	
		//bzero(buffer,MAX_LEN);
		cout<<"\n\t  ====== Welcome to the chat-room ======   "<<endl;
		cout << "Input \"name\" or \"chatroom\" by which you wanna send msg to." <<endl;
		fgets(name1,MAX_LEN,stdin);
		send(client_socket,name1,sizeof(name1),0);
	
		///////////////////// Thread create ////////////////
		thread t1(send_message, client_socket);
		thread t2(recv_message, client_socket);

		t_send=move(t1);
		t_recv=move(t2);

		if(t_send.joinable())
			t_send.join();
		if(t_recv.joinable())
			t_recv.join();
		}			
	return 0;
}

// Use "Ctrl + C" to exit(if "Ctrl + C", then kill thread)
void catch_exit(int signal) 
{
	char str[MAX_LEN] = "Exit";
	send(client_socket,str,sizeof(str),0);
	exit_flag = true;
	t_send.detach();
	t_recv.detach();
	close(client_socket);
	exit(signal);
}


// Erase text from terminal
int eraseText(int count)
{
	char back_space = 8;
	for(int i = 0; i < count; i++)
	{
		cout << back_space;
	}	
}


// Send message to everyone
void send_message(int client_socket)
{
	while(1)
	{
		cout << "You : ";
		char str[MAX_LEN];
		fgets(str,MAX_LEN,stdin);
		str[strlen(str) - 1] = '\0';
		send(client_socket,str,sizeof(str),0);
		if(strcmp(str,"bye") == 0)
		{
			exit_flag=true;
			t_recv.detach();	
			close(client_socket);
			return;
		}	
	}		
}

// Receive message
void recv_message(int client_socket)
{
	while(1)
	{
		char name[MAX_LEN], str[MAX_LEN];
		int bytes = recv(client_socket,name,sizeof(name),0); // bytes = msg from server
		if(exit_flag)
			return;
		
		if(bytes < 0)
			continue;
		recv(client_socket,str,sizeof(str),0);
		eraseText(6);
		if(strcmp(name,"#NULL") != 0)
			cout << name << " : " << str  <<endl;
		else if(strcmp(name,"not exist") == 0)
			cout << "Not Exist" << endl;
		else
			cout << str << endl;
		cout <<  "You : ";
		fflush(stdout);
	}	
}


