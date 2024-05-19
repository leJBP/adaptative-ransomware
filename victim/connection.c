/* This code is inspired by this stackoverflow post: 
 * https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response 
 */
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#include "connection.h"

/* Connect to the server and return socket descriptor */
static int connect_to_server(char* p_host, int port) {
    struct hostent *p_server;
    struct sockaddr_in serv_addr;
    int sockfd;

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("[-] ERROR opening socket");
        exit(1);
    }

    /* lookup the ip address */
    p_server = gethostbyname(p_host);
    printf("server: %s\n", p_server->h_name);
    if (p_server == NULL) 
    {
        perror("[-] ERROR, no such host");
        exit(1);
    }

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, p_server->h_addr_list[0], p_server->h_length);

    /* connect the socket */
    int serverfd  = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if (serverfd < 0)
    {
        perror("[-] ERROR connecting to server");
        exit(1);
    }

    printf("[+] Connected to %s:%d\n", p_host, port);

    return sockfd;
}

static void json_body(char* p_identifier, char* p_body) {
    sprintf(p_body, "{\"identifier\": \"%s\"}", p_identifier);
}

static void format_request(char* p_url, char* p_host, char* p_endpoint, char* p_body,  char* p_request) {
    sprintf(p_request, 
        "POST %s%s HTTP/1.0\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %ld\r\n"
        "\r\n"
        "%s",
        p_url, p_endpoint,
        p_host, strlen(p_body), p_body);
}

char* get_encryption_key(char* p_identifier) {

    int sent, bytes, received = 0;

    /* Connect to the server and get the encryption key. */
    int serverfd = connect_to_server(KEY_HOST, KEY_PORT);

    /* send the request */
    char message[MSG_SIZE];
    char response[RESPONSE_SIZE];
    char p_body[strlen(p_identifier) + 20];

    json_body(p_identifier, p_body);
    format_request(API_URL, KEY_HOST, GET_ENC_KEY_ENDPOINT, p_body,  message);

    int total = strlen(message);
    do
    {
        bytes = write(serverfd, message+sent, total-sent);
        if (bytes < 0)
            perror("[-] ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    do
    {
        bytes = read(serverfd, response+received, total-received);
        if (bytes < 0)
            perror("[-] ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    close(serverfd);

    /* process response */
    printf("Response:\n%s\n",response);

    return "encryption_key";
}

char* get_decryption_key(char* p_identifier) {
    // Connect to the server and get the decryption key.
    return "decryption_key";
}