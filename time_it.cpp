#include <chrono>
#include <set>
#include <unordered_set>
#include <array>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <optional>

/*
 * timeFindMax is extremely specific.
 * It is type specific and it ignores generalities
 * that are easily made.
 *
std::optional<int> timeFindMax(std::vector<int> const& v)
{
    std::chrono::time_point start = std::chrono::steady_clock::now();
    std::vector<int>::const_iterator i = std::max_element(v.begin(), v.end());
    std::chrono::time_point end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " nanoseconds\n";
    if(i == v.end()) return {};
    return *i;
}

* This is a revision in the right direction.
* The internal types are more general but our interface
* is still just as specific as before.
*
std::optional<int> timeFindMax(std::vector<int> const& v)
{
    auto start = std::chrono::steady_clock::now();
    auto i = std::max_element(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " nanoseconds\n";
    if(i == v.end()) return {};
    return *i;
}

* The interface is now a little more flexible,
* being able to handle vectors of different types
* as long as they compare with the operator<
*
template <class ContainedType>
std::optional<ContainedType> timeFindMax(std::vector<ContainedType> const& v)
{
    auto start = std::chrono::steady_clock::now();
    auto i = std::max_element(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " nanoseconds\n";
    if(i == v.end()) return {};
    return *i;
}
*/

/*
 * Now we have generalized to a family of container types.
 * All STL containers and custom containers that implement the iterator interface
 * and value_type can be timed using this function.
 * At this point, things are pretty well type agnostic
 * but what about the more general case of timing any function.
 */
template <class ContainerType>
std::optional<typename ContainerType::value_type> timeFindMax(ContainerType const& v)
{
    auto start = std::chrono::steady_clock::now();
    auto i = std::max_element(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " nanoseconds\n";
    if(i == v.end()) return {};
    return *i;
}

/*
 * Now we are getting somewhere! We can pass in any function,
 * functor, callable object, lambda expression, (anything that behaves like a function)
 * and time it then return the functions value.
 */
template <class FunctionType>
auto timeFunction(FunctionType&& func)
{
    auto start = std::chrono::steady_clock::now();
    auto i = func();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Duration: " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " nanoseconds\n";
    return i;
}

/*
 * This takes the generalization one step further.
 * All the previous examples assume the end goal of timing the process
 * is to print the value to the console. The below implementation
 * allows the user to inject a callback (again, any function, funtor, lambda, etc)
 * and perform any desired action with the timing information.
 */
template <class LogCallbackType, class FunctionType>
auto timeFunction(LogCallbackType&& log_callback, FunctionType&& func)
{
    auto start = std::chrono::steady_clock::now();
    auto i = func();
    auto end = std::chrono::steady_clock::now();
    log_callback(end - start);
    return i;
}

/*
 * Now we are getting serious. Pass in any log_callback callable, any function type callable,
 * and any arguments to be passed to the function to be timed and get the result as
 * well as timing information injected into the callback.
 * This is pretty general and will last a long time.
 */
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
    /*
     * This section is just to illustrate the simple interface
     * that generic code provides to the developer while
     * not greatly limiting the developer in how he/she
     * desires to interface with it.
     */
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
        std::cout << "nanoseconds: " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(t).count() << "\n"; 
    };
    auto us_min_func = MinElementCallableObject(us);
    auto us_min_iter = timeFunction(log_func, us_min_func);

    auto us_max_iter = timeFunction(log_func, std::max_element<decltype(us)::iterator>, us.begin(), us.end());
    return 0;
}
