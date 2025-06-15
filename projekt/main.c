#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

char* imie = NULL;

// Obsługa sygnałów
void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("%s: Czekam na rozkazy\n", imie);
    } else if (sig == SIGUSR2) {
        printf("%s: Rozpoczynam misje\n", imie);
    } else if (sig == SIGTERM) {
        printf("%s: Misja zakonczona\n", imie);
        exit(0);
    }
}

int main() {
    int fd[2];
    pipe(fd);

    char* agenci[] = {"Gamma", "Beta", "Alpha", "Delta"};
    pid_t dzieci[4];

    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Proces dziecka – agent
            close(fd[1]); // zamknij wyjście (dziecko tylko czyta)
            imie = agenci[i];

            signal(SIGUSR1, signal_handler);
            signal(SIGUSR2, signal_handler);
            signal(SIGTERM, signal_handler);

            printf("%s: Gotowy do misji\n", imie);

            char bufor[64];
            while (read(fd[0], bufor, sizeof(bufor)) > 0) {
                if (strncmp(bufor, "Identify!", 9) == 0) {
                    printf("My codename is %s\n", imie);
                } else if (strncmp(bufor, "Status?", 7) == 0) {
                    printf("%s: My PID is: %d\n", imie, getpid());
                }
            }
            exit(0);
        } else {
            dzieci[i] = pid;
        }
    }

    close(fd[0]); // rodzic nie czyta
    sleep(1);

    // Test: wysłanie komend
    write(fd[1], "Identify!", 10);
    sleep(1);
    write(fd[1], "Status?", 8);
    sleep(1);

    // Test: wysłanie sygnałów
    for (int i = 0; i < 4; i++) kill(dzieci[i], SIGUSR1);
    sleep(1);
    for (int i = 0; i < 4; i++) kill(dzieci[i], SIGUSR2);
    sleep(1);
    for (int i = 0; i < 4; i++) kill(dzieci[i], SIGTERM);

    for (int i = 0; i < 4; i++) wait(NULL);
    return 0;
}

