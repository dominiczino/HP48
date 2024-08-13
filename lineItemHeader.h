#ifndef lineItemHeader_H
#define lineItemHeader_H

#include <map>
#include <string>


class lineItem {
private:
    float value;
    std::map<std::string,float> units;
    lineItem convertUnitSimple(std::string newUnit);
    


public:
    lineItem(float val, std::string unit, float power);

    float getValue() const;
    
    void appendUnit(std::string newUnit, float power);

    void setUnitDict(std::map<std::string,float> newUnits);

    lineItem convertUnit(std::string newUnit);

    lineItem reduceToBaseUnits();

    void cleanUpUnitDict();

    lineItem operator*(lineItem const other);

    lineItem operator+(lineItem const other);

    lineItem operator-(lineItem const other);

    lineItem operator/(lineItem const other);

    lineItem convertUnitDerived(std::string newUnit);

};

#endif