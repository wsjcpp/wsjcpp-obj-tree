#include <wsjcpp_unit_tests.h>
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

// ---------------------------------------------------------------------
// CompStruct

class CompStruct : public WsjcppObjTree {
    public:
        CompStruct() {
            addSupportType<WsjcppObjTreeNodeString>();
            addSupportType<WsjcppObjTreeNodeInteger>();
            addSupportType<WsjcppObjTreeNodeFloat>();
            addSupportType<WsjcppObjTreeNodeDouble>();
        }
};

// ---------------------------------------------------------------------
// UnitTestFindNodes

class UnitTestFindNodes : public WsjcppUnitTestBase {
    public:
        UnitTestFindNodes();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestFindNodes)

UnitTestFindNodes::UnitTestFindNodes()
    : WsjcppUnitTestBase("UnitTestFindNodes") {
}

// ---------------------------------------------------------------------

bool UnitTestFindNodes::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestFindNodes::executeTest() {

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
    compare("Find 'frequency' ", vFoundNodes.size(), 1);

    if (vFoundNodes.size() == 1) {
        WsjcppObjTreeNode *pNode = vFoundNodes[0];
        compare("'frequency' childs size", pNode->getChilds().size(), 1);
        if (pNode->getChilds().size() == 1) {
            WsjcppObjTreeNode *pNode2 = pNode->getChilds()[0];
            compare("'frequency' parent of the child", pNode2->getParent() == pNode, true);
            compare("'frequency' child type", pNode2->getType(), WSJCPP_OBJ_TREE_NODE_DOUBLE);
            WsjcppObjTreeNodeDouble *pDouble = (WsjcppObjTreeNodeDouble *)pNode2;
            compare("'frequency' child value", std::to_string(pDouble->getValue()), std::to_string(3.2));
        }
    }
    
    vFoundNodes[0]->getChilds()[0];

    
    comp.findNodes("count", vFoundNodes);
    compare("Find 'count' and 'frequency' ", vFoundNodes.size(), 3);

    vFoundNodes.clear();
    comp.findNodes("count", vFoundNodes);
    compare("Find 'count' ", vFoundNodes.size(), 2);
}

// ---------------------------------------------------------------------

bool UnitTestFindNodes::doAfterTest() {
    // nothing
    return true;
}
