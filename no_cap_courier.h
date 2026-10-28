#ifndef NO_CAP_COURIER_H
#define NO_CAP_COURIER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN          30     /* size of every text field (arrays of char) */
#define MAX_SHOPS    3
#define MAX_MENU     20
#define MAX_CART     8      /* different items in one order */
#define MAX_RUNNERS  50
#define MAX_ORDERS   300
#define ADMIN_PASS   "admin123"

/* ---------- colours (ANSI escape codes) ---------- */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define COLOR_RESET RESET

/* ---------- structures ---------- */
struct User {
    char roll[LEN];
    char name[LEN];
    char phone[LEN];
    char location[LEN];
};

struct Runner {
    char roll[LEN];
    char name[LEN];
    char phone[LEN];
    char location[LEN];
    int  rating_sum;
    int  rating_count;
    char status[LEN];            /* FREE or BOOKED */
};

struct Order {
    int  id;
    char user_roll[LEN];
    int  shop;
    int  count;                  /* how many different items in this order */
    char item[MAX_CART][LEN];
    int  qty[MAX_CART];
    int  price[MAX_CART];        /* price of ONE piece of each item */
    int  fee;
    char runner_roll[LEN];       /* "-" until a runner accepts */
    char status[LEN];            /* PENDING, ACCEPTED, DELIVERED, CANCELLED */
    long placed;
    long accepted;
    int  rated;                  /* 0 = not rated, 1 = rated */
};

/* Data files (every field is ONE word, so use _ instead of spaces):
   users.txt   : roll name phone location
   runners.txt : roll name phone location rating_sum rating_count status
   shops.txt   : shop_no shop_name
   menu.txt    : shop_no item price
   orders.txt  : id user shop count {item qty price}... fee runner status placed accepted rated */

/* ---------- util.c : Input helpers, status colours, areas and delivery fees ---------- */
int get_int(char *msg);
void get_word(char *msg, char *dest);
char *status_color(char *status);
void choose_location(char *loc);
int delivery_fee(char *loc);

/* ---------- shop.c : Shops and menus (admin side) ---------- */
int load_shops(char names[][LEN]);
int load_menu(int shop, char items[][LEN], int prices[]);
void get_shop_name(int shop, char *name);
int choose_shop(void);
void seed_shops(void);
void add_menu_item(void);
void show_all_menus(void);

/* ---------- user.c : Users: register, login, profile ---------- */
int find_user(char *roll, struct User *u);
void register_user(void);
int login_user(struct User *u);
void print_user_profile(struct User *u);

/* ---------- order.c : Orders: place, view, cancel, rate and order storage ---------- */
int load_orders(struct Order o[]);
void write_order(FILE *fp, struct Order *o);
void save_orders(struct Order o[], int n);
int items_total(struct Order *o);
int next_order_id(void);
void print_order(struct Order *o);
void place_order(struct User *u);
void view_user_orders(char *roll);
void cancel_order(char *roll);
void rate_order(char *roll);
void view_all_orders(void);

/* ---------- runner.c : Delivery men: profiles, ratings, accepting and delivering ---------- */
int load_runners(struct Runner r[]);
void save_runners(struct Runner r[], int n);
void seed_runners(void);
int find_runner(char *roll, struct Runner *r);
void register_runner(void);
int login_runner(struct Runner *r);
void print_runner_profile(struct Runner *r);
void list_runners(void);
void set_runner_status(char *roll, char *status);
void add_rating(char *roll, int stars);
void view_pending_orders(void);
void accept_order(char *runner_roll);
void mark_delivered(char *runner_roll);

#endif
