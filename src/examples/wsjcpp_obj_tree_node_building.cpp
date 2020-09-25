
#include "wsjcpp_obj_tree_node_building.h"
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

// ---------------------------------------------------------------------
// Address

Address::Address() {

}

// ---------------------------------------------------------------------

Address::Address(const Address& address) {
    m_sStreetName = address.getStreetName();
    m_sBuildingNumber = address.getBuildingNumber();
}

// ---------------------------------------------------------------------

Address::Address(std::string sStreetName, std::string sBuildingNumber) {
    m_sStreetName = sStreetName;
    m_sBuildingNumber = sBuildingNumber;
}

// ---------------------------------------------------------------------

std::string Address::getStreetName() const {
    return m_sStreetName;
}

// ---------------------------------------------------------------------

std::string Address::getBuildingNumber() const {
    return m_sBuildingNumber;
}

// ---------------------------------------------------------------------

bool Address::operator==(const Address& rhs) const {
    return this->getStreetName() == rhs.getStreetName()
        && this->getBuildingNumber() == rhs.getBuildingNumber();
}

Address& Address::operator=(const Address& rhs) {
    m_sStreetName = rhs.getStreetName();
    m_sBuildingNumber = rhs.getBuildingNumber();
    return *this;
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeBuilding

WsjcppObjTreeNodeBuilding::WsjcppObjTreeNodeBuilding(WsjcppObjTree *pTree, const Address &nValue)
: WsjcppObjTreeNode(pTree, WsjcppObjTreeNodeBuilding::staticType()) {
    this->setValue(nValue);
}

// ---------------------------------------------------------------------

const Address &WsjcppObjTreeNodeBuilding::getValue() {
    return m_value;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeBuilding::setValue(const Address &nValue) {
    m_value = nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeBuilding::setNumberOfFloors(int n) {
    m_nNumberOfFloors = n;
}

// ---------------------------------------------------------------------

int WsjcppObjTreeNodeBuilding::getNumberOfFloors() {
    return m_nNumberOfFloors;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::writeDataPartToFile(std::ofstream &f, std::string &sError) {
    
    if (!this->writeUInt32(f, m_nNumberOfFloors, sError)) {
        return false;    
    }

    if (!this->writeString(f, m_value.getStreetName(), sError)) {
        return false;    
    }

    if (!this->writeString(f, m_value.getBuildingNumber(), sError)) {
        return false;    
    }

    return true;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    uint32_t nFloors = 0;
    if (!this->readUInt32(f, nFloors, sError)) {
        return false;
    }
    m_nNumberOfFloors = nFloors;

    std::string sStreet;
    if (!this->readString(f, sStreet, sError)) {
        return false;
    }

    std::string sBuilding;
    if (!this->readString(f, sBuilding, sError)) {
        return false;
    }
    
    m_value = Address(sStreet, sBuilding);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeBuilding::toString(const std::string &sIntent) {
    return
        "Building: st. " + m_value.getStreetName() + ", " + m_value.getBuildingNumber()
        + "\n" + sIntent + "number-of-floors: " + std::to_string(m_nNumberOfFloors);
    ;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::readUInt32(std::ifstream &f, uint32_t &nVal, std::string &sError) {
    uint32_t nStringLen = 0;
    char arrInteger[4];
    f.read(arrInteger, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeBuilding. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nVal = *reinterpret_cast<uint32_t*>(arrInteger);
    return true; 
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::writeUInt32(std::ofstream &f, uint32_t nVal, std::string &sError) {
    static_assert(sizeof(uint32_t) == 4, "Expected sizeof(uint32_t) == 4");
    const char *pData = reinterpret_cast<const char *>(&nVal);
    f.write(pData, 4);
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::readString(std::ifstream &f, std::string &sVal, std::string &sError) {
    uint32_t nStringLen = 0;
    if (!this->readUInt32(f, nStringLen, sError)) {
        return false;
    }
    char *pStr = new char[nStringLen];
    f.read(pStr, nStringLen);
    if (!f) {
        delete[] pStr;
        sError = "WsjcppObjTreeNodeBuilding. Could not read string data. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    sVal = std::string(pStr, nStringLen);
    delete[] pStr;
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::writeString(std::ofstream &f, std::string sVal, std::string &sError) {
    if (!writeUInt32(f, sVal.size(), sError)) {
        return false;
    }
    f.write(sVal.c_str(), sVal.size());
    return true;
}