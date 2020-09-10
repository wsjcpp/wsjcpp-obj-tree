#ifndef WSJCPP_OBJ_TREE_H
#define WSJCPP_OBJ_TREE_H

#include <wsjcpp_core.h>
#include <string>
#include <map>

// not enum - because need more extendable from different modules

static uint16_t WSJCPP_OBJ_TREE_NODE_NONE = 0;
static uint16_t WSJCPP_OBJ_TREE_NODE_STRING = 1;
static uint16_t WSJCPP_OBJ_TREE_NODE_INTEGER = 2;
static uint16_t WSJCPP_OBJ_TREE_NODE_FLOAT = 3;
static uint16_t WSJCPP_OBJ_TREE_NODE_DOUBLE = 4;

class WsjcppObjTree;

class WsjcppObjTreeNode {
    friend class WsjcppObjTree;

    public:
        WsjcppObjTreeNode(WsjcppObjTree *pTree, uint16_t nType);
        virtual ~WsjcppObjTreeNode();
        uint16_t getType();

        // virtual zero methods will be deny create basic class
        virtual int getDataSize() = 0;
        virtual const char *getData() = 0;

        WsjcppObjTreeNode *getParent();
        uint32_t getId();
        const std::vector<WsjcppObjTreeNode *> &getChilds();

    protected:
        void setParent(WsjcppObjTreeNode *pParent);
        void addChild(WsjcppObjTreeNode *pChild);
        void setId(uint32_t id);

    private:
        WsjcppObjTree *m_pTree;
        WsjcppObjTreeNode *m_pParent;
        uint32_t m_nId;
        std::vector<WsjcppObjTreeNode *> m_vChilds;
        uint16_t m_nType;  
};

class IFabricWsjcppObjTreeNode {
    public:
        virtual ~IFabricWsjcppObjTreeNode() {};
        virtual WsjcppObjTreeNode *create() = 0;
};


template<class T>
class FabricWsjcppObjTreeNode : public IFabricWsjcppObjTreeNode {
    public:
        FabricWsjcppObjTreeNode(WsjcppObjTree *pTree) : m_pTree(pTree) { };
        ~FabricWsjcppObjTreeNode() {};
        virtual WsjcppObjTreeNode *create() override {
            return new T(m_pTree);
        };
    private:
        WsjcppObjTree *m_pTree;
};


class WsjcppObjTree {
    friend class WsjcppObjTreeNode;

    public:
        WsjcppObjTree();
        ~WsjcppObjTree();

        bool readTreeFromFile(const std::string &sFilename, std::string &sError);
        bool writeTreeToFile(const std::string &sFilename, std::string &sError);
        void clearNodes();
        void addNode(WsjcppObjTreeNode *pParent, WsjcppObjTreeNode *pChild);

        template<class T>
        bool addSupportType() {
            std::map<uint16_t, IFabricWsjcppObjTreeNode*>::iterator it;
            uint16_t nType = T::staticType();
            it = m_mapFabricTreeNode.find(nType);
            if (it != m_mapFabricTreeNode.end()) {
                WsjcppLog::err(TAG, "WsjcppObjTreeNode '" + std::to_string(nType) + "' type already registered.");
                return false;
            }
            m_mapFabricTreeNode[nType] = new FabricWsjcppObjTreeNode<T>(this);
            return true;
        }
        bool isSupportType(uint16_t );
        bool hasNode(WsjcppObjTreeNode *);

        template<class T, class A>
        int findNodes(const A &val, std::vector<T *> &vFoundNodes) {
            int nRet = 0;
            uint16_t nType = T::staticType();
            std::vector<WsjcppObjTreeNode *>::iterator it = m_vNodes.begin();
            for (it = m_vNodes.begin(); it != m_vNodes.end(); ++it) {
                if ((*it)->getType() == nType) {
                    T *pNode = static_cast<T *>(*it);
                    if (pNode->getValue() == val) {
                        vFoundNodes.push_back(pNode);
                        nRet++;
                    }
                }
            }
            return nRet;
        };
        
    private:
        std::string TAG;
        std::vector<WsjcppObjTreeNode *> m_vNodes;
        int m_nLastId;
        std::map<uint16_t, IFabricWsjcppObjTreeNode*> m_mapFabricTreeNode;

        void writeUInt32(std::ofstream &f, uint32_t nVal);
        void writeUInt16(std::ofstream &f, uint16_t nVal);
};

class WsjcppObjTreeChainDeclare {
    public:
        WsjcppObjTreeChainDeclare(WsjcppObjTree *pTree);
        
        WsjcppObjTreeChainDeclare &addNode(WsjcppObjTreeNode *);

        template<class T, class A>
        WsjcppObjTreeChainDeclare &addT(const A &val) {
            WsjcppObjTreeNode *pNode = new T(m_pTree, val);
            return addNode(pNode);
        }
        template<typename T> WsjcppObjTreeChainDeclare &add(const T &val);
        template<typename T> WsjcppObjTreeChainDeclare &add(const T *val);
        WsjcppObjTreeChainDeclare &up();
        WsjcppObjTreeChainDeclare &switchTo(WsjcppObjTreeNode *);

    private:
        std::string TAG;
        WsjcppObjTree *m_pTree;
        WsjcppObjTreeNode *m_pCurrentNode;
};

// define for inline templates
#define WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE( type0, classname ) \
    template<> inline \
    WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::add< type0 >(type0 const &val) { \
        return this->addT< classname >(val); \
    }

#define WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE_P( type0, classname ) \
    template<> inline \
    WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::add< type0 >(type0 const *val) { \
        return this->addT< classname >(val); \
    }

class WsjcppObjTreeNodeString : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeString(WsjcppObjTree *pTree, const std::string &sValue = "");
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_STRING; }; // string
        
        std::string getValue();
        void setValue(const std::string &sValue);
        
        // TreeNode
        virtual int getDataSize() override;
        virtual const char *getData() override;

    private:
        std::string m_sValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(std::string, WsjcppObjTreeNodeString)
WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE_P(char, WsjcppObjTreeNodeString)

class WsjcppObjTreeNodeInteger : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeInteger(WsjcppObjTree *pTree, const int32_t &nValue = 0);
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_INTEGER; }; // integer 32
        
        int32_t getValue();
        void setValue(int32_t nValue);

        // WsjcppObjTreeNode
        virtual int getDataSize() override;
        virtual const char *getData() override;

    private:
        int32_t m_nValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(int, WsjcppObjTreeNodeInteger)

class WsjcppObjTreeNodeFloat : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeFloat(WsjcppObjTree *pTree, const float &nValue = 0);
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_FLOAT; }; // float
        
        float getValue();
        void setValue(float nValue);

        // TreeNode
        virtual int getDataSize() override;
        virtual const char *getData() override;

    private:
        float m_nValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(float, WsjcppObjTreeNodeFloat)


class WsjcppObjTreeNodeDouble : public WsjcppObjTreeNode {
    public:
        WsjcppObjTreeNodeDouble(WsjcppObjTree *pTree, const double &nValue = 0);
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_DOUBLE; };
        
        float getValue();
        void setValue(float nValue);

        // TreeNode
        virtual int getDataSize() override;
        virtual const char *getData() override;

    private:
        double m_nValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(double, WsjcppObjTreeNodeDouble)

#endif // WSJCPP_OBJ_TREE_H
