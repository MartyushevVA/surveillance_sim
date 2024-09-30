#include <gtest/gtest.h>
#include "sclass.h"

TEST(TaskTest, DefaultConstructor){
    task t;
    EXPECT_EQ(t.getName(), "");
    EXPECT_EQ(t.getGrade(), 0);
    EXPECT_EQ(t.getFirst(), 0);
    EXPECT_EQ(t.getLast(), 0);
}

TEST(TaskTest, ParameterizedConstructor){
    task t("TaskA", 3, 0, 5);
    EXPECT_EQ(t.getName(), "TaskA");
    EXPECT_EQ(t.getGrade(), 3);
    EXPECT_EQ(t.getFirst(), 0);
    EXPECT_EQ(t.getLast(), 5);
}

TEST(TaskTest, SetName){
    task t;
    t.setName("NewTask");
    EXPECT_EQ(t.getName(), "NewTask");
}

TEST(TaskTest, SetGrade_Valid){
    task t;
    t.setGrade(4);
    EXPECT_EQ(t.getGrade(), 4);
}

TEST(TaskTest, SetGrade_OutOfRange){
    task t;
    EXPECT_THROW(t.setGrade(6), std::range_error);
    EXPECT_THROW(t.setGrade(-1), std::range_error);
}

TEST(TaskTest, SetGrade_Invalid){
    task t;
    EXPECT_THROW(t.setGrade(6), std::range_error);
    EXPECT_THROW(t.setGrade(1), std::range_error);
}

TEST(TaskTest, SetFirst_LastOrder){
    task t;
    t.setFirst(2);
    t.setLast(5);
    EXPECT_EQ(t.getFirst(), 2);
    EXPECT_EQ(t.getLast(), 5);
}

TEST(TaskTest, SetFirst_Invalid){
    task t;
    t.setFirst(5);
    EXPECT_THROW(t.setLast(3), std::range_error);
}

TEST(TaskTest, SetFirst_LastOrder_Invalid){
    task t;
    t.setFirst(6);
    EXPECT_THROW(t.setLast(5), std::range_error);
    t.setLast(10);
    EXPECT_THROW(t.setFirst(11), std::range_error);
}

TEST(TaskTest, Evaluate_Valid){
    task t("TaskA", 3, 0, 5);
    t.evaluate(4);
    EXPECT_EQ(t.getGrade(), 4);
}

TEST(TaskTest, Evaluate_Invalid){
    task t("TaskA", 3, 0, 5);
    EXPECT_THROW(t.evaluate(1), ofuncs::UnexpectedGradeException);
    EXPECT_THROW(t.evaluate(6), ofuncs::UnexpectedGradeException);
}

TEST(TaskTest, Fragmentation){
    task t("FragmentTask", 2, 1, 3);
    task* sheets = t.fragmentation();
    EXPECT_EQ(sheets[0].getName(), "FragmentTask");
    EXPECT_EQ(sheets[0].getGrade(), 2);
    EXPECT_EQ(sheets[0].getFirst(), 1);
    EXPECT_EQ(sheets[1].getFirst(), 2);
    EXPECT_EQ(sheets[2].getFirst(), 3);
    delete[] sheets;
}

TEST(TaskTest, OperatorPlus_Valid){
    task t1("TaskA", 2, 1, 3);
    task t2("TaskA", 2, 4, 4);
    task t3 = t1 + t2;
    EXPECT_EQ(t3.getFirst(), 1);
    EXPECT_EQ(t3.getLast(), 4);
}

TEST(TaskTest, OperatorPlus_Invalid){
    task t1("TaskA", 2, 1, 3);
    task t2("TaskB", 2, 4, 4);
    EXPECT_THROW(t1 + t2, ofuncs::WrongPositioningException);
}

TEST(TaskTest, OperatorPlus_ValidConditions){
    task t1("TaskA", 3, 1, 5);
    task t2("TaskA", 3, 6, 10);
    task result = t1 + t2;
    EXPECT_EQ(result.getName(), "TaskA");
    EXPECT_EQ(result.getFirst(), 1);
    EXPECT_EQ(result.getLast(), 10);
}

TEST(TaskTest, OperatorEqual_True){
    task t1("TaskA", 2, 1, 3);
    task t2("TaskA", 2, 1, 3);
    EXPECT_TRUE(t1 == t2);
}

TEST(TaskTest, OperatorEqual_False){
    task t1("TaskA", 2, 1, 3);
    task t2("TaskB", 2, 1, 3);
    EXPECT_FALSE(t1 == t2);
}

TEST(TaskTest, OperatorLess_True){
    task t1("TaskA", 2, 1, 3);
    task t2("TaskB", 2, 1, 3);
    EXPECT_TRUE(t1 < t2);
}

TEST(TaskTest, OperatorLess_TrueByFirst){
    task t1("TaskA", 2, 1, 3);
    task t2("TaskA", 2, 2, 3);
    EXPECT_TRUE(t1 < t2);
}

TEST(TaskTest, OperatorLess_False){
    task t1("TaskB", 2, 1, 3);
    task t2("TaskA", 2, 1, 3);
    EXPECT_FALSE(t1 < t2);
}

TEST(TaskTest, SpaceShipOperatorLessByName){
    task t1("TaskA", 3, 1, 5);
    task t2("TaskB", 3, 1, 5);
    EXPECT_EQ(t1<=>t2, std::strong_ordering::less);
}

TEST(TaskTest, SpaceShipOperatorGreaterByFirst){
    task t1("TaskA", 3, 4, 5);
    task t2("TaskA", 3, 1, 5);
    EXPECT_EQ(t1<=>t2, std::strong_ordering::greater);
}

TEST(TaskTest, SpaceShipOperatorEqual){
    task t1("TaskA", 3, 1, 5);
    task t2("TaskA", 3, 1, 7);
    EXPECT_EQ(t1<=>t2, std::strong_ordering::equal);
}

TEST(FindFunctionTest, FindExistingTask){
    task tasks[] = {task("TaskA", 3, 0, 5), task("TaskB", 4, 1, 6)};
    size_t index = ofuncs::find(tasks, 2, task("TaskB"));
    EXPECT_EQ(index, 1);
}

TEST(FindFunctionTest, FindNonExistingTask){
    task tasks[] = {task("TaskA", 3, 0, 5), task("TaskB", 4, 1, 6)};
    EXPECT_EQ(ofuncs::find(tasks, 2, task("TaskC")), 2);
}

TEST(FindFunctionTest, FindInNullSpace){
    EXPECT_THROW(ofuncs::find(nullptr, 0, task("TaskA")), std::runtime_error);
}
TEST(TaskTest, GetNumOfSheets){
    task t("TaskA", 3, 0, 5);
    EXPECT_EQ(t.getNumOfSheets(), 6);
}
TEST(TaskTest, OutputStreamOperator){
    task t("TaskA", 3, 0, 5);
    std::ostringstream oss;
    oss << t;
    EXPECT_EQ(oss.str(), "TaskA: 3 0<->5");
}
TEST(TaskTest, InputStreamOperator){
    task t;
    std::istringstream iss("TaskA 2 3 4\n");
    iss >> t;
    EXPECT_EQ(t.getName(), "TaskA");
    EXPECT_EQ(t.getLast(), 4);
}
