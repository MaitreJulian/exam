#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20   // Taille maximale du plateau

// Vérifie si une reine peut être placée à (ligne, colonne)
int safe(int queens[], int ligne, int colonne) {
    int i = 0;
    while (i < ligne) 
    {
        if (queens[i] == colonne) 
            return 0;
        if (abs(queens[i] - colonne) == ligne - i) 
            return 0;
        i++;
    }
    return 1;
}

// Affiche une solution
void printSolution(int queens[], int n) {
    static int sol = 1;
    printf("Solution %d:\n", sol++);
    int i = 0;
    while (i < n)
    {
        printf("%d", queens[i]);
        if(!(i == n - 1))
            printf(" ");
        i++;
    }
    printf("\n");
}

// Backtracking pour placer les reines
void solve(int queens[], int ligne, int n) {
    if (ligne == n) {
        printSolution(queens, n);
        return;
    }

    int colonne = 0;
    while (colonne < n) {
        if (safe(queens, ligne, colonne)) {
            queens[ligne] = colonne;         // Place une reine
            solve(queens, ligne + 1, n); // Appel récursif
        }
        colonne++;
    }
}

int main()
{
    int n;
    int queens[MAX]; // queens[i] = colonne de la reine sur la ligne i
    
    printf("Entrer le nombre de reines (N) : ");
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("N doit être entre 1 et %d\n", MAX);
        return 1;
    }
    solve(queens, 0, n);
    return 0;
}
