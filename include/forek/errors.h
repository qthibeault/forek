#pragma once

#include <stdexcept>
#include <string>

namespace forek::errors {

class Error : public std::exception {
   public:
    explicit Error(std::string msg) : m_msg{std::move(msg)} {}

    [[nodiscard]] auto what() const noexcept -> const char* override { return m_msg.c_str(); }

   private:
    std::string m_msg;
};

class ParserError : public Error {
   public:
    using Error::Error;
};

class LexerError : public Error {
   public:
    using Error::Error;
};

}  // namespace forek::errors
