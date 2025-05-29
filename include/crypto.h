#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>

// Encrypt 16-byte input block with 16-byte key
void aes_encrypt(uint8_t *input, uint8_t *key, uint8_t *output);

// Decrypt 16-byte ciphertext block with 16-byte key
void aes_decrypt(uint8_t *input, uint8_t *key, uint8_t *output);

// Derive 16-byte key from password
void key_from_password(const char *password, uint8_t *key);
// Example crypto.h
void aes_encrypt(uint8_t *input, uint8_t *key, uint8_t *output);
void aes_decrypt(uint8_t *input, uint8_t *key, uint8_t *output);
void key_from_password(const char *password, uint8_t *key); // should fill 16 bytes

#endif
