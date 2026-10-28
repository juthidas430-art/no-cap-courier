#include "no_cap_courier.h"

/* Users: register, login, profile */

int find_user(char *roll, struct User *u)
{
    FILE *fp = fopen("users.txt", "r");
    struct User t;

    if (fp == NULL)
        return 0;
    while (fscanf(fp, "%s %s %s %s", t.roll, t.name, t.phone, t.location) == 4) {
        if (strcmp(t.roll, roll) == 0) {
            *u = t;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void register_user(void)
{
    struct User u, tmp;
    FILE *fp;

    printf("\n" BOLD CYAN "--- User Registration ---" RESET "\n");
    get_word("KUET roll: ", u.roll);
    if (find_user(u.roll, &tmp)) {
        printf(RED "Already registered. Please login." RESET "\n");
        return;
    }
    get_word("Name (use _ for space): ", u.name);
    get_word("Phone number: ", u.phone);
    choose_location(u.location);

    fp = fopen("users.txt", "a");
    fprintf(fp, "%s %s %s %s\n", u.roll, u.name, u.phone, u.location);
    fclose(fp);
    printf(GREEN "Registered. You can login now." RESET "\n");
}

int login_user(struct User *u)
{
    char roll[LEN];

    get_word("KUET roll: ", roll);
    if (find_user(roll, u) == 0) {
        printf(RED "Roll not found. Please register first." RESET "\n");
        return 0;
    }
    printf(GREEN "Welcome, %s!" RESET "\n", u->name);
    return 1;
}

void print_user_profile(struct User *u)
{
    printf("\n" BOLD CYAN "--- Profile ---" RESET "\n");
    printf("Name: %s\nRoll: %s\nPhone: %s\n", u->name, u->roll, u->phone);
    printf("Location: %s (delivery fee %d Tk)\n", u->location, delivery_fee(u->location));
}
