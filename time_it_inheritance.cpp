#include <iostream>
#include <vector>
#include <set>
#include <chrono>

template <class CallableType>
class Timeable {};

/*
 * Inherit from this class if you want to time yourself!
 */
template <class ReturnType, class... ArgsTypePack>
class Timeable<ReturnType(ArgsTypePack...)>
{
public:
    auto timeIt(ArgsTypePack&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        auto retVal = timed_function(std::forward<ArgsTypePack&&>(args)...);
        auto end = std::chrono::steady_clock::now();
        log_callback(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        return retVal;
    }
private:
    /*
     * pure virtual function to be timed
     * This function is implemented by the derived class
     */
    virtual ReturnType timed_function(ArgsTypePack...) = 0;

    /*
     * function used as a log callback for the duration
     * pure virtual
     */
    virtual void log_callback(long long) = 0;
};

/*
 * Example of a class that inherits from Timeable
 */
template <class ContainerType>
class GetMaxElement : public Timeable<typename ContainerType::value_type(ContainerType const&)>
{
public:
    typename ContainerType::value_type operator()(ContainerType const& v)
    {
        return *std::max_element(v.begin(), v.end());
    }
private:
    virtual typename ContainerType::value_type timed_function(ContainerType const& v) override final
    {
        return this->operator()(v);
    }
    virtual void log_callback(long long d) override final
    {
        std::cout << "Nanoseconds: " << d << "\n";
    }
};

int main()
{
    auto v = std::vector<int>{1,2,3,4,5,6,7,8,9};
    auto timeableMaxElementObject = GetMaxElement<decltype(v)>();
    timeableMaxElementObject.timeIt(v);
}

