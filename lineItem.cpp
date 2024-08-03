#include <iostream>
#include <functional>
#include <math.h>
#include <map>
#include "lineItemHeader.h"
using namespace std;


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

    map<string,float> getLibrary(string unit) {
        for(int i = 0; i<sizeof(libraries); i++) {
            if (libraries[i].find(unit)!=libraries[i].end()) {
                return libraries[i];
            }
        }
        throw runtime_error("Didn't find a library");
    }

    map<string,float> getCommonLibrary(string oldUnit, string newUnit) {
        map<string,float> lib1 = getLibrary(oldUnit);
        map<string,float> lib2 = getLibrary(newUnit);
        if (lib1!=lib2) {
            throw runtime_error("no common library");
        } else {
            return lib1;
        }
    }
 
    lineItem createSimpleConverter(std::string oldUnit, std::string newUnit) {
        map<string,float> lib = getCommonLibrary(oldUnit,newUnit);
        float factor = lib[newUnit] / lib[oldUnit];
        lineItem converter = lineItem(factor,"none",0);
        converter.appendUnit(newUnit,1);
        converter.appendUnit(oldUnit,-1);
        return converter;
    }

    lineItem createDerivedConverter(std::map<std::string,float> oldDict, std::string newUnit) {
        return lineItem(2.5,"in",1);
    }
    lineItem createBaseReductionConverter(std::map<std::string,float> oldDict) {
        return lineItem(2,"in",1);
    }

};






lineItem lineItem::convertUnitSimple(std::string newUnit) {
    this->cleanUpUnitDict();
    if(units.size() != 1) {
        throw runtime_error("[ERROR] Too many units, not simple");
    }

    string oldUnit = units.begin()->first;
    lineItem converter = unitLibrarian::createSimpleConverter(oldUnit,newUnit);
    return *this * &converter;
}


lineItem lineItem::convertUnitDerived(std::string newUnit) {
    return lineItem(2.5,"in",1);
}

lineItem::lineItem(float val, std::string unit, float power) {
    value=val;
    if(power!=0) {
        units[unit]=power;
    }
}


float lineItem::getValue() const { 
    return value;
}
    
void lineItem::appendUnit(string newUnit, float power) {
    if(units.find(newUnit)==units.end()) {
        units[newUnit] = power;
    }
    else {
        units[newUnit] = units[newUnit] + power;
    }
}

void lineItem::setUnitDict(map<string,float> newUnits) {
    units = newUnits;
}

void lineItem::test() {

}




lineItem lineItem::operator*(lineItem* const other) {
    float newVal = this->getValue() * other->getValue();
    lineItem result = lineItem(newVal, "none",0);

    //merge the unit maps
    for (auto i=this->units.begin(); i!= this->units.end(); i++) {
        result.appendUnit(i->first, i->second);
    }
    for (auto i=other->units.begin(); i!= other->units.end(); i++) {
        result.appendUnit(i->first, i->second);
    }
    result.cleanUpUnitDict();
    return result;
}






lineItem lineItem::convertUnit(string newUnit) {
    return this->convertUnitSimple(newUnit);
}


lineItem lineItem::reduceToBaseUnits() {
    lineItem converter = unitLibrarian::createBaseReductionConverter(units);

    return *this * &converter;
}

void lineItem::cleanUpUnitDict() {

    for (auto it = units.cbegin(); it != units.cend() /* not hoisted */; /* no increment */) {
        if (it->second==0) {
            units.erase(it++);    // or "it = m.erase(it)" since C++11
        }
        else {
            ++it;
        }
    }

}



