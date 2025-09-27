#include <stdio.h>
#include <math.h>

// Factorial function (recursion)
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    int choice;
    double x, y;

    do {
        printf("\n--- Scientific Calculator ---\n");
        printf("1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n");
        printf("5. Power\n6. Square Root\n7. Sine\n8. Cosine\n9. Tangent\n");
        printf("10. Natural Log\n11. Base-10 Log\n12. Factorial\n13. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 13) break;

        switch (choice) {
            case 1: case 2: case 3: case 4: case 5:
                printf("Enter two numbers: ");
                scanf("%lf %lf", &x, &y);
                if (choice == 1) printf("Result = %.2lf\n", x + y);
                else if (choice == 2) printf("Result = %.2lf\n", x - y);
                else if (choice == 3) printf("Result = %.2lf\n", x * y);
                else if (choice == 4) {
                    if (y != 0) printf("Result = %.2lf\n", x / y);
                    else printf("Error: Division by zero!\n");
                }
                else if (choice == 5) printf("Result = %.2lf\n", pow(x, y));
                break;

            case 6:
                printf("Enter number: ");
                scanf("%lf", &x);
                if (x >= 0) printf("Result = %.2lf\n", sqrt(x));
                else printf("Error: Negative input for sqrt!\n");
                break;

            case 7: case 8: case 9:
                printf("Enter angle in radians: ");
                scanf("%lf", &x);
                if (choice == 7) printf("Result = %.2lf\n", sin(x));
                else if (choice == 8) printf("Result = %.2lf\n", cos(x));
                else printf("Result = %.2lf\n", tan(x));
                break;

            case 10:
                printf("Enter number: ");
                scanf("%lf", &x);
                if (x > 0) printf("Result = %.2lf\n", log(x));
                else printf("Error: Non-positive input for log!\n");
                break;

            case 11:
                printf("Enter number: ");
                scanf("%lf", &x);
                if (x > 0) printf("Result = %.2lf\n", log10(x));
                else printf("Error: Non-positive input for log10!\n");
                break;

            case 12:
                printf("Enter an integer: ");
                scanf("%lf", &x);
                if (x >= 0 && floor(x) == x)
                    printf("Result = %lld\n", factorial((int)x));
                else
                    printf("Error: Factorial requires non-negative integer!\n");
                break;

            default: printf("Invalid choice!\n");
        }

    } while (1);

    printf("Calculator closed.\n");
    return 0;
}
