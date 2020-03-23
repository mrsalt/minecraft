#include "gtest/gtest.h"
#include "RingIterator.h"

using namespace std;

template <class T>
T getListOfNumbers(int start_value, int last_value)
{
    T ret;
    for (int v = start_value; v <= last_value; v++)
        ret.push_back(v);
    return ret;
}

TEST(RingIteratorTests, ForwardIteration1)
{
    auto number_list = getListOfNumbers<vector<int>>(1, 10);
    int expected_value = 1;
    for (auto i : number_list)
    {
    }
    // ring iterators begin and end are the same value, so a basic range based
    // iteration won't do anything.
    EXPECT_EQ(expected_value, 1);
}

TEST(RingIteratorTests, ForwardIteration2)
{
    auto number_list = getListOfNumbers<vector<int>>(0, 9);
    auto it = number_list.begin();
    it++;
    int expected_value = 1;
    for (; it != number_list.end(); it++)
    {
        EXPECT_EQ(*it, expected_value);
        expected_value = (expected_value + 1) % number_list.size();
    }
    EXPECT_EQ(expected_value, 0); // we should end at the start
}

TEST(RingIteratorTests, ForwardIteration3)
{
    auto number_list = getListOfNumbers<vector<int>>(0, 9);
    auto it = RingIterator<int>(number_list, 5);
    auto end = it;
    it++;
    int expected_value = 6;
    for (; it != end; it++)
    {
        EXPECT_EQ(*it, expected_value);
        expected_value = (expected_value + 1) % number_list.size();
    }
    EXPECT_EQ(expected_value, 5); // we should end at the start
}

TEST(RingIteratorTests, BackwardIteration1)
{
    // iterate using reverse iterator, but with increment operator
    auto number_list = getListOfNumbers<vector<int>>(0, 9);
    auto fwit = RingIterator<int>(number_list, 5);
    auto it = ReverseRingIterator<int>(number_list, 5);
    EXPECT_EQ(*fwit, *it);
    auto end = it;
    it++;
    int expected_value = 4;
    for (; it != end; it++)
    {
        EXPECT_EQ(*it, expected_value);
        expected_value = (expected_value + (int)number_list.size() - 1) % (int)number_list.size();
    }
    EXPECT_EQ(expected_value, 5); // we should end at the start
}

TEST(RingIteratorTests, ForwardBackward1)
{
    // same as previous test, but use plain vector
    auto number_list = getListOfNumbers<vector<int>>(0, 9);
    auto right_it = RingIterator<int>(number_list);
    auto left_it = ReverseRingIterator<int>(number_list);
    EXPECT_EQ(*right_it, *left_it);
    auto end = left_it;
    EXPECT_EQ(end, left_it);
    left_it++;
    right_it++;
    int left_value = 9;
    int right_value = 1;
    for (; left_it != end; left_it++, right_it++)
    {
        EXPECT_EQ(*left_it, left_value);
        EXPECT_EQ(*right_it, right_value);
        left_value = (left_value + (int)number_list.size() - 1) % (int)number_list.size();
        right_value = (right_value + 1) % number_list.size();
    }
    EXPECT_EQ(left_value, 0); // we should end at the start
    EXPECT_EQ(right_value, 0); // we should end at the start
}

TEST(RingIteratorTests, ConstForwardBackward1)
{
    // same as previous test, but use plain vector
    auto number_list = getListOfNumbers<vector<int>>(0, 9);
    int start_value = 2;
    auto right_it = ConstRingIterator<int>(number_list, 2);
    auto left_it = ConstReverseRingIterator<int>(number_list, 2);
    EXPECT_EQ(*right_it, *left_it);
    auto end = left_it;
    EXPECT_EQ(end, left_it);
    left_it++;
    right_it++;
    int left_value = start_value - 1;
    int right_value = start_value + 1;
    for (; left_it != end; left_it++, right_it++)
    {
        EXPECT_EQ(*left_it, left_value);
        EXPECT_EQ(*right_it, right_value);
        left_value = (left_value + (int)number_list.size() - 1) % (int)number_list.size();
        right_value = (right_value + 1) % number_list.size();
    }
    EXPECT_EQ(left_value, start_value); // we should end at the start
    EXPECT_EQ(right_value, start_value); // we should end at the start
}