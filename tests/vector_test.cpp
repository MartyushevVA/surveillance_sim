#include <gtest/gtest.h>
#include <type_traits>
#include <iterator>
#include "../myvector/myvector.h"

TEST(VectorTest, DefaultConstructor) {
    Vector<int> vec;
    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, ConstructorWithCapacity) {
    Vector<int> vec(5);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 5);
}

TEST(VectorTest, PushBack) {
    Vector<int> vec;
    vec.push_back(1);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 1);
}

TEST(VectorTest, PushBackMultiple) {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], 20);
}

TEST(VectorTest, PopBack) {
    Vector<int> vec;
    vec.push_back(1);
    vec.pop_back();
    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, AccessOperator) {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    EXPECT_EQ(vec[1], 20);
}

TEST(VectorTest, static_assert) {
    static_assert(std::is_same<typename std::iterator_traits<Vector<int>::VectorIterator<false>>::iterator_category, std::random_access_iterator_tag>::value);
}

TEST(VectorIteratorTest, IteratorCategory) {
    using Iterator = Vector<int>::iterator;
    static_assert(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, 
                               std::random_access_iterator_tag>::value,
                  "Iterator is not a random access iterator");
}

TEST(VectorTest, Resize) {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.resize(5, 0);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[2], 0);
}

TEST(VectorTest, Clear) {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.clear();
    EXPECT_EQ(vec.size(), 0);
    EXPECT_TRUE(vec.empty());
}

TEST(VectorTest, ShrinkToFit) {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.shrink_to_fit();
    EXPECT_EQ(vec.capacity(), 2);
}