#include "no_cap_courier.h"

int get_int(char *msg)
{
    char buf[20];

    printf("%s", msg);
    if (scanf("%19s", buf) != 1) { 
        printf("\nGoodbye!\n");
        exit(0);
    }
    return atoi(buf); 
}

void get_word(char *msg, char *dest)
{
    printf("%s", msg);
    if (scanf("%29s", dest) != 1) {
        printf("\nGoodbye!\n");
        exit(0);
    }
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
        return 50;
}
int load_shops(char names[][LEN])
{
    FILE *fp = fopen("shops.txt", "r");
    int no, n = 0;
    char name[LEN];
    if (fp == NULL)
        return 0;
    while (n < MAX_SHOPS && fscanf(fp, "%d %s", &no, name) == 2) {
        strcpy(names[n], name);
        n++;
    }
    fclose(fp);
    return n;
}
int load_menu(int shop, char items[][LEN], int prices[])
{
    FILE *fp = fopen("menu.txt", "r");
    int no, price, n = 0;
    char item[LEN];
    if (fp == NULL)
        return 0;
    while (fscanf(fp, "%d %s %d", &no, item, &price) == 3) {
        if (no == shop && n < MAX_MENU) {
            strcpy(items[n], item);
            prices[n] = price;
            n++;
        }
    }
    fclose(fp);
    return n;
}
void get_shop_name(int shop, char *name)
{
        char names[MAX_SHOPS][LEN];
    int n = load_shops(names);
    strcpy(name, "Unknown");
    if (shop >= 1 && shop <= n)
        strcpy(name, names[shop - 1]);
}
int choose_shop(void)
{
    char names[MAX_SHOPS][LEN];
    int i, c, n = load_shops(names);
    if (n == 0) {
        printf("No shops linked yet. Ask the admin.\n");
        return 0;
    }
    printf("\n--- Shops ---\n");
    for (i = 0; i < n; i++)
        printf("%d. %s\n", i + 1, names[i]);
    c = get_int("Choose shop: ");
    if (c < 1 || c > n) {
        printf("Invalid shop.\n");
        return 0;
    }
    return c;
}
void seed_shops(void)
{
    FILE *fp = fopen("shops.txt", "r");
    if (fp != NULL) {
        fclose(fp);
        return;
    }
    fp = fopen("shops.txt", "w");
    fprintf(fp, "1 Stationery_Shop\n2 Food_Shop\n3 Book_Shop\n");
    fclose(fp);
    fp = fopen("menu.txt", "w");
    fprintf(fp, "1 Lab_Pad 25\n1 Matador_Pen 10\n1 Eraser 5\n1 Sharpener 5\n");
    fprintf(fp, "1 Pencil 10\n1 Assignment_Cover_Page 10\n1 Lab_Report_File 10\n");
    fprintf(fp, "1 Notepad 80\n1 Stapler 50\n1 Stapler_Pins 20\n1 Ruler 15\n");
    fprintf(fp, "1 Highlighter 30\n1 Glue_Stick 25\n1 Graph_Paper 5\n");
    fprintf(fp, "1 Marker_Pen 30\n1 Calculator 850\n");
    fprintf(fp, "2 Burger 90\n2 Sandwich 60\n2 Biriyani 100\n2 Fried_Rice 120\n");
    fprintf(fp, "2 Chicken_Roll 50\n2 Egg_Roll 40\n2 Noodles 70\n2 French_Fries 60\n");
    fprintf(fp, "2 Singara 10\n2 Tea 15\n2 Coffee 30\n2 Cold_Drink 30\n");
    fprintf(fp, "3 Engg_Math_Grewal 750\n3 Physics_Halliday 900\n");
    fprintf(fp, "3 Engg_Chemistry_Jain 450\n3 C_Programming_Balaguruswamy 450\n");
    fprintf(fp, "3 Data_Structures_Schaum 600\n3 Digital_Logic_Mano 800\n");
    fprintf(fp, "3 Engg_Mechanics_Hibbeler 1000\n3 Thermodynamics_Cengel 1100\n");
    fprintf(fp, "3 Fluid_Mechanics_Cengel 1000\n3 Circuit_Analysis_Boylestad 1000\n");
    fclose(fp);
}
void add_menu_item(void)
{
    char items[MAX_MENU][LEN], item[LEN];
    int prices[MAX_MENU];
    int shop, price;
    FILE *fp;
    shop = choose_shop();
    if (shop == 0)
        return;
    if (load_menu(shop, items, prices) >= MAX_MENU) {
        printf("This menu is full.\n");
        return;
    }
    get_word("Item name (use _ for space): ", item);
    price = get_int("Price (Tk): ");
    if (price < 1) {
        printf("Invalid price.\n");
        return;
    }
    fp = fopen("menu.txt", "a");
    fprintf(fp, "%d %s %d\n", shop, item, price);
    fclose(fp);
    printf("Item added.\n");
}
void show_all_menus(void)
{
    char names[MAX_SHOPS][LEN], items[MAX_MENU][LEN];
    int prices[MAX_MENU];
    int i, j, m, n = load_shops(names);
    if (n == 0)
        printf("No shops linked yet.\n");
    for (i = 0; i < n; i++) {
        printf("\n=== Shop %d: %s ===\n", i + 1, names[i]);
        m = load_menu(i + 1, items, prices);
        if (m == 0)
            printf("  (no items yet)\n");
        for (j = 0; j < m; j++)
            printf("  %2d. %-30s %d Tk\n", j + 1, items[j], prices[j]);
    }
}
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
    printf("\n--- User Registration ---\n");
    get_word("KUET roll: ", u.roll);
    if (find_user(u.roll, &tmp)) {
        printf("Already registered. Please login.\n");
        return;
    }
    get_word("Name (use _ for space): ", u.name);
    get_word("Phone number: ", u.phone);
    choose_location(u.location);
    fp = fopen("users.txt", "a");
    fprintf(fp, "%s %s %s %s\n", u.roll, u.name, u.phone, u.location);
    fclose(fp);
    printf("Registered. You can login now.\n");
}
int login_user(struct User *u)
{
    char roll[LEN];
    get_word("KUET roll: ", roll);
    if (find_user(roll, u) == 0) {
        printf("Roll not found. Please register first.\n");
        return 0;
    }
    printf("Welcome, %s!\n", u->name);
    return 1;
}
void print_user_profile(struct User *u)
{
    printf("\n--- Profile ---\n");
    printf("Name: %s\nRoll: %s\nPhone: %s\n", u->name, u->roll, u->phone);
    printf("Location: %s (delivery fee %d Tk)\n", u->location, delivery_fee(u->location));
}
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
    printf("\n--- Delivery Man Registration ---\n");
    get_word("KUET roll: ", r.roll);
    if (find_runner(r.roll, &tmp)) {
        printf("Already registered. Please login.\n");
        return;
    }
    get_word("Name (use _ for space): ", r.name);
    get_word("Phone number: ", r.phone);
    choose_location(r.location);
    fp = fopen("runners.txt", "a");
    fprintf(fp, "%s %s %s %s 0 0 FREE\n", r.roll, r.name, r.phone, r.location);
    fclose(fp);
    printf("Registered. You can login now.\n");
}
int login_runner(struct Runner *r)
{
    char roll[LEN];

    get_word("KUET roll: ", roll);
    if (find_runner(roll, r) == 0) {
        printf("Roll not found. Please register first.\n");
        return 0;
    }
    printf("Welcome, %s!\n", r->name);
    return 1;
}
void print_runner_profile(struct Runner *r)
{
    printf("\n--- Delivery Man Profile ---\n");
    printf("Name: %s\nRoll: %s\nPhone: %s\nLocation: %s\n", r->name, r->roll, r->phone, r->location);
    if (r->rating_count == 0)
        printf("Rating: no ratings yet\n");
    else
        printf("Rating: %.1f / 5 (%d ratings)\n", (float)r->rating_sum / r->rating_count, r->rating_count);
    printf("Status: %s\n", r->status);
}
void list_runners(void)
{
    struct Runner list[MAX_RUNNERS];
    int i, n = load_runners(list);
    if (n == 0)
        printf("No delivery men yet.\n");
    for (i = 0; i < n; i++) {
        printf("%-8s %-16s %-12s ", list[i].roll, list[i].name, list[i].location);
        if (list[i].rating_count == 0)
            printf("no ratings   ");
        else
            printf("%.1f stars    ", (float)list[i].rating_sum / list[i].rating_count);
        printf("%s\n", list[i].status);
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
int items_total(struct Order *o)
{
    int i, total = 0;
    for (i = 0; i < o->count; i++)
        total = total + o->qty[i] * o->price[i];
    return total;
}
void save_orders(struct Order o[], int n)
{
    FILE *fp = fopen("orders.txt", "w");
    int i;
    for (i = 0; i < n; i++)
        write_order(fp, &o[i]);
    fclose(fp);
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
    printf("\nOrder #%d [%s]\n", o->id, o->status);
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
        printf("This shop has no items yet.\n");
        return;
    }
    printf("\n--- Menu ---\n");
    for (i = 0; i < n; i++)
        printf("%2d. %-30s %d Tk\n", i + 1, items[i], prices[i]);
    o = (struct Order *)malloc(sizeof(struct Order));   /* record made only when ordering */
    if (o == NULL) {
        printf("Out of memory.\n");
        return;
    }
    o->count = 0;
    printf("\nAdd items one by one (up to %d different items).\n", MAX_CART);
    while (o->count < MAX_CART) {
        choice = get_int("Item no (0 = finish order): ");
        if (choice == 0)
            break;
        if (choice < 1 || choice > n) {
            printf("Invalid item.\n");
            continue;
        }
        qty = get_int("Quantity (1-10): ");
        if (qty < 1 || qty > 10) {
            printf("Invalid quantity.\n");
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
        printf("Added. Different items so far: %d\n", o->count);
    }
    if (o->count == 0) {
        printf("Nothing selected. Order not placed.\n");
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
    printf("\nOrder #%d placed.\n", o->id);
    for (i = 0; i < o->count; i++)
        printf("  %s x %d = %d Tk\n", o->item[i], o->qty[i], o->qty[i] * o->price[i]);
    printf("Items %d Tk + delivery %d Tk = %d Tk total.\n",
           items_total(o), o->fee, items_total(o) + o->fee);
    printf("Waiting for a delivery man to accept it.\n");
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
        printf("You have no orders yet.\n");
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
        printf("No such order on your account.\n");
    else if (strcmp(list[found].status, "PENDING") != 0)
        printf("Only PENDING orders can be cancelled.\n");
    else {
        strcpy(list[found].status, "CANCELLED");
        save_orders(list, n);
        printf("Order #%d cancelled.\n", id);
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
        printf("No delivered orders waiting for a rating.\n");
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
        printf("That order cannot be rated.\n");
        return;
    }
    stars = get_int("Rate the delivery man (1-5): ");
    if (stars < 1 || stars > 5) {
        printf("Invalid rating.\n");
        return;
    }
    add_rating(list[found].runner_roll, stars);
    list[found].rated = 1;
    save_orders(list, n);
    printf("Thanks! Rating added to the delivery man's profile.\n");
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
        printf("No pending orders right now.\n");
}
void accept_order(char *runner_roll)
{
    struct Runner r;
    struct Order list[MAX_ORDERS];
    int i, id, found = -1, n;

    if (find_runner(runner_roll, &r) == 0)
        return;
    if (strcmp(r.status, "BOOKED") == 0) {
        printf("You already have a delivery. Mark it delivered first.\n");
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
        printf("That order is not available.\n");
        return;
    }
    strcpy(list[found].runner_roll, runner_roll);
    strcpy(list[found].status, "ACCEPTED");
    list[found].accepted = time(NULL);
    save_orders(list, n);
    set_runner_status(runner_roll, "BOOKED");
    printf("You accepted order #%d. Deliver it, then mark it delivered.\n", id);
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
        printf("You have no active delivery.\n");
        return;
    }
    strcpy(list[found].status, "DELIVERED");
    save_orders(list, n);
    set_runner_status(runner_roll, "FREE");
    printf("Order #%d delivered. You are FREE again.\n", list[found].id);
}
void view_all_orders(void)
{
    struct Order list[MAX_ORDERS];
    int i, n = load_orders(list);
    if (n == 0)
        printf("No orders yet.\n");
    for (i = 0; i < n; i++) {
        printf("\nBuyer roll: %s", list[i].user_roll);
        print_order(&list[i]);
    }
}
