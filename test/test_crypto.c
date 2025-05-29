#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "crypto.h"

void print_hex(const char *label, uint8_t *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main() {
    char plaintext_input[1024];

    printf("Enter a message (up to 1023 characters): ");
    fgets(plaintext_input, sizeof(plaintext_input), stdin);
    plaintext_input[strcspn(plaintext_input, "\n")] = 0;

    // Derive key from first 16 bytes of the message
    char keysource[17] = {0}; // 16 bytes + null terminator
    strncpy(keysource, plaintext_input, 16);
    uint8_t key[16];
    key_from_password(keysource, key);

    // Padding to a multiple of 16
    size_t input_len = strlen(plaintext_input);
    size_t padded_len = (input_len % 16 == 0) ? input_len : (input_len + (16 - (input_len % 16)));

    uint8_t *plaintext = calloc(padded_len, 1);
    uint8_t *encrypted = calloc(padded_len, 1);
    uint8_t *decrypted = calloc(padded_len, 1);
    memcpy(plaintext, plaintext_input, input_len);  // null-padded

    // Show info
    printf("\nOriginal plaintext: '%s'\n", plaintext_input);
    print_hex("Derived key", key, 16);

    // Encrypt block-by-block
    for (size_t i = 0; i < padded_len; i += 16) {
        aes_encrypt(plaintext + i, key, encrypted + i);
    }
    print_hex("Encrypted (hex)", encrypted, padded_len);

    // Decrypt
    for (size_t i = 0; i < padded_len; i += 16) {
        aes_decrypt(encrypted + i, key, decrypted + i);
    }
    print_hex("Decrypted (hex)", decrypted, padded_len);

    printf("Decrypted text: '%.*s'\n", (int)padded_len, decrypted);

    free(plaintext);
    free(encrypted);
    free(decrypted);
    return 0;
}

