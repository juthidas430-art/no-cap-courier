#include "no_cap_courier.h"

/* Shops and menus (admin side) */

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

/* Shows the shops and returns the chosen shop number (0 if none). */
int choose_shop(void)
{
    char names[MAX_SHOPS][LEN];
    int i, c, n = load_shops(names);

    if (n == 0) {
        printf(YELLOW "No shops linked yet. Ask the admin." RESET "\n");
        return 0;
    }
    printf("\n" BOLD CYAN "--- Shops ---" RESET "\n");
    for (i = 0; i < n; i++)
        printf("%d. %s\n", i + 1, names[i]);
    c = get_int("Choose shop: ");
    if (c < 1 || c > n) {
        printf(RED "Invalid shop." RESET "\n");
        return 0;
    }
    return c;
}

/* First run only: creates the 3 fixed shops and their menus. */
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
    /* shop 1: stationery */
    fprintf(fp, "1 Lab_Pad 25\n1 Matador_Pen 10\n1 Eraser 5\n1 Sharpener 5\n");
    fprintf(fp, "1 Pencil 10\n1 Assignment_Cover_Page 10\n1 Lab_Report_File 10\n");
    fprintf(fp, "1 Notepad 80\n1 Stapler 50\n1 Stapler_Pins 20\n1 Ruler 15\n");
    fprintf(fp, "1 Highlighter 30\n1 Glue_Stick 25\n1 Graph_Paper 5\n");
    fprintf(fp, "1 Marker_Pen 30\n1 Calculator 850\n");
    /* shop 2: food */
    fprintf(fp, "2 Burger 90\n2 Sandwich 60\n2 Biriyani 100\n2 Fried_Rice 120\n");
    fprintf(fp, "2 Chicken_Roll 50\n2 Egg_Roll 40\n2 Noodles 70\n2 French_Fries 60\n");
    fprintf(fp, "2 Singara 10\n2 Tea 15\n2 Coffee 30\n2 Cold_Drink 30\n");
    /* shop 3: engineering books */
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
        printf(RED "This menu is full." RESET "\n");
        return;
    }
    get_word("Item name (use _ for space): ", item);
    price = get_int("Price (Tk): ");
    if (price < 1) {
        printf(RED "Invalid price." RESET "\n");
        return;
    }
    fp = fopen("menu.txt", "a");
    fprintf(fp, "%d %s %d\n", shop, item, price);
    fclose(fp);
    printf(GREEN "Item added." RESET "\n");
}

void show_all_menus(void)
{
    char names[MAX_SHOPS][LEN], items[MAX_MENU][LEN];
    int prices[MAX_MENU];
    int i, j, m, n = load_shops(names);

    if (n == 0)
        printf(YELLOW "No shops linked yet." RESET "\n");
    for (i = 0; i < n; i++) {
        printf("\n" BOLD CYAN "=== Shop %d: %s ===" RESET "\n", i + 1, names[i]);
        m = load_menu(i + 1, items, prices);
        if (m == 0)
            printf("  (no items yet)\n");
        for (j = 0; j < m; j++)
            printf("  %2d. %-30s %d Tk\n", j + 1, items[j], prices[j]);
    }
}
