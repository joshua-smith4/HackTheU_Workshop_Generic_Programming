#include <chrono>
#include <set>
#include <unordered_set>
#include <array>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <optional>

std::optional<int> timeFindMax(std::vector<int> const& v)
{
    std::chrono::time_point start = std::chrono::steady_clock::now();
    std::vector<int>::const_iterator i = std::max_element(v.begin(), v.end());
    std::chrono::time_point end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " microseconds\n";
    if(i == v.end()) return {};
    return *i;
}

std::optional<int> timeFindMax(std::vector<int> const& v)
{
    auto start = std::chrono::steady_clock::now();
    auto i = std::max_element(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " microseconds\n";
    if(i == v.end()) return {};
    return *i;
}

template <class ContainedType>
std::optional<ContainedType> timeFindMax(std::vector<ContainedType> const& v)
{
    auto start = std::chrono::steady_clock::now();
    auto i = std::max_element(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " microseconds\n";
    if(i == v.end()) return {};
    return *i;
}

template <class ContainerType>
std::optional<typename ContainerType::value_type> timeFindMax(ContainerType const& v)
{
    auto start = std::chrono::steady_clock::now();
    auto i = std::max_element(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " microseconds\n";
    if(i == v.end()) return {};
    return *i;
}

template <class FunctionType>
auto timeFunction(FunctionType&& func)
{
    auto start = std::chrono::steady_clock::now();
    auto i = func();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " microseconds\n";
    return i;
}

template <class LogCallbackType, class FunctionType>
auto timeFunction(LogCallbackType&& log_callback, FunctionType&& func)
{
    auto start = std::chrono::steady_clock::now();
    auto i = func();
    auto end = std::chrono::steady_clock::now();
    log_callback(end - start);
    return i;
}

template <class LogCallbackType, class FunctionType, class... ArgsTypePack>
auto timeFunction(LogCallbackType&& log_callback, FunctionType&& func, ArgsTypePack&&... args)
{
    auto start = std::chrono::steady_clock::now();
    auto i = func(std::forward<ArgsTypePack&&>(args)...);
    auto end = std::chrono::steady_clock::now();
    log_callback(end - start);
    return i;
}

template <class ContainerType>
struct MinElementCallableObject
{
    MinElementCallableObject(ContainerType c)
        : container(c)
    {
    }

    auto operator()()
    {
        return std::min_element(container.begin(), container.end());
    }

    private:
    ContainerType container;
};

int main()
{
    auto v = std::vector<int>{1,2,3,4,5,6,7,8,9};
    auto s = std::set<double>{1,2,3,4,5,6,7,8,9};
    auto us = std::unordered_set<float>{1,2,3,4,5,6,7,8,9};

    auto v_max = timeFindMax(v);
    auto s_max = timeFindMax(s);
    auto us_max = timeFindMax(us);

    auto v_max_func = [&v](){ return std::max_element(v.begin(), v.end()); };
    auto v_max_iter = timeFunction(v_max_func);

    auto s_min_func = MinElementCallableObject(s);
    auto s_min_iter = timeFunction(s_min_func);

    auto log_func = [](auto t)
    { 
        std::cout << "microseconds: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t).count() << "\n"; 
    };
    auto us_min_func = MinElementCallableObject(us);
    auto us_min_iter = timeFunction(log_func, us_min_func);

    auto us_max_iter = timeFunction(log_func, std::max_element<decltype(us)::iterator>, us.begin(), us.end());
    return 0;
}
