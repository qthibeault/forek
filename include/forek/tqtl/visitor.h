#pragma once

#include "forek/tptl/visitor.h"

namespace forek::tqtl {
template <typename T>
class Visitor : public virtual tptl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace forek::tqtl
