#ifndef NO_CAP_COURIER_H
#define NO_CAP_COURIER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN          30     
#define MAX_SHOPS    3
#define MAX_MENU     20
#define MAX_CART     8      
#define MAX_RUNNERS  50
#define MAX_ORDERS   300
#define ADMIN_PASS   "admin123"
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
    char status[LEN];            
};

struct Order {
    int  id;
    char user_roll[LEN];
    int  shop;
    int  count;                  
    char item[MAX_CART][LEN];
    int  qty[MAX_CART];
    int  price[MAX_CART];       
    int  fee;
    char runner_roll[LEN];       
    char status[LEN];            
    long placed;
    long accepted;
    int  rated;                 
};
int  get_int(char *msg);
void get_word(char *msg, char *dest);
void seed_shops(void);
void add_menu_item(void);
void show_all_menus(void);
void register_user(void);
int  login_user(struct User *u);
void print_user_profile(struct User *u);
void seed_runners(void);
void register_runner(void);
int  login_runner(struct Runner *r);
int  find_runner(char *roll, struct Runner *r);
void print_runner_profile(struct Runner *r);
void list_runners(void);
void place_order(struct User *u);
void view_user_orders(char *roll);
void cancel_order(char *roll);
void rate_order(char *roll);
void view_pending_orders(void);
void accept_order(char *runner_roll);
void mark_delivered(char *runner_roll);
void view_all_orders(void);

#endif
