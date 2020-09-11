# wsjcpp-obj-tree
[![Build Status](https://api.travis-ci.com/wsjcpp/wsjcpp-obj-tree.svg?branch=master)](https://travis-ci.com/wsjcpp/wsjcpp-obj-tree)

Multi Object Tree

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


## Example

### Define tree use a chain declaration

```
#include <wsjcpp_core.h>
#include <wsjcpp_obj_tree.h>

int main(int argc, const char* argv[]) {
   
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

    // add node directly
    WsjcppObjTreeNode *pRootNode = new WsjcppObjTreeNodeString(&tree, "Motherboard"); 
    tree.addNode(nullptr, pRootNode);

    // in next step use a chain declaration
    WsjcppObjTreeChainDeclare chain(&tree);
    chain.switchTo(pRootNode); // switch to specific node
    chain
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
    ;

    return 0;
}
```

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
where:
- "Building" - suffic of class 
- "Address" - name of class for initialization
- "6" - id of type
