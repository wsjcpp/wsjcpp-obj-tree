#include "unit_test_add_node.h"
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestAddNode)

UnitTestAddNode::UnitTestAddNode()
    : WsjcppUnitTestBase("UnitTestAddNode") {
}

// ---------------------------------------------------------------------

void UnitTestAddNode::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestAddNode::run() {
    bool bTestSuccess = true;

    WsjcppObjTree tree;
    tree.addSupportType<WsjcppObjTreeNodeString>();
    tree.addSupportType<WsjcppObjTreeNodeInteger>();
    tree.addSupportType<WsjcppObjTreeNodeFloat>();
    
    auto *pNodeRoot = new WsjcppObjTreeNodeInteger(&tree, 8);
    auto *pNodeBar = new WsjcppObjTreeNodeString(&tree, "bar");
    auto *pNodeBaz = new WsjcppObjTreeNodeString(&tree, "baz");
    auto *pNode1dot2 = new WsjcppObjTreeNodeFloat(&tree, 1.2);

    compareN(bTestSuccess, "pNodeRoot->getType()", pNodeRoot->getType(), WSJCPP_OBJ_TREE_NODE_INTEGER);
    compareN(bTestSuccess, "pNodeBar->getType()", pNodeBar->getType(), WSJCPP_OBJ_TREE_NODE_STRING);
    compareN(bTestSuccess, "pNodeBaz->getType()", pNodeBaz->getType(), WSJCPP_OBJ_TREE_NODE_STRING);
    compareN(bTestSuccess, "pNode1dot2->getType()", pNode1dot2->getType(), WSJCPP_OBJ_TREE_NODE_FLOAT);

    compareB(bTestSuccess, "pNodeRoot getParent", pNodeRoot->getParent() == nullptr, true);
    compareB(bTestSuccess, "pNodeBar getParent", pNodeBar->getParent() == nullptr, true);
    compareB(bTestSuccess, "pNodeBaz getParent", pNodeBaz->getParent() == nullptr, true);
    compareB(bTestSuccess, "pNode1dot2 getParent", pNode1dot2->getParent() == nullptr, true);

    tree.addNode(nullptr, pNodeRoot);
    tree.addNode(pNodeRoot, pNodeBar);
    tree.addNode(pNodeRoot, pNodeBaz);

    compareB(bTestSuccess, "tree.hasNode pNodeRoot", tree.hasNode(pNodeRoot), true);
    compareB(bTestSuccess, "tree.hasNode pNodeBar", tree.hasNode(pNodeBar), true);
    compareB(bTestSuccess, "tree.hasNode pNodeBaz", tree.hasNode(pNodeBaz), true);
    compareB(bTestSuccess, "tree.hasNode pNode1dot2", tree.hasNode(pNode1dot2), false);
    
    tree.addNode(pNodeBaz, pNode1dot2);
    compareB(bTestSuccess, "tree.hasNode pNode1dot2", tree.hasNode(pNode1dot2), true);

    const std::vector<WsjcppObjTreeNode *> &vRootChilds = pNodeRoot->getChilds();
    compareN(bTestSuccess, "vRootChilds.size()", vRootChilds.size(), 2);
    if (vRootChilds.size() == 2) {
        compareB(bTestSuccess, "vRootChilds[0] == pNodeBar", vRootChilds[0] == (WsjcppObjTreeNode *)pNodeBar, true);
        compareB(bTestSuccess, "vRootChilds[1] == pNodeBaz", vRootChilds[1] == (WsjcppObjTreeNode *)pNodeBaz, true);
    }

    const std::vector<WsjcppObjTreeNode *> &vBazChilds = pNodeBaz->getChilds();
    compareN(bTestSuccess, "vBazChilds.size()", vBazChilds.size(), 1);
    if (vBazChilds.size() == 1) {
        compareB(bTestSuccess, "vBazChilds[0] == pNode1dot2", vBazChilds[0] == (WsjcppObjTreeNode *)pNode1dot2, true);
    }

    return bTestSuccess;
}

