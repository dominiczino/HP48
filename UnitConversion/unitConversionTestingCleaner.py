#HP48 Unit Conversion Testing
#This program is intended to develop a framework for how
#to mimic the unit conversion processes of the HP48 calculators
#prior to porting this all to C++ for embedded use.



class unitLibrarian():
#TODO reformulate slightly into a static class
    def __init__(self):
        #First set up fundamental unit libraries
        #self.fundamentals={"m","kg","s","A","K"}
        length={"NAME":"length","BASE":"m","m":1, "cm":100, "mm":1000, "in":39.3700787402, "ft":3.28083989502}
        mass={"NAME":"mass","BASE":"kg","kg":1, "g":1000, "lbm":2.20462262185}
        time={"NAME":"time", "BASE":"s","s":1,"ms":1000,"us":1e6}
        current={"NAME":"current","BASE":"A","A":1,"mA":1000}
        temp={"NAME":"temp","BASE":"K","K":1,"R":1.8}

        self.baseLibraries=[length,mass,time,current,temp]

        #Setting up derived unit libraries
        velocity={"NAME":"velocity","BASE":"m/s","m/s":1, "mph":2.23693629205}
        velocity["DEF"]={'m':1,'s':-1}
        
        force={"NAME":"force","BASE":"N","N":1, "lbf":0.2248089431}
        force["DEF"]={"kg":1,"m":1,"s":-2}
        
        energy={"NAME":"energy","BASE":"J", "J":1}
        energy["DEF"]={"kg":1,"m":2,'s':-2}

        self.derivedLibraries=[velocity,force,energy]
        self.libraries=self.baseLibraries+self.derivedLibraries

    def getCommonLibrary(self,oldUnit,newUnit):
        for library in self.libraries:
            #print(oldUnit,library['NAME'])
            if oldUnit in library:
                if newUnit in library:
                    return library
                else:
                    return None
        return None

    def getLibrary(self,unitString):
        for library in self.libraries:
            if unitString in library:
                return library
        return None


    def createSimpleUnitConversionFactor(self,oldUnit,newUnit):
        library=self.getCommonLibrary(oldUnit,newUnit)
        assert library
        factor=library[newUnit]/library[oldUnit]

        converter=entry(factor)
        converter.appendUnit(newUnit)
        converter.appendUnit(oldUnit,-1)
        return converter


    def creatDerivedUnitConverter(self,oldDict,newDerivedUnit):
        #This requires the unit to be reduced to base units before calling this function!
        converter=entry(1)
        library=self.getLibrary(newDerivedUnit)
        assert library

        defDict = library['DEF']
        assert oldDict==defDict
        
        converter.appendUnit(newDerivedUnit)
        for unit in defDict:
            power=defDict[unit]
            converter.appendUnit(unit,-power)
        return converter
        

    def createBaseReductionConverter(self,oldDict):
        newEntry=entry(1)
        factor=1
        for oldUnit in oldDict:
            lib=self.getLibrary(oldUnit)
            assert lib
            power=oldDict[oldUnit]
            factor*=(1/lib[oldUnit])**power
            if "DEF" in lib:
                for unit in lib["DEF"]:
                    newEntry.appendUnit(unit,lib['DEF'][unit])
            else:
                newEntry.appendUnit(lib["BASE"],power)
            newEntry.appendUnit(oldUnit,-power)
        newEntry*=entry(factor)
        return newEntry


################################################################################################################################################################

################################################################################################################################################################

################################################################################################################################################################

################################################################################################################################################################


            

class entry():
    def __init__(self,inputValue,inputUnit=None,unitPow=1):
        self.value=inputValue
        self.librarian=unitLibrarian()
        self.units=dict()
        if inputUnit is not None:
            self.units[inputUnit]=unitPow

    def appendUnit(self,newUnit,power=1):
        if newUnit in self.units:
            self.units[newUnit]+=power
        else:
            self.units[newUnit]=power

    def setUnitDict(self,newUnitDict):
        self.units=newUnitDict





        

    def __mul__(self,other):
        newEntry=entry(self.value*other.value)
        newUnits=dict()
        for unit in self.units:
            newUnits[unit]=self.units[unit]
        for unit in other.units:
            if unit in newUnits:
                newUnits[unit]+=other.units[unit]
            else:
                newUnits[unit]=other.units[unit]
        newEntry.setUnitDict(newUnits)
        newEntry.cleanUpUnitDict()
        return newEntry

    def __truediv__(self,other):
        newEntry=entry(self.value/other.value)
        newUnits=dict()
        for unit in self.units:
            newUnits[unit]=self.units[unit]
        for unit in other.units:
            if unit in newUnits:
                newUnits[unit]-=other.units[unit]
            else:
                newUnits[unit]=-other.units[unit]
        newEntry.setUnitDict(newUnits)
        newEntry.cleanUpUnitDict()
        return newEntry

    def __add__(self, other):
        if self.units!=other.units:
            return ValueError("Units do not agree")
        newEntry=entry(self.value+other.value)
        newEntry.setUnitDict(self.units)
        return newEntry

    def __sub__(self, other):
        if self.units!=other.units:
            return ValueError("Units do not agree")
        newEntry=entry(self.value-other.value)
        newEntry.setUnitDict(self.units)
        return newEntry

    def __pow__(self,other):
        if type(other) is int or type(other) is float:
            other=entry(other)
        for unit in other.units:
            if other.units[unit]!=0:
                raise ValueError("Exponent is not unitless")

        newEntry=entry(self.value**other.value)
        for unit in self.units:
            newEntry.appendUnit(unit,self.units[unit]*other.value)
        return newEntry




    def convertUnit(self,newUnit,fullpower=False):
        '''The general function for converting units.
            This function should be able to handle (or reject) any pair of unit names
        '''
        #First, check if a simple conversion will apply
        try:
            return self.convertUnitSimple(newUnit,fullpower)
        except AssertionError as e:
            pass

        #If it's not a simple conversion, then we need to try derived units
        try:
            reduced=self.reduceToBaseUnits()
            return reduced.convertUnitDerived(newUnit,fullpower)
        except AssertionError as e:
            print(e, "Not a derived conversion")


    def convertUnitDerived(self,newDerivedUnit,fullpower=False):
        defDict=self.librarian.getLibrary(newDerivedUnit)['DEF']

        #This verifies that we at have a power of the derived unit, if fullpower is enabled
        if defDict==self.units or (defDict!=self.units and fullpower):
            power=0
            for unit in self.units:
                if unit not in defDict:
                    if self.units[unit]!=0:
                        print(unit,self.units[unit],self.units,defDict)
                        assert False, "Missing a unit"
                elif power==0:
                    power=self.units[unit]/defDict[unit]
                else:
                    assert power==self.units[unit]/defDict[unit], "Unit Powers Mismatch"
        else:
            assert False

        converter = self.librarian.creatDerivedUnitConverter(self.units, newDerivedUnit)
        if fullpower:
            converter=converter**power
        return self*converter
        

    def convertUnitSimple(self,newUnit,fullpower=False):
        self.cleanUpUnitDict()
        assert len(self.units)==1, "Inconsistent Units"
        for unit in self.units: #Note that this doesn't iterate at all, because the dictionary is of length 1.
            oldUnit=unit
            power=self.units[unit]

        converter=self.librarian.createSimpleUnitConversionFactor(oldUnit,newUnit)
        if fullpower:
            converter=converter**entry(power)

        return self*converter


    def reduceToBaseUnits(self):
        converter=self.librarian.createBaseReductionConverter(self.units)
        return self*converter

    def cleanUpUnitDict(self):
        badbois=set()
        for unit in self.units:
            if self.units[unit]==0:
                badbois.add(unit)
        for badboi in badbois:
            del self.units[badboi]
        

    def __repr__(self):
        rep=""
        rep+=str(self.value)
        rep+=" "
        for unit in self.units:
            power=int(self.units[unit])
            if power>1 or power<=-1:
                rep+=unit
                rep+="^"
                rep+=str(power)
                rep+=","
            elif power==1:
                rep+=unit
                rep+=","
        return rep
            

################################################################################################################################################################

################################################################################################################################################################

################################################################################################################################################################

################################################################################################################################################################







A=entry(1,'m')
B=entry(20,'cm')
C=entry(5,'kg')

A.appendUnit("s",-1)
D=A.convertUnit("m/s")

E=entry(1,"N")
F=E*A
G=E*B
print(G.convertUnit("J"))






