
#include "wsjcpp_obj_tree.h"
#include <wsjcpp_core.h>
#include <algorithm>
#include <fstream>

// ---------------------------------------------------------------------
// WsjcppObjTreeNode

WsjcppObjTreeNode::WsjcppObjTreeNode(WsjcppObjTree *pTree, uint16_t nType) {
    m_pTree = pTree;
    m_pParent = nullptr;
    m_nType = nType;
    // TODO regestry in global factory
}

// ---------------------------------------------------------------------

uint16_t WsjcppObjTreeNode::getType() {
    return m_nType;
}

// ---------------------------------------------------------------------

WsjcppObjTreeNode::~WsjcppObjTreeNode() {
    // std::cout << "TreeNode destroyed" << std::endl;
}

// ---------------------------------------------------------------------

WsjcppObjTreeNode *WsjcppObjTreeNode::getParent() {
    return m_pParent;
}

// ---------------------------------------------------------------------

uint32_t WsjcppObjTreeNode::getId() {
    return m_nId;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppObjTreeNode *> &WsjcppObjTreeNode::getChilds() {
    return m_vChilds;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNode::setParent(WsjcppObjTreeNode *pParent) {
    m_pParent = pParent;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNode::addChild(WsjcppObjTreeNode *pChild) {
    m_vChilds.push_back(pChild);
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNode::setId(uint32_t nId) {
    m_nId = nId;
}

// ---------------------------------------------------------------------
// WsjcppObjTree

WsjcppObjTree::WsjcppObjTree() {
    m_nLastId = 0;
    TAG = "WsjcppObjTree";
}

// ---------------------------------------------------------------------

WsjcppObjTree::~WsjcppObjTree() {
    clearNodes();
    std::map<uint16_t, IFabricWsjcppObjTreeNode*>::iterator it;
    for (it = m_mapFabricTreeNode.begin(); it != m_mapFabricTreeNode.end(); ++it) {
        delete it->second;
    }
    m_mapFabricTreeNode.clear();

}

// ---------------------------------------------------------------------

bool WsjcppObjTree::readTreeFromFile(const std::string &sFilename, std::string &sError) {
    if (!WsjcppCore::fileExists(sFilename)) {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppObjTree::writeTreeToFile(const std::string &sFilename, std::string &sError) {
    std::ofstream f(sFilename.c_str(), std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to write " << sFilename << std::endl;
        return false;
    }

    // m_nLastId
    int nTreeSize = m_vNodes.size();
    this->writeUInt32(f, nTreeSize);
    this->writeUInt32(f, m_nLastId);

    unsigned char arrShort[4];

    for (int i = 0; i < nTreeSize; i++) {
        WsjcppObjTreeNode *pNode = m_vNodes[i];

        // write type
        this->writeUInt16(f, pNode->getType());

        // write parent id
        if (pNode->getParent() != nullptr) {
            this->writeUInt32(f, pNode->getParent()->getId());
        } else {
            this->writeUInt32(f, 0);
        }
        
        // write id
        this->writeUInt32(f, pNode->getId());

        // write data size
        int nDataSize = pNode->getDataSize();
        this->writeUInt32(f, nDataSize);

        // write data
        f.write(pNode->getData(), nDataSize);
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

void WsjcppObjTree::clearNodes() {
    int nSize = m_vNodes.size();
    for (int i = nSize - 1; i >=0; i--) {
        delete m_vNodes[i];
    }
    m_vNodes.clear();
}

// ---------------------------------------------------------------------

void WsjcppObjTree::addNode(WsjcppObjTreeNode *pParent, WsjcppObjTreeNode *pChild) {
    if (pChild == nullptr) {
        WsjcppLog::throw_err(TAG, "::addNode - Child could not be null");
    }
    // check type of node
    if (!this->isSupportType(pChild->getType())) {
        WsjcppLog::throw_err(TAG, "::addNode - Not supported type");
    }

    std::vector<WsjcppObjTreeNode *>::iterator it;

    // check that parent node already exists in a list
    if (pParent != nullptr && !this->hasNode(pParent)) {
        WsjcppLog::throw_err(TAG, "::addNode - Not found parent node in the tree");
    }

    // check child node - exclude circles
    if (this->hasNode(pChild)) {
        WsjcppLog::throw_err(TAG, "::addNode - Child already added in the tree");
    }

    pChild->setParent(pParent);
    if (pParent != nullptr) {
        pParent->addChild(pChild);
    }
    m_nLastId++;
    pChild->setId(m_nLastId);
    m_vNodes.push_back(pChild);
}

// ---------------------------------------------------------------------

bool WsjcppObjTree::isSupportType(uint16_t nType) {
    std::map<uint16_t, IFabricWsjcppObjTreeNode*>::iterator it;
    it = m_mapFabricTreeNode.find(nType);
    return it != m_mapFabricTreeNode.end();
}

// ---------------------------------------------------------------------

bool WsjcppObjTree::hasNode(WsjcppObjTreeNode *pNode) {
    std::vector<WsjcppObjTreeNode *>::iterator it;
    it = std::find(m_vNodes.begin(), m_vNodes.end(), pNode);
    return it != m_vNodes.end();
}

// ---------------------------------------------------------------------

void WsjcppObjTree::writeUInt32(std::ofstream &f, uint32_t nVal) {
    // not for multithreading
    // TODO redesign to reinterpret_cast<const char *>(&m_nValue);
    static unsigned char arrInteger[4];
    arrInteger[0] = (nVal >> 24) & 0xFF;
    arrInteger[1] = (nVal >> 16) & 0xFF;
    arrInteger[2] = (nVal >> 8) & 0xFF;
    arrInteger[3] = nVal & 0xFF;
    f.write((const char *)arrInteger, 4);
}

// ---------------------------------------------------------------------

void WsjcppObjTree::writeUInt16(std::ofstream &f, uint16_t nVal) {
    // not for multithreading
    static unsigned char arrShort[2];
    arrShort[0] = (nVal >> 8) & 0xFF;
    arrShort[1] = nVal & 0xFF;
    f.write((const char *)arrShort, 2);
}

// ---------------------------------------------------------------------
//WsjcppObjTreeChainDeclare

WsjcppObjTreeChainDeclare::WsjcppObjTreeChainDeclare(WsjcppObjTree *pTree) {
    TAG = "WsjcppObjTreeChainDeclare";
    m_pTree = pTree;
    m_pCurrentNode = nullptr;
}

// ---------------------------------------------------------------------

WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::addNode(WsjcppObjTreeNode *pNode) {
    m_pTree->addNode(m_pCurrentNode, pNode);
    m_pCurrentNode = pNode;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::up() {
    if (m_pCurrentNode != nullptr) {
        m_pCurrentNode = m_pCurrentNode->getParent();
    }
    return *this;
}

// ---------------------------------------------------------------------

WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::switchTo(WsjcppObjTreeNode *pNode) {
    if (!m_pTree->hasNode(pNode)) {
        WsjcppLog::throw_err(TAG, "::switchTo Not existed node in tree");
    }
    m_pCurrentNode = pNode;
    return *this;
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeString

WsjcppObjTreeNodeString::WsjcppObjTreeNodeString(WsjcppObjTree *pTree, const std::string &sValue) 
: WsjcppObjTreeNode(pTree, WsjcppObjTreeNodeString::staticType()) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeString::getValue() {
    return m_sValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeString::setValue(const std::string &sValue) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

int WsjcppObjTreeNodeString::getDataSize() {
    return m_sValue.size();
}

// ---------------------------------------------------------------------

const char *WsjcppObjTreeNodeString::getData() {
    return m_sValue.c_str();
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeInteger

WsjcppObjTreeNodeInteger::WsjcppObjTreeNodeInteger(WsjcppObjTree *pTree, const int32_t &nValue)
: WsjcppObjTreeNode(pTree, WsjcppObjTreeNodeInteger::staticType()) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

int32_t WsjcppObjTreeNodeInteger::getValue() {
    return m_nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeInteger::setValue(int32_t nValue) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

int WsjcppObjTreeNodeInteger::getDataSize() {
    return sizeof(uint32_t);
}

// ---------------------------------------------------------------------

const char *WsjcppObjTreeNodeInteger::getData() {
    const char *p = reinterpret_cast<const char *>(&m_nValue);
    return p;
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeFloat

WsjcppObjTreeNodeFloat::WsjcppObjTreeNodeFloat(WsjcppObjTree *pTree, const float &nValue)
: WsjcppObjTreeNode(pTree, WsjcppObjTreeNodeFloat::staticType()) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

float WsjcppObjTreeNodeFloat::getValue() {
    return m_nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeFloat::setValue(float nValue) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

int WsjcppObjTreeNodeFloat::getDataSize() {
    static_assert(sizeof(float) == 4, "Expected sizeof(float) == 4");
    return sizeof(float);
}

// ---------------------------------------------------------------------

const char *WsjcppObjTreeNodeFloat::getData() {
    return reinterpret_cast<const char *>(&m_nValue);
}


// ---------------------------------------------------------------------
// WsjcppObjTreeNodeDouble

WsjcppObjTreeNodeDouble::WsjcppObjTreeNodeDouble(WsjcppObjTree *pTree, const double &nValue)
: WsjcppObjTreeNode(pTree, WsjcppObjTreeNodeDouble::staticType()) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

float WsjcppObjTreeNodeDouble::getValue() {
    return m_nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeDouble::setValue(float nValue) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

int WsjcppObjTreeNodeDouble::getDataSize() {
    static_assert(sizeof(double) == 8, "Expected sizeof(double) == 8");
    return sizeof(double);
}

// ---------------------------------------------------------------------

const char *WsjcppObjTreeNodeDouble::getData() {
    return reinterpret_cast<const char *>(&m_nValue);
}

