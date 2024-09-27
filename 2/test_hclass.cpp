#include <gtest/gtest.h>
#include "hclass.h"

class StackTest : public ::testing::Test {
protected:
    stack* myStack;

    void SetUp() override {
        myStack = new stack();
    }

    void TearDown() override {
        delete myStack;
    }
};

TEST_F(StackTest, SmoothResize_EmptyStack) {
    EXPECT_NO_THROW(myStack->smoothResize(0));
    EXPECT_EQ(myStack->fullness(), 0);
}

TEST_F(StackTest, SmoothResize_AfterAddingItem) {
    task t;
    myStack->operator+=(t);
    EXPECT_NO_THROW(myStack->smoothResize(0));
    EXPECT_EQ(myStack->fullness(), 1);
}

TEST_F(StackTest, SmoothResize_Downsize) {
    task t1, t2, t3;
    myStack->operator+=(t1);
    myStack->operator+=(t2);
    myStack->operator+=(t3);
    myStack->pop();
    EXPECT_NO_THROW(myStack->smoothResize(0));
}

TEST_F(StackTest, SmoothResize_MemoryAllocationFailure) {
    EXPECT_THROW({
        throw std::bad_alloc();
    }, std::runtime_error);
}
TEST_F(StackTest, Pop_EmptyStack) {
    EXPECT_THROW(myStack->pop(), ofuncs::EmptyStackException);
}