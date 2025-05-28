#include <stdio.h>
#include <string.h>
#include "tables.c"

#define BUFFER_SIZE 128
#define START_FINAL_STATES 12
#define FAIL_STATE 19
#define STATE_TOKENID_DIFFERENCE 9

/*
  maps the ASCII values of the DFA alphabet to their respective index

  @charToIndex: pointer to the array where the values will be mapped

  Return: none
*/
void mapSymbols(int charToIndex[128])
{
  // any characters out of the alphabet
  for (int i = 0; i < 128; i++)
    charToIndex[i] = 16;

  // A-Z
  for (int i = 'A'; i < 'Z'; i++)
    charToIndex[i] = 3;

  // a-z
  for (int i = 'a'; i < 'z'; i++)
    charToIndex[i] = 3;

  // 0-9
  for (int i = '0'; i < '9'; i++)
    charToIndex[i] = 4;

  // special characters
  charToIndex['_'] = 5;
  charToIndex['('] = 6;
  charToIndex[')'] = 7;
  charToIndex['{'] = 8;
  charToIndex['}'] = 9;

  // ignoring strings
  charToIndex['"'] = 10;
  charToIndex['\''] = 11;
  charToIndex['`'] = 12;

  // space delimiters
  charToIndex[' '] = 0;
  charToIndex['\n'] = 1;
  charToIndex['\t'] = 2;

  // comments
  charToIndex['/'] = 13;
  charToIndex['*'] = 14;
  charToIndex['#'] = 15;

  /*
    special case for EOF, as some languages like python
    it's possible there is no delimiter between a lexeme and EOF

    e.g.
    a = b + c
            ^
            |
    this is valid final line in python, there is no delimiter
  */
  charToIndex[EOF] = 0;
}

/*
  Determines if the DFA should consume the next character

  @state: current state
  @ch: current character

  e.g.
  if we have reached a delimiter for a lexeme, it will be consumed if it is a space,
  but it will be kept if it is a semicolon

  Return: 1 if it should advance, 0 if not
*/
int advance(int state, char ch)
{
  return ch != EOF && (state != 17) && (state != 18) &&
         (state != START_FINAL_STATES ||
          (state == START_FINAL_STATES && ch == ' ')) &&
         !(state == 19 && ch == '\n');
}

/*
  Determines if a state is accepted

  @state: current state

  states 12-17 are accepted

  Return: 1 if it is accepted, 0 if not
*/
int accept(int state)
{
  return state >= START_FINAL_STATES && state < FAIL_STATE;
}

/*
  Determines if a character should be stored in the buffer

  @state: current state
  @ch: current character value
  @len: current buffer length

  characters that will be relevant later on are stored in the buffer, such as unrecognized chars for
  error messages or an identifier for the symbol table
  stops buffering if the lexeme is over 128 chars

  Return: 1 if it should buffer it, 0 if not
*/
int shouldBuffer(int state, char ch, int len)
{
  return len < BUFFER_SIZE - 1 && ch != '\n' &&
             (state != 10 && state != 11) &&
             (state != START_FINAL_STATES) ||
         ((state == 5 || state == 6 || state == 9));
}

/*
  Gets the token id for a identifier or reserved word

  @buffer: buffer that stores the lexeme

  Return: token id
*/
int getWordId(char *buffer)
{
  int id = 0;
  if (!strcmp(buffer, "class"))
    id = 1;
  else if (!strcmp(buffer, "def"))
    id = 2;
  else if (!strcmp(buffer, "main"))
    id = 3;
  return id;
}

/*
  Gets the token id based on the state

  @state: final state
  @buffer: buffer that stores the lexeme

  Return: token id
*/
int getTokenId(int state, char *buffer)
{
  int id;
  if (state == START_FINAL_STATES)
    id = getWordId(buffer);
  else
    id = state - STATE_TOKENID_DIFFERENCE;
  return id;
}

/*
  Writes the result to a file

  @filename: name of the output file
  @tokens: token table
  @identifiers: symbol table
  @errors: error table

  Return: none
*/
void saveToFile(char *filename, tokenTable *tokens, charTable *identifiers, charTable *errors)
{
  FILE *file = fopen(filename, "w");

  fprintf(file, "Tokens:\n");
  for (int i = 0; i < tokens->position; i++)
    fprintf(file, "<%d, %d>\n", tokens->tokens[i][0], tokens->tokens[i][1]);

  fprintf(file, "\nSymbols:\n");
  for (int i = 0; i < identifiers->position; i++)
    fprintf(file, "%d: %s\n", i, identifiers->symbols[i]);

  fprintf(file, "\nErrors:");
  for (int i = 0; i < errors->position; i++)
    fprintf(file, "\nLexeme %s not recognized", errors->symbols[i]);
}

/*
  Main method

  @argv[0]: filename of the input
  @argv[1]: filename of the output

  If the output file doesn't exist, it will be created
*/
int main(int argc, char **argv)
{
  static const int transitionTable[12][17] = {
      {19, 10, 19, 1, 19, 1, 13, 14, 15, 16, 2, 3, 4, 5, 19, 9, 19},
      {12, 12, 12, 1, 1, 1, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
      {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 19, 2, 2, 2, 2, 2, 2},
      {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 19, 3, 3, 3, 3, 3},
      {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 19, 4, 4, 4, 4},
      {19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 6, 7, 19, 19},
      {6, 19, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
      {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 7, 7},
      {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 19, 7, 7, 7},
      {9, 19, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
      {11, 10, 11, 18, 19, 18, 13, 14, 15, 16, 2, 3, 4, 5, 19, 9, 19},
      {11, 10, 11, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 5, 17, 9, 17},
  };

  int charToIndex[128];
  mapSymbols(charToIndex);

  // initialization of tables (definition on tables.c)
  tokenTable *tokens = initTokenTable();
  charTable *identifiers = initCharTable();
  charTable *errors = initCharTable();

  // buffer to temporarily store lexemes
  char buffer[BUFFER_SIZE];
  int bufferLen;

  int state;
  char ch;
  int tokenid;
  FILE *fileptr = fopen(argv[1], "r");
  ch = fgetc(fileptr);
  int charVal = charToIndex[ch];
  int symbolIndex;

  /*
    DFA simulation
    based on the pseudocode from:
      R. Castelló, Class Lecture, Topic: “Chapter 2 – Lexical Analysis.” TC3002,
      School of Engineering and Science, ITESM, Zapopan, Jalisco, April, 2025.
  */
  while (ch != EOF)
  {
    state = 0;
    bufferLen = 0;

    // states >= 11 are final
    while (state < START_FINAL_STATES)
    {
      state = transitionTable[state][charVal];
      if (shouldBuffer(state, ch, bufferLen))
        buffer[bufferLen++] = ch;
      if (advance(state, ch))
      {
        ch = fgetc(fileptr);
        charVal = charToIndex[ch];
      }
    }
    // adding an end of string to the buffer
    buffer[bufferLen] = '\0';
    if (accept(state))
    {
      tokenid = getTokenId(state, buffer);

      // if it is an identifier, add it to the symbol table and get the index
      // if not, use -1
      if (tokenid == 0)
        symbolIndex = recordLexeme(identifiers, buffer);
      else
        symbolIndex = -1;

      recordToken(tokens, tokenid, symbolIndex);
    }
    else
    {
      buffer[bufferLen] = '\0';
      recordLexeme(errors, buffer);
    }
  }
  saveToFile(argv[2], tokens, identifiers, errors);
}