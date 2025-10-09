#include "Calculator.h"
#include <chrono>

void test(Calculator& calculator, std::string expression, double expected, int& testNumber) {

    using namespace std::chrono;

    time_point<system_clock> start;
    time_point<system_clock> end;
    duration<double> diff;
    

    start = system_clock::now();
    double res = calculator.calc(expression);
    end = system_clock::now();

    diff = end - start;
    printf("TEST  %d:\t", ++testNumber);
    printf("%lf ms\t", diff.count() * 1000);
    if (abs(res - expected) < 1e-7) {
        printf("OK\n"); 
    } else {
        printf("WA:\n");
        printf("\texpression: \"%s\"\n", expression.c_str());
        printf("\texpected: %lf\n", expected);
        printf("\tfound: %lf\n", res);
    }
}

int main() {
    Calculator calc;
    int testCnt = 0;
    
    using namespace std::chrono;
    time_point<system_clock> testingStart;
    time_point<system_clock> testingEnd;
    duration<double> testingDiff;

    testingStart = system_clock::now();    
    test(calc, "2+3*4", 14.0, testCnt);
    test(calc, "2*(3+4)", 14.0, testCnt);
    test(calc, "2^3", 8.0, testCnt);
    test(calc, "(2+3)*4", 20.0, testCnt);
    test(calc, "2*(3+4)*5", 70.0, testCnt);
    test(calc, "2^3^2", 64.0, testCnt);
    test(calc, "sin(0)", 0.0, testCnt);
    test(calc, "cos(0)", 1.0, testCnt);
    test(calc, "sin(4.15)^2+cos(4.15)^2", 1.0, testCnt);
    test(calc, "2+2*(3^(2+3))", 488.0, testCnt);
    test(calc, "(sqrt(6^2))", 6.0, testCnt);
    test(calc, "(sqrt(6^6))", 216.0, testCnt);
    testingEnd = system_clock::now();

    testingDiff = testingEnd - testingStart;

    printf("Total time:  %.3lf ms\n", testingDiff.count() * 1000);

}