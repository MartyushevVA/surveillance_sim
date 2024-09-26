#include <gtest/gtest.h>
#include "sclass.h"

TEST(TaskTests, ConstructorAndGetters)
{
    task t("Example Task", 3, 1, 5);
    EXPECT_EQ(t.getName(), "Example Task");
    EXPECT_EQ(t.getGrade(), 3);
    EXPECT_EQ(t.getFirst(), 1);
    EXPECT_EQ(t.getLast(), 5);
}

TEST(TaskTests, Setters)
{
    task t;
    t.setName("New Task");
    t.setGrade(4);
    t.setFirst(2);
    t.setLast(6);
    
    EXPECT_EQ(t.getName(), "New Task");
    EXPECT_EQ(t.getGrade(), 4);
    EXPECT_EQ(t.getFirst(), 2);
    EXPECT_EQ(t.getLast(), 6);
}

TEST(TaskTests, SetGradeOutOfRange)
{
    task t;
    EXPECT_THROW(t.setGrade(1), std::range_error);
    EXPECT_THROW(t.setGrade(6), std::range_error);
}

TEST(TaskTests, SetFirstAndLastOutOfRange)
{
    task t;
    t.setName("Task");
    t.setGrade(3);
    t.setFirst(1);
    t.setLast(5);
    
    EXPECT_THROW(t.setFirst(0), std::range_error);
    EXPECT_THROW(t.setLast(0), std::range_error);
    EXPECT_THROW(t.setLast(2), std::range_error);
}

TEST(FindFunctionTests, FindTaskInArray)
{
    task tasks[] = { task("Task1", 3, 1, 3), task("Task2", 4, 2, 4) };
    
    EXPECT_EQ(ofuncs::find(tasks, 2, task("Task1")), 0);
    EXPECT_EQ(ofuncs::find(tasks, 2, task("Task2")), 1);
}

TEST(FindFunctionTests, FindTaskNotFound)
{
    task tasks[] = { task("Task1", 3, 1, 3) };
    
    EXPECT_THROW(ofuncs::find(tasks, 1, task("Task2")), ofuncs::TaskNotFoundException);
}

TEST(FindFunctionTests, FindTaskNullSpace)
{
    EXPECT_THROW(ofuncs::find(nullptr, 0, task("Task1")), std::runtime_error);
}

TEST(TaskTests, OperatorPlus)
{
    task t1("Task1", 3, 1, 3);
    task t2("Task1", 3, 4, 6);
    
    task t3 = t1 + t2;
    EXPECT_EQ(t3.getFirst(), 1);
    EXPECT_EQ(t3.getLast(), 6);
}

TEST(TaskTests, Operators)
{
    task t1("Task1", 3, 1, 3);
    task t2("Task1", 3, 1, 4);
    task t3("Task2", 4, 1, 3);
    
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 < t3);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}