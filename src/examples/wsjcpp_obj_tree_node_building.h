#ifndef WSJCPP_OBJ_TREE_NODE_BUILDING_H
#define WSJCPP_OBJ_TREE_NODE_BUILDING_H

#include <wsjcpp_obj_tree.h>

class Address {
    public:
        Address();
        Address(const Address& address);
        Address(std::string sStreet, std::string sBuilding);
        std::string getStreetName() const;
        std::string getBuildingNumber() const;
        bool operator==(const Address& rhs) const;

    private:
        std::string m_sStreetName;
        std::string m_sBuildingNumber;
};

static uint16_t WSJCPP_OBJ_TREE_NODE_BUILDING = 60;

class WsjcppObjTreeNodeBuilding : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeBuilding(WsjcppObjTree *pTree, const Address &nValue = Address());
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_BUILDING; };
        
        const Address &getValue();
        void setValue(const Address &nValue);
        
        // custom
        void setNumberOfFloors(int n);
        int getNumberOfFloors();

        // WsjcppObjTreeNode
        virtual int getDataSize() override;
        virtual const char *getData() override;
        virtual std::string toString(const std::string &sIntent = "") override;

    private:
        Address m_value;
        int m_nNumberOfFloors;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(Address, WsjcppObjTreeNodeBuilding)

#endif // WSJCPP_OBJ_TREE_NODE_BUILDING_H
