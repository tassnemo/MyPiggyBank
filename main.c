#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COIN_KINDS 5U
#define TOY_COUNT 4U
#define NAME_LEN 16U

static const uint16_t COIN_VALUE[COIN_KINDS] = {25U, 50U, 100U, 200U, 500U};
static uint16_t coinCount[COIN_KINDS];

typedef struct {
    char name[NAME_LEN];
    uint32_t price;
} Toy_t;

static Toy_t shop[TOY_COUNT];

static void seedBank(void) {
    uint8_t i;

    for (i = 0U; i < COIN_KINDS; ++i) {
        coinCount[i] = 0U;
    }

    coinCount[0] = 4U;
    coinCount[1] = 3U;
    coinCount[2] = 2U;
    coinCount[3] = 1U;

    strcpy(shop[0].name, "Car");
    shop[0].price = 250U;
    strcpy(shop[1].name, "Train");
    shop[1].price = 400U;
    strcpy(shop[2].name, "Ball");
    shop[2].price = 175U;
    strcpy(shop[3].name, "Puzzle");
    shop[3].price = 300U;
}

static void addCoins(void) {
    int coinId = -1;
    int qty = 0;
    int ch;

    printf("Which coin (0-4)? ");
    if (scanf("%d", &coinId) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        printf("Bad input. No coins were added.\n");
        return;
    }

    printf("How many? ");
    if (scanf("%d", &qty) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        printf("Bad input. No coins were added.\n");
        return;
    }

    if (coinId < 0 || coinId >= (int)COIN_KINDS || qty < 0) {
        printf("That coin does not exist. No change was made.\n");
        return;
    }

    if ((uint32_t)coinCount[coinId] + (uint32_t)qty > UINT16_MAX) {
        printf("That would overflow the bank.\n");
        return;
    }

    coinCount[coinId] += (uint16_t)qty;
    printf("Added %d %u-piastre coins.\n", qty, COIN_VALUE[coinId]);
}

static void takeCoins(void) {
    int coinId = -1;
    int qty = 0;
    int ch;

    printf("Which coin (0-4)? ");
    if (scanf("%d", &coinId) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        printf("Bad input. Nothing was taken.\n");
        return;
    }

    printf("How many? ");
    if (scanf("%d", &qty) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        printf("Bad input. Nothing was taken.\n");
        return;
    }

    if (coinId < 0 || coinId >= (int)COIN_KINDS || qty < 0) {
        printf("Invalid choice. Nothing was taken.\n");
        return;
    }

    if ((uint32_t)qty > coinCount[coinId]) {
        printf("You do not have that many. Nothing was taken.\n");
        return;
    }

    coinCount[coinId] -= (uint16_t)qty;
    printf("Removed %d %u-piastre coins.\n", qty, COIN_VALUE[coinId]);
}

static uint32_t bankTotal(void) {
    uint8_t i;
    uint32_t total = 0U;

    for (i = 0U; i < COIN_KINDS; ++i) {
        total += (uint32_t)coinCount[i] * COIN_VALUE[i];
    }

    return total;
}

static uint32_t sumCoins(const uint16_t *counts, uint8_t n) {
    if (n == 0U) {
        return 0U;
    }

    return (uint32_t)counts[n - 1U] + sumCoins(counts, n - 1U);
}

static uint8_t biggestPile(void) {
    uint8_t best = 0U;
    uint8_t i;

    for (i = 1U; i < COIN_KINDS; ++i) {
        if (coinCount[i] > coinCount[best]) {
            best = i;
        }
    }

    return best;
}

static void drawBar(uint16_t value, uint16_t full, uint8_t width) {
    uint8_t i;
    uint8_t filled = 0U;

    if (full == 0U) {
        for (i = 0U; i < width; ++i) {
            putchar('-');
        }
        return;
    }

    if (value >= full) {
        filled = width;
    } else {
        filled = (uint8_t)((((uint32_t)value) * width) / full);
    }

    for (i = 0U; i < filled; ++i) {
        putchar('#');
    }
    for (i = filled; i < width; ++i) {
        putchar('-');
    }
}

static void showBank(void) {
    uint8_t i;
    uint16_t maxCount = 0U;

    for (i = 0U; i < COIN_KINDS; ++i) {
        if (coinCount[i] > maxCount) {
            maxCount = coinCount[i];
        }
    }

    printf("\nPiggy Bank\n");
    for (i = 0U; i < COIN_KINDS; ++i) {
        printf("%3u piastres: %3u  ", COIN_VALUE[i], coinCount[i]);
        drawBar(coinCount[i], maxCount, 12U);
        printf("\n");
    }
}

static void buyToy(void) {
    uint8_t choice = 0U;
    uint8_t i;
    uint32_t total = bankTotal();

    printf("\nToys in the shop\n");
    for (i = 0U; i < TOY_COUNT; ++i) {
        printf("%u) %-10s %u piastres\n", i, shop[i].name, shop[i].price);
    }

    printf("Which toy (0-3)? ");
    if (scanf("%hhu", &choice) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        printf("Bad input. No purchase made.\n");
        return;
    }

    if (choice >= TOY_COUNT) {
        printf("No such toy.\n");
        return;
    }

    if (total >= shop[choice].price) {
        printf("You can buy it. You will have %u piastres left.\n",
               total - shop[choice].price);
    } else {
        printf("You need %u more piastres.\n",
               shop[choice].price - total);
    }
}

static void bankReport(void) {
    uint8_t i;
    uint8_t affordable = 0U;
    uint8_t tallest = biggestPile();
    uint32_t total = bankTotal();
    uint32_t totalCoins = sumCoins(coinCount, COIN_KINDS);

    printf("\nBank report\n");
    printf("Total money: %u piastres\n", total);
    printf("Coins inside: %u\n", totalCoins);
    printf("Tallest pile: %u piastres\n", COIN_VALUE[tallest]);

    for (i = 0U; i < TOY_COUNT; ++i) {
        if (shop[i].price <= total) {
            ++affordable;
        }
    }

    printf("Toys you can afford today: %u\n", affordable);
}

int main(void) {
    int choice = 0;

    seedBank();

    do {
        printf("\nPiggy Bank Menu\n");
        printf("1) Add coins\n");
        printf("2) Take coins\n");
        printf("3) Show bank\n");
        printf("4) Buy toy\n");
        printf("5) Bank report\n");
        printf("6) Reset bank\n");
        printf("0) Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Please enter a valid number.\n");
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1:
                addCoins();
                break;
            case 2:
                takeCoins();
                break;
            case 3:
                showBank();
                break;
            case 4:
                buyToy();
                break;
            case 5:
                bankReport();
                break;
            case 6:
                seedBank();
                printf("The bank has been reset.\n");
                break;
            case 0:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
