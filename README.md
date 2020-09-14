# wsjcpp-obj-tree
[![Build Status](https://api.travis-ci.com/wsjcpp/wsjcpp-obj-tree.svg?branch=master)](https://travis-ci.com/wsjcpp/wsjcpp-obj-tree)

Multi Object Tree

* Check circles in tree
* Every node have list of pointers to childs and pointer to parent
* Tree can has several root nodes
* Chain style declaration for more comfortable
* Every node can be customized
* Tree has full list of pointers to nodes so searching in nodes is non recursive
* Tree contains list of supported types and check it
* Every type of node must have id (for example like this: `static uint16_t WSJCPP_OBJ_TREE_NODE_INTEGER = 2;`)
* Write tree to binary file
* Read tree from binary file

## Integrate

via wsjcpp

```
$ wsjcpp install https://github.com/wsjcpp/wsjcpp-obj-tree:master
```

or download and include sources to your project:

* src.wsjcpp/wsjcpp_core/wsjcpp_core.h
* src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp
* src/wsjcpp_obj_tree.h
* src/wsjcpp_obj_tree.cpp

### Build test app

Please install first: g++ cmake pkg-config

Build:
``` bash
$ ./build_simple.sh
```

Run help:
``` bash
$ ./wsjcpp-obj-tree --help

  Usage: ./wsjcpp-obj-tree [OPTIONS]
  Options: 
    --help | -h         help
  Input or source data can be (only one): 
    --input <file> | -i <file>     input binary file with tree
    --random N | -r N              generate random data (more than 0 and less than 1000000)
    --example | -e                 hardcoded example
  Output data can be (only one): 
    --output <file> | -o <file>    output binary file for tree
    --print | -p                   output binary file for tree

  Command examples:
    Print example of the tree: 
        './wsjcpp-obj-tree -e -p'
    Print random tree: 
        './wsjcpp-obj-tree -r 20 -p'
    Output example of the tree to file: 
        './wsjcpp-obj-tree -e -o some.wsjcpp-obj-tree'
    Input from file and output to file: 
        './wsjcpp-obj-tree -i some.wsjcpp-obj-tree -o some2.wsjcpp-obj-tree'
```

Print example tree
``` bash
% ./wsjcpp-obj-tree -e -p 
Root (ver: 1)
├─ Building: st. Morpheus, 1/35a
│  number-of-floors: 5
└─ string: Motherboard
   ├─ string: CPU_SOCKET
   │  ├─ string: count
   │  │  └─ int: 1
   │  └─ string: frequency
   │     └─ double: 3.200000
   ├─ string: GPU_SOCKET
   │  └─ int: 1
   ├─ string: USB-A
   │  └─ int: 4
   ├─ string: PCI
   │  └─ int: 3
   └─ string: PCI_EXPRESS
      └─ int: 1
Printed. Time elapsed 0ms
```

### Build and run unit-tests

``` bash
$ cd unit-tests.wsjcpp
$ ./build_simple.sh
$ ./unit-tests
2020-09-14 21:39:37.040, 0x0x110c9adc0 [INFO] UnitTests: All tests count 4
2020-09-14 21:39:37.041, 0x0x110c9adc0 [INFO] UnitTests: Run test UnitTestAddNode
2020-09-14 21:39:37.043, 0x0x110c9adc0 [OK] UnitTestAddNode: Test passed
2020-09-14 21:39:37.043, 0x0x110c9adc0 [INFO] UnitTests: Run test UnitTestWriteTree
2020-09-14 21:39:37.043, 0x0x110c9adc0 [INFO] UnitTestWriteTree: 
...
2020-09-14 21:39:37.044, 0x0x110c9adc0 [OK] UnitTestWriteTree: Test passed
2020-09-14 21:39:37.044, 0x0x110c9adc0 [INFO] UnitTests: Run test UnitTestFindNodes
2020-09-14 21:39:37.044, 0x0x110c9adc0 [OK] UnitTestFindNodes: Test passed
2020-09-14 21:39:37.045, 0x0x110c9adc0 [INFO] UnitTests: Run test UnitTestReadTree
2020-09-14 21:39:37.045, 0x0x110c9adc0 [INFO] UnitTestReadTree: 
...
2020-09-14 21:39:37.045, 0x0x110c9adc0 [OK] UnitTestReadTree: Test passed
2020-09-14 21:39:37.045, 0x0x110c9adc0 [INFO] UnitTests: Passed tests 4 / 4
```

## Examples

### Define your tree example

``` cpp
class CompStruct : public WsjcppObjTree {
    public:
        CompStruct() {
            addSupportType<WsjcppObjTreeNodeString>();
            addSupportType<WsjcppObjTreeNodeInteger>();
            addSupportType<WsjcppObjTreeNodeFloat>();
            addSupportType<WsjcppObjTreeNodeDouble>();
        }
};
```

and use then

``` cpp
CompStruct comp;
WsjcppObjTreeChainDeclare chain(&comp);
// chain.add("111")

```




### Define your node container

1 way: use a generator of code from wsjcpp

```
$ wsjcpp generate WsjcppObjTreeNode Building Address 6 
```
Where:
- "Building" - suffix of class 
- "Address" - name of class for initialization
- "6" - id of type

*So please not forget implemenet some methods*

2 way: example of full code

example of header:
``` cpp
#ifndef WSJCPP_OBJ_TREE_NODE_BUILDING_H
#define WSJCPP_OBJ_TREE_NODE_BUILDING_H

#include <wsjcpp_obj_tree.h>

class Address {
    public:
        Address();
        Address(const Address& address);
        Address(std::string sStreet, std::string sBuilding);
        std::string getStreetName() const;
        std::string getBuildingNumber() const;
        bool operator==(const Address& rhs) const;

    private:
        std::string m_sStreetName;
        std::string m_sBuildingNumber;
};

static uint16_t WSJCPP_OBJ_TREE_NODE_BUILDING = 60;

class WsjcppObjTreeNodeBuilding : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeBuilding(WsjcppObjTree *pTree, const Address &nValue = Address());
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_BUILDING; };
        
        const Address &getValue();
        void setValue(const Address &nValue);
        
        // custom
        void setNumberOfFloors(int n);
        int getNumberOfFloors();

        // WsjcppObjTreeNode
        virtual bool writeDataPartToFile(std::ofstream &f, std::string &sError) override;
        virtual bool readDataPartFromFile(std::ifstream &f, std::string &sError) override;
        virtual std::string toString(const std::string &sIntent = "") override;

    private:
        Address m_value;
        int m_nNumberOfFloors;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(Address, WsjcppObjTreeNodeBuilding)

#endif // WSJCPP_OBJ_TREE_NODE_BUILDING_H
```

example of implementation
``` cpp

#include "wsjcpp_obj_tree_node_building.h"
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

// ---------------------------------------------------------------------
// Address

Address::Address() {

}

// ---------------------------------------------------------------------

Address::Address(const Address& address) {
    m_sStreetName = address.getStreetName();
    m_sBuildingNumber = address.getBuildingNumber();
}

// ---------------------------------------------------------------------

Address::Address(std::string sStreetName, std::string sBuildingNumber) {
    m_sStreetName = sStreetName;
    m_sBuildingNumber = sBuildingNumber;
}

// ---------------------------------------------------------------------

std::string Address::getStreetName() const {
    return m_sStreetName;
}

// ---------------------------------------------------------------------

std::string Address::getBuildingNumber() const {
    return m_sBuildingNumber;
}

// ---------------------------------------------------------------------

bool Address::operator==(const Address& rhs) const {
    return this->getStreetName() == rhs.getStreetName()
        && this->getBuildingNumber() == rhs.getBuildingNumber();
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeBuilding

WsjcppObjTreeNodeBuilding::WsjcppObjTreeNodeBuilding(WsjcppObjTree *pTree, const Address &nValue)
: WsjcppObjTreeNode(pTree, WsjcppObjTreeNodeBuilding::staticType()) {
    this->setValue(nValue);
}

// ---------------------------------------------------------------------

const Address &WsjcppObjTreeNodeBuilding::getValue() {
    return m_value;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeBuilding::setValue(const Address &nValue) {
    m_value = nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeBuilding::setNumberOfFloors(int n) {
    m_nNumberOfFloors = n;
}

// ---------------------------------------------------------------------

int WsjcppObjTreeNodeBuilding::getNumberOfFloors() {
    return m_nNumberOfFloors;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::writeDataPartToFile(std::ofstream &f, std::string &sError) {
    sError = "WsjcppObjTreeNodeBuilding Not implemented";
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeBuilding::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    sError = "WsjcppObjTreeNodeBuilding Not implemented";
    return false;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeBuilding::toString(const std::string &sIntent) {
    return
        "Building: " + m_value.getStreetName() + ", " + m_value.getBuildingNumber()
        + "\n" + sIntent + "number-of-floors: " + std::to_string(m_nNumberOfFloors);
    ;
}

```

### Define tree use a addNode directrly

```
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

int main(int argc, const char* argv[]) {
   
    WsjcppObjTree tree;
    tree.addSupportType<WsjcppObjTreeNodeString>();
    tree.addSupportType<WsjcppObjTreeNodeInteger>();
    tree.addSupportType<WsjcppObjTreeNodeFloat>();
    tree.addSupportType<WsjcppObjTreeNodeDouble>();

    WsjcppObjTreeNode *pRootNode = new WsjcppObjTreeNodeString(&tree, "Motherboard"); 
    tree.addNode(nullptr, pRootNode);

    WsjcppObjTreeNode *pCPUNode = new WsjcppObjTreeNodeString(&tree, "CPU"); 
    tree.addNode(pRootNode, pCPUNode);

    // etc... but in this case I like "chain declaration" see next

    return 0;
}
```

### Define tree use a chain declaration

```
#include <iostream>
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

int main(int argc, const char* argv[]) {
   
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
```

rhis example will print next:

```
├─ Building: st. Morpheus, 1/35a
│  number-of-floors: 5
└─ string: Motherboard
   ├─ string: CPU_SOCKET
   │  ├─ string: count
   │  │  └─ int: 1
   │  └─ string: frequency
   │     └─ double: 3.200000
   ├─ string: GPU_SOCKET
   │  └─ int: 1
   ├─ string: USB-A
   │  └─ int: 4
   ├─ string: PCI
   │  └─ int: 3
   └─ string: PCI_EXPRESS
      └─ int: 1
```

