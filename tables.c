#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SIZE 50

struct tokenTable
{
  size_t position;
  size_t size;
  int (*tokens)[2];
};
typedef struct tokenTable tokenTable;

tokenTable *initTokenTable()
{
  tokenTable *table = malloc(sizeof(tokenTable));

  table->position = 0;
  table->size = DEFAULT_SIZE;

  table->tokens = malloc(table->size * sizeof(int[2]));

  return table;
}

/*
  Saves an entry in the token table

  @tokenTable: token table
  @id: token id to be stored
  @symbolEntry: index of the entry in the symbol table

  if the table has reached its size limit, it doubles in capacity

  Return: none
*/
void recordToken(tokenTable *tokenTable, int tokenId, int symbolEntry)
{
  if (tokenTable->position >= tokenTable->size)
  {
    tokenTable->size *= 2;
    int (*newTokens)[2] = realloc(tokenTable->tokens, tokenTable->size * sizeof(int[2]));
    tokenTable->tokens = newTokens;
  }

  tokenTable->tokens[tokenTable->position][0] = tokenId;
  tokenTable->tokens[tokenTable->position][1] = symbolEntry;
  tokenTable->position++;
}

/*
  This struct is used for both the symbol table and the errors table
*/
struct charTable
{
  size_t position;
  size_t size;
  char **symbols;
};
typedef struct charTable charTable;

charTable *initCharTable()
{
  charTable *table = malloc(sizeof(charTable));
  table->position = 0;
  table->size = DEFAULT_SIZE;
  table->symbols = malloc(table->size * sizeof(char *));

  return table;
}

/*
  Saves an entry in the symbol or the errors table

  @identifiers: table to be used
  @buffer: buffer that stores the string to be saved

  if the table has reached its size limit, it doubles in capacity

  Return: index of the entry
*/
int recordLexeme(charTable *charTable, char *buffer)
{

  if (charTable->position >= charTable->size)
  {
    charTable->size *= 2;
    char **newSymbols = realloc(charTable->symbols, charTable->size * sizeof(char *));
    charTable->symbols = newSymbols;
  }

  charTable->symbols[charTable->position] = strdup(buffer);

  return charTable->position++;
}