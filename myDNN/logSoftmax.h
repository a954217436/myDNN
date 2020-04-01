#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>


double myfunction(double num) 
{
    return exp(num);
}


template <typename T>
void logSoftmax(typename::std::vector<T> &v)
{
    double sum = 0.0;
    transform(v.begin(), v.end(), v.begin(), myfunction);
    sum = accumulate(v.begin(), v.end(), sum);
    std::cout << sum << std::endl;
    for (size_t i = 0; i < v.size(); ++i)
    {
        v.at(i) /= sum;
        v.at(i) = log(v.at(i));
    }

}


template<class ForwardIterator>
inline size_t argmax(ForwardIterator first, ForwardIterator last)
{
    return std::distance(first, std::max_element(first, last));
}
