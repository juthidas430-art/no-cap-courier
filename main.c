#include "no_cap_courier.h"
void admin_mode(void)
{
    char pass[LEN];
    int choice = -1;
    get_word("Admin password: ", pass);
    if (strcmp(pass, ADMIN_PASS) != 0) {
        printf("Wrong password.\n");
        return;
    }
    while (choice != 0) {
        printf("\n===== ADMIN =====\n");
        printf("1. View shops and menus\n2. Add a menu item\n");
        printf("3. View delivery men\n4. View all orders\n0. Exit\n");
        choice = get_int("Choice: ");
        if (choice == 1)
            show_all_menus();
        else if (choice == 2)
            add_menu_item();
        else if (choice == 3)
            list_runners();
        else if (choice == 4)
            view_all_orders();
    }
}
void user_session(struct User *u)
{
    int choice = -1;
    while (choice != 0) {
        printf("\n===== %s =====\n", u->name);
        printf("1. Place an order\n2. My orders\n3. Cancel a pending order\n");
        printf("4. Rate a delivery man\n5. See delivery men (free / booked)\n");
        printf("6. My profile\n0. Logout\n");
        choice = get_int("Choice: ");
        if (choice == 1)
            place_order(u);
        else if (choice == 2)
            view_user_orders(u->roll);
        else if (choice == 3)
            cancel_order(u->roll);
        else if (choice == 4)
            rate_order(u->roll);
        else if (choice == 5)
            list_runners();
        else if (choice == 6)
            print_user_profile(u);
    }
}
void user_mode(void)
{
    struct User u;
    int choice = -1;
    while (choice != 0) {
        printf("\n===== USER =====\n1. Register\n2. Login\n0. Exit\n");
        choice = get_int("Choice: ");

        if (choice == 1)
            register_user();
        else if (choice == 2) {
            if (login_user(&u))
                user_session(&u);
        }
    }
}
void runner_session(struct Runner *r)
{
    struct Runner latest;
    int choice = -1;
    while (choice != 0) {
        printf("\n===== %s =====\n", r->name);
        printf("1. My profile\n2. View pending orders\n3. Accept an order\n");
        printf("4. Mark my delivery as delivered\n0. Logout\n");
        choice = get_int("Choice: ");
        if (choice == 1) {
            if (find_runner(r->roll, &latest))      /* reload to see new rating/status */
                print_runner_profile(&latest);
        }
        else if (choice == 2)
            view_pending_orders();
        else if (choice == 3)
            accept_order(r->roll);
        else if (choice == 4)
            mark_delivered(r->roll);
    }
}
void runner_mode(void)
{
    struct Runner r;
    int choice = -1;
    while (choice != 0) {
        printf("\n===== DELIVERY MAN =====\n1. Register\n2. Login\n0. Exit\n");
        choice = get_int("Choice: ");
        if (choice == 1)
            register_runner();
        else if (choice == 2) {
            if (login_runner(&r))
                runner_session(&r);
        }
    }
}
int main(int argc, char *argv[])
{
    seed_shops();
    seed_runners();
    if (argc != 2) {
        printf("Usage: %s admin | user | runner\n", argv[0]);
        return 1;
    }
    printf("*** No Cap Courier - KUET campus delivery ***\n");
    if (strcmp(argv[1], "admin") == 0)
        admin_mode();
    else if (strcmp(argv[1], "user") == 0)
        user_mode();
    else if (strcmp(argv[1], "runner") == 0)
        runner_mode();
    else {
        printf("Unknown mode. Use admin, user or runner.\n");
        return 1;
    }
    printf("Goodbye!\n");
    return 0;
}
