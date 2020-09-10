#include "unit_test_find_nodes.h"
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestFindNodes)

UnitTestFindNodes::UnitTestFindNodes()
    : WsjcppUnitTestBase("UnitTestFindNodes") {
}

// ---------------------------------------------------------------------

void UnitTestFindNodes::init() {
    // nothing
}

// ---------------------------------------------------------------------


class CompStruct : public WsjcppObjTree {
    public:
        CompStruct() {
            addSupportType<WsjcppObjTreeNodeString>();
            addSupportType<WsjcppObjTreeNodeInteger>();
            addSupportType<WsjcppObjTreeNodeFloat>();
            addSupportType<WsjcppObjTreeNodeDouble>();
        }
};

bool UnitTestFindNodes::run() {
    bool bTestSuccess = true;

    CompStruct comp;

    WsjcppObjTreeChainDeclare chain(&comp);

    chain
        .add("Motherboard")
            .add("CPU_SOCKET")
                .add("count")
                    .add(1).up()
                .up()
                .add("frequency")
                    .add(3.2).up()
                .up()
            .add("GPU_SOCKET")
                .add("count")
                    .add(1).up()
                .up()
            .add("USB-A")
                .add(4).up()
            .add("PCI")
                .add(3).up()
            .add("PCI_EXPRESS")
                .add(1).up()
            .up()
        .up()
    ;

    std::vector<WsjcppObjTreeNodeString *> vFoundNodes;
    comp.findNodes("frequency", vFoundNodes);
    compareN(bTestSuccess, "Find 'frequency' ", vFoundNodes.size(), 1);

    if (vFoundNodes.size() == 1) {
        WsjcppObjTreeNode *pNode = vFoundNodes[0];
        compareN(bTestSuccess, "'frequency' childs size", pNode->getChilds().size(), 1);
        if (pNode->getChilds().size() == 1) {
            WsjcppObjTreeNode *pNode2 = pNode->getChilds()[0];
            compareB(bTestSuccess, "'frequency' parent of the child", pNode2->getParent() == pNode, true);
            compareN(bTestSuccess, "'frequency' child type", pNode2->getType(), WSJCPP_OBJ_TREE_NODE_DOUBLE);
            WsjcppObjTreeNodeDouble *pDouble = (WsjcppObjTreeNodeDouble *)pNode2;
            compareN(bTestSuccess, "'frequency' child data size", pDouble->getDataSize(), 8);
            compareS(bTestSuccess, "'frequency' child value", std::to_string(pDouble->getValue()), std::to_string(3.2));
        }
    }
    
    vFoundNodes[0]->getChilds()[0];

    
    comp.findNodes("count", vFoundNodes);
    compareN(bTestSuccess, "Find 'count' and 'frequency' ", vFoundNodes.size(), 3);

    vFoundNodes.clear();
    comp.findNodes("count", vFoundNodes);
    compareN(bTestSuccess, "Find 'count' ", vFoundNodes.size(), 2);
    
    return bTestSuccess;
}

