#include <wsjcpp_unit_tests.h>
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

// ---------------------------------------------------------------------
// CompStruct

class CompStruct : public WsjcppObjTree {
    public:
        CompStruct() {
            setUserVersionOfTree(1);
            addSupportType<WsjcppObjTreeNodeString>();
            addSupportType<WsjcppObjTreeNodeInteger>();
            addSupportType<WsjcppObjTreeNodeFloat>();
            addSupportType<WsjcppObjTreeNodeDouble>();
        }
};

// ---------------------------------------------------------------------
// UnitTestWriteTree

class UnitTestWriteTree : public WsjcppUnitTestBase {
    public:
        UnitTestWriteTree();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestWriteTree)

UnitTestWriteTree::UnitTestWriteTree()
    : WsjcppUnitTestBase("UnitTestWriteTree") {
}

// ---------------------------------------------------------------------

bool UnitTestWriteTree::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestWriteTree::executeTest() {

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
    std::string sFilename = "./data/tmp/example.wsjcpp-obj-tree";
    std::string sError;
    bool bWrote = comp.writeTreeToFile(sFilename, sError);
    compare("write to file", bWrote, true);

    char *pBuffer = nullptr;
    int nBufferSize = 0;
    WsjcppCore::readFileToBuffer(sFilename, &pBuffer, nBufferSize);

    compare("write to file", nBufferSize, 296);
}

// ---------------------------------------------------------------------

bool UnitTestWriteTree::doAfterTest() {
    // nothing
    return true;
}