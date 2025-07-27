#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>
#include <sys/socket.h>

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void testWriteBehavior(const char* mode, int fd) {
    const size_t dataSize = 1024 * 1024; // 1MB
    char* buffer = new char[dataSize];
    memset(buffer, 'A', dataSize);

    size_t totalWritten = 0;
    while (true) {
        ssize_t written = write(fd, buffer + totalWritten, dataSize - totalWritten);
        if (written > 0) {
            totalWritten += written;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "[" << mode << "] Write would block after " << totalWritten << " bytes.\n";
            } else {
                std::cerr << "[" << mode << "] Write error: " << strerror(errno) << "\n";
            }
            break;
        }
    }

    delete[] buffer;
}

int main() {
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1) {
        perror("socketpair");
        return 1;
    }

    // Blocking test (default)
    testWriteBehavior("Blocking", fds[0]);

    // Reset the pair (create fresh sockets again)
    close(fds[0]);
    close(fds[1]);
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1) {
        perror("socketpair");
        return 1;
    }

    // Set non-blocking mode
    setNonBlocking(fds[0]);

    // Non-blocking test
    testWriteBehavior("Non-blocking", fds[0]);

    close(fds[0]);
    close(fds[1]);
    return 0;
}
