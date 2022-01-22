#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE (128)

void send_msg(int tx, char const *str);

int child_main(int rx, int tx) {
    while (true) {
        char buffer[BUFFER_SIZE];
        ssize_t ret = read(rx, buffer, BUFFER_SIZE - 1);

        if (ret == 0) {
            // ret == 0 signals EOF

            fprintf(stderr, "[INFO]: parent closed the pipe\n");
            break;
        } else if (ret == -1) {
            // ret == -1 signals error
            fprintf(stderr, "[ERR]: read failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "[INFO]: parent sent %zd B\n", ret);
        buffer[ret] = 0;
        fputs(buffer, stdout);
        send_msg(tx, "GAWK");
    }
    close(rx);
    return 0;
}

// helper function to send messages
// retries to send whatever was not sent in the begginning
void send_msg(int tx, char const *str) {
    size_t len = strlen(str);
    size_t written = 0;

    while (written < len) {
        ssize_t ret = write(tx, str + written, len - written);
        if (ret < 0) {
            fprintf(stderr, "[ERR]: write failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        written += ret;
    }
}

int parent_main(int tx, int rx) {
    // The parent is likes classic rock:
    // https://www.youtube.com/watch?v=lrpXArn3hII

    char buffer[BUFFER_SIZE];
    ssize_t ret;

    send_msg(tx, "Hey little girl\n");
    sleep(1);
    ret = read(rx, buffer, BUFFER_SIZE - 1);
    buffer[ret] = 0;
    fputs(buffer, stderr);

    send_msg(tx, "Is your daddy home?\n");
    sleep(1);
    ret = read(rx, buffer, BUFFER_SIZE - 1);
    buffer[ret] = 0;
    fputs(buffer, stderr);

    send_msg(tx, "Did he go and leave you all alone? ( ͡o ͜ʖ ͡o)\n");
    sleep(1);
    ret = read(rx, buffer, BUFFER_SIZE - 1);
    buffer[ret] = 0;
    fputs(buffer, stderr);

    fprintf(stderr, "[INFO]: closing pipe\n");
    close(tx);

    // parent waits for the child
    wait(NULL);
    return 0;
}

int main() {
    int filedes[2];
    int bruh[2];
    if (pipe(filedes) != 0) {
        fprintf(stderr, "[ERR]: pipe() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pipe(bruh) != 0) {
        fprintf(stderr, "[ERR]: pipe() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        // we need to close the ends we are not using
        // otherwise, the child will be perpetually waiting for a message that
        // will never come
        return child_main(filedes[0], bruh[1]);
    } else {
        return parent_main(filedes[1], bruh[0]);
    }
}
