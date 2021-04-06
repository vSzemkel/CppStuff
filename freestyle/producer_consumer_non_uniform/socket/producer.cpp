#include <pthread.h>
#include "common.h"

int g_msg_count;
magazine_t g_magazine;

static int sock_init()
{
    const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        ERREXIT("socket");

    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
        ERREXIT("bind");

    if (listen(sockfd, 1) != 0)
        ERREXIT("listen");

    return sockfd;
}

static void* produce(void* parm)
{
    auto& data = *((std::ifstream*)parm);
    for (int i = 0; i < g_msg_count; ++i) {
        int kind, value; 
        std::string text;
        data >> kind >> value >> text;
        const auto mk = kind == 1 ? msg_type_t::msg1 : msg_type_t::msg2;

        msg_t msg{mk, value, text.c_str()};
        g_magazine.put(std::move(msg));
        DBGPRN("Message [%i]: %s of size %i", i, msg.text, msg.size);

        data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return NULL;
}

static void deliver(const int sfd)
{
    char msgbuf[sizeof(msg_t)];
    auto msg = (msg_t*)msgbuf;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int cli_fd = accept(sfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (cli_fd <= 0)
        ERREXIT("accept");
    for (int i = 0; i < g_msg_count; ++i) {
        g_magazine.get(msgbuf);
        write(cli_fd, msg, msg->size);
    }
    close(cli_fd);
}

int main(int, char**)
{
    // read messages count
    std::ifstream data("../test_input.dat");
    if (!data.is_open())
        ERREXIT("opening test_input.dat");
    data >> g_msg_count;
    // produce messages into cyclic memory buffer
    pthread_t child;
    pthread_create(&child, NULL, produce, (void*)&data);
    // message delivery loop
    auto sfd = sock_init();
    deliver(sfd);
    // clean
    pthread_join(child, NULL);
    close(sfd);
}

// g++ -Wall -Wextra -ggdb3 -std=c++17 -fsanitize=address producer.cpp -lrt -pthread -o sock_producer