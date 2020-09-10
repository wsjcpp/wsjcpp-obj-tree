#include <string>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

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

    WsjcppObjTreeChainDeclare chain(&tree);

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
                .add(1).up()
            .add("USB-A")
                .add(4).up()
            .add("PCI")
                .add(3).up()
            .add("PCI_EXPRESS")
                .add(1).up()
            .up()
        .up()
    ;

    return 0;
}

