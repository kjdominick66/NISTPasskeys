#include "generator.h"

// In this generator, I could use a built-in randomizer.
// For the sake of this class I will use /dev/random

// /dev/random helper function
uint32_t dev_random (uint32_t max) {
    FILE *inFile = fopen("/dev/random", "rb");
    if (inFile == NULL) {
        printf("Failed to open /dev/random");
        exit(1);
    }

    uint32_t value;
    if (fread(&value, sizeof(value), 1, inFile) != 1) {
        printf("Failed to read from /dev/random");
        fclose(inFile);
        exit(1);
    }

    fclose(inFile);
    return value % max;
}

void generate_passphrase (char* passphrase) {
    FILE *inFile = fopen("words.txt", "r");
    if (inFile == NULL) {
        printf("Failed to open word dictionary file.");
        exit(1);
    }

    // Array to store all words from dictionary
    int total_words = 5000;
    char words[total_words][6];
    int count = 0;

    // Parse all words from dictionary
    while (count < total_words && fgets((words[count]), sizeof(words[count]),inFile)) {
        // Remove special characters
        words[count][strcspn(words[count], "\r\n")] = '\0';
        if (strlen(words[count]) == 5) {
            count++;
        }
    }

    fclose(inFile);

    // Generate 6 random words from the dictionary using /dev/random
    passphrase[0] = '\0';
    for (int i = 0; i < 6; i++) {
        int index = dev_random(count);
        strcat(passphrase, words[index]);
        if (i != 5) {
            strcat(passphrase, "-");
        }
    }
}