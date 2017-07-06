#include <iostream>

//主模板
template<bool>
struct StaticAssert;

// 完全特化
template<> 
struct StaticAssert<true>
{};

// 辅助宏
#define STATIC_ASSERT(exp)\
{ StaticAssert<((exp) != 0)> StaticAssertFailed; }

int main()
{
    STATIC_ASSERT(0>1);
}
