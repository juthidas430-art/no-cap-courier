#include "no_cap_courier.h"

/* Delivery men: profiles, ratings, accepting and delivering */

int load_runners(struct Runner r[])
{
    FILE *fp = fopen("runners.txt", "r");
    int n = 0;

    if (fp == NULL)
        return 0;
    while (n < MAX_RUNNERS &&
           fscanf(fp, "%s %s %s %s %d %d %s", r[n].roll, r[n].name, r[n].phone,
                  r[n].location, &r[n].rating_sum, &r[n].rating_count, r[n].status) == 7)
        n++;
    fclose(fp);
    return n;
}

void save_runners(struct Runner r[], int n)
{
    FILE *fp = fopen("runners.txt", "w");
    int i;

    for (i = 0; i < n; i++)
        fprintf(fp, "%s %s %s %s %d %d %s\n", r[i].roll, r[i].name, r[i].phone,
                r[i].location, r[i].rating_sum, r[i].rating_count, r[i].status);
    fclose(fp);
}

/* First run only: the 4 starting delivery men (edit runners.txt for real details). */
void seed_runners(void)
{
    FILE *fp = fopen("runners.txt", "r");

    if (fp != NULL) {
        fclose(fp);
        return;
    }
    fp = fopen("runners.txt", "w");
    fprintf(fp, "2000001 Delivery_Man_1 01700000001 Fulbari_Gate 0 0 FREE\n");
    fprintf(fp, "2000002 Delivery_Man_2 01700000002 Fulbari_Gate 0 0 FREE\n");
    fprintf(fp, "2000003 Delivery_Man_3 01700000003 Residential_Area 0 0 FREE\n");
    fprintf(fp, "2000004 Delivery_Man_4 01700000004 Residential_Area 0 0 FREE\n");
    fclose(fp);
}

int find_runner(char *roll, struct Runner *r)
{
    struct Runner list[MAX_RUNNERS];
    int i, n = load_runners(list);

    for (i = 0; i < n; i++) {
        if (strcmp(list[i].roll, roll) == 0) {
            *r = list[i];
            return 1;
        }
    }
    return 0;
}

void register_runner(void)
{
    struct Runner r, tmp;
    FILE *fp;

    printf("\n" BOLD CYAN "--- Delivery Man Registration ---" RESET "\n");
    get_word("KUET roll: ", r.roll);
    if (find_runner(r.roll, &tmp)) {
        printf(RED "Already registered. Please login." RESET "\n");
        return;
    }
    get_word("Name (use _ for space): ", r.name);
    get_word("Phone number: ", r.phone);
    choose_location(r.location);

    fp = fopen("runners.txt", "a");
    fprintf(fp, "%s %s %s %s 0 0 FREE\n", r.roll, r.name, r.phone, r.location);
    fclose(fp);
    printf(GREEN "Registered. You can login now." RESET "\n");
}

int login_runner(struct Runner *r)
{
    char roll[LEN];

    get_word("KUET roll: ", roll);
    if (find_runner(roll, r) == 0) {
        printf(RED "Roll not found. Please register first." RESET "\n");
        return 0;
    }
    printf(GREEN "Welcome, %s!" RESET "\n", r->name);
    return 1;
}

void print_runner_profile(struct Runner *r)
{
    printf("\n" BOLD CYAN "--- Delivery Man Profile ---" RESET "\n");
    printf("Name: %s\nRoll: %s\nPhone: %s\nLocation: %s\n", r->name, r->roll, r->phone, r->location);
    if (r->rating_count == 0)
        printf("Rating: no ratings yet\n");
    else
        printf("Rating: %.1f / 5 (%d ratings)\n", (float)r->rating_sum / r->rating_count, r->rating_count);
    printf("Status: %s%s%s\n", status_color(r->status), r->status, RESET);
}

/* Buyers see who is FREE and who is BOOKED. */
void list_runners(void)
{
    struct Runner list[MAX_RUNNERS];
    int i, n = load_runners(list);

    if (n == 0)
        printf(YELLOW "No delivery men yet." RESET "\n");
    for (i = 0; i < n; i++) {
        printf("%-8s %-16s %-12s ", list[i].roll, list[i].name, list[i].location);
        if (list[i].rating_count == 0)
            printf("no ratings   ");
        else
            printf("%.1f stars    ", (float)list[i].rating_sum / list[i].rating_count);
        printf("%s%s%s\n", status_color(list[i].status), list[i].status, RESET);
    }
}

void set_runner_status(char *roll, char *status)
{
    struct Runner list[MAX_RUNNERS];
    int i, n = load_runners(list);

    for (i = 0; i < n; i++)
        if (strcmp(list[i].roll, roll) == 0)
            strcpy(list[i].status, status);
    save_runners(list, n);
}

void add_rating(char *roll, int stars)
{
    struct Runner list[MAX_RUNNERS];
    int i, n = load_runners(list);

    for (i = 0; i < n; i++) {
        if (strcmp(list[i].roll, roll) == 0) {
            list[i].rating_sum = list[i].rating_sum + stars;
            list[i].rating_count++;
        }
    }
    save_runners(list, n);
}

void view_pending_orders(void)
{
    struct Order list[MAX_ORDERS];
    struct User u;
    int i, count = 0, n = load_orders(list);

    for (i = 0; i < n; i++) {
        if (strcmp(list[i].status, "PENDING") == 0) {
            print_order(&list[i]);
            if (find_user(list[i].user_roll, &u))
                printf("  Buyer: %s, %s, deliver to %s\n", u.name, u.phone, u.location);
            count++;
        }
    }
    if (count == 0)
        printf(YELLOW "No pending orders right now." RESET "\n");
}

void accept_order(char *runner_roll)
{
    struct Runner r;
    struct Order list[MAX_ORDERS];
    int i, id, found = -1, n;

    if (find_runner(runner_roll, &r) == 0)
        return;
    if (strcmp(r.status, "BOOKED") == 0) {
        printf(RED "You already have a delivery. Mark it delivered first." RESET "\n");
        return;
    }
    view_pending_orders();
    id = get_int("\nOrder id to accept (0 = back): ");
    if (id == 0)
        return;
    n = load_orders(list);
    for (i = 0; i < n; i++)
        if (list[i].id == id && strcmp(list[i].status, "PENDING") == 0)
            found = i;
    if (found == -1) {
        printf(RED "That order is not available." RESET "\n");
        return;
    }
    strcpy(list[found].runner_roll, runner_roll);
    strcpy(list[found].status, "ACCEPTED");
    list[found].accepted = time(NULL);
    save_orders(list, n);
    set_runner_status(runner_roll, "BOOKED");
    printf(GREEN "You accepted order #%d. Deliver it, then mark it delivered." RESET "\n", id);
}

void mark_delivered(char *runner_roll)
{
    struct Order list[MAX_ORDERS];
    int i, found = -1, n = load_orders(list);

    for (i = 0; i < n; i++)
        if (strcmp(list[i].runner_roll, runner_roll) == 0 &&
            strcmp(list[i].status, "ACCEPTED") == 0)
            found = i;
    if (found == -1) {
        printf(RED "You have no active delivery." RESET "\n");
        return;
    }
    strcpy(list[found].status, "DELIVERED");
    save_orders(list, n);
    set_runner_status(runner_roll, "FREE");
    printf(GREEN "Order #%d delivered. You are FREE again." RESET "\n", list[found].id);
}
