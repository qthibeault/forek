#pragma once

#include <any>
#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>

#include "forek/visitors.h"

namespace forek::IR {
class Node {
   public:
    virtual ~Node() = default;
};

namespace ltl {
class Node : public IR::Node {
   public:
    ~Node() override = default;
};

class Always : public Node {
   public:
    explicit Always(std::unique_ptr<Node> inner) : inner_{std::move(inner)} {}

   private:
    std::unique_ptr<Node> inner_;
};

class PastNode : public IR::Node {
   public:
    ~PastNode() override = default;
};
}  // namespace ltl

namespace pl {
template <typename T>
class AnyVisitor : public forek::pl::Visitor<std::any> {
    public:
     explicit AnyVisitor(const forek::pl::Visitor<T>& v) : v_{v} {}

     auto visit_true() -> std::any override {
         return v_.visit_true();
     }

     auto visit_false() -> std::any override {
         return v_.visit_false();
     }

     auto visit_conjunction(std::any left, std::any right) -> std::any override {
         return v_.visit_conjunction(std::any_cast<T>(left), std::any_cast<T>(right));
     }

    private:
     const forek::pl::Visitor<T>& v_;
};

class Node : public ltl::Node {
   public:
    ~Node() override = default;

    [[nodiscard]]
    virtual auto visit(forek::pl::Visitor<std::any>& v) const -> std::any = 0;
};

class True : public Node {
   public:
    ~True() override = default;

    [[nodiscard]]
    auto visit(forek::pl::Visitor<std::any>& v) const -> std::any override {
        return v.visit_true();
    }
};

class False : public Node {
   public:
    ~False() override = default;

    auto visit(forek::pl::Visitor<std::any>& v) const -> std::any override {
        return v.visit_false();
    }
};

class Proposition : public Node {
   public:
    Proposition() = delete;
    ~Proposition() override = default;

    explicit Proposition(std::string name) : name_{std::move(name)} {}
    explicit Proposition(std::string&& name) : name_{name} {}

    auto name() -> std::string& { return name_; }
    
    template <typename T>
    auto visit(forek::pl::Visitor<T>& v) -> T { return v.visit_proposition(name_); }

   private:
    std::string name_;
};

class Negation : public Node {
   public:
    explicit Negation(std::unique_ptr<Node> inner) : inner_{std::move(inner)} {}

    template <typename T>
    auto visit(forek::pl::Visitor<T>& v) -> T { return v.visit_negation(inner_->visit(v)); }

   private:
    std::unique_ptr<Node> inner_;
};

class BinaryNode : public Node {
   public:
    explicit BinaryNode(std::unique_ptr<Node> left, std::unique_ptr<pl::Node> right)
        : left_{std::move(left)}, right_{std::move(right)} {}

   private:
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
};

class Conjunction : public BinaryNode {
   public:
    explicit Conjunction(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : BinaryNode{std::move(left), std::move(right)} {}
};

class Disjunction : public BinaryNode {
   public:
    explicit Disjunction(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : BinaryNode{std::move(left), std::move(right)} {}
};

class Implication : public BinaryNode {
   public:
    explicit Implication(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : BinaryNode{std::move(left), std::move(right)} {}
};

class Equivalence : public BinaryNode {
   public:
    explicit Equivalence(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : BinaryNode{std::move(left), std::move(right)} {}
};
}  // namespace pl
}  // namespace forek::IR
