# <h1 align="center"> rstd <br/>Reasonable/Robust/Rapid <br/>C++ Standard Library Replacement </h1>

# General info
I don't like api of default C++ standard library and the way its containers and other things allocate memory is too slow for my gamedev purposes. So I decided to create my own replacement for it.
The library is very earily in development.
It is sort of a side project for me and I make it along with my main, video game project. I add things here when I need them in my game.

# Features
### For now library supports only windows !!!

### Library has a couple of containers to offer:
- string<size, character>
- array<type, size>
- pushable_array<type, size>
- doubly_linked_list\<type>, doubly_linked_list_with_counter\<type>
- singly_linked_list\<type>, singly_linked_list_with_counter\<type>
- backward_singly_linked_list\<type>, backward_singly_linked_list_with_counter\<type>
  
### Arena
Arena allocator (also called push allocator) in this library is the basic allocator on which doubly_linked_list, singly_linked_list, backward_singly_linked_list (and their versions with counters) base their memory allocation. You have to assign arena to those containers before you use them, which is a little bit of pain in the ass, but in reward you gain a lot of performance and control. <br/>
You have to assign arena to container by using OwnArena(arena&) or ShareArena(arena&). Both of them return arena_ref which can be passed to constructors of these containers. <br/>
OwnArena() copies arena to container. <br/>
ShareArena() passes pointer to the arena to container so the same arena can be used outside of this container. It could be shared by multiple containers.
```cpp
rstd::arena Arena = rstd::AllocateArenaZero(10_MB);
auto S = rstd::PushStructZero(Arena, some_struct);
rstd::doubly_linked_list<some_struct> List = rstd::OwnArena(rstd::SubArena(Arena, 1_MB));
rstd::singly_linked_list<some_struct> List2 = rstd::ShareArena(Arena);
```

### Thread pool
### File io
### Time utilities
### Simple random number generator
### String parsing utilities
### Optional
  
### Defer
You can wrap your code in rstd_defer function macro and it will execute at the end of scope.
```cpp
{
auto File = rstd::OpenFile("file.txt", rstd::io_mode::Write);
rstd_defer(rstd::Close(File));

// writing to file
  
// file is closed here
}
```
  
### And other utilities
  
# Compilation
Rstd is a single header library. <br/>
Copy rstd.h file to your project directory or to whatever directory you like. <br/>
And then define rstd_Implementation in only one of C++ files that include this header. <br/>
You also have to define rstd_Debug before including rstd.h (in all files) <br/>
Define rstd_Debug to 1 if you want to have assertions and debug only code turn on <br/>
Define rstd_Debug to 0 to turn off assertions and to make release only code be compiled 

a.cpp
```cpp
#define rstd_Implementation
#define rstd_Debug 1
#include "rstd.h"
```
  
b.cpp
```cpp
#define rstd_Debug 1
#include "rstd.h"
```
  
c.cpp
```cpp
#define rstd_Debug 1
#include "rstd.h"
```
  
You can modify rstd.h and define rstd_Debug there so you don't have to do it every time you include rstd.h. <br/>
You could also create my_rstd.h file which sets up rstd configuration macros and then includes rstd.h.
  
