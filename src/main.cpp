#include <string>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>
#include <wsjcpp_obj_tree_node_building.h>

class CustomTree : public WsjcppObjTree {
    public:
        CustomTree() {
            setUserVersionOfTree(1);
            addSupportType<WsjcppObjTreeNodeString>();
            addSupportType<WsjcppObjTreeNodeInteger>();
            addSupportType<WsjcppObjTreeNodeFloat>();
            addSupportType<WsjcppObjTreeNodeDouble>();
            addSupportType<WsjcppObjTreeNodeBuilding>();
        }
        void fillExample() {
            clearNodes();
            Address addr0("Morpheus","1/35a");
            WsjcppObjTreeChainDeclare chain(this);
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
            if (this->findNodes(addr0, vFoundNodes) > 0) {
                for (int i = 0; i < vFoundNodes.size(); i++) {
                    vFoundNodes[i]->setNumberOfFloors(5);
                }
            }
        }

        void randomTree(int nSize) {
            long nTime = WsjcppCore::getCurrentTimeInMilliseconds();
            std::cout << "Starting generate random tree with " << nSize << " nodes..." << std::endl;
            WsjcppCore::initRandom();
            long nTimeStep = nTime;
            for (int i = 0; i < nSize; i++) {
                uint16_t nType = rand() % 4;
                WsjcppObjTreeNode *pParentNode = nullptr;
                if (getAllNodes().size() > 0) {
                    int nParentIdx = rand() % getAllNodes().size();
                    pParentNode = getAllNodes()[nParentIdx];
                }

                if (nType == 0) {
                    auto *pNode = new WsjcppObjTreeNodeString(this, WsjcppCore::createUuid());
                    addNode(pParentNode, pNode);
                } else if (nType == 1) {
                    auto *pNode = new WsjcppObjTreeNodeInteger(this, rand() % 100000);
                    addNode(pParentNode, pNode);
                } else if (nType == 2) {
                    double nVal = rand() % 1000000;
                    nVal /= 100;
                    auto *pNode = new WsjcppObjTreeNodeDouble(this, nVal);
                    addNode(pParentNode, pNode);
                } else if (nType == 3) {
                    std::string s1 = WsjcppCore::createUuid();
                    std::string s2 = WsjcppCore::createUuid();
                    Address(s1,s2);
                    auto *pNode = new WsjcppObjTreeNodeBuilding(this, Address(s1,s2));
                    pNode->setNumberOfFloors(rand() % 100);
                    addNode(pParentNode, pNode);
                }
                if (WsjcppCore::getCurrentTimeInMilliseconds() - nTimeStep > 3000) {
                    long nTimeElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - nTime;
                    long nTimeLeft = (nTimeElapsed * (nSize - i)) / i;
                    int nProcents = i*100/nSize;
                    std::cout 
                        << "Generated (" << nProcents << "%) "
                        << i << "/" << nSize << " nodes."
                        << " Time elapsed: " << (WsjcppCore::getCurrentTimeInMilliseconds() - nTime) << "ms. "
                        << " Time left until completion " << nTimeLeft << "ms"
                        << std::endl;

                    nTimeStep = WsjcppCore::getCurrentTimeInMilliseconds();
                }
            }
            nTime = WsjcppCore::getCurrentTimeInMilliseconds() - nTime;
            std::cout << "Finished. Time elapsed " << nTime << "ms" << std::endl;
        }

};

void printHelp(std::string sProgramName) {
    std::cout << std::endl 
        << "  Usage: " << sProgramName << " [OPTIONS]" << std::endl 
        << "  Options: " << std::endl
        << "    --help | -h         help" << std::endl
        << "  Input or source data can be (only one): " << std::endl
        << "    --input <file> | -i <file>     input binary file with tree" << std::endl
        << "    --random N | -r N              generate random data (more than 0 and less than 1000000)" << std::endl
        << "    --example | -e                 hardcoded example" << std::endl
        << "  Output data can be (only one): " << std::endl
        << "    --output <file> | -o <file>    output binary file for tree" << std::endl
        << "    --print | -p                   output binary file for tree" << std::endl
        << std::endl
        << "  Command examples:" << std::endl
        << "    Print example of the tree: " << std::endl
        << "        '" << sProgramName << " -e -p'" <<  std::endl
        << "    Print random tree: " << std::endl
        << "        '" << sProgramName << " -r 20 -p'" <<  std::endl
        << "    Output example of the tree to file: " << std::endl
        << "        '" << sProgramName << " -e -o some.wsjcpp-obj-tree'" <<  std::endl
        << "    Input from file and output to file: " << std::endl
        << "        '" << sProgramName << " -i some.wsjcpp-obj-tree -o some2.wsjcpp-obj-tree'" <<  std::endl
        << std::endl
    ;
};

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("wsjcpp");
    WsjcppLog::setLogDirectory(".logs");

    std::vector<std::string> vArgs;
    for (int i = 0; i < argc; i++) {
        vArgs.push_back(std::string(argv[i]));
    }
    std::string sProgramName = vArgs[0];

    // print help
    for (int i = 0; i < vArgs.size(); i++) {
        if (vArgs[i] == "-h" || vArgs[i] == "--help") {
            printHelp(sProgramName);
            return 0;
        }
    }

    // select input 
    int nInputDefined = -1;
    int nRandomSize = -1;
    std::string sInputFile = "";
    for (int i = 0; i < vArgs.size(); i++) {
        std::string sArg = vArgs[i];
        if (sArg == "-i" || sArg == "--input") {
            nInputDefined = 1;
            if (i + 1 >= vArgs.size()) {
                std::cout << "FAILED: after '" << sArg << "' expected filename" << std::endl;
                return -1;
            }
            sInputFile = vArgs[i+1];
            if (!WsjcppCore::fileExists(sInputFile)) {
                std::cout << "FAILED: Input file '" << sInputFile << "' not exists" << std::endl;
                return -1;
            }
        } else if (sArg == "-r" || sArg == "--random") {
            nInputDefined = 2;
            if (i + 1 >= vArgs.size()) {
                std::cout << "FAILED: after '" << sArg << "' expected number argument" << std::endl;
                return -1;
            }
            nRandomSize = std::atoi(vArgs[i+1].c_str());
            if (nRandomSize < 0) {
                std::cout << "FAILED: after '" << sArg << "' expected number more than 0" << std::endl;
                return -1;
            }
            if (nRandomSize > 1000000) {
                std::cout << "FAILED: after '" << sArg << "' expected number less than 1000000" << std::endl;
                return -1;
            }
        } else if (sArg == "-e" || sArg == "--example") {
            nInputDefined = 3;
        }
    }

    if (nInputDefined == -1) {
        std::cout << "FAILED: way for input data not defined" << std::endl
            << "  Please try " << sProgramName << " --help " << std::endl
            << std::endl;
        return -1;
    }

    // select output
    int nOutputDefined = -1;
    std::string sOutputFile = "";
    for (int i = 0; i < vArgs.size(); i++) {
        std::string sArg = vArgs[i];
        if (sArg == "-o" || sArg == "--output") {
            nOutputDefined = 1;
            if (i + 1 >= vArgs.size()) {
                std::cout << "FAILED: after '" << sArg << "' expected filename" << std::endl;
                return -1;
            }
            sOutputFile = vArgs[i+1];
        } else if (sArg == "-p" || sArg == "--print") {
            nOutputDefined = 2;
        }
    }

    if (nOutputDefined == -1) {
        std::cout << "FAILED: way for output data not defined" << std::endl
            << "  Please try " << sProgramName << " --help " << std::endl
            << std::endl;
        return -1;
    }

    std::string sError;
    CustomTree tree;
    
    long nTime = WsjcppCore::getCurrentTimeInMilliseconds();
    switch(nInputDefined) {
        case 1: // input file
            std::cout << "Reading file... " << sInputFile << std::endl;
            if (!tree.readTreeFromFile(sInputFile, sError)) {
                std::cout << "FAILED: Could not read file " << sInputFile << std::endl
                    << sError << std::endl
                    << std::endl;
                return -1;
            }
            nTime = WsjcppCore::getCurrentTimeInMilliseconds() - nTime;
            std::cout << "Readed. Time elapsed " << nTime << "ms" << std::endl;
            break;
        case 2: // random
            tree.randomTree(nRandomSize);
            break;
        case 3: // example
            tree.fillExample();
            break;
    }
    
    nTime = WsjcppCore::getCurrentTimeInMilliseconds();
    switch(nOutputDefined) {
        case 1: 
            std::cout << "Writing file... " << sOutputFile << std::endl;
            if (!tree.writeTreeToFile(sOutputFile, sError)) {
                std::cout << "FAILED: Could not write file " << sOutputFile << std::endl
                    << sError << std::endl
                    << std::endl;
                return -1;
            }
            nTime = WsjcppCore::getCurrentTimeInMilliseconds() - nTime;
            std::cout << "Wrote. Time elapsed " << nTime << "ms" << std::endl;
            break;
        case 2:
            std::cout << tree.toString();
            nTime = WsjcppCore::getCurrentTimeInMilliseconds() - nTime;
            std::cout << "Printed. Time elapsed " << nTime << "ms" << std::endl;
            break;
    }
    return 0;
}

