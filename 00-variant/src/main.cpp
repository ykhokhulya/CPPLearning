/**
 * MIT License
 *
 * Copyright (c) 2017 Yuriy Khokhulya
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <variant>

namespace {

////////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct MaxSize;

template <typename T, typename... Ts>
struct MaxSize<T, Ts...>
{
    static constexpr std::size_t value = sizeof(T) > MaxSize<Ts...>::value
                                             ? sizeof(T)
                                             : MaxSize<Ts...>::value;
};

template <>
struct MaxSize<>
{
    static constexpr std::size_t value = 0;
};

///*****************************************************************************

template <typename I, typename... Ts>
struct TypeIndex;

template <typename I, typename T, typename... Ts>
struct TypeIndex<I, T, Ts...>
{
    static constexpr std::size_t value = 1 + TypeIndex<I, Ts...>::value;
};

template <typename I, typename... Ts>
struct TypeIndex<I, I, Ts...>
{
    static constexpr std::size_t value = 1;
};

template <typename I>
struct TypeIndex<I, I>
{
    static constexpr std::size_t value = 1;
};

template <typename I, typename T>
struct TypeIndex<I, T>
{
};

///*****************************************************************************

template <std::size_t I, typename... Ts>
struct IndexType;

template <std::size_t I, typename T, typename... Ts>
struct IndexType<I, T, Ts...>
{
    using type = typename IndexType<I - 1, Ts...>::type;
};

template <typename T, typename... Ts>
struct IndexType<1, T, Ts...>
{
    using type = T;
};

template <std::size_t I>
struct IndexType<I>
{
};

////////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct Variant
{
    template <typename T>
    explicit Variant(T&& t)
        : m_type_index(TypeIndex<T, Ts...>::value)
    {
        new (m_data) T{std::move(t)};
    }

    explicit Variant()
        : m_type_index{0}
        , m_data{}
    {
    }

    ~Variant() = default;

    std::size_t getType() const { return m_type_index; }

    template <std::size_t I>
    const typename IndexType<I, Ts...>::type& getValue() const
    {
        return *reinterpret_cast<const typename IndexType<I, Ts...>::type*>(m_data);
    }

    template <typename T>
    const T& getValue() const
    {
        if (TypeIndex<T, Ts...>::value == m_type_index)
        {
            return *reinterpret_cast<const T*>(m_data);
        }
        throw std::bad_cast();
    }

    std::size_t m_type_index;
    std::byte m_data[MaxSize<Ts...>::value];
};

///*****************************************************************************

template <typename V, typename... Ts>
struct StaticVisitor
{
    static constexpr std::size_t index = 0;
    template <typename F>
    static void visit(const F& functor, const V& variant)
    {
    }
};

template <typename V, typename T, typename... Ts>
struct StaticVisitor<V, T, Ts...>
{
    static constexpr std::size_t index = 1 + StaticVisitor<V, Ts...>::index;

    template <typename F>
    static void visit(const F& functor, const V& variant)
    {
        if (variant.getType() == index)
        {
            const T& value = variant.template getValue<index>();
            functor(value);
        }
        else
        {
            StaticVisitor<V, Ts...>::template visit<F>(functor, variant);
        }
    }
};

template <typename F, typename... Ts>
void visit(const Variant<Ts...>& variant, const F& functor)
{
    StaticVisitor<Variant<Ts...>, Ts...>::template visit<F>(functor, variant);
}

} // namespace

int main(int argc, const char* argv[])
{
    std::cout << MaxSize<char>::value << std::endl;
    std::cout << TypeIndex<char, char, short, int>::value << std::endl;
    std::cout << sizeof(IndexType<1, char, short, int>::type) << std::endl;

    Variant<char, short, int> v{45};
    std::cout << "getType(): " << v.getType() << std::endl;
    std::cout << "getValue<3>(): " << v.getValue<3>() << std::endl;
    std::cout << "getValue<int>(): " << v.getValue<int>() << std::endl;

    visit(v, [](const int& a) { std::cout << "visited: " << a << std::endl; });

    return EXIT_SUCCESS;
}
