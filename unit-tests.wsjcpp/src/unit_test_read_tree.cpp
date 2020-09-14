#include "unit_test_read_tree.h"
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestReadTree)

UnitTestReadTree::UnitTestReadTree()
    : WsjcppUnitTestBase("UnitTestReadTree") {
}

// ---------------------------------------------------------------------

void UnitTestReadTree::init() {
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

bool UnitTestReadTree::run() {
    bool bTestSuccess = true;

    // TODO will be chnaged this to keeped files on the disk
    CompStruct comp;

    std::string sFilename = "./data/readtreetest/example1.wsjcpp-obj-tree";
    std::string sError;
    bool bWrote = comp.readTreeFromFile(sFilename, sError);
    compareB(bTestSuccess, "read from file", bWrote, true);
    if (!bWrote) {
        WsjcppLog::err(TAG, sError);
    }

    WsjcppLog::info(TAG, "\n" + comp.toString());
    compareN(bTestSuccess, "user version ", comp.getUserVersionOfTree(), 1);

    std::vector<WsjcppObjTreeNodeString *> vFoundNodes;

    comp.findNodes("PCI_EXPRESS", vFoundNodes);

    compareN(bTestSuccess, "find PCI_EXPRESS", vFoundNodes.size(), 1);
    if (vFoundNodes.size() == 1) {
        WsjcppObjTreeNodeString *pNode = vFoundNodes[0];
        compareN(bTestSuccess, "node id", pNode->getId(), 13);
        compareN(bTestSuccess, "find PCI_EXPRESS", pNode->getChilds().size(), 1);
        if (pNode->getChilds().size() == 1) {
            WsjcppObjTreeNode *pNodeChild_ = pNode->getChilds()[0];
            compareN(bTestSuccess, "child node type", pNodeChild_->getType(), WSJCPP_OBJ_TREE_NODE_INTEGER);
            compareN(bTestSuccess, "child node id", pNodeChild_->getId(), 14);
            compareN(bTestSuccess, "child node parent id (1)", pNode->getId(), pNodeChild_->getParent()->getId());
            compareN(bTestSuccess, "child node parent id (2)", pNodeChild_->getParent()->getId(), 13);
        }
    }

    return bTestSuccess;
}

