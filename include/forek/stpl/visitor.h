#pragma once

#include "forek/tqtl/visitor.h"

namespace forek::stpl {
template <typename T>
class Visitor : public virtual tqtl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace forek::stpl
