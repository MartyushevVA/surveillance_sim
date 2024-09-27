#include <gtest/gtest.h>
#include "sclass.h"

// Test for task class constructors and methods
TEST(TaskTest, DefaultConstructor)
{
    task t;
    EXPECT_EQ(t.getName(), "");
    EXPECT_EQ(t.getGrade(), 0);
    EXPECT_EQ(t.getFirst(), 0);
    EXPECT_EQ(t.getLast(), 0);
}

TEST(TaskTest, ParameterizedConstructor)
{
    task t("Task1", 3, 0, 5);
    EXPECT_EQ(t.getName(), "Task1");
    EXPECT_EQ(t.getGrade(), 3);
    EXPECT_EQ(t.getFirst(), 0);
    EXPECT_EQ(t.getLast(), 5);
}

TEST(TaskTest, SetName)
{
    task t;
    t.setName("NewTask");
    EXPECT_EQ(t.getName(), "NewTask");
}

TEST(TaskTest, SetGrade_Valid)
{
    task t;
    t.setGrade(4);
    EXPECT_EQ(t.getGrade(), 4);
}

TEST(TaskTest, SetGrade_Invalid)
{
    task t;
    EXPECT_THROW(t.setGrade(6), std::range_error);
    EXPECT_THROW(t.setGrade(1), std::range_error);
}

TEST(TaskTest, SetFirst_LastOrder)
{
    task t;
    t.setFirst(2);
    t.setLast(5);
    EXPECT_EQ(t.getFirst(), 2);
    EXPECT_EQ(t.getLast(), 5);
}

TEST(TaskTest, SetFirst_Invalid)
{
    task t;
    t.setFirst(5);
    EXPECT_THROW(t.setLast(3), std::range_error);
}

TEST(TaskTest, Fragmentation)
{
    task t("FragmentTask", 2, 1, 3);
    task *sheets = t.fragmentation();
    EXPECT_EQ(sheets[0].getName(), "FragmentTask");
    EXPECT_EQ(sheets[0].getGrade(), 2);
    EXPECT_EQ(sheets[0].getFirst(), 1);
    EXPECT_EQ(sheets[0].getLast(), 1);
    EXPECT_EQ(sheets[1].getFirst(), 2);
    EXPECT_EQ(sheets[2].getFirst(), 3);
    delete[] sheets; // Clean up dynamically allocated memory
}

// Test for ofuncs::find function
TEST(FindFunctionTest, FindExistingTask)
{
    task tasks[] = {task("Task1", 3, 0, 5), task("Task2", 4, 1, 6)};
    size_t index = ofuncs::find(tasks, 2, task("Task2"));
    EXPECT_EQ(index, 1);
}

TEST(FindFunctionTest, FindNonExistingTask)
{
    task tasks[] = {task("Task1", 3, 0, 5), task("Task2", 4, 1, 6)};
    EXPECT_THROW(ofuncs::find(tasks, 2, task("Task3")), ofuncs::TaskNotFoundException);
}

TEST(FindFunctionTest, FindInNullSpace)
{
    EXPECT_THROW(ofuncs::find(nullptr, 0, task("Task1")), std::runtime_error);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}