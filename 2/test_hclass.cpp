#include <gtest/gtest.h>
#include "hclass.h"

class StackTest : public ::testing::Test{
protected:
    stack s;
    task t1, t2, t3;

    void SetUp() override{
        t1 = task("TaskA");
        t2 = task("TaskB", 3, 2, 6);
        t3 = task("TaskB", 0, 7, 9);
        s += t1;
        s += t2;
    }
};
TEST_F(StackTest, DefaultConstructor) {
    stack emptyStack;
    EXPECT_EQ(emptyStack.getSize(), 0);
    EXPECT_EQ(emptyStack.getAllctd(), 10);
}
TEST_F(StackTest, CopyConstructor) {
    stack ss = s;
    EXPECT_EQ(ss.getSize(), s.getSize());
    EXPECT_EQ(ss.getAllctd(), s.getAllctd());
    EXPECT_NE(ss.getVector(), s.getVector());
}
TEST_F(StackTest, MoveConstructorTransfersOwnership) {
    stack ss = std::move(s);
    EXPECT_EQ(s.getSize(), 0);
    EXPECT_GT(ss.getSize(), 0);
}
TEST_F(StackTest, AssignmentOperator) {
    stack ss;
    ss = s;
    EXPECT_EQ(ss.getSize(), s.getSize());
    EXPECT_NE(ss.getVector(), s.getVector());
}
TEST_F(StackTest, MoveAssignmentTransfersOwnership) {
    stack ss;
    ss = std::move(s);
    EXPECT_EQ(s.getAllctd(), 0);
    EXPECT_GT(ss.getAllctd(), 0);
}
TEST_F(StackTest, IncrementOperator){
    EXPECT_EQ((s++).getAllctd(), 2);
    EXPECT_EQ(s.getAllctd(), 3);
}
TEST_F(StackTest, BackIncrementOperator){
    EXPECT_EQ(s.getAllctd(), 2);
    EXPECT_EQ((++s).getAllctd(), 3);
}
TEST_F(StackTest, DecrementOperator){
    s += t3;
    EXPECT_EQ((s--).getAllctd(), 4);
    EXPECT_EQ(s.getAllctd(), 3);
}
TEST_F(StackTest, BackDecrementOperator){
    s += t3;
    EXPECT_EQ(s.getAllctd(), 4);
    EXPECT_EQ((--s).getAllctd(), 3);
}
TEST_F(StackTest, Indexation){
    const stack ss = s;
    EXPECT_EQ(ss[0].getName(), "TaskA");
}
TEST_F(StackTest, ModifyIndexation){
    EXPECT_EQ(s[0].getName(), "TaskA");
    s[0].setName("TT");
    EXPECT_EQ(s[0].getName(), "TT");
}
TEST_F(StackTest, PushOperation){
    s += t3;
    EXPECT_EQ(s.getSize(), 3);
}
TEST_F(StackTest, PopOperation){
    task poppedTask = s.pop();
    EXPECT_EQ(poppedTask.getName(), "TaskB");
    EXPECT_EQ(s.getSize(), 1);
    poppedTask = s.pop();
    EXPECT_EQ(poppedTask.getName(), "TaskA");
    EXPECT_EQ(s.getSize(), 0);
}
TEST_F(StackTest, PopFromEmptyStack){
    while(s.getSize())
        s.pop();
    EXPECT_THROW(s.pop(), ofuncs::EmptyStackException);
}
TEST_F(StackTest, UnioningTest){
    s += t3;
    s.unioning();
    EXPECT_EQ(s.getSize(), 2);
}
TEST_F(StackTest, FragmentationTest){
    s.fragmentation();
    EXPECT_EQ(s.getSize(), 6);
}
TEST_F(StackTest, ExtractNextUngraded){
    task t1("TaskA", 0, 3, 7);
    task t2("TaskB", 5, 2, 7);
    s += t1;
    s += t2;
    task extractedTask = s.extractNextUngraded();
    EXPECT_EQ(extractedTask.getName(), "TaskA");
    EXPECT_EQ(s.getSize(), 3);
}
TEST_F(StackTest, ExtractFromEmptyStack){
    while(s.getSize())
        s.pop();
    EXPECT_THROW(s.extractNextUngraded(), ofuncs::EmptyStackException);
}
TEST_F(StackTest, ExtractFromAllGradedStack){
    task ts[2] = {task("TaskA", 2, 3, 7), task("TaskB", 5, 2, 7)};
    stack ss(2, ts);
    EXPECT_THROW(ss.extractNextUngraded(), ofuncs::TaskNotFoundException);
}
TEST_F(StackTest, CheckFullness){
    EXPECT_EQ(s.fullness(), 2);
}
TEST_F(StackTest, WrongEvaluating){
    EXPECT_THROW(s.getVector()[0].evaluate(-1), ofuncs::UnexpectedGradeException);
}
TEST_F(StackTest, OutputStreamOperator) {
    std::ostringstream oss;
    oss << s;
    std::string expectedOutput =
        "TaskA: 0 0<->0\n"
        "TaskB: 3 2<->6\n";
    EXPECT_EQ(oss.str(), expectedOutput);
}
TEST_F(StackTest, InputStreamOperator) {
    std::istringstream iss("2\nTaskA 2 3 4\nTaskB 3 4 5\n");
    iss >> s;
    EXPECT_EQ(s.getSize(), 4);
    EXPECT_EQ(s.getVector()[2].getName(), "TaskA");
    EXPECT_EQ(s.getVector()[3].getName(), "TaskB");
}