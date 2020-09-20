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
// UnitTestReadTree

class UnitTestReadTree : public WsjcppUnitTestBase {
    public:
        UnitTestReadTree();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestReadTree)

UnitTestReadTree::UnitTestReadTree()
    : WsjcppUnitTestBase("UnitTestReadTree") {
}

// ---------------------------------------------------------------------

bool UnitTestReadTree::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestReadTree::executeTest() {

    // TODO will be changed this to keeped files on the disk
    CompStruct comp;

    std::string sFilename = "./data/readtreetest/example1.wsjcpp-obj-tree";
    std::string sError;
    bool bWrote = comp.readTreeFromFile(sFilename, sError);
    compare("read from file", bWrote, true);
    if (!bWrote) {
        WsjcppLog::err(TAG, sError);
    }

    WsjcppLog::info(TAG, "\n" + comp.toString());
    compare("user version ", comp.getUserVersionOfTree(), 1);

    std::vector<WsjcppObjTreeNodeString *> vFoundNodes;

    comp.findNodes("PCI_EXPRESS", vFoundNodes);

    compare("find PCI_EXPRESS", vFoundNodes.size(), 1);
    if (vFoundNodes.size() == 1) {
        WsjcppObjTreeNodeString *pNode = vFoundNodes[0];
        compare("node id", pNode->getId(), 13);
        compare("find PCI_EXPRESS", pNode->getChilds().size(), 1);
        if (pNode->getChilds().size() == 1) {
            WsjcppObjTreeNode *pNodeChild_ = pNode->getChilds()[0];
            compare("child node type", pNodeChild_->getType(), WSJCPP_OBJ_TREE_NODE_INTEGER);
            compare("child node id", pNodeChild_->getId(), 14);
            compare("child node parent id (1)", pNode->getId(), pNodeChild_->getParent()->getId());
            compare("child node parent id (2)", pNodeChild_->getParent()->getId(), 13);
        }
    }
}

// ---------------------------------------------------------------------

bool UnitTestReadTree::doAfterTest() {
    // nothing
    return true;
}
