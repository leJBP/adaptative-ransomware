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

#include "server.h"
#include "benchmark.h"

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

    printf("\n[+] Connected to %s:%d\n", p_host, port);

    return sockfd;
}

static char* json_body(char* p_identifier, benchmarkData* p_data, char* p_structKey) {
    char* p_body = NULL;
    p_body = (char*)malloc(250);
    if (p_data == NULL)
    {
        sprintf(p_body, "{\"identifier\": \"%s\"}", p_identifier);
    } else {

        char* p_tmp = (char*)malloc(250);
        /* Header json with identifier value */
        sprintf(p_body, "{\n\"identifier\": \"%s\",\n", p_identifier);

        /* Add the benchmark data */
        sprintf(p_tmp, "\"%s\": {\n\t\"dataSize\": %ld,\n\t", p_structKey, p_data->dataSize);
        strcat(p_body, p_tmp);

        sprintf(p_tmp, "\"cpuCores\": %d,\n\t", p_data->cpuCore);
        strcat(p_body, p_tmp);

        sprintf(p_tmp, "\"cpuMinFreq\": %d,\n\t", p_data->cpuMinFreq);
        strcat(p_body, p_tmp);

        sprintf(p_tmp, "\"cpuMaxFreq\": %d,\n\t", p_data->cpuMaxFreq);
        strcat(p_body, p_tmp);

        sprintf(p_tmp, "\"memorySize\": %d\n\t}\n", p_data->memorySize);
        strcat(p_body, p_tmp);

        strcat(p_body, "}");

        free(p_tmp);
    }

    return p_body;
}

static char* format_request(char* p_url, char* p_host, char* p_endpoint, char* p_body) {
    char* p_request = (char*)malloc(strlen(p_url) + strlen(p_endpoint) + strlen(p_host) + strlen(p_body) + 100);
    sprintf(p_request, 
        "POST %s%s HTTP/1.0\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %ld\r\n"
        "\r\n"
        "%s",
        p_url, p_endpoint,
        p_host, strlen(p_body), p_body);
    return p_request;
}

static char* extract_value(const char* p_json, char* p_key) {
    char *p_keyPos = strstr(p_json, p_key);
    char* p_value = NULL;
    if (p_keyPos) {
        char *start = strchr(p_keyPos, ':');
        if (start) {
            start += 2; // Ignorer ": "
            char *end = strchr(start, '"');
            if (end) {
                size_t length = end - start;
                p_value = (char*)malloc(length + 1);
                strncpy(p_value, start, length);
                p_value[length] = '\0';
            }
        }
    }

    return p_value;
}

static char* extract_json_body(const char* p_response) {
    const char *body_start = strstr(p_response, "\r\n\r\n");
    char* json_body = (char*)malloc(RESPONSE_SIZE);
    if (body_start) {
        body_start += 4; // Ignore "\r\n\r\n"
        strcpy(json_body, body_start);
    } else {
        perror("[-] ERROR extracting json body");
        exit(1);
    }

    return json_body;
}

void save_key(char* p_key, char* p_filename) {
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

static char* contact_server(char* p_identifier, benchmarkData* p_data, char* p_structKey, char* p_endpoint, char* p_api_url, char* p_host, int p_port) {

    int sent = 0;
    int received = 0;
    int bytes = 0;
    int total = 0;

    /* Connect to the server and get the encryption key. */
    int serverfd = connect_to_server(p_host, p_port);
    //printf("Serverfd: %d\n", serverfd);

    /* Send the request */
    char* p_message = NULL;
    char* p_response = NULL;
    char* p_body = NULL;

    p_body = json_body(p_identifier, p_data, p_structKey);
    p_message = format_request(p_api_url, p_host, p_endpoint, p_body);

    //printf("Request:\n%s\n", p_message);

    printf("[+] Sending request to server\n");

     total = strlen(p_message);
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
    p_response = (char*)malloc(RESPONSE_SIZE);
    if (p_response == NULL)
    {
        perror("[-] malloc failed");
        exit(1);
    }

    total = RESPONSE_SIZE;
    do
    {
        bytes = read(serverfd, p_response+received, total-received);
        if (bytes < 0)
            perror("[-] ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if(close(serverfd) < 0)
    {
        perror("[-] ERROR closing socket");
        exit(1);
    }

    /* Save the key to a file */
    //if (p_key_name != NULL)
    //{
    //    save_key(p_key, p_key_name);
    //}

    /* Free memory */
    free(p_message);
    p_message = NULL;
    free(p_body);
    p_body = NULL;

    return p_response; 
}

char* get_encryption_key(char* p_identifier, benchmarkData* p_data, char** p_algo, char** p_iv) {
    char* p_response = contact_server(p_identifier, p_data, "benchmark", GET_ENC_KEY_ENDPOINT, API_URL, KEY_HOST, KEY_PORT);

    char* json_body = extract_json_body(p_response);

    /* Get key value */
    *p_algo = extract_value(p_response, "algorithm");

    char* p_key = extract_value(json_body, "encryptKey");

    if (strcmp(*p_algo, "AES-256") == 0)
    {
        printf("AES\n");
        *p_iv = extract_value(json_body, "iv");
    } else if (strcmp(*p_algo, "CHACHA20") == 0)
    {
        *p_iv = extract_value(json_body, "nonce");
    }

    free(json_body);
    free(p_response);

    return p_key;
}

char* get_decryption_key(char* p_identifier, char** p_algo, char** p_iv) {
    char* p_response = contact_server(p_identifier, NULL, NULL, GET_DEC_KEY_ENDPOINT, API_URL, KEY_HOST, KEY_PORT);

    char* json_body = extract_json_body(p_response);

    /* Get key value */
    *p_algo = extract_value(p_response, "algorithm");

    char* p_key = extract_value(json_body, "decryptKey");

    if (strcmp(*p_algo, "AES") == 0)
    {
        *p_iv = extract_value(json_body, "iv");
    } else if (strcmp(*p_algo, "CHACHA20") == 0)
    {
        *p_iv = extract_value(json_body, "nonce");
    }

    free(json_body);
    free(p_response);

    return p_key;
}
