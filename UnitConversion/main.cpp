#include <iostream>
#include "lineItemHeader.h"
using namespace std;



int main() {
    try {
        lineItem A = lineItem::lineItem(100,"cm",1);
        A.appendUnit("m",0);
        A.cleanUpUnitDict();
        lineItem B = A.convertUnit("m");
        lineItem C = lineItem::lineItem(10,"s",-1);
        lineItem D = B*C;
        lineItem E = D.convertUnitDerived("m/s");


    }
    catch (std::exception &e) {
        std::cerr << e.what();
    }
}