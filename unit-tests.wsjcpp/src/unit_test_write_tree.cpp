#include "unit_test_write_tree.h"
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestWriteTree)

UnitTestWriteTree::UnitTestWriteTree()
    : WsjcppUnitTestBase("UnitTestWriteTree") {
}

// ---------------------------------------------------------------------

void UnitTestWriteTree::init() {
    // nothing
}

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

bool UnitTestWriteTree::run() {
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

    if (!WsjcppCore::dirExists("./data/tmp")) {
        WsjcppCore::makeDir("./data/tmp");
    }

    WsjcppLog::info(TAG, "\n" + comp.toString());
    std::string sFilename = "./data/tmp/example.obj-tree";
    std::string sError;
    bool bWrote = comp.writeTreeToFile(sFilename, sError);
    compareB(bTestSuccess, "write to file", bWrote, true);

    char *pBuffer = nullptr;
    int nBufferSize = 0;
    WsjcppCore::readFileToBuffer(sFilename, &pBuffer, nBufferSize);

    compareN(bTestSuccess, "write to file", nBufferSize, 316);
    
    return bTestSuccess;
}

