#ifndef UNIT_TEST_ADD_NODE_H
#define UNIT_TEST_ADD_NODE_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestAddNode : public WsjcppUnitTestBase {
    public:
        UnitTestAddNode();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_ADD_NODE_H

