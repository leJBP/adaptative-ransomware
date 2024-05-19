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

static char* process_response(char* p_response, char* key) {
    const char *begin_marker = "-----BEGIN PUBLIC KEY-----";

    /* Find start and end pem marker */
    const char *begin = strstr(p_response, begin_marker);
    //printf("Response:\n%s\n",p_response);

    if (begin) {
        /* Compute the length of the key */
        size_t key_length = strlen(begin);
        key = (char*)malloc(key_length + 1);
        /* Copy the key */
        strncpy(key, begin, key_length);
        key[key_length] = '\0';  // End the string
        printf("[+] Public key found in response.\n");
    } else {
        perror("[-] Public key markers not found in response.\n");
        exit(1);
    }
    return key;
}

static void save_key(char* p_key, char* p_filename) {
    FILE *p_f = fopen(p_filename, "w");
    if (p_f == NULL)
    {
        perror("[-] Error opening file.\n");
        exit(1);
    }

    if (fprintf(p_f, "%s", p_key) < 0) {
        perror("[-] ERROR writing to file");
        fclose(p_f);
        exit(1);
    }

    fclose(p_f);
    printf("[+] Key saved to %s\n", p_filename);
}

static void get_key(char* p_identifier, char* p_key_name, char* p_endpoint, char* p_api_url, char* p_host, int p_port) {

    int sent, bytes, received = 0;

    /* Connect to the server and get the encryption key. */
    int serverfd = connect_to_server(p_host, p_port);

    /* Send the request */
    char p_message[MSG_SIZE];
    char p_response[RESPONSE_SIZE];
    char p_body[strlen(p_identifier) + 20];
    char* p_key = NULL;

    json_body(p_identifier, p_body);
    format_request(p_api_url, p_host, p_endpoint, p_body,  p_message);

    int total = strlen(p_message);
    do
    {
        bytes = write(serverfd, p_message+sent, total-sent);
        if (bytes < 0)
            perror("[-] ERROR writing p_message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* Receive the response */
    memset(p_response,0,sizeof(p_response));
    total = sizeof(p_response) - 1;
    do
    {
        bytes = read(serverfd, p_response+received, total-received);
        if (bytes < 0)
            perror("[-] ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    close(serverfd);

    /* Process response */
    p_key = process_response(p_response, p_key);
    printf("Response:\n%s\n", p_key);

    /* Save the key to a file */
    save_key(p_key, p_key_name);

    /* Free memory */
    free(p_key);

}

void get_encryption_key(char* p_identifier) {
    get_key(p_identifier, ENC_KEY_NAME, GET_ENC_KEY_ENDPOINT, API_URL, KEY_HOST, KEY_PORT);
}

char* get_decryption_key(char* p_identifier) {
    // Connect to the server and get the decryption key.
    return "decryption_key";
}