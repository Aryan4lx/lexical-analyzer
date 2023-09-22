#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    STRING
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

Token getNextToken(FILE *file)
{
    Token token;
    char currentChar = fgetc(file);
    int lexemeIndex = 0;

    while (isspace(currentChar))
        currentChar = fgetc(file);

    if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')
    {
        token.type = OPERATOR;
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        return token;
    }

    if (currentChar == ';' || currentChar == ',' || currentChar == '(' || currentChar == ')' || currentChar == '.')
    {
        token.type = DELIMITER;
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        return token;
    }

    if (currentChar == '"')
    {
        token.type = STRING;
        while ((currentChar = fgetc(file)) != '"' && lexemeIndex < MAX_TOKEN_LENGTH - 1)
        {
            token.lexeme[lexemeIndex++] = currentChar;
        }
        token.lexeme[lexemeIndex] = '\0';
        return token;
    }

    if (isdigit(currentChar))
    {
        token.type = NUMBER;
        while (isdigit(currentChar))
        {
            token.lexeme[lexemeIndex++] = currentChar;
            currentChar = fgetc(file);
        }
        token.lexeme[lexemeIndex] = '\0';
        ungetc(currentChar, file);
        return token;
    }

    if (isalpha(currentChar))
    {
        token.type = IDENTIFIER;
        while (isalnum(currentChar))
        {
            token.lexeme[lexemeIndex++] = currentChar;
            currentChar = fgetc(file);
        }
        token.lexeme[lexemeIndex] = '\0';
        ungetc(currentChar, file);

        if (strcmp(token.lexeme, "var") == 0 || strcmp(token.lexeme, "console") == 0)
        {
            token.type = KEYWORD;
        }
        return token;
    }

    token.type = KEYWORD;
    token.lexeme[0] = '\0';
    return token;
}

int main()
{
    FILE *file;
    Token token;

    file = fopen("input.js", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    while (!feof(file))
    {
        token = getNextToken(file);

        switch (token.type)
        {
        case KEYWORD:
            printf("Keyword: %s\n", token.lexeme);
            break;
        case IDENTIFIER:
            printf("Identifier: %s\n", token.lexeme);
            break;
        case NUMBER:
            printf("Number: %s\n", token.lexeme);
            break;
        case OPERATOR:
            printf("Operator: %s\n", token.lexeme);
            break;
        case DELIMITER:
            printf("Delimiter: %s\n", token.lexeme);
            break;
        case STRING:
            printf("String: %s\n", token.lexeme);
            break;
        }
    }

    fclose(file);
    return 0;
}
