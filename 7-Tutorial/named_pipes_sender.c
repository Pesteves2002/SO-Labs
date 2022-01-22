#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_PATHNAME "fifo.pipe"
#define ANOTHER_ONE "bruh.pipe"
#define BUFFER_SIZE (128)

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

int main() {
    // remove pipe if it does exist
    if (unlink(FIFO_PATHNAME) != 0 && errno != ENOENT) {
        fprintf(stderr, "[ERR]: unlink(%s) failed: %s\n", FIFO_PATHNAME,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    // create pipe
    if (mkfifo(FIFO_PATHNAME, 0640) != 0) {
        fprintf(stderr, "[ERR]: mkfifo failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open pipe for writing
    // this waits for someone to open it for reading
    int tx = open(FIFO_PATHNAME, O_WRONLY);
    if (tx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open pipe for reading
    // this waits for someone to open it for writing
    int rx = open(ANOTHER_ONE, O_RDONLY);
    if (rx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    char buffer[BUFFER_SIZE];

    ssize_t ret;

    // The parent is likes classic rock:
    // https://www.youtube.com/watch?v=lrpXArn3hII
    send_msg(tx, "Tell me now\n");
    ret = read(rx, buffer, BUFFER_SIZE - 1);
    buffer[ret] = 0;
    fputs(buffer, stdout);

    send_msg(tx, "Is he good to you?\n");
    ret = read(rx, buffer, BUFFER_SIZE - 1);
    buffer[ret] = 0;
    fputs(buffer, stdout);

    send_msg(tx, "Can you make you the meals that I do?\n");
    ret = read(rx, buffer, BUFFER_SIZE - 1);
    buffer[ret] = 0;
    fputs(buffer, stdout);

    fprintf(stderr, "[INFO]: closing pipe\n");
    close(tx);
}
