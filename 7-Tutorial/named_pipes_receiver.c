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
    if (unlink(ANOTHER_ONE) != 0 && errno != ENOENT) {
        fprintf(stderr, "[ERR]: unlink(%s) failed: %s\n", ANOTHER_ONE,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    // create pipe
    if (mkfifo(ANOTHER_ONE, 0640) != 0) {
        fprintf(stderr, "[ERR]: mkfifo failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open pipe for writing
    // this waits for someone to open it for reading
    int tx = open(ANOTHER_ONE, O_WRONLY);
    if (tx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open pipe for reading
    // this waits for someone to open it for writing
    int rx = open(FIFO_PATHNAME, O_RDONLY);
    if (rx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (true) {
        char buffer[BUFFER_SIZE];
        ssize_t ret = read(rx, buffer, BUFFER_SIZE - 1);
        if (ret == 0) {
            // ret == 0 signals EOF
            fprintf(stderr, "[INFO]: pipe closed\n");
            return 0;
        } else if (ret == -1) {
            // ret == -1 signals error
            fprintf(stderr, "[ERR]: read failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "[INFO]: received %zd B\n", ret);
        buffer[ret] = 0;
        fputs(buffer, stdout);
        send_msg(tx, "GAWK GAWK\n");
    }
    close(tx);
    close(rx);
}
