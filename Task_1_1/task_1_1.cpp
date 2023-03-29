#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

int32_t GetMinDayCount(int32_t requestedBacteriaCount) {
    std::vector<int32_t> cache(requestedBacteriaCount + 1);
    cache.push_back(0);
    cache.push_back(0);
    for (int32_t i = 2; i <= requestedBacteriaCount; ++i) {
        cache[i] = cache[i - 1];
        if (i % 4 == 0)
            cache[i] = std::min(cache[i], cache[i / 4]);
        if (i % 5 == 0)
            cache[i] = std::min(cache[i], cache[i / 5]);
        ++cache[i];
    }
    return cache[requestedBacteriaCount];
}

int main() {
    int32_t requestedBacteriaCount = 0;
    std::cin >> requestedBacteriaCount;
    std::cout << GetMinDayCount(requestedBacteriaCount) << std::endl;
    return 0;
}
