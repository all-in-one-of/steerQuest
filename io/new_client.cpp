#include <cstdlib>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include<fstream>
using namespace std;
void error(const char* msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[])
{

	int time, sheep_id, x, y;
	char data[4096];
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
	char *message;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    

bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

FILE* in;
in = fopen("./sim_data.txt", "r");

while (fgets(data, sizeof(data), in) != NULL){
//while ((read = getline(&data, &len, in)) != -1){
printf("Sending: %s\n", data);   

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) 
        error("ERROR opening socket");

if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");    

	
	if( send(sockfd , data, strlen(data),0 )<0){
		cout<<"Send failed";
		return 1;
	}
}

fclose(in);

/*ifstream infile("./sim_data.txt"); 
while (infile >> time >> sheep_id >> x >> y){
	if( send(sockfd , itoa(sheep_id,data,10), strlen(data) , 0) < 0){
		cout<<"Send failed";
		return 1;
	}
}*/

return 0;
}



