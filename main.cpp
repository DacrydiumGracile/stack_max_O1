#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

int main()
{
    std::vector<int> v1(100);
    std::vector<int> v2(100);

    auto it1 = v1.begin();
    auto it2 = v1.end();
    auto it3 = v2.begin();
    // std::copy(it1, it2, it3);
    auto d2 = v2.data();
    auto d1 = v1.data();
    std::memcpy(d2, d1, 100 * sizeof(int));
    std::cout << "Hello, World!" << std::endl;
    return 0;
}