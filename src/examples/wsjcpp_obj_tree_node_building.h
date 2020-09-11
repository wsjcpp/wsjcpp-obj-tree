#ifndef WSJCPP_OBJ_TREE_NODE_BUILDING_H
#define WSJCPP_OBJ_TREE_NODE_BUILDING_H

#include <wsjcpp_obj_tree.h>

class Address {
    public:
        Address();
        Address(const Address& address);
        Address(std::string sStreet, std::string sBuilding);
        std::string getStreet() const;
        std::string getBuilding() const;
    private:
        std::string m_sStreet;
        std::string m_sBuilding;
};

static uint16_t WSJCPP_OBJ_TREE_NODE_BUILDING = 6;

class WsjcppObjTreeNodeBuilding : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeBuilding(WsjcppObjTree *pTree, const Address &nValue = Address());
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_BUILDING; };
        
        const Address &getValue();
        void setValue(const Address &nValue);

        // WsjcppObjTreeNode
        virtual int getDataSize() override;
        virtual const char *getData() override;

    private:
        Address m_value;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(Address, WsjcppObjTreeNodeBuilding)

#endif // WSJCPP_OBJ_TREE_NODE_BUILDING_H
