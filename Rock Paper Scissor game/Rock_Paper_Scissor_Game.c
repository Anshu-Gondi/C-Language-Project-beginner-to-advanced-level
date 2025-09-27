// Rock-Paper-Scissors Game in C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int game(char you, char computer) {
    if (you == computer)
        return -1;

    if ((you == 's' && computer == 'z') || 
        (you == 'p' && computer == 's') || 
        (you == 'z' && computer == 'p')) {
        return 1; // user wins
    }
    return 0; // computer wins
}

const char* choiceName(char c) {
    switch (c) {
        case 's': return "Stone";
        case 'p': return "Paper";
        case 'z': return "Scissor";
        default: return "Invalid";
    }
}

char computerChoice() {
    int n = rand() % 3; // 0,1,2
    if (n == 0) return 's';
    else if (n == 1) return 'p';
    else return 'z';
}

int main() {
    srand(time(NULL));
    int wins = 0, losses = 0, draws = 0;
    char playAgain;

    printf("🎮 Welcome to Rock–Paper–Scissors!\n");

    do {
        char you, computer;
        int result;

        printf("\nEnter s for STONE, p for PAPER, z for SCISSOR: ");
        scanf(" %c", &you);
        you = tolower(you);

        if (you != 's' && you != 'p' && you != 'z') {
            printf("❌ Invalid choice! Try again.\n");
            continue;
        }

        computer = computerChoice();
        result = game(you, computer);

        printf("\nYou chose: %s", choiceName(you));
        printf("\nComputer chose: %s\n", choiceName(computer));

        if (result == -1) {
            printf("🤝 It's a Draw!\n");
            draws++;
        } else if (result == 1) {
            printf("🎉 You Win!\n");
            wins++;
        } else {
            printf("💀 You Lose!\n");
            losses++;
        }

        printf("\n📊 Score → Wins: %d | Losses: %d | Draws: %d\n", wins, losses, draws);

        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playAgain);

    } while (tolower(playAgain) == 'y');

    printf("\n👋 Thanks for playing! Final Score → Wins: %d | Losses: %d | Draws: %d\n", wins, losses, draws);

    return 0;
}
