#pragma once

#include "aabb.h"
#include "hittable.h"
#include <memory>

class BVHNode : public Hittable
{
  public:


    AABB bounding_box() const override
    {
        return bbox;
    }

  private:
    std::shared_ptr<BVHNode> left;
    std::shared_ptr<BVHNode> right;
    AABB bbox;
};