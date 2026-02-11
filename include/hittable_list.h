#pragma once

#include "hittable.h"

class HittableList : public Hittable
{
  public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> hittable)
    {
        add(hittable);
    };

    void clear()
    {
        hittables.clear();
    }

    void add(std::shared_ptr<Hittable> hittable)
    {
        hittables.push_back(hittable);
    }

    bool hit(const Ray &ray, Interval t, HitRecord &rec) const override
    {
        bool hit_anything = false;
        double closest_so_far = t.max;
        for (const auto &hittable : hittables)
        {
            if (hittable->hit(ray, Interval(t.min, closest_so_far), rec))
            {
                hit_anything = true;
                closest_so_far = rec.t;
            }
        }
        return hit_anything;
    }

  private:
    std::vector<std::shared_ptr<Hittable>> hittables;
};