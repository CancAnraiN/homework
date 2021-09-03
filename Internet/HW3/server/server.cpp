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

struct info
{
	int id;
	string name;
	int socket;
	char *ip;
	thread th;
};

vector<info> clients;
int counter = 0;
mutex cout_mtx,clients_mtx;

void set_name(int id, char name[]);
bool bool_name(string name);
void shared_print(string str);
int broadcast_msg(string msg, int sender_id);
int broadcast_msg(int num, int sender_id);
int broadcast_msg(string msg, string name, int sender_id);
int broadcast_msg(int num, string name, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);

int main()
{
	int server_socket;
	if((server_socket=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket: ");
		exit(1);
	}

	struct sockaddr_in server;
	socklen_t len = sizeof(sockaddr_in);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(8989);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");

	if((bind(server_socket,(struct sockaddr *)&server,len)) < 0)
	{
		perror("bind error: ");
		exit(1);
	}

	if((listen(server_socket,8)) < 0)
	{
		perror("listen error: ");
		exit(1);
	}

	struct sockaddr_in client;
	int client_socket;

	cout << "\n\t  ====== Welcome to the chat-room ======   " << endl;

	while(1)
	{
		if((client_socket = accept(server_socket,(struct sockaddr *)&client,&len)) < 0)
		{
			perror("accept error: ");
			exit(1);
		}
		counter++;
		thread t(handle_client,client_socket,counter);
		lock_guard<mutex> guard(clients_mtx);
		clients.push_back({counter, string("None"),client_socket,inet_ntoa(client.sin_addr),(move(t))});
	}

	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
	return 0;
}


// Set name of client
void set_name(int id, int index, char name[])
{
	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				index = i;
				clients[i].name=string(name);
			}
	}	
}
// find name
bool bool_name(string name)
{
	for(int i=0; i < clients.size(); i++)
	{
			if(clients[i].name == name)	
				return true;
	}
	return false;	
}

// For synchronisation of cout statements
void shared_print(string str)
{	
	lock_guard<mutex> guard(cout_mtx);
	cout << str << endl;

}

// Broadcast message to all clients except the sender
int broadcast_msg(string msg, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,msg.c_str());
	for(int i=0; i < clients.size(); i++)
	{
		if(clients[i].id != sender_id)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}		
}

// Broadcast a number to all clients except the sender
int broadcast_msg(int num, int sender_id)
{
	for(int i=0; i < clients.size(); i++)
	{
		if(clients[i].id != sender_id)
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}		
}

int broadcast_msg(string msg, string name, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,msg.c_str());
	for(int i=0; i < clients.size(); i++)
	{
		if(clients[i].name == name)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
			return 1;
		}
	}		
}

int broadcast_msg(int num, string name,int sender_id)
{
	for(int i=0; i < clients.size(); i++)
	{
		if(clients[i].name == name)
		{
			send(clients[i].socket,&num,sizeof(num),0);
			return 1;
		}
	}		
}

void end_connect(int id)
{
	for(int i = 0; i < clients.size(); i++)
	{
		if(clients[i].id==id)	
		{
			lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach(); // kill threads
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}

void handle_client(int client_socket, int id)
{
	int index;
	char name[MAX_LEN],str[MAX_LEN],command[MAX_LEN];
	recv(client_socket,name,sizeof(name),0);
	set_name(id,index,name);
	
	recv(client_socket,command,sizeof(command),0);
	command[strlen(command) - 1] = '\0';
	//cout <<command << endl;	

	// Display welcome message
	string welcome_message = string("User ") + string(name) + string(" is on-line, IP address: ") + string(clients[index].ip) + string(">");
	broadcast_msg("#NULL",id);									
	broadcast_msg(welcome_message,id);	
	shared_print(welcome_message);
	
	while(1)
	{
		int bytes_received = recv(client_socket,str,sizeof(str),0);
		if(bytes_received <= 0)
			return;
		if(strcmp(str,"bye") == 0)
		{
			string leave_message = string(name) + string(" has left");		
			broadcast_msg("#NULL",id);								
			broadcast_msg(leave_message,id);
			shared_print(leave_message);
			end_connect(id);							
			return;
		}
		else if(strcmp(command,"chatroom") == 0) //all
		{
			string msg = string(name) + " : " + string(str);
			broadcast_msg(string(name),id);							
			broadcast_msg(string(str),id);
			shared_print(msg);	
		} 
		else if(bool_name(command)) // specific name
		{
			string msg = string(name) + " send to " + string(command) + " : " + string(str);
			broadcast_msg(string(name),string(command),id);							
			broadcast_msg(string(str),string(command),id);
			shared_print(msg);	
		}
		else
		{
			broadcast_msg(string(name),clients[index].name,id);							
			broadcast_msg("not exist",clients[index].name,id);
		}		
	}	
}



