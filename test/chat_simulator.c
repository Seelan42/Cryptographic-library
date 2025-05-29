#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/crypto.h"

#define MAX_LEN 1024

// ANSI escape codes for colors
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_MAGENTA "\033[1;35m"

void print_hex(const char *label, uint8_t *data, size_t len) {
    printf(COLOR_YELLOW "%s: " COLOR_RESET, label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void encrypt_message(const char *message, const char *password, uint8_t **encrypted, size_t *len_out) {
    uint8_t key[16];
    key_from_password(password, key);

    size_t msg_len = strlen(message);
    size_t padded_len = (msg_len % 16 == 0) ? msg_len : (msg_len + (16 - msg_len % 16));

    *encrypted = calloc(padded_len, 1);
    uint8_t *plain_padded = calloc(padded_len, 1);
    memcpy(plain_padded, message, msg_len);

    for (size_t i = 0; i < padded_len; i += 16) {
        aes_encrypt(plain_padded + i, key, (*encrypted) + i);
    }

    *len_out = padded_len;
    free(plain_padded);
}

void decrypt_message(uint8_t *encrypted, size_t len, const char *password, char **output) {
    uint8_t key[16];
    key_from_password(password, key);

    uint8_t *decrypted = calloc(len, 1);

    for (size_t i = 0; i < len; i += 16) {
        aes_decrypt(encrypted + i, key, decrypted + i);
    }

    *output = (char *)decrypted;
}

void clear_screen() {
    printf("\033[2J\033[1;1H");
}

void print_header(const char *username) {
    clear_screen();
    printf(COLOR_MAGENTA "╔════════════════════════════════════════════════╗\n");
    printf("║       AES Encrypted Chat Simulator (TUI)      ║\n");
    printf("╚════════════════════════════════════════════════╝\n" COLOR_RESET);
    printf(COLOR_GREEN "Logged in as: %s\n" COLOR_RESET, username);
}

int main() {
    char user_input[MAX_LEN];
    char user_name[128];
    char password[128];

    printf(COLOR_CYAN "Enter Username: " COLOR_RESET);
    fgets(user_name, sizeof(user_name), stdin);
    user_name[strcspn(user_name, "\n")] = '\0';

    printf(COLOR_CYAN "Enter a shared password: " COLOR_RESET);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    print_header(user_name);

    while (1) {
        printf(COLOR_MAGENTA "\n╭────────────────────────────────────────────╮\n");
        printf("│ [User A] Enter message (or 'exit'): " COLOR_RESET);
        fgets(user_input, sizeof(user_input), stdin);
        user_input[strcspn(user_input, "\n")] = '\0';

        if (strcmp(user_input, "exit") == 0) break;

        // Encrypt
        uint8_t *encrypted = NULL;
        size_t encrypted_len;
        encrypt_message(user_input, password, &encrypted, &encrypted_len);

        printf(COLOR_YELLOW "├────────────────────────────────────────────\n");
        printf("│ [Encrypted Hex] ");
        print_hex("", encrypted, encrypted_len);

        // Decrypt simulation
        char *decrypted_msg = NULL;
        decrypt_message(encrypted, encrypted_len, password, &decrypted_msg);

        printf(COLOR_GREEN "│ [User B Received] %.*s\n", (int)encrypted_len, decrypted_msg);
        printf(COLOR_MAGENTA "╰────────────────────────────────────────────╯\n" COLOR_RESET);

        free(encrypted);
        free(decrypted_msg);
    }

    printf(COLOR_MAGENTA "\nChat ended. Goodbye!\n" COLOR_RESET);
    return 0;
}

// #include <stdio.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ncurses.h>
// #include "../include/crypto.h"

// #define MAX_LEN 1024

// void print_hex_to_window(WINDOW *win, const char *label, uint8_t *data, size_t len) {
//     wprintw(win, "%s: ", label);
//     for (size_t i = 0; i < len; i++) {
//         wprintw(win, "%02X ", data[i]);
//     }
//     wprintw(win, "\n");
//     wrefresh(win);
// }

// void encrypt_message(const char *message, const char *password, uint8_t **encrypted, size_t *len_out) {
//     uint8_t key[16];
//     key_from_password(password, key);

//     size_t msg_len = strlen(message);
//     size_t padded_len = (msg_len % 16 == 0) ? msg_len : (msg_len + (16 - msg_len % 16));

//     *encrypted = calloc(padded_len, 1);
//     uint8_t *plain_padded = calloc(padded_len, 1);
//     memcpy(plain_padded, message, msg_len);

//     for (size_t i = 0; i < padded_len; i += 16) {
//         aes_encrypt(plain_padded + i, key, (*encrypted) + i);
//     }

//     *len_out = padded_len;
//     free(plain_padded);
// }

// void decrypt_message(uint8_t *encrypted, size_t len, const char *password, char **output) {
//     uint8_t key[16];
//     key_from_password(password, key);

//     uint8_t *decrypted = calloc(len, 1);

//     for (size_t i = 0; i < len; i += 16) {
//         aes_decrypt(encrypted + i, key, decrypted + i);
//     }

//     *output = (char *)decrypted;
// }

// int main() {
//     char user_input[MAX_LEN];
//     char user_name[128];
//     char password[128];

//     // Get username and password before ncurses starts
//     printf("Enter Username: ");
//     fgets(user_name, sizeof(user_name), stdin);
//     user_name[strcspn(user_name, "\n")] = '\0';

//     printf("Enter a shared password: ");
//     fgets(password, sizeof(password), stdin);
//     password[strcspn(password, "\n")] = '\0';

//     // Start ncurses
//     initscr();
//     cbreak();
//     noecho();
//     keypad(stdscr, TRUE);
//     curs_set(1);

//     int rows, cols;
//     getmaxyx(stdscr, rows, cols);

//     // Create windows
//     WINDOW *msg_win = newwin(rows - 3, cols, 0, 0);
//     WINDOW *input_win = newwin(3, cols, rows - 3, 0);

//     scrollok(msg_win, TRUE);
//     box(input_win, 0, 0);
//     wrefresh(msg_win);
//     wrefresh(input_win);

//     // Greeting
//     wprintw(msg_win, "Welcome, %s! Type 'exit' to quit.\n", user_name);
//     wrefresh(msg_win);

//     while (1) {
//         // Clear and prepare input box
//         werase(input_win);
//         box(input_win, 0, 0);
//         mvwprintw(input_win, 1, 1, "> ");
//         wrefresh(input_win);

//         // Get user input
//         wgetnstr(input_win, user_input, MAX_LEN - 1);

//         if (strcmp(user_input, "exit") == 0) break;

//         // Encrypt
//         uint8_t *encrypted = NULL;
//         size_t encrypted_len;
//         encrypt_message(user_input, password, &encrypted, &encrypted_len);

//         // Display encrypted hex
//         wprintw(msg_win, "[Encrypted] ");
//         print_hex_to_window(msg_win, "", encrypted, encrypted_len);

//         // Decrypt
//         char *decrypted_msg = NULL;
//         decrypt_message(encrypted, encrypted_len, password, &decrypted_msg);

//         // Display decrypted
//         wprintw(msg_win, "[Decrypted] %.*s\n", (int)encrypted_len, decrypted_msg);
//         wrefresh(msg_win);

//         free(encrypted);
//         free(decrypted_msg);
//     }

//     delwin(msg_win);
//     delwin(input_win);
//     endwin();
//     printf("Chat ended.\n");
//     return 0;
// }
