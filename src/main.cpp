#include <string>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>
#include <wsjcpp_obj_tree_node_building.h>

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("wsjcpp");
    WsjcppLog::setLogDirectory(".logs");
    
    WsjcppObjTree tree;
    tree.addSupportType<WsjcppObjTreeNodeString>();
    tree.addSupportType<WsjcppObjTreeNodeInteger>();
    tree.addSupportType<WsjcppObjTreeNodeFloat>();
    tree.addSupportType<WsjcppObjTreeNodeDouble>();
    tree.addSupportType<WsjcppObjTreeNodeBuilding>();

    Address addr0("Morpheus","1/35a");

    WsjcppObjTreeChainDeclare chain(&tree);
    chain
        .add(addr0)
        .up()
        .add("Motherboard")
            .add("CPU_SOCKET")
                .add("count")
                    .add(1).up()
                .up()
                .add("frequency")
                    .add(3.2).up()
                .up()
            .up()
            .add("GPU_SOCKET")
                .add(1).up()
            .up()
            .add("USB-A")
                .add(4).up()
            .up()
            .add("PCI")
                .add(3).up()
            .up()
            .add("PCI_EXPRESS")
                .add(1).up()
            .up()
        .up()
    ;
    
    std::vector<WsjcppObjTreeNodeBuilding *> vFoundNodes;
    if (tree.findNodes(addr0, vFoundNodes) > 0) {
        for (int i = 0; i < vFoundNodes.size(); i++) {
            vFoundNodes[i]->setNumberOfFloors(5);
        }
    }

    std::cout << tree.toString();

    return 0;
}

