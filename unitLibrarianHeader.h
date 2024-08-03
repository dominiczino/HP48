#ifndef unitLib_H
#define unitLib_H

#include <map>
#include <string>
#include <lineItemHeader.h>


namespace unitLibrarian {
    static std::map<std::string,float> length = {{"m",1},
            {"cm",100},
            {"mm",1000},
            {"in",39.3700787402},
            {"ft",3.28083989502}};

    static std::map<std::string,float> mass = {{"kg",1},
            {"g",1000},
            {"lbm",2.20462262185}};

    static std::map<std::string,float> libraries[2] = {length,mass};
 
    lineItem createSimpleConverter(std::string oldUnit, std::string newUnit);
    lineItem createDerivedConverter(std::map<std::string,float> oldDict, std::string newUnit);
    lineItem createBaseReductionConverter(std::map<std::string,float> oldDict);

};

#endif