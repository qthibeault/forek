
// Generated from /home/andersonjwan/Projects/forek/build/antlr/grammars/LinearTemporalLogicLexer.g4 by ANTLR 4.11.1


#include "LinearTemporalLogicLexer.h"


using namespace antlr4;

using namespace gen;


using namespace antlr4;

namespace {

struct LinearTemporalLogicLexerStaticData final {
  LinearTemporalLogicLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LinearTemporalLogicLexerStaticData(const LinearTemporalLogicLexerStaticData&) = delete;
  LinearTemporalLogicLexerStaticData(LinearTemporalLogicLexerStaticData&&) = delete;
  LinearTemporalLogicLexerStaticData& operator=(const LinearTemporalLogicLexerStaticData&) = delete;
  LinearTemporalLogicLexerStaticData& operator=(LinearTemporalLogicLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag lineartemporallogiclexerLexerOnceFlag;
LinearTemporalLogicLexerStaticData *lineartemporallogiclexerLexerStaticData = nullptr;

void lineartemporallogiclexerLexerInitialize() {
  assert(lineartemporallogiclexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<LinearTemporalLogicLexerStaticData>(
    std::vector<std::string>{
      "EventuallyOperator", "AlwaysOperator", "NextOperator", "UntilOperator", 
      "ReleaseOperator", "NegationOperator", "ConjunctionOperator", "DisjunctionOperator", 
      "ImplicationOperator", "IffOperator", "True", "False", "At", "Ampersand", 
      "Apostrophe", "Asterick", "Bar", "Caret", "Colon", "Comma", "DollarSign", 
      "Dot", "Equal", "ExclamationMark", "GraveAccent", "Hash", "Minus", 
      "PercentSign", "Plus", "QuestionMark", "QuotationMark", "Semicolon", 
      "Tilde", "Underscore", "Slash", "BackSlash", "LeftBrace", "RightBrace", 
      "LeftBracket", "RightBracket", "LeftChevron", "RightChevron", "LeftParenthesis", 
      "RightParenthesis", "BlockComment", "LineComment", "Infinity", "Scalar", 
      "Integer", "DecimalInteger", "BinaryInteger", "OctalInteger", "HexInteger", 
      "Floating", "PointFloat", "ExponentFloat", "Identifier", "LETTER", 
      "NONDIGIT", "BINDIGIT", "HEXDIGIT", "OCTALDIGIT", "FRACTION", "EXPONENT", 
      "NONZERODIGIT", "DIGIT", "Whitespace"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "'true'", "'false'", "'@'", 
      "'&'", "'''", "'*'", "'|'", "'^'", "':'", "','", "'$'", "'.'", "'='", 
      "'!'", "'`'", "'#'", "'-'", "'%'", "'+'", "'\\u003F'", "'\"'", "';'", 
      "'~'", "'_'", "'/'", "'\\'", "'{'", "'}'", "'['", "']'", "'<'", "'>'", 
      "'('", "')'", "", "", "'inf'"
    },
    std::vector<std::string>{
      "", "EventuallyOperator", "AlwaysOperator", "NextOperator", "UntilOperator", 
      "ReleaseOperator", "NegationOperator", "ConjunctionOperator", "DisjunctionOperator", 
      "ImplicationOperator", "IffOperator", "True", "False", "At", "Ampersand", 
      "Apostrophe", "Asterick", "Bar", "Caret", "Colon", "Comma", "DollarSign", 
      "Dot", "Equal", "ExclamationMark", "GraveAccent", "Hash", "Minus", 
      "PercentSign", "Plus", "QuestionMark", "QuotationMark", "Semicolon", 
      "Tilde", "Underscore", "Slash", "BackSlash", "LeftBrace", "RightBrace", 
      "LeftBracket", "RightBracket", "LeftChevron", "RightChevron", "LeftParenthesis", 
      "RightParenthesis", "BlockComment", "LineComment", "Infinity", "Scalar", 
      "Integer", "DecimalInteger", "BinaryInteger", "OctalInteger", "HexInteger", 
      "Floating", "PointFloat", "ExponentFloat", "Identifier", "Whitespace"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,58,455,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,
  	7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,
  	7,49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,
  	7,56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,
  	7,63,2,64,7,64,2,65,7,65,2,66,7,66,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
  	0,1,0,1,0,3,0,147,8,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,156,8,1,1,2,1,2,
  	1,2,1,2,1,2,3,2,163,8,2,1,3,1,3,1,3,1,3,1,3,1,3,3,3,171,8,3,1,4,1,4,1,
  	4,1,4,1,4,1,4,1,4,1,4,3,4,181,8,4,1,5,1,5,1,5,1,5,3,5,187,8,5,1,6,1,6,
  	1,6,1,6,1,6,3,6,194,8,6,1,7,1,7,1,7,1,7,3,7,200,8,7,1,8,1,8,1,8,1,8,1,
  	8,1,8,1,8,1,8,1,8,3,8,211,8,8,1,9,1,9,1,9,1,9,1,9,1,9,3,9,219,8,9,1,10,
  	1,10,1,10,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,13,1,13,
  	1,14,1,14,1,15,1,15,1,16,1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,20,1,20,
  	1,21,1,21,1,22,1,22,1,23,1,23,1,24,1,24,1,25,1,25,1,26,1,26,1,27,1,27,
  	1,28,1,28,1,29,1,29,1,30,1,30,1,31,1,31,1,32,1,32,1,33,1,33,1,34,1,34,
  	1,35,1,35,1,36,1,36,1,37,1,37,1,38,1,38,1,39,1,39,1,40,1,40,1,41,1,41,
  	1,42,1,42,1,43,1,43,1,44,1,44,1,44,1,44,5,44,300,8,44,10,44,12,44,303,
  	9,44,1,44,1,44,1,44,1,44,1,44,1,45,1,45,1,45,1,45,5,45,314,8,45,10,45,
  	12,45,317,9,45,1,45,3,45,320,8,45,1,45,1,45,1,45,1,45,1,46,1,46,1,46,
  	1,46,1,47,1,47,3,47,332,8,47,1,48,1,48,1,48,1,48,3,48,338,8,48,1,49,1,
  	49,5,49,342,8,49,10,49,12,49,345,9,49,1,49,4,49,348,8,49,11,49,12,49,
  	349,3,49,352,8,49,1,50,1,50,1,50,4,50,357,8,50,11,50,12,50,358,1,51,1,
  	51,1,51,4,51,364,8,51,11,51,12,51,365,1,52,1,52,1,52,4,52,371,8,52,11,
  	52,12,52,372,1,53,1,53,3,53,377,8,53,1,54,4,54,380,8,54,11,54,12,54,381,
  	3,54,384,8,54,1,54,1,54,4,54,388,8,54,11,54,12,54,389,1,54,1,54,3,54,
  	394,8,54,1,55,4,55,397,8,55,11,55,12,55,398,1,55,3,55,402,8,55,1,55,1,
  	55,1,56,4,56,407,8,56,11,56,12,56,408,1,56,1,56,5,56,413,8,56,10,56,12,
  	56,416,9,56,1,57,1,57,1,58,1,58,3,58,422,8,58,1,59,1,59,1,60,1,60,1,61,
  	1,61,1,62,1,62,4,62,432,8,62,11,62,12,62,433,1,63,1,63,3,63,438,8,63,
  	1,63,4,63,441,8,63,11,63,12,63,442,1,64,1,64,1,65,1,65,1,66,4,66,450,
  	8,66,11,66,12,66,451,1,66,1,66,2,301,315,0,67,1,1,3,2,5,3,7,4,9,5,11,
  	6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,18,
  	37,19,39,20,41,21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,57,29,59,
  	30,61,31,63,32,65,33,67,34,69,35,71,36,73,37,75,38,77,39,79,40,81,41,
  	83,42,85,43,87,44,89,45,91,46,93,47,95,48,97,49,99,50,101,51,103,52,105,
  	53,107,54,109,55,111,56,113,57,115,0,117,0,119,0,121,0,123,0,125,0,127,
  	0,129,0,131,0,133,58,1,0,12,2,0,66,66,98,98,2,0,79,79,111,111,2,0,88,
  	88,120,120,2,0,65,90,97,122,1,0,48,49,3,0,48,57,65,70,97,102,1,0,48,55,
  	2,0,69,69,101,101,2,0,43,43,45,45,1,0,49,57,1,0,48,57,3,0,9,10,13,13,
  	32,32,483,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,
  	0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,
  	1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,
  	0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,
  	0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,0,0,53,
  	1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,61,1,0,0,0,0,63,1,0,
  	0,0,0,65,1,0,0,0,0,67,1,0,0,0,0,69,1,0,0,0,0,71,1,0,0,0,0,73,1,0,0,0,
  	0,75,1,0,0,0,0,77,1,0,0,0,0,79,1,0,0,0,0,81,1,0,0,0,0,83,1,0,0,0,0,85,
  	1,0,0,0,0,87,1,0,0,0,0,89,1,0,0,0,0,91,1,0,0,0,0,93,1,0,0,0,0,95,1,0,
  	0,0,0,97,1,0,0,0,0,99,1,0,0,0,0,101,1,0,0,0,0,103,1,0,0,0,0,105,1,0,0,
  	0,0,107,1,0,0,0,0,109,1,0,0,0,0,111,1,0,0,0,0,113,1,0,0,0,0,133,1,0,0,
  	0,1,146,1,0,0,0,3,155,1,0,0,0,5,162,1,0,0,0,7,170,1,0,0,0,9,180,1,0,0,
  	0,11,186,1,0,0,0,13,193,1,0,0,0,15,199,1,0,0,0,17,210,1,0,0,0,19,218,
  	1,0,0,0,21,220,1,0,0,0,23,225,1,0,0,0,25,231,1,0,0,0,27,233,1,0,0,0,29,
  	235,1,0,0,0,31,237,1,0,0,0,33,239,1,0,0,0,35,241,1,0,0,0,37,243,1,0,0,
  	0,39,245,1,0,0,0,41,247,1,0,0,0,43,249,1,0,0,0,45,251,1,0,0,0,47,253,
  	1,0,0,0,49,255,1,0,0,0,51,257,1,0,0,0,53,259,1,0,0,0,55,261,1,0,0,0,57,
  	263,1,0,0,0,59,265,1,0,0,0,61,267,1,0,0,0,63,269,1,0,0,0,65,271,1,0,0,
  	0,67,273,1,0,0,0,69,275,1,0,0,0,71,277,1,0,0,0,73,279,1,0,0,0,75,281,
  	1,0,0,0,77,283,1,0,0,0,79,285,1,0,0,0,81,287,1,0,0,0,83,289,1,0,0,0,85,
  	291,1,0,0,0,87,293,1,0,0,0,89,295,1,0,0,0,91,309,1,0,0,0,93,325,1,0,0,
  	0,95,331,1,0,0,0,97,337,1,0,0,0,99,351,1,0,0,0,101,353,1,0,0,0,103,360,
  	1,0,0,0,105,367,1,0,0,0,107,376,1,0,0,0,109,393,1,0,0,0,111,401,1,0,0,
  	0,113,406,1,0,0,0,115,417,1,0,0,0,117,421,1,0,0,0,119,423,1,0,0,0,121,
  	425,1,0,0,0,123,427,1,0,0,0,125,429,1,0,0,0,127,435,1,0,0,0,129,444,1,
  	0,0,0,131,446,1,0,0,0,133,449,1,0,0,0,135,147,5,70,0,0,136,137,5,101,
  	0,0,137,138,5,118,0,0,138,139,5,101,0,0,139,140,5,110,0,0,140,141,5,116,
  	0,0,141,142,5,117,0,0,142,143,5,97,0,0,143,144,5,108,0,0,144,145,5,108,
  	0,0,145,147,5,121,0,0,146,135,1,0,0,0,146,136,1,0,0,0,147,2,1,0,0,0,148,
  	156,5,71,0,0,149,150,5,97,0,0,150,151,5,108,0,0,151,152,5,119,0,0,152,
  	153,5,97,0,0,153,154,5,121,0,0,154,156,5,115,0,0,155,148,1,0,0,0,155,
  	149,1,0,0,0,156,4,1,0,0,0,157,163,5,88,0,0,158,159,5,110,0,0,159,160,
  	5,101,0,0,160,161,5,120,0,0,161,163,5,116,0,0,162,157,1,0,0,0,162,158,
  	1,0,0,0,163,6,1,0,0,0,164,171,5,85,0,0,165,166,5,117,0,0,166,167,5,110,
  	0,0,167,168,5,116,0,0,168,169,5,105,0,0,169,171,5,108,0,0,170,164,1,0,
  	0,0,170,165,1,0,0,0,171,8,1,0,0,0,172,181,5,82,0,0,173,174,5,114,0,0,
  	174,175,5,101,0,0,175,176,5,108,0,0,176,177,5,101,0,0,177,178,5,97,0,
  	0,178,179,5,115,0,0,179,181,5,101,0,0,180,172,1,0,0,0,180,173,1,0,0,0,
  	181,10,1,0,0,0,182,187,3,47,23,0,183,184,5,110,0,0,184,185,5,111,0,0,
  	185,187,5,116,0,0,186,182,1,0,0,0,186,183,1,0,0,0,187,12,1,0,0,0,188,
  	189,5,38,0,0,189,194,5,38,0,0,190,191,5,97,0,0,191,192,5,110,0,0,192,
  	194,5,100,0,0,193,188,1,0,0,0,193,190,1,0,0,0,194,14,1,0,0,0,195,196,
  	5,124,0,0,196,200,5,124,0,0,197,198,5,111,0,0,198,200,5,114,0,0,199,195,
  	1,0,0,0,199,197,1,0,0,0,200,16,1,0,0,0,201,202,5,45,0,0,202,211,5,62,
  	0,0,203,204,5,105,0,0,204,205,5,109,0,0,205,206,5,112,0,0,206,207,5,108,
  	0,0,207,208,5,105,0,0,208,209,5,101,0,0,209,211,5,115,0,0,210,201,1,0,
  	0,0,210,203,1,0,0,0,211,18,1,0,0,0,212,213,5,60,0,0,213,214,5,45,0,0,
  	214,219,5,62,0,0,215,216,5,105,0,0,216,217,5,102,0,0,217,219,5,102,0,
  	0,218,212,1,0,0,0,218,215,1,0,0,0,219,20,1,0,0,0,220,221,5,116,0,0,221,
  	222,5,114,0,0,222,223,5,117,0,0,223,224,5,101,0,0,224,22,1,0,0,0,225,
  	226,5,102,0,0,226,227,5,97,0,0,227,228,5,108,0,0,228,229,5,115,0,0,229,
  	230,5,101,0,0,230,24,1,0,0,0,231,232,5,64,0,0,232,26,1,0,0,0,233,234,
  	5,38,0,0,234,28,1,0,0,0,235,236,5,39,0,0,236,30,1,0,0,0,237,238,5,42,
  	0,0,238,32,1,0,0,0,239,240,5,124,0,0,240,34,1,0,0,0,241,242,5,94,0,0,
  	242,36,1,0,0,0,243,244,5,58,0,0,244,38,1,0,0,0,245,246,5,44,0,0,246,40,
  	1,0,0,0,247,248,5,36,0,0,248,42,1,0,0,0,249,250,5,46,0,0,250,44,1,0,0,
  	0,251,252,5,61,0,0,252,46,1,0,0,0,253,254,5,33,0,0,254,48,1,0,0,0,255,
  	256,5,96,0,0,256,50,1,0,0,0,257,258,5,35,0,0,258,52,1,0,0,0,259,260,5,
  	45,0,0,260,54,1,0,0,0,261,262,5,37,0,0,262,56,1,0,0,0,263,264,5,43,0,
  	0,264,58,1,0,0,0,265,266,5,63,0,0,266,60,1,0,0,0,267,268,5,34,0,0,268,
  	62,1,0,0,0,269,270,5,59,0,0,270,64,1,0,0,0,271,272,5,126,0,0,272,66,1,
  	0,0,0,273,274,5,95,0,0,274,68,1,0,0,0,275,276,5,47,0,0,276,70,1,0,0,0,
  	277,278,5,92,0,0,278,72,1,0,0,0,279,280,5,123,0,0,280,74,1,0,0,0,281,
  	282,5,125,0,0,282,76,1,0,0,0,283,284,5,91,0,0,284,78,1,0,0,0,285,286,
  	5,93,0,0,286,80,1,0,0,0,287,288,5,60,0,0,288,82,1,0,0,0,289,290,5,62,
  	0,0,290,84,1,0,0,0,291,292,5,40,0,0,292,86,1,0,0,0,293,294,5,41,0,0,294,
  	88,1,0,0,0,295,296,5,47,0,0,296,297,5,42,0,0,297,301,1,0,0,0,298,300,
  	9,0,0,0,299,298,1,0,0,0,300,303,1,0,0,0,301,302,1,0,0,0,301,299,1,0,0,
  	0,302,304,1,0,0,0,303,301,1,0,0,0,304,305,5,42,0,0,305,306,5,47,0,0,306,
  	307,1,0,0,0,307,308,6,44,0,0,308,90,1,0,0,0,309,310,5,47,0,0,310,311,
  	5,47,0,0,311,315,1,0,0,0,312,314,9,0,0,0,313,312,1,0,0,0,314,317,1,0,
  	0,0,315,316,1,0,0,0,315,313,1,0,0,0,316,319,1,0,0,0,317,315,1,0,0,0,318,
  	320,5,13,0,0,319,318,1,0,0,0,319,320,1,0,0,0,320,321,1,0,0,0,321,322,
  	5,10,0,0,322,323,1,0,0,0,323,324,6,45,0,0,324,92,1,0,0,0,325,326,5,105,
  	0,0,326,327,5,110,0,0,327,328,5,102,0,0,328,94,1,0,0,0,329,332,3,97,48,
  	0,330,332,3,107,53,0,331,329,1,0,0,0,331,330,1,0,0,0,332,96,1,0,0,0,333,
  	338,3,99,49,0,334,338,3,101,50,0,335,338,3,103,51,0,336,338,3,105,52,
  	0,337,333,1,0,0,0,337,334,1,0,0,0,337,335,1,0,0,0,337,336,1,0,0,0,338,
  	98,1,0,0,0,339,343,3,129,64,0,340,342,3,131,65,0,341,340,1,0,0,0,342,
  	345,1,0,0,0,343,341,1,0,0,0,343,344,1,0,0,0,344,352,1,0,0,0,345,343,1,
  	0,0,0,346,348,5,48,0,0,347,346,1,0,0,0,348,349,1,0,0,0,349,347,1,0,0,
  	0,349,350,1,0,0,0,350,352,1,0,0,0,351,339,1,0,0,0,351,347,1,0,0,0,352,
  	100,1,0,0,0,353,354,5,48,0,0,354,356,7,0,0,0,355,357,3,119,59,0,356,355,
  	1,0,0,0,357,358,1,0,0,0,358,356,1,0,0,0,358,359,1,0,0,0,359,102,1,0,0,
  	0,360,361,5,48,0,0,361,363,7,1,0,0,362,364,3,123,61,0,363,362,1,0,0,0,
  	364,365,1,0,0,0,365,363,1,0,0,0,365,366,1,0,0,0,366,104,1,0,0,0,367,368,
  	5,48,0,0,368,370,7,2,0,0,369,371,3,121,60,0,370,369,1,0,0,0,371,372,1,
  	0,0,0,372,370,1,0,0,0,372,373,1,0,0,0,373,106,1,0,0,0,374,377,3,109,54,
  	0,375,377,3,111,55,0,376,374,1,0,0,0,376,375,1,0,0,0,377,108,1,0,0,0,
  	378,380,3,131,65,0,379,378,1,0,0,0,380,381,1,0,0,0,381,379,1,0,0,0,381,
  	382,1,0,0,0,382,384,1,0,0,0,383,379,1,0,0,0,383,384,1,0,0,0,384,385,1,
  	0,0,0,385,394,3,125,62,0,386,388,3,131,65,0,387,386,1,0,0,0,388,389,1,
  	0,0,0,389,387,1,0,0,0,389,390,1,0,0,0,390,391,1,0,0,0,391,392,5,46,0,
  	0,392,394,1,0,0,0,393,383,1,0,0,0,393,387,1,0,0,0,394,110,1,0,0,0,395,
  	397,3,131,65,0,396,395,1,0,0,0,397,398,1,0,0,0,398,396,1,0,0,0,398,399,
  	1,0,0,0,399,402,1,0,0,0,400,402,3,109,54,0,401,396,1,0,0,0,401,400,1,
  	0,0,0,402,403,1,0,0,0,403,404,3,127,63,0,404,112,1,0,0,0,405,407,3,117,
  	58,0,406,405,1,0,0,0,407,408,1,0,0,0,408,406,1,0,0,0,408,409,1,0,0,0,
  	409,414,1,0,0,0,410,413,3,117,58,0,411,413,3,131,65,0,412,410,1,0,0,0,
  	412,411,1,0,0,0,413,416,1,0,0,0,414,412,1,0,0,0,414,415,1,0,0,0,415,114,
  	1,0,0,0,416,414,1,0,0,0,417,418,7,3,0,0,418,116,1,0,0,0,419,422,3,115,
  	57,0,420,422,5,95,0,0,421,419,1,0,0,0,421,420,1,0,0,0,422,118,1,0,0,0,
  	423,424,7,4,0,0,424,120,1,0,0,0,425,426,7,5,0,0,426,122,1,0,0,0,427,428,
  	7,6,0,0,428,124,1,0,0,0,429,431,5,46,0,0,430,432,3,131,65,0,431,430,1,
  	0,0,0,432,433,1,0,0,0,433,431,1,0,0,0,433,434,1,0,0,0,434,126,1,0,0,0,
  	435,437,7,7,0,0,436,438,7,8,0,0,437,436,1,0,0,0,437,438,1,0,0,0,438,440,
  	1,0,0,0,439,441,3,131,65,0,440,439,1,0,0,0,441,442,1,0,0,0,442,440,1,
  	0,0,0,442,443,1,0,0,0,443,128,1,0,0,0,444,445,7,9,0,0,445,130,1,0,0,0,
  	446,447,7,10,0,0,447,132,1,0,0,0,448,450,7,11,0,0,449,448,1,0,0,0,450,
  	451,1,0,0,0,451,449,1,0,0,0,451,452,1,0,0,0,452,453,1,0,0,0,453,454,6,
  	66,0,0,454,134,1,0,0,0,37,0,146,155,162,170,180,186,193,199,210,218,301,
  	315,319,331,337,343,349,351,358,365,372,376,381,383,389,393,398,401,408,
  	412,414,421,433,437,442,451,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  lineartemporallogiclexerLexerStaticData = staticData.release();
}

}

LinearTemporalLogicLexer::LinearTemporalLogicLexer(CharStream *input) : Lexer(input) {
  LinearTemporalLogicLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *lineartemporallogiclexerLexerStaticData->atn, lineartemporallogiclexerLexerStaticData->decisionToDFA, lineartemporallogiclexerLexerStaticData->sharedContextCache);
}

LinearTemporalLogicLexer::~LinearTemporalLogicLexer() {
  delete _interpreter;
}

std::string LinearTemporalLogicLexer::getGrammarFileName() const {
  return "LinearTemporalLogicLexer.g4";
}

const std::vector<std::string>& LinearTemporalLogicLexer::getRuleNames() const {
  return lineartemporallogiclexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& LinearTemporalLogicLexer::getChannelNames() const {
  return lineartemporallogiclexerLexerStaticData->channelNames;
}

const std::vector<std::string>& LinearTemporalLogicLexer::getModeNames() const {
  return lineartemporallogiclexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& LinearTemporalLogicLexer::getVocabulary() const {
  return lineartemporallogiclexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LinearTemporalLogicLexer::getSerializedATN() const {
  return lineartemporallogiclexerLexerStaticData->serializedATN;
}

const atn::ATN& LinearTemporalLogicLexer::getATN() const {
  return *lineartemporallogiclexerLexerStaticData->atn;
}




void LinearTemporalLogicLexer::initialize() {
  ::antlr4::internal::call_once(lineartemporallogiclexerLexerOnceFlag, lineartemporallogiclexerLexerInitialize);
}