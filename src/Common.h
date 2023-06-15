#pragma once

#define ASSERT(x, msg) do { if(!(x)) { std::cerr << msg << "\n"; __debugbreak(); } } while(0)
#define ASSERT2(x) do { if(!(x)) { __debugbreak(); } } while(0)