#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Fonction de comparaison pour qsort
int cmpfunc(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

// Affiche toutes les permutations possibles dans l'ordre alphabétique
void permute(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
        return;
    }
    
    for (int i = l; i <= r; i++) {
        // Pour éviter les permutations dupliquées, on ne permute que si le caractère n'a pas déjà été utilisé
        int unique = 1;
        for (int j = l; j < i; j++) {
            if (str[j] == str[i]) {
                unique = 0;
                break;
            }
        }
        if (!unique) continue;
        
        // Échange
        char temp = str[l];
        str[l] = str[i];
        str[i] = temp;
        
        permute(str, l + 1, r);
        
        // Retour à la configuration initiale (backtracking)
        temp = str[l];
        str[l] = str[i];
        str[i] = temp;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <chaine>\n", argv[0]);
        return 1;
    }
    
    char *input = argv[1];
    int n = strlen(input);
    
    // Copier la chaîne dans un buffer modifiable
    char *str = malloc((n + 1) * sizeof(char));
    strcpy(str, input);
    
    // Trier la chaîne pour démarrer avec la plus petite permutation lexicographique
    qsort(str, n, sizeof(char), cmpfunc);
    
    permute(str, 0, n - 1);
    
    free(str);
    return 0;
}
