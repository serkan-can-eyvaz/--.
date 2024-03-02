#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Method {
    char name[32];
    char return_type[32];
};

struct Class {
    char name[32];
    int num_variables;
    int num_methods;
    char** variables;
    struct Method* methods;
};

// Tokenizasyon iþlemi için yardýmcý fonksiyon
int tokenize(char line[], char** tokens) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    return i;
}

int main() {
    FILE* file = fopen("siniflar.txt", "r");
    if (!file) {
        printf("Error: Failed to open input file");
        return 1;
    }
    
    struct Class current_class = {0};
    int is_defining_class = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* tokens[16];
        int num_tokens = tokenize(line, tokens);

        if (num_tokens == 0) {
            continue; // Boþ satýr
        }

        if (strcmp(tokens[0], "class") == 0) {
            if (is_defining_class) {
                // Önceki sýnýfýn deðiþkenlerini ve sanal metotlarýný yazdýr
                printf("Degiskenler:\n");
                int i;
                for (i = 0; i < current_class.num_variables; i++) {
                    printf("%s\n", current_class.variables[i]);
                }
                
                printf("Virtual Metotlar:\n");
                for (i = 0; i < current_class.num_methods; i++) {
                    struct Method method = current_class.methods[i];
                    printf("%s %s();\n", method.return_type, method.name);
                }
                
                printf("\n");
            }
            // Yeni bir sýnýf tanýmlanýyor
            strncpy(current_class.name, tokens[1], strlen(tokens[1]) - 1);
            current_class.num_variables = 0;
            current_class.num_methods = 0;
            is_defining_class = 1;
        } else if (strcmp(tokens[0], "int") == 0 ||
                   strcmp(tokens[0], "char") == 0 ||
                   strcmp(tokens[0], "double") == 0) {
            // Deðiþken tanýmlandý
            char* new_variable = malloc(sizeof(char) * 32);
            sprintf(new_variable, "%s %s;", tokens[0], tokens[1]);
            current_class.variables = realloc(current_class.variables, 
                                               sizeof(char*) * (current_class.num_variables + 1));
            current_class.variables[current_class.num_variables] = new_variable;
            current_class.num_variables++;
        } else if (strcmp(tokens[0], "virtual") == 0) {
            // Sanal metot tanýmlandý
            struct Method new_method = {0};
            strncpy(new_method.return_type, tokens[1], strlen(tokens[1]));
            strncpy(new_method.name, tokens[2], strlen(tokens[2]) - 2);
            current_class.num_methods++;
            current_class.methods = realloc(current_class.methods,
            sizeof(struct Method) * current_class.num_methods);
            current_class.methods[current_class.num_methods - 1] = new_method;
        }
    }

    // Son sýnýfýn deðiþkenlerini ve sanal metotlarýný yazdýr
    if (is_defining_class) {
    	int i;
        printf("Degiskenler:\n");
        for (i = 0; i < current_class.num_variables; i++) {
            printf("%s\n", current_class.variables[i]);
        }
        
        printf("Metotlar:\n");
        for (i = 0; i < current_class.num_methods; i++) {
            struct Method method = current_class.methods[i];
            printf("%s %s();\n", method.return_type, method.name);
        }
    }

    fclose(file);
    return 0;
}
