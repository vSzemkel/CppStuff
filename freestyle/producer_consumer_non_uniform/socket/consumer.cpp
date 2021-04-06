#include "common.h"

static int sock_init()
{
    const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        ERREXIT("socket");

    struct hostent* server = gethostbyname(server_name);
    if (server == NULL)
        ERREXIT("gethostbyname %s", server_name);

    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        ERREXIT("connect");

    return sockfd;
}

static void consume(const int sfd)
{
    int counter{0};
    char msgbuf[sizeof(msg_t)];
    auto msg = (msg_t*)msgbuf;
    while (true) {
        ssize_t n = recv(sfd, msgbuf, sizeof(msg_t), 0);
        if (n <= 0) break;
        printf("Message [%i]: %i %s %i\n", counter++, msg->size, msg->text, msg->kind == msg_type_t::msg1 ? msg->u8 : msg->u32);
    }
}

int main(int, char**)
{
    const int sfd = sock_init();
    consume(sfd);
    close(sfd);
}

// g++ -Wall -Wextra -ggdb3 -std=c++17 -fsanitize=address consumer.cpp -lrt -o sock_consumer