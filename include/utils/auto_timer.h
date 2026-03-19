#include <chrono>
#include <iostream>
#include <string>


class AutoTimer
{
  public:
    AutoTimer(const std::string &name) : name_(name), start_time_(std::chrono::steady_clock::now())
    {
    }

    ~AutoTimer()
    {
        auto end_time = std::chrono::steady_clock::now();

        auto ns_count = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_).count();

        std::cout << "[Timer] " << name_ << " time elapsed: ";

        if (ns_count < 1'000)
        {
            std::cout << ns_count << "ns\n";
        }
        else if (ns_count < 1'000'000)
        {
            std::cout << (ns_count / 1'000.0) << "us\n";
        }
        else if (ns_count < 1'000'000'000)
        {
            std::cout << (ns_count / 1'000'000.0) << "ms\n";
        }
        else
        {
            std::cout << (ns_count / 1'000'000'000.0) << "s\n";
        }
    }

  private:
    std::string name_;
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
};