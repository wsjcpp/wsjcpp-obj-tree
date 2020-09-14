
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

bool WsjcppObjTreeNode::hasChilds() {
    return m_vChilds.size() > 0;
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
        sError = "File not exists";
        return false;
    }

    clearNodes();
    std::ifstream f;
    f.open(sFilename.c_str(), std::ios::in | std::ios::binary);

    char sExpectedFileHeader[20];
    f.read(sExpectedFileHeader, 20);
    if (!f) {
        sError = "readTreeFromFile. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }

    if (std::string(sExpectedFileHeader,20) != "WSJCPP-OBJ-TREE-FILE") {
        sError = "readTreeFromFile. Expected first 20 bytes of file like this WSJCPP-OBJ-TREE-FILE";
        return false;
    }

    uint32_t nTreeSize = 0;
    if (!this->readUInt32(f, nTreeSize, sError)) {
        return false;
    }

    if (!this->readUInt32(f, m_nLastId, sError)) {
        return false;
    }

    std::map<uint32_t, WsjcppObjTreeNode *> mapTempIdToNode;
    for (int i = 0; i < nTreeSize; i++) {
        uint16_t nNodeType = 0;
        if (!this->readUInt16(f, nNodeType, sError)) {
            return false;
        }

        if (m_mapFabricTreeNode.find(nNodeType) == m_mapFabricTreeNode.end()) {
            sError = "On read file could not found node type: " + std::to_string(nNodeType);
            return false;
        }

        // read parent id
        uint32_t nParentId = 0;
        if (!this->readUInt32(f, nParentId, sError)) {
            return false;
        }
        WsjcppObjTreeNode *pParentNode = nullptr; // TODO find by Id

        if (mapTempIdToNode.find(nParentId) != mapTempIdToNode.end()) {
            pParentNode = mapTempIdToNode[nParentId];
        }

        // read node id
        uint32_t nNodeId = 0;
        if (!this->readUInt32(f, nNodeId, sError)) {
            return false;
        }
        

        if (nNodeId > m_nLastId) {
            sError = "Node id '" + std::to_string(nNodeId) + "' could not more then last id " + std::to_string(m_nLastId);
            return false;
        }

        WsjcppObjTreeNode *pNode = m_mapFabricTreeNode[nNodeType]->create();
        pNode->setId(nNodeId);
        pNode->setParent(pParentNode);
        if (pParentNode != nullptr) {
            pParentNode->addChild(pNode);
        }
        if (!pNode->readDataPartFromFile(f, sError)) {
            return false;
        }
        mapTempIdToNode[nNodeId] = pNode;
        m_vNodes.push_back(pNode);
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppObjTree::writeTreeToFile(const std::string &sFilename, std::string &sError) {
    std::ofstream f(sFilename.c_str(), std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to write " << sFilename << std::endl;
        return false;
    }
    static const std::string sFileHeader = "WSJCPP-OBJ-TREE-FILE";
    f.write(sFileHeader.c_str(), sFileHeader.length());

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

int WsjcppObjTree::getRoots(std::vector<WsjcppObjTreeNode *> &vRoots) {
    int nRet = 0;
    std::vector<WsjcppObjTreeNode *>::iterator it = m_vNodes.begin();
    for (it = m_vNodes.begin(); it != m_vNodes.end(); ++it) {
        if ((*it)->getParent() == nullptr) {
            vRoots.push_back(*it);
            nRet++;
        }
    }
    return nRet;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTree::toString() { // for printing
    std::string sIntent = "";
    return toStringRecoursiveChilds(nullptr, sIntent);
}

// ---------------------------------------------------------------------

void WsjcppObjTree::writeUInt32(std::ofstream &f, uint32_t nVal) {
    const char *pBuffer = reinterpret_cast<const char *>(&nVal);
    f.write(pBuffer, 4);
}

// ---------------------------------------------------------------------

bool WsjcppObjTree::readUInt32(std::ifstream &f, uint32_t &nVal, std::string &sError) {
    // not for multithreading
    static char arrInteger[4];
    f.read(arrInteger, 4);
    if (!f) {
        sError = "Could not read. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nVal = *reinterpret_cast<uint32_t*>(arrInteger);
    return true;
}

// ---------------------------------------------------------------------

void WsjcppObjTree::writeUInt16(std::ofstream &f, uint16_t nVal) {
    const char *pBuffer = reinterpret_cast<const char *>(&nVal);
    f.write(pBuffer, 2);
}

// ---------------------------------------------------------------------

bool WsjcppObjTree::readUInt16(std::ifstream &f, uint16_t &nVal, std::string &sError) {
    // not for multithreading
    static char arrShort[2];
    f.read(arrShort, 2);
    if (!f) {
        sError = "Could not read. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nVal = *reinterpret_cast<uint16_t*>(arrShort);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTree::toStringRecoursiveChilds(WsjcppObjTreeNode *pNode, const std::string &sIntent) {
    std::string sRet;
    std::vector<WsjcppObjTreeNode *> vChilds;
    if (pNode == nullptr) {
        getRoots(vChilds);
    } else {
        vChilds = pNode->getChilds();
    }
    int nLen = vChilds.size();
    for (int i = 0; i < nLen; i++) {
        bool bLatestChild = (i == nLen-1);
        WsjcppObjTreeNode *pNode = vChilds[i];
        sRet += sIntent;
        sRet += bLatestChild ? "└─ " : "├─ ";
        std::string sIntentAppend = bLatestChild ? "   " : "│  ";
        sRet += pNode->toString(sIntent + sIntentAppend) + "\n";
        if (pNode->hasChilds()) {
            sRet += toStringRecoursiveChilds(pNode, sIntent + sIntentAppend);
        }
    }
    return sRet;
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

bool WsjcppObjTreeNodeString::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    uint32_t nStringLen = 0;
    char arrInteger[4];
    f.read(arrInteger, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeString. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nStringLen = *reinterpret_cast<uint32_t*>(arrInteger);
    char *pStr = new char[nStringLen];
    f.read(pStr, nStringLen);
    if (!f) {
        delete pStr;
        sError = "WsjcppObjTreeNodeString. Could not read string data. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    m_sValue = std::string(pStr, nStringLen);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeString::toString(const std::string &sIntent) {
    return "string: " + m_sValue;
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

bool WsjcppObjTreeNodeInteger::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    // size
    // TODO remove - because  this not need
    char arrBytes[4];
    f.read(arrBytes, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeInteger. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    // value 
    f.read(arrBytes, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeInteger. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    static_assert(sizeof(uint32_t) == 4, "Expected sizeof(uint32_t) == 4");
    m_nValue = *reinterpret_cast<uint32_t*>(arrBytes);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeInteger::toString(const std::string &sIntent) {
    return "int: " + std::to_string(m_nValue);
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

bool WsjcppObjTreeNodeFloat::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    static_assert(sizeof(float) == 4, "Expected sizeof(float) == 4");
    // size
    // TODO remove - because  this not need
    char arrBytes[4];
    f.read(arrBytes, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeInteger. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    // value 
    f.read(arrBytes, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeFloat. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    m_nValue = *reinterpret_cast<float*>(arrBytes);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeFloat::toString(const std::string &sIntent) {
    return "float: " + std::to_string(m_nValue);
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

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeDouble::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    static_assert(sizeof(double) == 8, "Expected sizeof(double) == 8");
    // size
    // TODO remove - because  this not need
    char arrBytes4[4];
    f.read(arrBytes4, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeInteger. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    // value
    char arrBytes[8];
    f.read(arrBytes, 8);
    if (!f) {
        sError = "WsjcppObjTreeNodeDouble. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    m_nValue = *reinterpret_cast<double*>(arrBytes);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeDouble::toString(const std::string &sIntent) {
    return "double: " + std::to_string(m_nValue);
}
