#include "no_cap_courier.h"

/* Input helpers, status colours, areas and delivery fees */

int get_int(char *msg)
{
    char buf[20];

    printf("%s", msg);
    if (scanf("%19s", buf) != 1) {      /* input finished */
        printf("\n" GREEN "Goodbye!" RESET "\n");
        exit(0);
    }
    return atoi(buf);                   /* wrong text gives 0 */
}

void get_word(char *msg, char *dest)
{
    printf("%s", msg);
    if (scanf("%29s", dest) != 1) {
        printf("\n" GREEN "Goodbye!" RESET "\n");
        exit(0);
    }
}

/* Colour for a status word. */
char *status_color(char *status)
{
    if (strcmp(status, "PENDING") == 0)
        return YELLOW;
    else if (strcmp(status, "ACCEPTED") == 0)
        return BLUE;
    else if (strcmp(status, "DELIVERED") == 0 || strcmp(status, "FREE") == 0)
        return GREEN;
    else if (strcmp(status, "CANCELLED") == 0 || strcmp(status, "BOOKED") == 0)
        return RED;
    return RESET;
}

void choose_location(char *loc)
{
    int c = 0;

    printf("  1. Fulbari_Gate      (delivery fee 20 Tk)\n");
    printf("  2. Residential_Area  (delivery fee 25 Tk)\n");
    printf("  3. Doulotpur         (delivery fee 40 Tk)\n");
    printf("  4. Shibbari          (delivery fee 45 Tk)\n");
    printf("  5. Gollamari         (delivery fee 50 Tk)\n");
    while (c < 1 || c > 5)
        c = get_int("Location (1-5): ");

    if (c == 1)
        strcpy(loc, "Fulbari_Gate");
    else if (c == 2)
        strcpy(loc, "Residential_Area");
    else if (c == 3)
        strcpy(loc, "Doulotpur");
    else if (c == 4)
        strcpy(loc, "Shibbari");
    else
        strcpy(loc, "Gollamari");
}

/* Delivery fee depends on the area (Tk). */
int delivery_fee(char *loc)
{
    if (strcmp(loc, "Fulbari_Gate") == 0)
        return 20;
    else if (strcmp(loc, "Residential_Area") == 0)
        return 25;
    else if (strcmp(loc, "Doulotpur") == 0)
        return 40;
    else if (strcmp(loc, "Shibbari") == 0)
        return 45;
    else
        return 50;                      /* Gollamari */
}
