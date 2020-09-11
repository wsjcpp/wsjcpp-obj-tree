
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

int WsjcppObjTreeNodeBuilding::getDataSize() {
    WsjcppLog::throw_err("WsjcppObjTreeNodeBuilding", "::getDataSize() Not implemented");
    return sizeof(Address);
}

// ---------------------------------------------------------------------

const char *WsjcppObjTreeNodeBuilding::getData() {
    WsjcppLog::throw_err("WsjcppObjTreeNodeBuilding", "::getData() Not implemented");
    return reinterpret_cast<const char *>(&m_value);
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeBuilding::toString(const std::string &sIntent) {
    return
        "Building: st. " + m_value.getStreetName() + ", " + m_value.getBuildingNumber()
        + "\n" + sIntent + "number-of-floors: " + std::to_string(m_nNumberOfFloors);
    ;
}
