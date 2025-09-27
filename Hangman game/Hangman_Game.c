// Powerful Hangman Game in C
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define EASY_TRIES 10
#define MEDIUM_TRIES 6
#define HARD_TRIES 4

// Struct to hold a word and its hint
struct WordWithHint {
    char word[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];
};

// Function prototypes
void displayWord(const char guessedWord[]);
void drawHangman(int tries);
int chooseDifficulty();
bool playRound(struct WordWithHint wordList[], int wordCount, int maxTries);

int main() {
    srand(time(NULL));

    // Word bank
    struct WordWithHint wordList[] = {
        {"geeksforgeeks", "Computer coding"},
        {"elephant", "A large mammal with a trunk"},
        {"pizza", "A popular Italian dish"},
        {"beach", "Sandy shore by the sea"},
        {"computer", "An electronic device"},
        {"guitar", "A stringed musical instrument"},
        {"python", "A programming language"},
        {"india", "A country in South Asia"},
        {"tiger", "National animal of India"},
        {"sunflower", "A yellow flower that follows the sun"}
    };
    int wordCount = sizeof(wordList) / sizeof(wordList[0]);

    int wins = 0, losses = 0;
    char playAgain;

    printf("🎮 Welcome to Hangman!\n");

    do {
        int maxTries = chooseDifficulty();
        bool won = playRound(wordList, wordCount, maxTries);

        if (won) {
            wins++;
        } else {
            losses++;
        }

        printf("\n📊 Score → Wins: %d | Losses: %d\n", wins, losses);
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &playAgain);

    } while (tolower(playAgain) == 'y');

    printf("\nThanks for playing! Final Score → Wins: %d | Losses: %d\n", wins, losses);
    return 0;
}

// ---------------------- FUNCTIONS ----------------------

int chooseDifficulty() {
    int choice;
    printf("\nSelect Difficulty:\n");
    printf("1. Easy (10 tries)\n");
    printf("2. Medium (6 tries)\n");
    printf("3. Hard (4 tries)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: return EASY_TRIES;
        case 2: return MEDIUM_TRIES;
        case 3: return HARD_TRIES;
        default: 
            printf("Invalid choice. Defaulting to Medium.\n");
            return MEDIUM_TRIES;
    }
}

bool playRound(struct WordWithHint wordList[], int wordCount, int maxTries) {
    int wordIndex = rand() % wordCount;
    const char* secretWord = wordList[wordIndex].word;
    const char* hint = wordList[wordIndex].hint;

    int wordLength = strlen(secretWord);
    char guessedWord[MAX_WORD_LENGTH];
    bool guessedLetters[26] = {false};

    for (int i = 0; i < wordLength; i++) {
        guessedWord[i] = '_';
    }
    guessedWord[wordLength] = '\0';

    printf("\n📝 Hint: %s\n", hint);

    int tries = 0;
    while (tries < maxTries) {
        printf("\n");
        displayWord(guessedWord);
        drawHangman(tries);

        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (!isalpha(guess)) {
            printf("❌ Invalid input! Please enter a letter.\n");
            continue;
        }

        if (guessedLetters[guess - 'a']) {
            printf("⚠️ Already guessed '%c'. Try another.\n", guess);
            continue;
        }

        guessedLetters[guess - 'a'] = true;
        bool found = false;

        for (int i = 0; i < wordLength; i++) {
            if (secretWord[i] == guess) {
                guessedWord[i] = guess;
                found = true;
            }
        }

        if (found) {
            printf("✅ Good guess!\n");
        } else {
            printf("❌ '%c' not in word.\n", guess);
            tries++;
        }

        if (strcmp(secretWord, guessedWord) == 0) {
            printf("\n🎉 Congratulations! You guessed the word: %s\n", secretWord);
            return true;
        }
    }

    drawHangman(tries);
    printf("\n💀 You ran out of tries! The word was: %s\n", secretWord);
    return false;
}

void displayWord(const char guessedWord[]) {
    printf("Word: ");
    for (int i = 0; guessedWord[i] != '\0'; i++) {
        printf("%c ", guessedWord[i]);
    }
    printf("\n");
}

void drawHangman(int tries) {
    const char* hangmanParts[] = {
        "     _________",
        "    |         |",
        "    |         O",
        "    |        /|\\",
        "    |        / \\",
        "    |"
    };

    printf("\n");
    for (int i = 0; i <= tries && i < 6; i++) {
        printf("%s\n", hangmanParts[i]);
    }
}
