
#include "wsjcpp_obj_tree_node_building.h"
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

// ---------------------------------------------------------------------
// Address

Address::Address() {

}

// ---------------------------------------------------------------------

Address::Address(const Address& address) {
    m_sStreet = address.getStreet();
    m_sBuilding = address.getBuilding();
}

// ---------------------------------------------------------------------

Address::Address(std::string sStreet, std::string sBuilding) {
    m_sStreet = sStreet;
    m_sBuilding = sBuilding;
}

// ---------------------------------------------------------------------

std::string Address::getStreet() const {
    return m_sStreet;
}

// ---------------------------------------------------------------------

std::string Address::getBuilding() const {
    return m_sBuilding;
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

int WsjcppObjTreeNodeBuilding::getDataSize() {
    WsjcppLog::throw_err("WsjcppObjTreeNodeBuilding", "::getDataSize() Not implemented");
    return sizeof(Address);
}

// ---------------------------------------------------------------------

const char *WsjcppObjTreeNodeBuilding::getData() {
    WsjcppLog::throw_err("WsjcppObjTreeNodeBuilding", "::getData() Not implemented");
    return reinterpret_cast<const char *>(&m_value);
}


