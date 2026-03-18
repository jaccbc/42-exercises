#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

struct client {
    int id;
    char msg[200000];
};

struct client clt[1024] = {0};
fd_set fds, read_fds, write_fds;
int max = 0, next = 0;
char readBuffer[300000] = {0}, writeBuffer[300000] = {0};

static void closeServer() {
    write(STDERR_FILENO, "Fatal error\n", 12);
    exit(1);
}

static int initServer(int port) {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        closeServer();
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 16777343;
    addr.sin_port = ((port & 0xFF) << 8 | port >> 8);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 || listen(fd, SOMAXCONN) < 0)
        closeServer();
    return fd;
}

static void broadcast(int sender) {
    for (int fd = 0; fd <= max; fd++) {
        if (FD_ISSET(fd, &write_fds) && fd != sender)
            send(fd, writeBuffer, strlen(writeBuffer), 0);
    }
}

static void handleNewConnection(int fd) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int client = accept(fd, (struct sockaddr*)&addr, &len);
    if (client < 0)
        closeServer();
    if (client > max)
        max = client;
    clt[client].id = next++;
    FD_SET(client, &fds);
    sprintf(writeBuffer, "server: client %d just arrived\n", clt[client].id);
    broadcast(client);
}

static void disconnect(int fd) {
    sprintf(writeBuffer, "server: client %d just left\n", clt[fd].id);
    broadcast(fd);
    FD_CLR(fd, &fds);
    close(fd);
    clt[fd].msg[0] = '\0';
}

static void handleMessage(int fd) {
    int r = recv(fd, readBuffer, sizeof(readBuffer), 0);
    if (r <= 0) {
        disconnect(fd);
        return ;
    }
    int len = strlen(clt[fd].msg);
    for (int i = 0; i < r; i++,  len++) {
        clt[fd].msg[len] = readBuffer[i];
        if (clt[fd].msg[len] == '\n') {
            clt[fd].msg[len] = '\0';
            sprintf(writeBuffer, "client %d: %s\n", clt[fd].id, clt[fd].msg);
            broadcast(fd);
            clt[fd].msg[0] = '\0';
            len = -1;
        }
    }
    clt[fd].msg[len] = '\0';
}

int main(int argc, char** argv) {
    if (argc != 2) {
        write(STDERR_FILENO, "Wrong number of arguments\n", 26);
        exit(1);
    }
    int server_port = atoi(argv[1]);
    if (server_port <= 0) closeServer();
    int server_socket = initServer(server_port);
    max = server_socket;
    FD_ZERO(&fds);
    FD_SET(server_socket, &fds);
    while (1) {
        read_fds = write_fds = fds;
        if (select(max+1, &read_fds, &write_fds, NULL, NULL) < 0)
            continue;
        for (int fd = 0; fd <= max; fd++) {
            if (!FD_ISSET(fd, &read_fds))
                continue;
            if (server_socket == fd)
                handleNewConnection(fd);
            else
                handleMessage(fd);
        }
    }
    return 0;
}