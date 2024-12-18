#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int rows = w.ws_row + 1;
    int cols = w.ws_col + 1;

    int buffer_size = rows * cols + 1;
    char *viewport = malloc(buffer_size * sizeof(char));

    if (viewport == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols - 1; j++) {
            viewport[i * cols + j] = ' ';
        }

        if (i < rows - 1) {
            viewport[i * cols + (cols - 1)] = '\n';
        } else {
            viewport[i * cols + (cols - 1)] = ' ';
        }
    }

    viewport[buffer_size - 1] = '\0';

    printf("\033[2J\033[H");
    printf("\033[40m%s\033[0m", viewport);

    free(viewport);

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');

    return 0;
}
