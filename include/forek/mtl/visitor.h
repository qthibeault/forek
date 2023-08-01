#pragma once

#include "forek/ltl/visitor.h"

namespace forek::mtl {
template <typename T>
class Visitor : public virtual ltl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace forek::mtl
