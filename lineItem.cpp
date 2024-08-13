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

    static std::map<std::string,float> time = {{"s",1}};

    static std::map<std::string,float> velocity = {{"m/s",1},
            {"mph",2.23693629205}};
    
    static std::map<std::string,float> velocityDef = {{"m",1},{"s",-1}};



    static std::map<std::string,float> libraries[3] = {length,mass,velocity};

    static std::map<string,std::map<string,float>> definitions = {{"m/s",velocityDef}};



    map<string,float> getLibrary(string unit) {
        for(int i = 0; i<sizeof(libraries); i++) {
            if (libraries[i].find(unit)!=libraries[i].end()) {
                return libraries[i];
            }
        }
        throw runtime_error("Could not find  library");
    }


    string getbaseVersion(string unit) {
        map<string,float> lib = getLibrary(unit);
        for(auto i=lib.begin(); i!=lib.end();i++) {
            if(i->second == 1) {
                return i->first;
            }
        }
        throw runtime_error("Could not find base unit");
    }

    map<string,float> getCommonLibrary(string oldUnit, string newUnit) {
        map<string,float> lib1 = getLibrary(oldUnit);
        map<string,float> lib2 = getLibrary(newUnit);
        if (lib1!=lib2) {
            throw runtime_error("Units belong to different libraries");
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
        map<string,float> lib = getLibrary(newUnit);
        float factor=1;

        string baseUnit = getbaseVersion(newUnit);
        map<string,float> definition = definitions[baseUnit];

        if (definition!=oldDict) {
            throw runtime_error("Units do not match definition");
        }

        lineItem converter = lineItem(1,"",0);
        converter.appendUnit(newUnit,1);
        for(auto i=oldDict.begin(); i!=oldDict.end();i++) {
            converter.appendUnit(i->first,-1 * i->second);
        }
        return converter;
    }


    lineItem createBaseReductionConverter(std::map<std::string,float> oldDict) {
        for (auto i=oldDict.begin(); i!= oldDict.end(); i++) {
            //TODO, this
        }
        return lineItem(10,"in",2);
    }

};






lineItem lineItem::convertUnitSimple(std::string newUnit) {
    this->cleanUpUnitDict();
    if(units.size() != 1) {
        throw runtime_error("[ERROR] Too many units, not simple");
    }

    string oldUnit = units.begin()->first;
    lineItem converter = unitLibrarian::createSimpleConverter(oldUnit,newUnit);
    return *this * converter;
}


lineItem lineItem::convertUnitDerived(std::string newUnit) {
    this->cleanUpUnitDict();
    lineItem converter = unitLibrarian::createDerivedConverter(this->units,newUnit);
    return *this * converter;
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
    cleanUpUnitDict();
}

void lineItem::setUnitDict(map<string,float> newUnits) {
    units = newUnits;
}



lineItem lineItem::operator*(lineItem const other) {
    float newVal = this->getValue() * other.getValue();
    lineItem result = lineItem(newVal, "none",0);

    //merge the unit maps
    for (auto i=this->units.begin(); i!= this->units.end(); i++) {
        result.appendUnit(i->first, i->second);
    }
    for (auto i=other.units.begin(); i!= other.units.end(); i++) {
        result.appendUnit(i->first, i->second);
    }
    result.cleanUpUnitDict();
    return result;
}






lineItem lineItem::convertUnit(string newUnit) {
    if(units.size() != 1) {
        return this->convertUnitDerived(newUnit);
    } else {
        return this->convertUnitSimple(newUnit);
    }
}


lineItem lineItem::reduceToBaseUnits() {
    lineItem converter = unitLibrarian::createBaseReductionConverter(units);
    return *this * converter;
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



