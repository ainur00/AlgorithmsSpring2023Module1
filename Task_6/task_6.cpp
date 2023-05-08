#include <cstdint>
#include <iostream>
#include <cmath>
#include <vector>

uint32_t GetNearestTwoPower(uint32_t n) {
    uint32_t nearestTwoPower = 1;
    while (nearestTwoPower < n)
        nearestTwoPower *= 2;
    return nearestTwoPower;
}

struct Complex {
    Complex() = default;
    Complex(const double&);
    Complex(const double&, const double&);
    double re = 0;
    double im = 0;
    Complex& operator+=(const Complex&);
    Complex& operator*=(const Complex&);
    Complex& operator/=(const Complex&);
    Complex GetReverse() const;
    friend std::ostream& operator<<(std::ostream&, const Complex&);
};
Complex::Complex(const double& re) : re(re) {}
Complex::Complex(const double& re, const double& im) : re(re), im(im) {}
Complex& Complex::operator+=(const Complex& num) {
    re += num.re;
    im += num.im;
    return *this;
}
Complex operator+(const Complex& num1, const Complex& num2) {
    Complex copy = num1;
    copy += num2;
    return copy;
}
Complex& Complex::operator*=(const Complex& num) {
    double reCopy = re;
    double imCopy = im;
    re = reCopy * num.re - imCopy * num.im;
    im = reCopy * num.im + imCopy * num.re;
    return *this;
}
Complex operator*(const Complex& num1, const Complex& num2) {
    Complex copy = num1;
    copy *= num2;
    return copy;
}
Complex& Complex::operator/=(const Complex& num) {
    return *this *= num.GetReverse();
}
Complex operator/(const Complex& num1, const Complex& num2) {
    Complex copy = num1;
    copy /= num2;
    return copy;
}
Complex Complex::GetReverse() const {
    Complex copy = *this;
    copy.re = re / (re * re + im * im);
    copy.im = -im / (re * re + im * im);
    return copy;
}
std::ostream& operator<<(std::ostream& os, const Complex& complexNum) {
    os << complexNum.re << " + " << complexNum.im << "i";
    return os;
}

std::vector<Complex> GetRootsFromOne(const uint32_t& rootCount, const bool& isReversed) {
    std::vector<Complex> roots;
    double pi = acos(-1);
    for (uint32_t i = 0; i < rootCount; ++i)
        roots.push_back({cos((isReversed ? -1 : 1) * 2 * pi * i / rootCount), sin((isReversed ? -1 : 1) * 2 * pi * i / rootCount)});
    return roots;
}

struct WavFileHeader {
    int8_t chunkId[4];
    uint32_t chunkSize;
    int8_t format[4];
    int8_t subchunk1Id[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    int8_t subchunk2Id[4];
    uint32_t subchunk2Size;
};

std::vector<Complex> FFTRecursion(const std::vector<Complex>& inputData, std::vector<Complex>& oneRRoots, const uint32_t& inputDataSize) {
    if (inputData.size() == 1)
        return inputData;
    std::vector<Complex> inputDataFFT(inputData.size());
    std::vector<std::vector<Complex>> splittedData(2, std::vector<Complex>(inputData.size() / 2));
    for (uint32_t i = 0; i < inputData.size(); ++i)
        splittedData[i % 2 == 0 ? 0 : 1][i / 2] = inputData[i];
    std::vector<std::vector<Complex>> splittedDataFFT(2, std::vector<Complex>(inputData.size() / 2));
    splittedDataFFT[0] = FFTRecursion(splittedData[0], oneRRoots, inputDataSize);
    splittedDataFFT[1] = FFTRecursion(splittedData[1], oneRRoots, inputDataSize);
    for (uint32_t i = 0; i < inputDataFFT.size(); ++i)
        inputDataFFT[i] = splittedDataFFT[0][i % splittedDataFFT[0].size()] + splittedDataFFT[1][i % splittedDataFFT[1].size()] * oneRRoots[i * (inputDataSize / splittedDataFFT[1].size()) % oneRRoots.size()];
    return inputDataFFT;
}

std::vector<Complex> FFT(const int16_t* inputData, uint32_t inputDataSize) {
    std::vector<Complex> inputDataVector;
    uint32_t i = 0;
    for (; i < inputDataSize; ++i)
        inputDataVector.push_back(Complex(static_cast<double>(inputData[i])));
    uint32_t nearestTwoPower = GetNearestTwoPower(inputDataVector.size());
    for (; i < nearestTwoPower; ++i)
        inputDataVector.push_back({0});
    std::vector<Complex> oneRRoots = GetRootsFromOne(inputDataVector.size(), true);
    return FFTRecursion(inputDataVector, oneRRoots, inputDataVector.size());
}

std::vector<Complex> Modify(const std::vector<Complex>& fftData) {
    std::vector<Complex> modifiedData = fftData;
    for (uint32_t i = 1 * modifiedData.size() / 15; i < modifiedData.size(); ++i)
        modifiedData[i] = Complex(0.0, 0.0);
    return modifiedData;
}

std::vector<Complex> IFFTRecursion(const std::vector<Complex>& modifiedData, const std::vector<Complex>& oneRoots, const uint32_t& modifiedDataSize) {
    if (modifiedData.size() == 1)
        return modifiedData;
    std::vector<Complex> modifiedDataIFFT(modifiedData.size());
    std::vector<std::vector<Complex>> splittedData(2, std::vector<Complex>(modifiedData.size() / 2));
    for (uint32_t i = 0; i < modifiedData.size(); ++i)
        splittedData[i % 2 == 0 ? 0 : 1][i / 2] = modifiedData[i];
    std::vector<std::vector<Complex>> splittedDataIFFT(2, std::vector<Complex>(modifiedData.size() / 2));
    splittedDataIFFT[0] = IFFTRecursion(splittedData[0], oneRoots, modifiedDataSize);
    splittedDataIFFT[1] = IFFTRecursion(splittedData[1], oneRoots, modifiedDataSize);
    for (uint32_t i = 0; i < modifiedDataIFFT.size(); ++i)
        modifiedDataIFFT[i] = splittedDataIFFT[0][i % splittedDataIFFT[0].size()] + splittedDataIFFT[1][i % splittedDataIFFT[1].size()] * oneRoots[i * (modifiedDataSize / splittedDataIFFT[1].size()) % oneRoots.size()];
    return modifiedDataIFFT;
}

int16_t* IFFT(const std::vector<Complex>& modifiedData) {
    int16_t* outputData = new int16_t[modifiedData.size()];
    std::vector<Complex> oneRoots = GetRootsFromOne(modifiedData.size(), false);
    std::vector<Complex> IFFTModifiedData = IFFTRecursion(modifiedData, oneRoots, modifiedData.size());
    for (uint32_t i = 0; i < modifiedData.size(); ++i)
        outputData[i] = std::round(IFFTModifiedData[i].re) / modifiedData.size();
    return outputData;
}

int16_t* GetOutputData(int16_t* inputData, uint32_t inputDataSize) {
    std::vector<Complex> inputDataFFT = FFT(inputData, inputDataSize);
    std::vector<Complex> modifiedData = Modify(inputDataFFT);
    int16_t* outputData = IFFT(modifiedData);
    return outputData;
}

int32_t main() {
    FILE* wavInputFile = fopen("/home/user/Documents/Study/Cpp/ProblemSolver/Documents/input.wav", "r");
    WavFileHeader wavFileHeader;
    fread(&wavFileHeader, sizeof(wavFileHeader), 1, wavInputFile);
    int8_t* data = new int8_t[wavFileHeader.subchunk2Size];
    int16_t* inputData = reinterpret_cast<int16_t*>(data);
    uint32_t inputDataSize = wavFileHeader.subchunk2Size / 2;
    fread(data, wavFileHeader.subchunk2Size, 1, wavInputFile);
    fclose(wavInputFile);
    
    int16_t* outputData = GetOutputData(inputData, inputDataSize);

    FILE* wavOutputFile = fopen("/home/user/Documents/Study/Cpp/ProblemSolver/Documents/output.wav", "w");
    fwrite(&wavFileHeader, sizeof(wavFileHeader), 1, wavOutputFile);
    fwrite(outputData, inputDataSize * 2, 1, wavOutputFile);
    fclose(wavOutputFile);

    delete [] inputData;
    delete [] outputData;

    return 0;
}
