#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

class ProbabilisticParadigmParser
{
private:
  std::vector<int> tokens;
  size_t currentPos;

  struct ParseResult
  {
    bool success;
    size_t tokensConsumed;
    double confidence;

    ParseResult(bool s = false, size_t consumed = 0, double conf = 0.0)
        : success(s), tokensConsumed(consumed), confidence(conf) {}
  };

  int getToken(size_t pos)
  {
    return (pos < tokens.size()) ? tokens[pos] : -1;
  }

  double calculateConfidence(size_t consumed, size_t total, bool hasSpecificPatterns = false)
  {
    if (total == 0)
      return 0.0;
    double baseConfidence = static_cast<double>(consumed) / total;
    if (hasSpecificPatterns)
      baseConfidence *= 1.2;
    return std::min(1.0, baseConfidence);
  }

  ParseResult tryParseOOP()
  {
    size_t pos = 0;
    size_t consumed = 0;
    bool hasClassPattern = false;
    bool hasInheritancePattern = false;

    while (pos < tokens.size())
    {
      if (getToken(pos) == 1)
      {
        consumed++;
        pos++;

        while (pos < tokens.size() && getToken(pos) == 0)
        {
          consumed++;
          pos++;
        }

        if (pos < tokens.size())
        {
          if (getToken(pos) == 4)
          {
            hasInheritancePattern = true;
            consumed++;
            pos++;
            while (pos < tokens.size() && getToken(pos) == 0)
            {
              consumed++;
              pos++;
            }
            if (pos < tokens.size() && getToken(pos) == 5)
            {
              consumed++;
              pos++;
            }
          }

          if (pos < tokens.size() && getToken(pos) == 6)
          {
            hasClassPattern = true;
            consumed++;
            pos++;

            while (pos < tokens.size() && getToken(pos) != 7)
            {
              consumed++;
              pos++;
            }

            if (pos < tokens.size() && getToken(pos) == 7)
            {
              consumed++;
              pos++;
            }
          }
        }
      }
      else
      {
        pos++;
      }
    }

    double confidence = calculateConfidence(consumed, tokens.size(), hasClassPattern || hasInheritancePattern);
    return ParseResult(consumed > 0, consumed, confidence);
  }

  ParseResult tryParsePP()
  {
    size_t pos = 0;
    size_t consumed = 0;
    bool hasFunctionPattern = false;
    bool hasMainPattern = false;

    while (pos < tokens.size())
    {
      if (getToken(pos) == 0 || getToken(pos) == 2)
      {
        bool isFunction = false;
        size_t startPos = pos;

        if (getToken(pos) == 2)
        {
          consumed++;
          pos++;
          isFunction = true;
        }
        else if (getToken(pos) == 0)
        {
          consumed++;
          pos++;
        }

        while (pos < tokens.size() && getToken(pos) == 0)
        {
          consumed++;
          pos++;
        }

        if (pos < tokens.size() && getToken(pos) == 4)
        {
          consumed++;
          pos++;
          hasFunctionPattern = true;

          while (pos < tokens.size() && getToken(pos) != 5)
          {
            consumed++;
            pos++;
          }

          if (pos < tokens.size() && getToken(pos) == 5)
          {
            consumed++;
            pos++;
          }

          if (pos < tokens.size() && (getToken(pos) == 6 || getToken(pos) == 8))
          {
            consumed++;
            pos++;

            if (pos - startPos <= 5)
            {
              hasMainPattern = true;
            }
          }
        }
      }
      else
      {
        pos++;
      }
    }

    double confidence = calculateConfidence(consumed, tokens.size(), hasFunctionPattern || hasMainPattern);
    return ParseResult(consumed > 0, consumed, confidence);
  }

  ParseResult tryParseMixed()
  {
    ParseResult oopResult = tryParseOOP();
    ParseResult ppResult = tryParsePP();

    bool hasBothPatterns = oopResult.success && ppResult.success;
    size_t totalConsumed = std::max(oopResult.tokensConsumed, ppResult.tokensConsumed);

    double confidence = 0.0;
    if (hasBothPatterns)
    {
      confidence = (oopResult.confidence + ppResult.confidence) / 2.0;
      confidence = std::min(1.0, confidence * 1.3);
    }
    else
    {
      confidence = std::max(oopResult.confidence, ppResult.confidence) * 0.3;
    }

    return ParseResult(hasBothPatterns, totalConsumed, confidence);
  }

public:
  ProbabilisticParadigmParser(const std::vector<int> &tokenSeq) : tokens(tokenSeq), currentPos(0) {}

  void analyzeProbabilities()
  {
    std::cout << "Input token sequence (" << tokens.size() << " tokens): ";
    for (size_t i = 0; i < tokens.size(); i++)
    {
      std::cout << tokens[i];
      if (i < tokens.size() - 1)
        std::cout << " ";
    }
    std::cout << std::endl
              << std::endl;

    ParseResult oopResult = tryParseOOP();
    ParseResult ppResult = tryParsePP();
    ParseResult mixedResult = tryParseMixed();

    double totalConfidence = oopResult.confidence + ppResult.confidence + mixedResult.confidence;

    if (totalConfidence == 0.0)
    {
      std::cout << "Unable to determine paradigm - no clear patterns found." << std::endl;
      return;
    }

    double oopProb = (oopResult.confidence / totalConfidence) * 100.0;
    double ppProb = (ppResult.confidence / totalConfidence) * 100.0;
    double mixedProb = (mixedResult.confidence / totalConfidence) * 100.0;

    std::string mostLikely;
    double highestProb = 0.0;
    size_t consumed = 0;

    if (oopProb > highestProb)
    {
      highestProb = oopProb;
      mostLikely = "Object-Oriented Programming (OOP)";
      consumed = oopResult.tokensConsumed;
    }
    if (ppProb > highestProb)
    {
      highestProb = ppProb;
      mostLikely = "Procedural Programming (PP)";
      consumed = ppResult.tokensConsumed;
    }
    if (mixedProb > highestProb)
    {
      highestProb = mixedProb;
      mostLikely = "Mixed/Hybrid Programming";
      consumed = mixedResult.tokensConsumed;
    }

    std::cout << "Paradigm Analysis Results:" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "OOP (Object-Oriented):     " << std::setw(5) << oopProb << "% ("
              << oopResult.tokensConsumed << " tokens consumed)" << std::endl;
    std::cout << "PP  (Procedural):          " << std::setw(5) << ppProb << "% ("
              << ppResult.tokensConsumed << " tokens consumed)" << std::endl;
    std::cout << "MIXED (Hybrid):            " << std::setw(5) << mixedProb << "% ("
              << mixedResult.tokensConsumed << " tokens consumed)" << std::endl;

    std::cout << std::endl;
    std::cout << "Most likely paradigm: " << mostLikely
              << " (" << std::setprecision(1) << highestProb << "% confidence)" << std::endl;
  }

  std::map<std::string, double> getProbabilities()
  {
    ParseResult oopResult = tryParseOOP();
    ParseResult ppResult = tryParsePP();
    ParseResult mixedResult = tryParseMixed();

    double totalConfidence = oopResult.confidence + ppResult.confidence + mixedResult.confidence;

    std::map<std::string, double> probabilities;
    if (totalConfidence > 0.0)
    {
      probabilities["OOP"] = (oopResult.confidence / totalConfidence) * 100.0;
      probabilities["PP"] = (ppResult.confidence / totalConfidence) * 100.0;
      probabilities["MIXED"] = (mixedResult.confidence / totalConfidence) * 100.0;
    }
    else
    {
      probabilities["OOP"] = 0.0;
      probabilities["PP"] = 0.0;
      probabilities["MIXED"] = 0.0;
    }

    return probabilities;
  }
};

int main()
{
  try
  {
    std::vector<int> tokens = {9, 0, 3, 4, 5, 6, 8, 0, 0, 8, 0, 0, 0, 8, 0, 4, 0, 5, 8, 0, 0, 0, 0, 8, 0, 7};

    ProbabilisticParadigmParser parser(tokens);
    parser.analyzeProbabilities();

    std::cout << std::endl
              << "Programmatic access:" << std::endl;
    auto probs = parser.getProbabilities();
    for (const auto &pair : probs)
    {
      std::cout << pair.first << ": " << std::fixed << std::setprecision(1)
                << pair.second << "%" << std::endl;
    }
  }
  catch (const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  return 0;
}