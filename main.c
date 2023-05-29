#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    char buffer[BUFFER_SIZE];
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";

    //create a socket
    int sfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sfd == -1){
        perror("webserver (socket)\n");
        return 1;
    }

    //create the address to bind
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = PF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind the socket to the address
    if(bind(sfd, (struct sockaddr *)&host_addr, sizeof(host_addr)) != 0){
        perror("webserver (bind)\n");
        return 1;
    }
   
    //listen to incoming connections
    if(listen(sfd, SOMAXCONN) != 0){
        perror("Webserver (listen)");
        return 1;
    }
    printf("Server listening for connections\n");

    while(1<2){
        int newsfd = accept(sfd, (struct sockaddr*)&host_addr, (socklen_t *)&host_addrlen);
        if(newsfd < 0){
            perror("webserver (accept)");
           continue;
        }
        printf("Connection Accepted\n");
        
        //Read from the socket
        int valread = read(newsfd, buffer, BUFFER_SIZE);
        if(valread < 0){
            perror("webserver (read)");
            continue;
        }
        
        //write to the socket
        int valwrite = write(newsfd, resp, strlen(resp));
        if(valwrite < 0){
            perror("webserver (write)");
            continue;
        }
        close(newsfd);
    }
    return 0;
}
