#include <iostream>
#include <vector>
#include <string>

class BigInteger {
    public:
        BigInteger() = default;
        BigInteger(const char*);
        BigInteger(const std::string&);
        BigInteger(int);
        explicit operator bool() const;
        BigInteger& operator+=(const BigInteger&);
        BigInteger& operator-=(const BigInteger&);
        BigInteger& operator*=(const BigInteger&);
        BigInteger& operator/=(const BigInteger&);
        BigInteger& operator%=(const BigInteger&);
        BigInteger operator-() const;
        BigInteger& operator++();
        BigInteger operator++(int);
        BigInteger& operator--();
        BigInteger operator--(int);
        friend bool operator<(const BigInteger&, const BigInteger&);
        friend bool operator>(const BigInteger&, const BigInteger&);
        friend bool operator<=(const BigInteger&, const BigInteger&);
        friend bool operator>=(const BigInteger&, const BigInteger&);
        friend bool operator==(const BigInteger&, const BigInteger&);
        friend bool operator!=(const BigInteger&, const BigInteger&);
        friend std::ostream& operator<<(std::ostream&, const BigInteger&);
        friend std::istream& operator>>(std::istream&, BigInteger&);
        std::string toString() const;
    private:
        std::vector<unsigned int> digits = {0};
        bool isNegative = false;
        static const unsigned int base = 10;
        static std::vector<unsigned int> GetVectorSum(const std::vector<unsigned int>&, const std::vector<unsigned int>&, const bool& isInRightOrder);
        static std::vector<unsigned int> GetVectorDifference(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static std::vector<unsigned int> GetVectorProduct(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static std::vector<unsigned int> GetVectorQuotient(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static std::vector<unsigned int> GetVectorRemainder(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static char GetVectorCompareResult(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static void DeleteLeadingZeros(std::vector<unsigned int>&);
        // secondary methods
        static std::vector<unsigned int> GetVectorProduct(const std::vector<unsigned int>&, const unsigned int&, const unsigned int&);
        static void Add(std::vector<unsigned int>&, const unsigned int&, const unsigned int&);
        static void PushBackVector(std::vector<unsigned int>&, const std::vector<unsigned int>&, const unsigned int&);
        static unsigned int GetTwoPower(const unsigned int&);
        static std::vector<unsigned int> GetVectorProduct(const std::vector<unsigned int>&, const std::vector<unsigned int>&, const bool&);
};
BigInteger::operator bool() const {
    return *this != BigInteger();
}
BigInteger::BigInteger(const char* num) : digits(std::vector<unsigned int>(0)) {
    if ((((num[0] == '-') || (num[0] == '+')) && (num[1] == '0')) || (num[0] == '0')) {
        digits.push_back(0);
        isNegative = false;
        return;
    }
    bool isStartFrom1 = false;
    switch (num[0]) {
        case '-':
            isNegative = true;
            isStartFrom1 = true;
            break;
        case '+':
            isStartFrom1 = true;
    }
    for (unsigned int i = (isStartFrom1 ? 1 : 0); num[i] != '\0'; ++i)
        digits.push_back(num[i] - '0');
    for (unsigned int i = 0; i < digits.size() / 2; ++i) {
        unsigned int temp = digits[i];
        digits[i] = digits[digits.size() - 1 - i];
        digits[digits.size() - 1 - i] = temp;
    }
}
BigInteger::BigInteger(const std::string& num) : BigInteger(&num[0]) {}
BigInteger::BigInteger(int num) : digits(std::vector<unsigned int>(0)) {
    if (num == 0) {
        isNegative = false;
        digits.push_back(0);
        return;
    }
    isNegative = num < 0;
    num = (isNegative ? -num : num);
    while (num != 0) {
        digits.push_back(num % base);
        num /= base;
    }
}
void BigInteger::DeleteLeadingZeros(std::vector<unsigned int>& digits) {
    while ((digits[digits.size() - 1] == 0) && (digits.size() > 1))
        digits.pop_back();
}
char BigInteger::GetVectorCompareResult(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2) {
    if (digits1.size() < digits2.size())
        return -1;
    if (digits2.size() < digits1.size())
        return 1;
    for (unsigned int i = digits1.size() - 1; i != static_cast<unsigned int>(-1); --i) {
        if (digits1[i] < digits2[i])
            return -1;
        if (digits1[i] > digits2[i])
            return 1;
    }
    return 0;
}
bool operator<(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    if (bigInt1.isNegative != bigInt2.isNegative)
        return bigInt1.isNegative;
    else {
        char compareResult = BigInteger::GetVectorCompareResult(bigInt1.digits, bigInt2.digits);
        return (bigInt1.isNegative ? (compareResult * (-1) == -1) : (compareResult == -1));
    }
}
bool operator>(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return bigInt2 < bigInt1;
}
bool operator==(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return !((bigInt1 < bigInt2) || (bigInt1 > bigInt2));
}
bool operator!=(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return !(bigInt1 == bigInt2);
}
bool operator<=(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return (bigInt1 < bigInt2) || (bigInt1 == bigInt2);
}
bool operator>=(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return (bigInt1 > bigInt2) || (bigInt1 == bigInt2);
}
std::vector<unsigned int> BigInteger::GetVectorSum(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2, const bool& isInRightOrder) {
    if (isInRightOrder) {
        std::vector<unsigned int> result;
        bool isOverflow = false;
        unsigned int i = 0;
        for (; i < digits1.size(); ++i) {
            unsigned int iSum = static_cast<unsigned int>(digits1[i]) + digits2[i] + (isOverflow ? 1 : 0);
            isOverflow = iSum >= base;
            iSum %= base;
            result.push_back(iSum);
        }
        for (; i < digits2.size(); ++i) {
            unsigned int iSum = digits2[i] + (isOverflow ? 1 : 0);
            isOverflow = iSum >= base;
            iSum %= base;
            result.push_back(iSum);
        }
        if (isOverflow)
            result.push_back(1);
        DeleteLeadingZeros(result);
        return result;
    } 
    else {
        if (digits1.size() < digits2.size())
            return GetVectorSum(digits1, digits2, true);
        else
            return GetVectorSum(digits2, digits1, true);
    }
}
std::vector<unsigned int> BigInteger::GetVectorDifference(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2) {
    std::vector<unsigned int> result = digits1;
    std::vector<unsigned int> digits2Copy = digits2;
    bool isTakeNext = false;
    for (unsigned int i = 0; i < digits1.size() - digits2.size(); ++i)
        digits2Copy.push_back(0);
    for (unsigned int i = 0; i < digits1.size(); ++i) {
        long long iDifference = static_cast<long long>(result[i]) - static_cast<long long>(digits2Copy[i]) - (isTakeNext ? 1 : 0);
        if (iDifference < 0) {
            isTakeNext = true;
            iDifference += base;
        } else
            isTakeNext = false;
        result[i] = iDifference;
    }
    DeleteLeadingZeros(result);
    return result;
}
std::vector<unsigned int> BigInteger::GetVectorProduct(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2) {
    unsigned int maxSize = GetTwoPower(std::max(digits1.size(), digits2.size()));
    std::vector<unsigned int> digits1Copy = digits1;
    std::vector<unsigned int> digits2Copy = digits2;
    for (unsigned int i = 0; i < maxSize - digits1.size(); ++i)
        digits1Copy.push_back(0);
    for (unsigned int i = 0; i < maxSize - digits2.size(); ++i)
        digits2Copy.push_back(0);
    std::vector<unsigned int> result = GetVectorProduct(digits1Copy, digits2Copy, true);
    for (unsigned int i = result.size() - 1; i != static_cast<unsigned int>(-1); --i)
        for (unsigned int j = i; j < result.size(); ++j)
            if (result[j] >= base) {
                if (j + 1 == result.size())
                    result.push_back(0);
                result[j + 1] += result[j] / base;
                result[j] %= base;
            }
            else
                break;
    DeleteLeadingZeros(result);
    return result;
}
std::vector<unsigned int> BigInteger::GetVectorProduct(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2, const bool&) {
    std::vector<unsigned int> result;
    unsigned int size = digits1.size();
    if (size == 1) {
        result.push_back(digits1[0] * digits2[0]);
        return result;
    }
    std::vector<unsigned int> digits11;
    std::vector<unsigned int> digits12;
    std::vector<unsigned int> digits21;
    std::vector<unsigned int> digits22;
    for (unsigned int i = 0; i < size; ++i) {
        if (i <= size / 2 - 1) {
            digits11.push_back(digits1[i]);
            digits21.push_back(digits2[i]);
        }
        else {
            digits12.push_back(digits1[i]);
            digits22.push_back(digits2[i]);
        }
    }
    std::vector<unsigned int> result0 = GetVectorProduct(GetVectorSum(digits11, digits12, false), GetVectorSum(digits21, digits22, false));
    std::vector<unsigned int> result1 = GetVectorProduct(digits11, digits21);
    std::vector<unsigned int> result3 = GetVectorProduct(digits12, digits22);
    std::vector<unsigned int> result2 = GetVectorDifference(result0, GetVectorSum(result1, result3, false));
    PushBackVector(result, result1, 0);
    PushBackVector(result, result2, size / 2);
    PushBackVector(result, result3, size);
    return result;
}
std::vector<unsigned int> BigInteger::GetVectorProduct(const std::vector<unsigned int>& digits, const unsigned int& num, const unsigned int& power) {
    std::vector<unsigned int> numV = {num};
    std::vector<unsigned int> base = {0, 1};
    for (unsigned int i = 0; i < power; ++i)
        numV = GetVectorProduct(numV, base);
    return GetVectorProduct(digits, numV);
}
std::vector<unsigned int> BigInteger::GetVectorQuotient(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2) {
    if (digits1.size() < digits2.size())
        return std::vector<unsigned int>(1, 0);
    std::vector<unsigned int> digits1Copy = digits1;
    std::vector<unsigned int> result(digits1.size() - digits2.size() + 1);
    unsigned int i = result.size() - 1;
    while (((digits1Copy.size() > digits2.size()) || ((digits1Copy.size() == digits2.size()) && (digits2[digits2.size() - 1] <= digits1Copy[digits1Copy.size() - 1]))) && (i != static_cast<unsigned int>(-1))) {
        while (GetVectorCompareResult(GetVectorProduct(digits2, result[i], i), digits1Copy) <= 0)
            ++result[i];
        --result[i];
        digits1Copy = GetVectorDifference(digits1Copy, GetVectorProduct(digits2, result[i], i));
        --i;
    }
    DeleteLeadingZeros(result);
    return result;
}
std::vector<unsigned int> BigInteger::GetVectorRemainder(const std::vector<unsigned int>& digits1, const std::vector<unsigned int>& digits2) {
    if (digits1.size() < digits2.size())
        return digits1;
    std::vector<unsigned int> digits1Copy = digits1;
    std::vector<unsigned int> result(digits1.size() - digits2.size() + 1);
    unsigned int i = result.size() - 1;
    while (((digits1Copy.size() > digits2.size()) || ((digits1Copy.size() == digits2.size()) && (digits2[digits2.size() - 1] <= digits1Copy[digits1Copy.size() - 1]))) && (i != static_cast<unsigned int>(-1))) {
        while (GetVectorCompareResult(GetVectorProduct(digits2, result[i], i), digits1Copy) <= 0)
            ++result[i];
        --result[i];
        digits1Copy = GetVectorDifference(digits1Copy, GetVectorProduct(digits2, result[i], i));
        --i;
    }
    DeleteLeadingZeros(digits1Copy);
    return digits1Copy;
}
BigInteger& BigInteger::operator+=(const BigInteger& bigInt) {
   if (isNegative == bigInt.isNegative)
       digits = GetVectorSum(digits, bigInt.digits, false);
   else {
       char digitsCompareResult = GetVectorCompareResult(digits, bigInt.digits);
       switch (digitsCompareResult) {
           case 1:
               digits = GetVectorDifference(digits, bigInt.digits);
               break;
           case -1:
               isNegative = !isNegative;
               digits = GetVectorDifference(bigInt.digits, digits);
               break;
           case 0:
               isNegative = false;
               digits = {0};
               break;
       }
   }
   return *this;
}
BigInteger operator+(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger result = bigInt1;
    result += bigInt2;
    return result;
}
BigInteger& BigInteger::operator-=(const BigInteger& bigInt) {
    return *this += (-bigInt);
}
BigInteger operator-(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger result = bigInt1;
    result -= bigInt2;
    return result;
}
BigInteger& BigInteger::operator*=(const BigInteger& bigInt) {
    isNegative = isNegative != bigInt.isNegative;
    digits = GetVectorProduct(digits, bigInt.digits);
    return *this;
}
BigInteger operator*(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger result = bigInt1;
    result *= bigInt2;
    return result;
}
BigInteger& BigInteger::operator/=(const BigInteger &bigInt) {
    isNegative = isNegative != bigInt.isNegative;
    digits = GetVectorQuotient(digits, bigInt.digits);
    if ((digits[0] == 0) && (digits.size() == 1))
        isNegative = false;
    return *this;
}
BigInteger operator/(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger result = bigInt1;
    result /= bigInt2;
    return result;
}
BigInteger& BigInteger::operator%=(const BigInteger& bigInt) {
    isNegative = isNegative != bigInt.isNegative;
    digits = GetVectorRemainder(digits, bigInt.digits);
    if ((digits[0] == 0) && (digits.size() == 1))
        isNegative = false;
    return *this;
}
BigInteger operator%(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger result = bigInt1;
    result %= bigInt2;
    return result;
}
BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    if (result != BigInteger())
        result.isNegative = !result.isNegative;
    return result;
}
BigInteger& BigInteger::operator++() {
    return *this = *this + 1;
}
BigInteger BigInteger::operator++(int) {
    BigInteger bigIntCopy = *this;
    ++(*this);
    return bigIntCopy;
}
BigInteger& BigInteger::operator--() {
    return *this = *this - 1;
}
BigInteger BigInteger::operator--(int) {
    BigInteger bigIntCopy = *this;
    --(*this);
    return bigIntCopy;
}
std::string BigInteger::toString() const {
    std::string bigIntString = "";
    if (isNegative)
        bigIntString += "-";
    for (auto it = digits.rbegin(); it != digits.rend(); ++it)
        bigIntString += std::to_string(*it);
    return bigIntString;
}
std::ostream& operator<<(std::ostream& os, const BigInteger& bigInt) {
    os << bigInt.toString();
    return os;
}
std::istream& operator>>(std::istream& is, BigInteger& bigInt) {
    std::string num = "";
    is >> num;
    bigInt = BigInteger(num);
    return is;
}
void BigInteger::Add(std::vector<unsigned int>& v, const unsigned int& index, const unsigned int& value) {
    unsigned int size = v.size();
    for (unsigned int i = 0; i + size <= index; ++i)
        v.push_back(0);
    v[index] += value;
}
void BigInteger::PushBackVector(std::vector<unsigned int>& to, const std::vector<unsigned int>& from, const unsigned int& index) {
    for (unsigned int i = 0; i < from.size(); ++i)
        Add(to, i + index, from[i]);
}
unsigned int BigInteger::GetTwoPower(const unsigned int& x) {
    unsigned int n = 1;
    while (x > n)
        n *= 2;
    return n;
}
