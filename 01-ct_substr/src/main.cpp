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

#include <cstdlib>
#include <iostream>

namespace {

template <std::size_t... I>
struct IntegerSeq
{
};

#if 0

template <typename L, std::size_t R>
struct JoinIntegerSeq;

template <std::size_t... L, std::size_t R>
struct JoinIntegerSeq<IntegerSeq<L...>, R>
{
    using type = IntegerSeq<L..., R>;
};

template <std::size_t N>
struct MakeIntegerSeq
{
    using type =
        typename JoinIntegerSeq<typename MakeIntegerSeq<N - 1>::type, N>::type;
};

template <>
struct MakeIntegerSeq<0>
{
    using type = IntegerSeq<0>;
};

template <std::size_t N>
using MakeIntegerSeq_t = typename MakeIntegerSeq<N>::type;
#else

template <std::size_t N, std::size_t... I>
struct MakeIntegerSeq : MakeIntegerSeq<N - 1, N, I...>
{
};

template <std::size_t... I>
struct MakeIntegerSeq<0, I...>
{
    using type = IntegerSeq<0, I...>;
};

template <std::size_t N>
using MakeIntegerSeq_t = typename MakeIntegerSeq<N>::type;

#endif

template <std::size_t... I>
void print(IntegerSeq<I...>)
{
    (std::cout << ... << I) << std::endl;
}

} // namespace

int main(int argc, const char* argv[])
{
    print(MakeIntegerSeq_t<9>());
    return EXIT_SUCCESS;
}
