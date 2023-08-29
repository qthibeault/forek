#pragma once

#include <exception>
#include <stdexcept>

#include "forek/errors.h"

#include "BaseErrorListener.h"
#include "Recognizer.h"
#include "Token.h"

namespace forek::errors {

class LexerErrorListener : public antlr4::BaseErrorListener {
   public:
    void syntaxError(antlr4::Recognizer *, antlr4::Token *token, size_t line, size_t charPosition,
                     const std::string &msg, std::exception_ptr eptr) override {
        throw LexerError{msg};
    }
};

class ParserErrorListener : public antlr4::BaseErrorListener {
   public:
    void syntaxError(antlr4::Recognizer *, antlr4::Token *token, size_t line, size_t charPosition,
                     const std::string &msg, std::exception_ptr eptr) override {
        throw ParserError{msg};
    }
};

}  // namespace forek::errors
