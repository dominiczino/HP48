#include <iostream>
#include "lineItemHeader.h"
using namespace std;



int main() {
    try {
        lineItem A = lineItem::lineItem(25.4,"cm",1);
        A.appendUnit("m",0);
        A.cleanUpUnitDict();
        A.test();
        lineItem B = A.convertUnit("in");
    }
    catch (std::exception &e) {
        std::cerr << e.what();
    }
}