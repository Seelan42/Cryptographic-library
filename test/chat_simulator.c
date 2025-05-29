#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/crypto.h"

#define MAX_LEN 1024

void print_hex(const char *label, uint8_t *data, size_t len) {
    printf("%s: ", label);
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

int main() {
    char user_input[MAX_LEN];
    char password[128];

    printf("Enter a shared password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (1) {
        printf("\n[User A] Enter message (or 'exit'): ");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[strcspn(user_input, "\n")] = '\0';

        if (strcmp(user_input, "exit") == 0) break;

        // Encrypt
        uint8_t *encrypted = NULL;
        size_t encrypted_len;
        encrypt_message(user_input, password, &encrypted, &encrypted_len);

        printf("[Encrypted Message] ");
        print_hex("", encrypted, encrypted_len);

        // Simulate sending to User B
        char *decrypted_msg = NULL;
        decrypt_message(encrypted, encrypted_len, password, &decrypted_msg);

        printf("[User B Received] %.*s\n", (int)encrypted_len, decrypted_msg);

        free(encrypted);
        free(decrypted_msg);
    }

    printf("Chat ended.\n");
    return 0;
}

