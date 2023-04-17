#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

void OverflowHandling(std::vector<uint32_t>& sum, bool& isOverflow, int32_t i) {
    isOverflow = sum[i] >= 10;
    sum[i] %= 10;
}

std::string GetSum(std::string bigUInt1, std::string bigUInt2) {
    std::string sumStr;
    std::vector<std::uint32_t> sum;
    bool isOverflow = false;
    std::reverse(bigUInt1.begin(), bigUInt1.end());
    std::reverse(bigUInt2.begin(), bigUInt2.end());
    if (bigUInt1.length() > bigUInt2.length())
        std::swap(bigUInt1, bigUInt2);
    uint32_t i = 0;
    for (; i < bigUInt1.length(); ++i) {
        sum.push_back((bigUInt1[i] - '0') + (bigUInt2[i] - '0') + (isOverflow ? 1 : 0));
        OverflowHandling(sum, isOverflow, i);
    }
    for (; i < bigUInt2.length(); ++i) {
        sum.push_back((bigUInt2[i] - '0') + (isOverflow ? 1 : 0));
        OverflowHandling(sum, isOverflow, i);
    }
    if (isOverflow)
        sum.push_back(1);
    for (uint32_t iDigit : sum)
        sumStr += std::to_string(iDigit);
    std::reverse(sumStr.begin(), sumStr.end());
    return sumStr;
}

int main() {
    std::string bigUInt1 = "";
    std::string bigUInt2 = "";
    std::cin >> bigUInt1 >> bigUInt2;
    std::cout << GetSum(bigUInt1, bigUInt2) << std::endl;
    return 0;
}
