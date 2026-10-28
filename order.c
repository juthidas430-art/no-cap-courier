#include "no_cap_courier.h"

/* Orders: place, view, cancel, rate and order storage */

int load_orders(struct Order o[])
{
    FILE *fp = fopen("orders.txt", "r");
    int n = 0, i, ok;

    if (fp == NULL)
        return 0;
    while (n < MAX_ORDERS) {
        ok = fscanf(fp, "%d %s %d %d", &o[n].id, o[n].user_roll, &o[n].shop, &o[n].count);
        if (ok != 4 || o[n].count < 1 || o[n].count > MAX_CART)
            break;
        for (i = 0; i < o[n].count; i++)
            if (fscanf(fp, "%s %d %d", o[n].item[i], &o[n].qty[i], &o[n].price[i]) != 3)
                ok = 0;
        if (ok == 0)
            break;
        if (fscanf(fp, "%d %s %s %ld %ld %d", &o[n].fee, o[n].runner_roll, o[n].status,
                   &o[n].placed, &o[n].accepted, &o[n].rated) != 6)
            break;
        n++;
    }
    fclose(fp);
    return n;
}

void write_order(FILE *fp, struct Order *o)
{
    int i;

    fprintf(fp, "%d %s %d %d", o->id, o->user_roll, o->shop, o->count);
    for (i = 0; i < o->count; i++)
        fprintf(fp, " %s %d %d", o->item[i], o->qty[i], o->price[i]);
    fprintf(fp, " %d %s %s %ld %ld %d\n", o->fee, o->runner_roll, o->status,
            o->placed, o->accepted, o->rated);
}

void save_orders(struct Order o[], int n)
{
    FILE *fp = fopen("orders.txt", "w");
    int i;

    for (i = 0; i < n; i++)
        write_order(fp, &o[i]);
    fclose(fp);
}

/* Price of all items in the order (without the delivery fee). */
int items_total(struct Order *o)
{
    int i, total = 0;

    for (i = 0; i < o->count; i++)
        total = total + o->qty[i] * o->price[i];
    return total;
}

int next_order_id(void)
{
    struct Order list[MAX_ORDERS];
    int i, max = 0, n = load_orders(list);

    for (i = 0; i < n; i++)
        if (list[i].id > max)
            max = list[i].id;
    return max + 1;
}

void print_order(struct Order *o)
{
    char shop[LEN];
    struct Runner r;
    time_t t;
    long wait;
    int i;

    get_shop_name(o->shop, shop);
    printf("\nOrder #%d [%s%s%s]\n", o->id, status_color(o->status), o->status, RESET);
    printf("  Shop: %s\n", shop);
    for (i = 0; i < o->count; i++)
        printf("  - %s x %d = %d Tk\n", o->item[i], o->qty[i], o->qty[i] * o->price[i]);
    printf("  Items: %d Tk   Delivery fee: %d Tk   Total: %d Tk\n",
           items_total(o), o->fee, items_total(o) + o->fee);
    if (strcmp(o->runner_roll, "-") != 0 && find_runner(o->runner_roll, &r))
        printf("  Delivery man: %s (%s)\n", r.name, r.phone);

    t = o->placed;
    printf("  Placed  : %s", ctime(&t));
    if (o->accepted != 0) {
        t = o->accepted;
        printf("  Accepted: %s", ctime(&t));
        wait = o->accepted - o->placed;
        printf("  Waited for runner: %ld min %ld sec\n", wait / 60, wait % 60);
    } else if (strcmp(o->status, "PENDING") == 0) {
        wait = time(NULL) - o->placed;
        printf("  Waiting so far: %ld min %ld sec\n", wait / 60, wait % 60);
    }
}

void place_order(struct User *u)
{
    char items[MAX_MENU][LEN];
    int prices[MAX_MENU];
    int i, n, shop, choice, qty, found;
    struct Order *o;
    FILE *fp;

    shop = choose_shop();
    if (shop == 0)
        return;
    n = load_menu(shop, items, prices);
    if (n == 0) {
        printf(YELLOW "This shop has no items yet." RESET "\n");
        return;
    }
    printf("\n" BOLD CYAN "--- Menu ---" RESET "\n");
    for (i = 0; i < n; i++)
        printf("%2d. %-30s %d Tk\n", i + 1, items[i], prices[i]);

    o = (struct Order *)malloc(sizeof(struct Order));   /* record made only when ordering */
    if (o == NULL) {
        printf(RED "Out of memory." RESET "\n");
        return;
    }
    o->count = 0;
    printf("\nAdd items one by one (up to %d different items).\n", MAX_CART);
    while (o->count < MAX_CART) {
        choice = get_int("Item no (0 = finish order): ");
        if (choice == 0)
            break;
        if (choice < 1 || choice > n) {
            printf(RED "Invalid item." RESET "\n");
            continue;
        }
        qty = get_int("Quantity (1-10): ");
        if (qty < 1 || qty > 10) {
            printf(RED "Invalid quantity." RESET "\n");
            continue;
        }
        found = -1;
        for (i = 0; i < o->count; i++)          /* same item again: just add quantity */
            if (strcmp(o->item[i], items[choice - 1]) == 0)
                found = i;
        if (found != -1)
            o->qty[found] = o->qty[found] + qty;
        else {
            strcpy(o->item[o->count], items[choice - 1]);
            o->qty[o->count] = qty;
            o->price[o->count] = prices[choice - 1];
            o->count++;
        }
        printf(GREEN "Added. Different items so far: %d" RESET "\n", o->count);
    }
    if (o->count == 0) {
        printf(RED "Nothing selected. Order not placed." RESET "\n");
        free(o);
        return;
    }

    o->id = next_order_id();
    strcpy(o->user_roll, u->roll);
    o->shop = shop;
    o->fee = delivery_fee(u->location);
    strcpy(o->runner_roll, "-");
    strcpy(o->status, "PENDING");
    o->placed = time(NULL);
    o->accepted = 0;
    o->rated = 0;

    fp = fopen("orders.txt", "a");
    write_order(fp, o);
    fclose(fp);

    printf("\n" GREEN "Order #%d placed." RESET "\n", o->id);
    for (i = 0; i < o->count; i++)
        printf("  %s x %d = %d Tk\n", o->item[i], o->qty[i], o->qty[i] * o->price[i]);
    printf("Items %d Tk + delivery %d Tk = %d Tk total.\n",
           items_total(o), o->fee, items_total(o) + o->fee);
    printf(YELLOW "Waiting for a delivery man to accept it." RESET "\n");
    free(o);                                            /* released after saving */
}

void view_user_orders(char *roll)
{
    struct Order list[MAX_ORDERS];
    int i, count = 0, n = load_orders(list);

    for (i = 0; i < n; i++) {
        if (strcmp(list[i].user_roll, roll) == 0) {
            print_order(&list[i]);
            count++;
        }
    }
    if (count == 0)
        printf(YELLOW "You have no orders yet." RESET "\n");
}

void cancel_order(char *roll)
{
    struct Order list[MAX_ORDERS];
    int i, id, found = -1, n;

    view_user_orders(roll);
    id = get_int("\nOrder id to cancel (0 = back): ");
    if (id == 0)
        return;
    n = load_orders(list);
    for (i = 0; i < n; i++)
        if (list[i].id == id && strcmp(list[i].user_roll, roll) == 0)
            found = i;

    if (found == -1)
        printf(RED "No such order on your account." RESET "\n");
    else if (strcmp(list[found].status, "PENDING") != 0)
        printf(RED "Only PENDING orders can be cancelled." RESET "\n");
    else {
        strcpy(list[found].status, "CANCELLED");
        save_orders(list, n);
        printf(GREEN "Order #%d cancelled." RESET "\n", id);
    }
}

void rate_order(char *roll)
{
    struct Order list[MAX_ORDERS];
    int i, id, stars, found = -1, count = 0, n = load_orders(list);

    for (i = 0; i < n; i++) {
        if (strcmp(list[i].user_roll, roll) == 0 &&
            strcmp(list[i].status, "DELIVERED") == 0 && list[i].rated == 0) {
            print_order(&list[i]);
            count++;
        }
    }
    if (count == 0) {
        printf(YELLOW "No delivered orders waiting for a rating." RESET "\n");
        return;
    }
    id = get_int("\nOrder id to rate (0 = back): ");
    if (id == 0)
        return;
    for (i = 0; i < n; i++)
        if (list[i].id == id && strcmp(list[i].user_roll, roll) == 0 &&
            strcmp(list[i].status, "DELIVERED") == 0 && list[i].rated == 0)
            found = i;
    if (found == -1) {
        printf(RED "That order cannot be rated." RESET "\n");
        return;
    }
    stars = get_int("Rate the delivery man (1-5): ");
    if (stars < 1 || stars > 5) {
        printf(RED "Invalid rating." RESET "\n");
        return;
    }
    add_rating(list[found].runner_roll, stars);
    list[found].rated = 1;
    save_orders(list, n);
    printf(GREEN "Thanks! Rating added to the delivery man's profile." RESET "\n");
}

void view_all_orders(void)
{
    struct Order list[MAX_ORDERS];
    int i, n = load_orders(list);

    if (n == 0)
        printf(YELLOW "No orders yet." RESET "\n");
    for (i = 0; i < n; i++) {
        printf("\nBuyer roll: %s", list[i].user_roll);
        print_order(&list[i]);
    }
}
