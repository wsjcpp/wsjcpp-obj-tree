#include "unit_test_add_node.h"
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestAddNode)

UnitTestAddNode::UnitTestAddNode()
    : WsjcppUnitTestBase("UnitTestAddNode") {
}

// ---------------------------------------------------------------------

bool UnitTestAddNode::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestAddNode::executeTest() {

    WsjcppObjTree tree;
    tree.addSupportType<WsjcppObjTreeNodeString>();
    tree.addSupportType<WsjcppObjTreeNodeInteger>();
    tree.addSupportType<WsjcppObjTreeNodeFloat>();
    
    auto *pNodeRoot = new WsjcppObjTreeNodeInteger(&tree, 8);
    auto *pNodeBar = new WsjcppObjTreeNodeString(&tree, "bar");
    auto *pNodeBaz = new WsjcppObjTreeNodeString(&tree, "baz");
    auto *pNode1dot2 = new WsjcppObjTreeNodeFloat(&tree, 1.2);

    compare("pNodeRoot->getType()", pNodeRoot->getType(), WSJCPP_OBJ_TREE_NODE_INTEGER);
    compare("pNodeBar->getType()", pNodeBar->getType(), WSJCPP_OBJ_TREE_NODE_STRING);
    compare("pNodeBaz->getType()", pNodeBaz->getType(), WSJCPP_OBJ_TREE_NODE_STRING);
    compare("pNode1dot2->getType()", pNode1dot2->getType(), WSJCPP_OBJ_TREE_NODE_FLOAT);

    compare("pNodeRoot getParent", pNodeRoot->getParent() == nullptr, true);
    compare("pNodeBar getParent", pNodeBar->getParent() == nullptr, true);
    compare("pNodeBaz getParent", pNodeBaz->getParent() == nullptr, true);
    compare("pNode1dot2 getParent", pNode1dot2->getParent() == nullptr, true);

    tree.addNode(nullptr, pNodeRoot);
    tree.addNode(pNodeRoot, pNodeBar);
    tree.addNode(pNodeRoot, pNodeBaz);

    compare("tree.hasNode pNodeRoot", tree.hasNode(pNodeRoot), true);
    compare("tree.hasNode pNodeBar", tree.hasNode(pNodeBar), true);
    compare("tree.hasNode pNodeBaz", tree.hasNode(pNodeBaz), true);
    compare("tree.hasNode pNode1dot2", tree.hasNode(pNode1dot2), false);
    
    tree.addNode(pNodeBaz, pNode1dot2);
    compare("tree.hasNode pNode1dot2", tree.hasNode(pNode1dot2), true);

    const std::vector<WsjcppObjTreeNode *> &vRootChilds = pNodeRoot->getChilds();
    compare("vRootChilds.size()", vRootChilds.size(), 2);
    if (vRootChilds.size() == 2) {
        compare("vRootChilds[0] == pNodeBar", vRootChilds[0] == (WsjcppObjTreeNode *)pNodeBar, true);
        compare("vRootChilds[1] == pNodeBaz", vRootChilds[1] == (WsjcppObjTreeNode *)pNodeBaz, true);
    }

    const std::vector<WsjcppObjTreeNode *> &vBazChilds = pNodeBaz->getChilds();
    compare("vBazChilds.size()", vBazChilds.size(), 1);
    if (vBazChilds.size() == 1) {
        compare("vBazChilds[0] == pNode1dot2", vBazChilds[0] == (WsjcppObjTreeNode *)pNode1dot2, true);
    }
}

// ---------------------------------------------------------------------

bool UnitTestAddNode::doAfterTest() {
    // nothing
    return true;
}