#include "input.h"

#include <stdio.h>

int ReadIntInRange(const char *prompt, int min, int max) {
    int value;
    int rc;

    for (;;) {
        printf("%s", prompt);
        rc = scanf("%d", &value);

        if (rc == 1 && value >= min && value <= max) {
            int c;
            do {
                c = getchar();
            } while (c != '\n' && c != EOF);
            return value;
        }

        printf("Entree invalide. Reessayez.\n");
        {
            int c;
            do {
                c = getchar();
            } while (c != '\n' && c != EOF);
        }
    }
}

void WaitForEnter(void) {
    int c;
    printf("Appuyez sur Entree...\n");
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
    if (c == EOF) {
        clearerr(stdin);
    }
}
