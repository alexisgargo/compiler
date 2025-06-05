#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <stdexcept>

class RecursiveDescentParser
{
private:
  std::vector<int> tokens;
  size_t currentPos;
  size_t initial;

  // get current token
  int getCurrentToken()
  {
    if (currentPos >= tokens.size())
    {
      return -1; // End of input
    }
    return tokens[currentPos];
  }

  // consume a token
  void consume(int expectedTokenId)
  {
    int current = getCurrentToken();
    if (current != expectedTokenId)
    {
      throw;
    }
    currentPos++;
  }

  // check if current token is in a set
  bool isInFirstPlus(const std::set<int> &firstPlusSet)
  {
    int current = getCurrentToken();
    return firstPlusSet.count(current) > 0 ||
           (current == -1 && firstPlusSet.count(-2) > 0); // -2 represents $
  }

  // check for epsilon
  bool hasEpsilon(const std::set<int> &firstPlusSet)
  {
    return firstPlusSet.count(-3) > 0; // -3 represents ε
  }

  // dealing with a grammar that breaks
  bool error(std::string paradigm)
  {
    std::cout << "At " << std::to_string(currentPos) << " tokens, the best guess is " << paradigm;
  }

public:
  RecursiveDescentParser(const std::vector<int> &tokens) : tokens(tokens), currentPos(0) {}

  // S -> PARADIGM S' | STATEMENTS PARADIGM S' | PYSTATEMENTS PARADIGM S'
  void parseS()
  {
    initial = currentPos;

    try
    {
      parsePARADIGM();
      parseSPrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseSTATEMENTS();
        parsePARADIGM();
        parseSPrime();
      }
      catch (...)
      {
        currentPos = initial;
        try
        {
          parsePYSTATEMENTS();
          parsePARADIGM();
          parseSPrime();
        }
        catch (...)
        {
          throw;
        }
      }
    }
  }

  // S' -> STATEMENTS | PYSTATEMENTS | ε
  void parseSPrime()
  {
    initial = currentPos;

    try
    {
      parseSTATEMENTS();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYSTATEMENTS();
      }
      catch (...)
      {
        currentPos = initial;
        // ε production - do nothing
      }
    }
  }

  // PARADIGM -> OOP | PP | MIXED
  void parsePARADIGM()
  {
    initial = currentPos;

    try
    {
      parseOOP();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePP();
      }
      catch (...)
      {
        currentPos = initial;
        try
        {
          parseMIXED();
        }
        catch (...)
        {
          throw;
        }
      }
    }
  }

  // OOP -> PYCLASS | CLASS
  void parseOOP()
  {
    initial = currentPos;

    try
    {
      parsePYCLASS();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseCLASS();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // CLASS -> PREFIX CLASSCOMPLEMENT | CLASSCOMPLEMENT
  void parseCLASS()
  {
    initial = currentPos;

    try
    {
      parsePREFIX();
      parseCLASSCOMPLEMENT();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseCLASSCOMPLEMENT();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // CLASSCOMPLEMENT -> IDS <1> IDS <6> STATEMENTS <7> CLASSCOMPLEMENT'
  void parseCLASSCOMPLEMENT()
  {
    parseIDS();
    consume(1);
    parseIDS();
    consume(6);
    parseSTATEMENTS();
    consume(7);
    parseCLASSCOMPLEMENTPrime();
  }

  // CLASSCOMPLEMENT' -> MAIN | STATEMENTS CLASS | ε
  void parseCLASSCOMPLEMENTPrime()
  {
    initial = currentPos;

    try
    {
      parseMAIN();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseSTATEMENTS();
        parseCLASS();
      }
      catch (...)
      {
        // ε production - do nothing
        currentPos = initial;
      }
    }
  }

  // MAIN -> PREFIX IDS <3> <4> IDS <5> <6> STATEMENTS <7> | IDS <3> <4> IDS <5> <6> STATEMENTS <7>
  void parseMAIN()
  {
    initial = currentPos;

    try
    {
      parsePREFIX();
      parseIDS();
      consume(3);
      consume(4);
      parseIDS();
      consume(5);
      consume(6);
      parseSTATEMENTS();
      consume(7);
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseIDS();
        consume(3);
        consume(4);
        parseIDS();
        consume(5);
        consume(6);
        parseSTATEMENTS();
        consume(7);
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYCLASS -> <9> <1> IDS PYCLASS' PYCLASS'' | <1> IDS PYCLASS' PYCLASS''
  void parsePYCLASS()
  {
    initial = currentPos;
    try
    {
      consume(9);
      consume(1);
      parseIDS();
      parsePYCLASSPrime();
      parsePYCLASSDoublePrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(1);
        parseIDS();
        parsePYCLASSPrime();
        parsePYCLASSDoublePrime();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYCLASS' -> INDENTEDBLOCK | <4> IDS <5> INDENTEDBLOCK
  void parsePYCLASSPrime()
  {
    initial = currentPos;

    try
    {
      parseINDENTEDBLOCK();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(4);
        parseIDS();
        consume(5);
        parseINDENTEDBLOCK();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYCLASS'' -> PYSTATEMENTS PYCLASS | ε
  void parsePYCLASSDoublePrime()
  {
    initial = currentPos;

    try
    {
      parsePYSTATEMENTS();
      parsePYCLASS();
    }
    catch (...)
    {
      // ε production - do nothing
      currentPos = initial;
    }
  }

  // PP -> PYFUNC | FUNC
  void parsePP()
  {
    initial = currentPos;

    try
    {
      parsePYFUNC();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseFUNC();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // FUNC -> <0> IDS <4> IDS <5> <6> STATEMENTS <7> FUNC' | PREFIX <0> IDS <4> IDS <5> <6> STATEMENTS <7> FUNC'
  void parseFUNC()
  {
    initial = currentPos;

    try
    {
      parseFUNCPrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseFUNCPrime();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // FUNC' -> STATEMENTS FUNC | ε
  void parseFUNCPrime()
  {
    initial = currentPos;

    try
    {
      parseFUNC();
    }
    catch (...)
    {
      currentPos = initial;
    }
  }

  // PYFUNC -> <2> IDS <4> IDS <5> INDENTEDBLOCK PYFUNC' | <9> <2> <3> <4> IDS <5> INDENTEDBLOCK PYFUNC'
  void parsePYFUNC()
  {
    initial = currentPos;

    try
    {
      parsePYFUNCPrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYFUNCPrime();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYFUNC' -> PYSTATEMENTS PYFUNC | ε
  void parsePYFUNCPrime()
  {
    initial = currentPos;

    try
    {
      parsePYFUNC();
    }
    catch (...)
    {
      currentPos = initial;
    }
  }

  // MIXED -> PYMIXED | MIXEDN
  void parseMIXED()
  {
    initial = currentPos;

    try
    {
      parsePYMIXED();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseMIXEDN();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // MIXEDN -> CLASS FUNC MIXEDCOMPLEMENT | FUNC CLASS MIXEDCOMPLEMENT
  void parseMIXEDN()
  {
    initial = currentPos;

    try
    {
      parseMIXEDCOMPLEMENT();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseMIXEDCOMPLEMENT();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // MIXEDCOMPLEMENT -> CLASS | FUNC | MIXEDN | MAIN | ε
  void parseMIXEDCOMPLEMENT()
  {
    initial = currentPos;

    try
    {
      parseCLASS();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseFUNC();
      }
      catch (...)
      {
        currentPos = initial;
        try
        {
          parseMIXEDN();
        }
        catch (...)
        {
          currentPos = initial;
          try
          {
            parseMAIN();
          }
          catch (...)
          {
            currentPos = initial;
          }
        }
      }
    }
  }

  // PYMIXED -> PYCLASS PYFUNC PYMIXEDCOMPLEMENT | PYFUNC PYCLASS PYMIXEDCOMPLEMENT
  void parsePYMIXED()
  {
    initial = currentPos;

    try
    {
      parsePYMIXEDCOMPLEMENT();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYMIXEDCOMPLEMENT();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYMIXEDCOMPLEMENT -> PYCLASS | PYFUNC | PYMIXED | ε
  void parsePYMIXEDCOMPLEMENT()
  {
    initial = currentPos;

    try
    {
      parsePYCLASS();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYFUNC();
      }
      catch (...)
      {
        currentPos = initial;
        try
        {
          parsePYMIXED();
        }
        catch (...)
        {
          currentPos = initial;
        }
      }
    }
  }

  // INDENTEDBLOCK -> <8> INDENTEDBLOCK' INDENTEDBLOCK''
  void parseINDENTEDBLOCK()
  {
    consume(8);
    parseINDENTEDBLOCKPrime();
    parseINDENTEDBLOCKDoublePrime();
  }

  // INDENTEDBLOCK' -> PYSTATEMENT | <2> IDS <4> IDS <5>
  void parseINDENTEDBLOCKPrime()
  {
    initial = currentPos;

    try
    {
      parsePYSTATEMENT();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(5);
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // INDENTEDBLOCK'' -> INDENTEDBLOCK | ε
  void parseINDENTEDBLOCKDoublePrime()
  {
    initial = currentPos;

    try
    {
      parseINDENTEDBLOCK();
    }
    catch (...)
    {
      currentPos = initial;
    }
  }

  // PYSTATEMENTS -> PYSTATEMENT PYSTATEMENTS' | <9> PYSTATEMENT PYSTATEMENTS'
  void parsePYSTATEMENTS()
  {
    initial = currentPos;

    try
    {
      parsePYSTATEMENTSPrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYSTATEMENTSPrime();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYSTATEMENTS' -> PYSTATEMENTS | INDENTEDBLOCK | ε
  void parsePYSTATEMENTSPrime()
  {
    initial = currentPos;

    try
    {
      parsePYSTATEMENTS();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parseINDENTEDBLOCK();
      }
      catch (...)
      {
        currentPos = initial;
      }
    }
  }

  // PYSTATEMENT -> <0> IDS PYSTATEMENT'
  void parsePYSTATEMENT()
  {
    consume(0);
    parseIDS();
    parsePYSTATEMENTPrime();
  }

  // PYSTATEMENT' -> <4> PYSTATEMENT'' | <6> PYSTATEMENT''' | ε
  void parsePYSTATEMENTPrime()
  {
    initial = currentPos;

    try
    {
      parsePYSTATEMENTDoublePrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYSTATEMENTTriplePrime();
      }
      catch (...)
      {
        currentPos = initial;
      }
    }
  }

  // PYSTATEMENT'' -> IDS <5> | INDENTEDBLOCK PYSTATEMENT''''
  void parsePYSTATEMENTDoublePrime()
  {
    initial = currentPos;

    try
    {
      consume(5);
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYSTATEMENTQuadruplePrime();
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // PYSTATEMENT''' -> IDS <7> | INDENTEDBLOCK PYSTATEMENT'''''
  void parsePYSTATEMENTTriplePrime()
  {
    initial = currentPos;

    try
    {
      consume(7);
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        parsePYSTATEMENTQuintuplePrime();
      }
      catch (...)
      {
        currentPos = initial;
      }
    }
  }

  // PYSTATEMENT'''' -> <5> | <8> <5>
  void parsePYSTATEMENTQuadruplePrime()
  {
    initial = currentPos;

    try
    {
      consume(5);
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(5);
      }
      catch (...)
      {
        currentPos = initial;
        throw;
      }
    }
  }

  // PYSTATEMENT''''' -> <7> | <8> <7>
  void parsePYSTATEMENTQuintuplePrime()
  {
    initial = currentPos;

    try
    {
      consume(7);
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(7);
      }
      catch (...)
      {
        currentPos = initial;
        throw;
      }
    }
  }

  // STATEMENTS -> STATEMENT STATEMENTS'
  void parseSTATEMENTS()
  {
    parseSTATEMENT();
    parseSTATEMENTSPrime();
  }

  // STATEMENTS' -> STATEMENTS | ε
  void parseSTATEMENTSPrime()
  {
    initial = currentPos;

    try
    {
      parseSTATEMENTS();
    }
    catch (...)
    {
      currentPos = initial;
    }
  }

  // STATEMENT -> PREFIX IDS STATEMENT'
  void parseSTATEMENT()
  {
    parsePREFIX();
    parseIDS();
    parseSTATEMENTPrime();
  }

  // STATEMENT' -> <4> STATEMENTS <5> STATEMENT'' | <6> STATEMENTS <7> | ε
  void parseSTATEMENTPrime()
  {
    initial = currentPos;

    try
    {
      parseSTATEMENTDoublePrime();
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(7);
      }
      catch (...)
      {
        currentPos = initial;
      }
    }
  }

  // STATEMENT'' -> <6> STATEMENTS <7> | ε
  void parseSTATEMENTDoublePrime()
  {
    initial = currentPos;

    try
    {
      consume(7);
    }
    catch (...)
    {
      throw;
    }
  }

  // PREFIX -> <8> | <9>
  void parsePREFIX()
  {
    initial = currentPos;

    try
    {
      consume(8);
    }
    catch (...)
    {
      currentPos = initial;
      try
      {
        consume(9);
      }
      catch (...)
      {
        throw;
      }
    }
  }

  // IDS -> <0> IDS | ε
  void parseIDS()
  {
    initial = currentPos;

    try
    {
      parseIDS();
    }
    catch (...)
    {
      currentPos = initial;
    }
  }

  // Main parse function
  void parse()
  {
    parseS();
    if (currentPos < tokens.size())
    {
      throw;
    }
  }
};

// Example usage
int main()
{
  try
  {
    // Example token sequence - replace with actual tokens
    std::vector<int> tokens = {1, 0, 8, 2, 0, 4, 0, 0, 0, 5, 8, 0, 0, 0, 9, 0, 0, 8, 0, 0, 4, 5, 8, 0, 0, 8, 0, 4, 0, 5};

    RecursiveDescentParser parser(tokens);
    parser.parse();
  }
  catch (std::runtime_error e)
  {
    std::cout << e.what();
  }
};