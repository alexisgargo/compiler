#include <stdio.h>
#include <string.h>
#include "tables.c"

#define BUFFER_SIZE 128
#define START_FINAL_STATES 10
#define FAIL_STATE 12
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
    charToIndex[i] = 9;

  // A-Z
  for (int i = 'A'; i < 'Z'; i++)
    charToIndex[i] = 8;

  // a-z
  for (int i = 'a'; i < 'z'; i++)
    charToIndex[i] = 8;

  // 0-9
  for (int i = '0'; i < '9'; i++)
    charToIndex[i] = 8;

  charToIndex['\n'] = 0;
  charToIndex['d'] = 1;
  charToIndex['e'] = 2;
  charToIndex['f'] = 3;
  charToIndex['c'] = 4;
  charToIndex['l'] = 5;
  charToIndex['a'] = 6;
  charToIndex['s'] = 7;
  charToIndex['_'] = 8;

  /*
    special case for EOF, as some languages like python
    it's possible there is no delimiter between a lexeme and EOF

    e.g.
    a = b + c
            ^
            |
    this is valid final line in python, there is no delimiter
  */
  charToIndex[EOF] = 10;
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
  return state < START_FINAL_STATES;
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
  Gets the token id based on the state

  @state: final state
  @buffer: buffer that stores the lexeme

  Return: token id
*/
int getTokenId(int state)
{
  return state - STATE_TOKENID_DIFFERENCE;
}

/*
  Writes the result to a file

  @filename: name of the output file
  @tokens: token table
  @identifiers: symbol table
  @errors: error table

  Return: none
*/
// void saveToFile(char *filename, tokenTable *tokens, charTable *identifiers, charTable *errors)
void saveToFile(char *filename, tokenTable *tokens)
{
  FILE *file = fopen(filename, "w");

  int pp = 0;
  int oop = 0;

  fprintf(file, "Tokens:\n");
  for (int i = 0; i < tokens->position; i++)
  {
    fprintf(file, "<%d, %d>\n", tokens->tokens[i][0], tokens->tokens[i][1]);
    if (tokens->tokens[i][0] == 1)
    {
      pp = 1;
    }
    else if (tokens->tokens[i][0] == 2)
    {
      oop = 1;
    }
  }
}

/*
  Main method

  @argv[0]: filename of the input
  @argv[1]: filename of the output

  If the output file doesn't exist, it will be created
*/
int main(int argc, char **argv)
{
  static const int transitionTable[10][11] = {
      {0, 2, 1, 1, 5, 1, 1, 1, 1, 1, 12},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 12},
      {0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 12},
      {0, 1, 1, 4, 1, 1, 1, 1, 1, 1, 12},
      {10, 1, 1, 1, 1, 1, 1, 1, 1, 10, 12},
      {0, 1, 1, 1, 1, 6, 1, 1, 1, 1, 12},
      {0, 1, 1, 1, 1, 1, 7, 1, 1, 1, 12},
      {0, 1, 1, 1, 1, 1, 1, 8, 1, 1, 12},
      {0, 1, 1, 1, 1, 1, 1, 9, 1, 1, 12},
      {11, 1, 1, 1, 1, 1, 1, 1, 1, 11, 12}};

  int charToIndex[128];
  mapSymbols(charToIndex);

  // initialization of tables (definition on tables.c)
  tokenTable *tokens = initTokenTable();
  charTable *identifiers = initCharTable();
  charTable *errors = initCharTable();

  int state;
  char ch;
  int tokenid;
  FILE *fileptr = fopen(argv[1], "r");
  // FILE *fileptr = fopen("tests/t.py", "r");
  ch = fgetc(fileptr);
  int charVal = charToIndex[ch];
  int symbolIndex;

  int pp = 0;
  int oop = 0;

  /*
    DFA simulation
    based on the pseudocode from:
      R. Castelló, Class Lecture, Topic: “Chapter 2 – Lexical Analysis.” TC3002,
      School of Engineering and Science, ITESM, Zapopan, Jalisco, April, 2025.
  */
  while (ch != EOF)
  {
    state = 0;
    while (state < START_FINAL_STATES)
    {
      state = transitionTable[state][charVal];
      if (advance(state, ch))
      {
        ch = fgetc(fileptr);
        charVal = charToIndex[ch];
      }
    }

    if (accept(state))
    {

      tokenid = getTokenId(state);

      if (tokenid == 1)
        pp = 1;
      else
        oop = 1;
    }
  }
  // FILE *file = fopen(argv[2], "w");

  if (pp && oop)
    printf("\nParadigm: MIXED");
  else if (pp)
    printf("\nParadigm: PP");
  else
    printf("\nParadigm: OOP");
}