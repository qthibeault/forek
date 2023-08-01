#pragma once

#include "forek/mtl/visitor.h"

namespace forek::stl {
template <typename T>
class Visitor : public virtual mtl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace forek::stl
