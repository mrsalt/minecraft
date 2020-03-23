#pragma once
// https://stackoverflow.com/questions/2616643/is-there-a-standard-cyclic-iterator-in-c
// courtesty of Mikal Keenan, Feb 6, 2012

#include <vector>

template <typename T, typename reftype, typename Container = std::vector<T>, typename Iterator = Container::iterator>
class RingIteratorBase : public std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t>
{
protected:
    Container &data;

    Iterator cursor;
    Iterator begin;
    Iterator end;

public:
    RingIteratorBase(Container &v) : data(v), cursor(v.begin()), begin(v.begin()), end(v.end()) {}

    RingIteratorBase(Container &v, const Iterator &i) : data(v), cursor(i), begin(v.begin()), end(v.end()) {}

    RingIteratorBase(Container &v, const Iterator &i, const Iterator &j) : data(v), cursor(i), begin(i), end(j) {}

    RingIteratorBase(Container &v, size_t i) : data(v), cursor(v.begin() + i % v.size()), begin(v.begin()), end(v.end()) {}

    bool operator==(const RingIteratorBase &x) const
    {
        return cursor == x.cursor;
    }

    bool operator!=(const RingIteratorBase &x) const
    {
        return !(*this == x);
    }

    reftype operator*() const
    {
        return *cursor;
    }

    RingIteratorBase &operator++()
    {
        ++cursor;
        if (cursor == end)
            cursor = begin;
        return *this;
    }

    RingIteratorBase operator++(int)
    {
        RingIteratorBase ring = *this;
        ++*this;
        return ring;
    }

    RingIteratorBase &operator--()
    {
        if (cursor == begin)
            cursor = end;
        --cursor;
        return *this;
    }

    RingIteratorBase operator--(int)
    {
        RingIteratorBase ring = *this;
        --*this;
        return ring;
    }

    RingIteratorBase insert(const T &x)
    {
        return RingIteratorBase(data, data.insert(cursor, x));
    }

    RingIteratorBase erase()
    {
        return RingIteratorBase(data, data.erase(cursor));
    }
};

template <typename T, typename Container = std::vector<T>, typename Iterator = Container::iterator>
class RingIterator : public RingIteratorBase<T, T &, Container, Iterator>
{
public:
    RingIterator(Container &v) : RingIteratorBase(v) {}

    RingIterator(Container &v, const Iterator &i) : RingIteratorBase(v, i) {}

    RingIterator(Container &v, const Iterator &i, const Iterator &j) : RingIteratorBase(v, i, j) {}

    RingIterator(Container &v, size_t i) : RingIteratorBase(v, i) {}
};

template <typename T, typename Container = std::vector<T>, typename Iterator = Container::iterator>
class ConstRingIterator : public RingIteratorBase<T, const T &, Container, Iterator>
{
public:
    ConstRingIterator(Container &v) : RingIteratorBase(v) {}

    ConstRingIterator(Container &v, const Iterator &i) : RingIteratorBase(v, i) {}

    ConstRingIterator(Container &v, const Iterator &i, const Iterator &j) : RingIteratorBase(v, i, j) {}

    ConstRingIterator(Container &v, size_t i) : RingIteratorBase(v, i) {}
};

template <typename T, typename Container = std::vector<T>, typename reftype = T &, typename ptrtype = T*, typename Iterator = Container::iterator>
class ReverseRingIterator : public RingIteratorBase<T, reftype, Container, Iterator>
{
public:
    ReverseRingIterator(Container &v) : RingIteratorBase(v) {}

    ReverseRingIterator(Container &v, const Iterator &i) : RingIteratorBase(v, i) {}

    ReverseRingIterator(Container &v, const Iterator &i, const Iterator &j) : RingIteratorBase(v, i, j) {}

    ReverseRingIterator(Container &v, size_t i) : RingIteratorBase(v, i) {}

    constexpr reftype operator*() const {
        return *cursor;
    }

    constexpr ptrtype operator->() const {
        return cursor;
    }

    ReverseRingIterator &operator++()
    {
        if (cursor == begin)
            cursor = end;
        --cursor;
        return *this;
    }

    ReverseRingIterator operator++(int)
    {
        ReverseRingIterator ring = *this;
        ++*this;
        return ring;
    }

    ReverseRingIterator &operator--()
    {
        ++cursor;
        if (cursor == end)
            cursor = begin;
        return *this;
    }

    ReverseRingIterator operator--(int)
    {
        ReverseRingIterator ring = *this;
        --*this;
        return ring;
    }
};

template <typename T, typename Container = std::vector<T>, typename Iterator = Container::iterator>
class ConstReverseRingIterator : public ReverseRingIterator<T, Container, const T &, const T *, Iterator>
{
public:
    ConstReverseRingIterator(Container &v) : ReverseRingIterator(v) {}

    ConstReverseRingIterator(Container &v, const Iterator &i) : ReverseRingIterator(v, i) {}

    ConstReverseRingIterator(Container &v, const Iterator &i, const Iterator &j) : ReverseRingIterator(v, i, j) {}

    ConstReverseRingIterator(Container &v, size_t i) : ReverseRingIterator(v, i) {}
};
