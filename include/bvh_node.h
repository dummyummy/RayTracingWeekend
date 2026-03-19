#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include <memory>
#include <vector>

class BVHNode : public Hittable
{
  public:
    BVHNode() = default;
    BVHNode(HittableList list) : BVHNode(list.hittables, 0, list.hittables.size())
    {
    }

    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end)
    {
        bbox = AABB::empty;

        for (size_t i = start; i < end; i++)
            bbox = AABB(bbox, objects[i]->bounding_box());

        int axis = bbox.longest_axis();

        auto cmp = axis == 0 ? box_x_compare : axis == 1 ? box_y_compare : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 0)
        {
            left = right = nullptr;
        }
        else if (object_span == 1)
        {
            left = right = objects[start];
        }
        else if (object_span == 2)
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            std::sort(objects.begin() + start, objects.begin() + end, cmp);
            auto mid = start + object_span / 2;
            left = make_shared<BVHNode>(objects, start, mid);
            right = make_shared<BVHNode>(objects, mid, end);
        }
    }

    static bool box_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b, int axis)
    {
        auto a_axis_interval = a->bounding_box().axis_interval(axis);
        auto b_axis_interval = b->bounding_box().axis_interval(axis);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
    {
        return box_compare(a, b, 0);
    }
    static bool box_y_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
    {
        return box_compare(a, b, 1);
    }
    static bool box_z_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
    {
        return box_compare(a, b, 2);
    }

    bool hit(const Ray &ray, Interval t, HitRecord &rec) const override
    {
        if (!bbox.hit(ray, t))
            return false;

        // Test both child nodes
        bool hit_left = left->hit(ray, t, rec);
        bool hit_right = right->hit(ray, Interval(t.min, hit_left ? rec.t : t.max), rec);

        return hit_left || hit_right;
    }

    AABB bounding_box() const override
    {
        return bbox;
    }

  private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;
};