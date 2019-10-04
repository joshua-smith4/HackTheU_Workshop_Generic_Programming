#include <vector>
#include <functional>
#include <iostream>
#include <cmath>

template <class DataType>
auto pipeline_impl(DataType const& d)
{
    return d;
}

template <class DataType, class FirstFunctionType, class... FunctionTypePack>
auto pipeline_impl(DataType const& d, FirstFunctionType&& func, FunctionTypePack&&... funcs)
{
    return pipeline_impl(func(d), std::forward<FunctionTypePack&&>(funcs)...);
}

template <class DataType, class... FunctionTypePack>
auto pipeline(DataType const& d, FunctionTypePack&&... funcs)
{
    return pipeline_impl(d, std::forward<FunctionTypePack&&>(funcs)...);
}

double dotproduct(std::vector<double> const& w, std::vector<double> const& x)
{
    auto sum = 0.0;
    for(auto i = 0; i < x.size() && i < w.size(); ++i)
    {
        sum += x[i] * w[i];
    }
    return sum;
}

double add(double x, double y) { return x + y; }

float sigmoid(double logits) 
{ 
    auto exponential = std::exp(logits);
    return exponential / (1.0 + exponential);
}

bool isCat(float prob) { return prob >= 0.5; }

int main()
{
    std::vector<double> cat_data = {1,5,3,0.2,0.4,9};
    std::vector<double> weights = {-1,3,-10,8,19,6};
    double bias = 1.4;
    std::cout << "Is it a cat?\n";

    /* This is application specific code that can an should be abstracted
     * 
    auto bound_dotproduct = std::bind(dotproduct, std::cref(weights), std::placeholders::_1);
    auto product = bound_dotproduct(cat_data);
    auto bound_add = std::bind(add, std::placeholders::_1, bias);
    auto logit = bound_add(product);
    auto probs = sigmoid(logit);
    auto itIsACat = isCat(probs);
    */

    auto itIsACat = pipeline(
            cat_data,
            std::bind(dotproduct, std::cref(weights), std::placeholders::_1),
            std::bind(add, std::placeholders::_1, bias),
            sigmoid,
            isCat);

    std::cout << "The answer is: " << (itIsACat ? "YES!" : "Sadly, no") << "\n";

    return 0;
}

