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

    std::string sFilename = "./data/readtreetest/example1.obj-tree";
    std::string sError;
    bool bWrote = comp.readTreeFromFile(sFilename, sError);
    compareB(bTestSuccess, "read from file", bWrote, true);
    if (!bWrote) {
        WsjcppLog::err(TAG, sError);
    }

    WsjcppLog::info(TAG, "\n" + comp.toString());
        
    // char *pBuffer = nullptr;
    // int nBufferSize = 0;
    // WsjcppCore::readFileToBuffer(sFilename, &pBuffer, nBufferSize);

    // compareN(bTestSuccess, "read to file", nBufferSize, 296);
    
    return bTestSuccess;
}

