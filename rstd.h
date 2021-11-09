
/*
RSTD
Reasonale/Robust/Rapid C++ Standard Library Replacement
github.com/Czapa10/rstd

Made by Grzegorz "Czapa" Bednorz

YOU HAVE TO
define rstd_Implementation in one of C++ files that include this header
You also have to define rstd_Debug before including rstd.h (in all files)
Define rstd_Debug to 1 if you want to have assertions and debug only code turn on
Define rstd_Debug to 0 to turn off assertions and to make release only code be compiled

#define rstd_Implementation
#define rstd_Debug 1
#include <FastMath.h>
*/

#include <cstdint>

// TODO: Get rid of these headers
#include <algorithm>
#include <cstdio>
#include <cmath>

#ifndef rstd_Debug
#error "You have to define rstd_Debug. Define it to 0 for release build. Define it to 1 for debug build (in which rstd_Assert() works"
#endif

#ifndef rstd_MemoryProfileFunction
#define rstd_MemoryProfileFunction
#endif

// TODO: Make memory profiler not suck
#ifndef rstd_MemoryProfilerEnabled
#define rstd_MemoryProfilerEnabled 0
#endif

#ifndef rstd_FileDebugEnabled
#define rstd_FileDebugEnabled 0
#endif

#ifndef rstd_MultiThreadingEnabled
#define rstd_MultiThreadingEnabled 1
#endif

#ifndef rstd_bool
#define rstd_bool bool
#endif

#ifdef rstd_size
using size = rstd_size;
#else
using size = size_t;
#endif

#ifdef _WIN32
#include "intrin.h"
#endif

namespace rstd
{
    //////////////////////
    // TYPE DEFINITIONS //
    //////////////////////
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    
    using f32 = float;
    using f64 = double;
    
    ///////////////
    // constants //
    ///////////////
#ifndef rstd_FastMathAlreadyIncludedRstdAndFastMathSharedFeature
    // NOTE: define this macro when you're using FastMath library and you declare both 'using namespace fm' and 'using namespace rstd'
    // (or if you declared 'using namespace fm' and included FastMath.h before including rstd.h
    
    static constexpr uint8_t MaxU8 = 255;
    static constexpr uint16_t MaxU16 = 65535;
    static constexpr uint32_t MaxU32 = 4294967295;
    static constexpr uint64_t MaxU64 = 18446744073709551615;
    
    static constexpr int8_t MaxI8 = 127;
    static constexpr int8_t MinI8 = -MaxI8 - 1;
    
    static constexpr int16_t MaxI16 = 32767;
    static constexpr int16_t MinI16 = -MaxI16 - 1;
    
    static constexpr int32_t MaxI32 = 2147483647;
    static constexpr int32_t MinI32 = -MaxI32 - 1;
    
    static constexpr int64_t MaxI64 = 9223372036854775807;
    static constexpr int64_t MinI64 = -MaxI64 - 1;
    
    static constexpr float MinPositiveF32 = 1.401298464e-45f;
    static constexpr float MaxF32 = 3.402823466e+38f;
    static constexpr float MinF32 = -MaxF32;
    
    static constexpr double MinPositiveF64 = 4.9406564584124654e-324; 
    static constexpr double MaxF64 = 1.7976931348623158e+308;
    static constexpr double MinF64 = -MaxF64;
    
    static constexpr float EpsilonF32 = 1.19209290E-07f;
    static constexpr double EpsilonF64 = 2.2204460492503131e-16;
    
    static constexpr float Pi32 = 3.14159265359f;
    static constexpr double Pi64 = 3.14159265358979323846;
    static constexpr float Tau32 = Pi32 * 2;
    static constexpr double Tau64 = Pi64 * 2;
    
    template<class t> class constants
    {
        static constexpr rstd_bool IsSpecialized = false;
        static constexpr t Epsilon() { return {}; }
        static constexpr t Min() { return {}; }
        static constexpr t Max() { return {}; }
        static constexpr t Pi() { return {}; }
        static constexpr t Tau() { return {}; }
        static constexpr t MinPositive() { return {}; }
    };
    
    template<> class constants<float>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr float Epsilon() { return EpsilonF32; }
        static constexpr float Min() { return MinF32; }
        static constexpr float Max() { return MaxF32; }
        static constexpr float Pi() { return Pi32; }
        static constexpr float Tau() { return Tau32; }
        static constexpr float MinPositive() { return MinPositiveF32; }
    };
    
    template<> class constants<double>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr double Epsilon() { return EpsilonF64; }
        static constexpr double Min() { return MinF64; }
        static constexpr double Max() { return MaxF64; }
        static constexpr double Pi() { return Pi64; }
        static constexpr double Tau() { return Tau64; }
        static constexpr double MinPositive() { return MinPositiveF64; }
    };
    
    template<> class constants<uint8_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint8_t Epsilon() { return 0; }
        static constexpr uint8_t Min() { return 0; }
        static constexpr uint8_t Max() { return MaxU8; }
        static constexpr uint8_t Pi() { return 0; }
        static constexpr uint8_t Tau() { return 0; }
        static constexpr uint8_t MinPositive() { return 1; }
    };
    
    template<> class constants<int8_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int8_t Epsilon() { return 0; }
        static constexpr int8_t Min() { return MinI8; }
        static constexpr int8_t Max() { return MaxI8; }
        static constexpr int8_t Pi() { return 0; }
        static constexpr int8_t Tau() { return 0; }
        static constexpr int8_t MinPositive() { return 1; }
    };
    
    template<> class constants<uint16_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint16_t Epsilon() { return 0; }
        static constexpr uint16_t Min() { return 0; }
        static constexpr uint16_t Max() { return MaxU16; }
        static constexpr uint16_t Pi() { return 0; }
        static constexpr uint16_t Tau() { return 0; }
        static constexpr uint16_t MinPositive() { return 1; }
    };
    
    template<> class constants<int16_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int16_t Epsilon() { return 0; }
        static constexpr int16_t Min() { return MinI16; }
        static constexpr int16_t Max() { return MaxI16; }
        static constexpr int16_t Pi() { return 0; }
        static constexpr int16_t Tau() { return 0; }
        static constexpr int16_t MinPositive() { return 1; }
    };
    
    template<> class constants<uint32_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint32_t Epsilon() { return 0; }
        static constexpr uint32_t Min() { return 0; }
        static constexpr uint32_t Max() { return MaxU32; }
        static constexpr uint32_t Pi() { return 0; }
        static constexpr uint32_t Tau() { return 0; }
        static constexpr uint32_t MinPositive() { return 1; }
    };
    
    template<> class constants<int32_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int32_t Epsilon() { return 0; }
        static constexpr int32_t Min() { return MinI32; }
        static constexpr int32_t Max() { return MaxI32; }
        static constexpr int32_t Pi() { return 0; }
        static constexpr int32_t Tau() { return 0; }
        static constexpr int32_t MinPositive() { return 1; }
    };
    
    template<> class constants<uint64_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint64_t Epsilon() { return 0; }
        static constexpr uint64_t Min() { return 0; }
        static constexpr uint64_t Max() { return MaxU64; }
        static constexpr uint64_t Pi() { return 0; }
        static constexpr uint64_t Tau() { return 0; }
        static constexpr uint64_t MinPositive() { return 1; }
    };
    
    template<> class constants<int64_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int64_t Epsilon() { return 0; }
        static constexpr int64_t Min() { return MinI64; }
        static constexpr int64_t Max() { return MaxI64; }
        static constexpr int64_t Pi() { return 0; }
        static constexpr int64_t Tau() { return 0; }
        static constexpr int64_t MinPositive() { return 1; }
    };
    
    ////////////////////
    // invalid values //
    ////////////////////
    static constexpr uint16_t InvalidU16 = MaxU16;
    static constexpr uint32_t InvalidU32 = MaxU32;
    static constexpr uint64_t InvalidU64 = MaxU64;
    static constexpr int16_t InvalidI16 = MinI16;
    static constexpr int32_t InvalidI32 = MinI32;
    static constexpr int64_t InvalidI64 = MinI64;
    static constexpr float InvalidF32 = MinF32;
    static constexpr double InvalidF64 = MinF64;
#endif
    
#ifndef rstd_ExcludeDebugPrintingFunctions
    ////////////////////
    // DEBUG PRINTING //
    ////////////////////
    u32 GetSystemErrorCode();
    void InternalPrintInDebugger(const char* Message);
    void InternalWarningMessageBox(const char* Message);
    void ShowErrorMessageBox(const char* Message);
    void ShowErrorMessageBoxAndExitProcess(const char* Message);
#endif
    
    ////////////
    // STRING //
    ////////////
    static rstd_bool AdvanceIfStringsMatch(char** APtr, char* B);
    
    // TODO: Try to change it into template function
#define rstd_PrintInDebugger(_Fmt, ...) \
InternalPrintInDebugger(Format<string<>>(_Fmt, __VA_ARGS__));
    
    //////////////////
    // DEBUG MACROS //
    //////////////////
    // TODO: Try to change it into template function
#define rstd_RInvalidCodePath(_String, ...) \
{ ShowErrorMessageBoxAndExitProcess(Format<string<>>(_String, __VA_ARGS__)); }
    
    // TODO: Try to change it into template function
#define rstd_WarningMessageBox(_String, ...) \
{ InternalWarningMessageBox(Format<string<1020>>(_String, __VA_ARGS__).GetCString()); }
    
#define rstd_RAssert(Expression, String, ...) \
if(!(Expression)){rstd_RInvalidCodePath(String, __VA_ARGS__)}
    
#define rstd_Concat2(_A, _B) _A ## _B
#define rstd_Concat(_A, _B) rstd_Concat2(_A, _B)
#define rstd_LineName(_NameWithoutLine) rstd_Concat(_NameWithoutLine, __LINE__)
    
#if rstd_Debug
    
#define rstd_DebugOnly(Code) Code
#define rstd_ReleaseOnly(Code)
#define rstd_BreakInDebugger __debugbreak()
    
#define rstd_InvalidCodePath {\
rstd_WarningMessageBox("InvalidCodePath! file:% line:%", __FILE__, __LINE__); \
rstd_BreakInDebugger;}
    
#define rstd_InvalidCodePathM(Message, ...) {\
rstd_WarningMessageBox("InvalidCodePath! \n%\n file:% line:%", Message, __VA_ARGS__, __FILE__, __LINE__); \
rstd_BreakInDebugger;}
    
#define rstd_RawAssert(_Expr) if(!(_Expr)){\
ShowErrorMessageBox("Assertion failed!");\
rstd_BreakInDebugger;}
    
#define rstd_Assert(Expr) if(!(Expr)){\
rstd_WarningMessageBox("Assertion failed! \nfile:% line:%", __FILE__, __LINE__); \
rstd_BreakInDebugger;}
    
#define rstd_AssertM(Expr, Message, ...) \
if(!(Expr))\
{\
rstd_WarningMessageBox("Assertion failed! \n%\n file:% line:%",\
Format<string<1020>>(Message, __VA_ARGS__), __FILE__, __LINE__);\
rstd_BreakInDebugger;\
}
    
#define rstd_InvalidDefaultCase default: rstd_InvalidCodePath; break;
    
#else
    
#define rstd_DebugOnly(Code)
#define rstd_ReleaseOnly(Code) Code
#define rstd_Assert(Expr) 
#define rstd_AssertM(Expr, Message) 
#define rstd_InvalidCodePath
#define rstd_InvalidDefaultCase
    
#endif
    
    template<class code> struct _defer 
    {
        code Code;
        _defer(code Code) : Code(Code) {}
        ~_defer() { Code(); }
    };
    
    //TODO: Do we need this?
    template<class code> _defer<code> MakeDefer(code Code) 
    { return _defer<code>(Code); }
    
#define rstd_Defer1(x, y) x##y
#define rstd_Defer2(x, y) rstd_Defer1(x, y)
#define rstd_Defer3(x)    rstd_Defer2(x, __COUNTER__)
#define rstd_defer(code)   auto rstd_Defer3(_defer_) = rstd::MakeDefer([&](){code;})
    
    template<class type> struct optional
    {
        optional& operator=
        (type& D)
        {
            Data = D;
            Valid = true;
            return *this;
        }
        
        optional(type& D)
        { *this = D; }
        
        optional()
        { Valid = false; }
        
        operator rstd_bool()
        { return Valid; }
        
        type* operator->()
        {
            rstd_Assert(Valid);
            return &Data;
        }
        
        type& operator*()
        {
            rstd_Assert(Valid);
            return Data;
        }
        
        private:
        
        type Data;
        rstd_bool Valid;
    };
    
    //////////
    // TIME //
    //////////
    enum class month : u8
    {
        January = 1,
        February = 2,
        March = 3,
        April = 4,
        May = 5,
        June = 6,
        July = 7,
        August = 8,
        September = 9,
        October = 10,
        November = 11,
        December = 12,
    };
    
    enum class day_of_week : u8
    {
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thursday = 4,
        Friday = 5,
        Saturday = 6,
        Sunday = 7,
    };
    
    struct time
    {
        union
        {
            struct
            {
                u8 Day;	
                month Month;
                u32 Year;
            };
            u64 DayMonthYearPack;
        };
        
        day_of_week DayOfWeek;
        
        union
        {
            struct
            {
                u16 Millisecond;
                u8 Second;
                u8 Minute;
                u8 Hour;
            };
            u64 MillisecondSecondMinuteHourPack;
        };
    };
    
    time GetUtcTime();
    time GetLocalTime();
    
    ////////////
    // RANDOM //
    //////////// TODO: Replace it with your own random number generator implementation
    struct random_sequence
    { u32 A; };
    
    static random_sequence MakeRandomSequenceFromTime()
    {
        auto T = GetUtcTime();
        return {(u32)T.Millisecond * (u32)T.Day * (u32)T.DayOfWeek / (u32)T.Second};
    }
    
    static uint32_t RandomU32
    (random_sequence& S)
    {
        // Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
        S.A ^= S.A << 13;
        S.A ^= S.A >> 17;
        S.A ^= S.A << 5;
        return S.A;
    }
    
    static u16 RandomU16(random_sequence& S)
    { return (u16)(RandomU32(S) % MaxU16); }
    
    ///////////////
    // ITERATION //
    ///////////////
#define rstd_For(Index, MaxIndex) for(u32 Index = 0; Index < MaxIndex; ++Index)
#define rstd_ForF32(Index, MaxIndex) for(f32 Index = 0; Index < MaxIndex; ++Index)
#define rstd_ArrayCount(Arr) (sizeof(Arr)/sizeof((Arr)[0]))
    
#define internal_rstd_RestOfIteratorFunctions \
const iterator Begin() const \
{ return const_cast<const iterator>(Begin()); } \
    \
const iterator End() const \
{ return const_cast<const iterator>(End()); } \
    \
iterator begin() \
{ return Begin(); } \
    \
iterator end() \
{ return End(); } \
    \
const iterator begin() const \
{ return Begin(); } \
    \
const iterator end() const \
{ return End(); } \
    \
view<iterator> GetView() \
{ return {Begin(), End()}; } \
    
    template<class iterator> struct view
    {
        iterator Begin;
        iterator End;
        
        iterator begin()
        { return Begin; }
        
        iterator end()
        { return End; }
    };
    
    ////////////
    // STRING //
    ////////////
    template<u32 size = 256 - sizeof(u32), class character = char> struct string
    {
        character Characters[size];
        u32 Count;
        
        using iterator = character*;
        using string_type = string<size, character>;
        
        constexpr u32 GetMaxCount()
        { return size - 1; }
        
        constexpr u32 GetMaxCountWithNull()
        { return size; }
        
        u32 GetCount() const
        { return Count; }
        
        u32 GetCountWithNull() const
        { return Count + 1; }
        
        void InsertNullTerminator()
        { Characters[Count] = 0; }
        
        iterator Begin()
        { return Characters; }
        
        iterator End()
        { return Characters + Count; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        void UpdateCount()
        {
            char* C = Characters;
            while(*C)
                ++C;
            Count = (u32)(C - Characters);
        }
        
        string_type& operator=
        (const character* CString)
        { 
            rstd_AssertM(CString, "Call Clear() instead of assigning null or assign empty c-string");
            Count = 0;
            while(*CString)
            {
                rstd_Assert(Count < GetMaxCount());
                Characters[Count++] = *CString++;
            }
            rstd_DebugOnly(InsertNullTerminator());
            return *this;
        }
        
        string_type& operator=(character* CString)
        { return *this = const_cast<const character*>(CString); }
        
        template<class other_string>
            string<size, character>& operator=
        (const other_string& OtherString)
        {
            rstd_Assert(OtherString.Count <= GetMaxCount());
            For(CharIndex, OtherString.Count)
                Characters[CharIndex] = OtherString[CharIndex];
            Count = OtherString.Count;
            Internal(InsertNullTerminator();)
                return *this;
        }
        
        character* GetCString()
        {
            InsertNullTerminator();
            return Characters;
        }
        
        operator character*()
        { return GetCString(); }
        
        void Clear()
        { 
            Count = 0;
            rstd_DebugOnly(InsertNullTerminator();)
        }
        
        string()
        { Clear(); }
        
        string(character* CString)
        { *this = CString; }
        
        string(const character* CString)
        { *this = CString; }
        
        rstd_bool Empty()
        { return Count == 0; }
        
        character& operator[]
        (u32 Index)
        {
            rstd_AssertM(Index < Count, Format<string<>>("You tried to get character [%], but this string has only % characters", Index, Count));
            return Characters[Index];
        }
        
        character operator[]
        (u32 Index) const
        {
            rstd_AssertM(Index < Count, Format<string<>>("You tried to get character [%], but this string has only % characters", Index, Count));
            return Characters[Index];
        }
        
        void operator+=
        (char C)
        {
            Characters[Count++] = C;
            rstd_DebugOnly(InsertNullTerminator());
        }
        
        void operator+=
        (char* CString)
        {
            for(char* C = CString; *C; ++C)
            {
                rstd_AssertM(Count < GetMaxCount(), "C string is too long to append it to this string<>");
                Characters[Count++] = *C;
            }
            rstd_DebugOnly(InsertNullTerminator();)
        }
        
        void operator+=(const char* CString)
        { *this += const_cast<char*>(CString); }
        
        template<class appended_string_type> void operator+=
        (const appended_string_type& AppendedString)
        {
            rstd_Assert(Count + AppendedString.Count < GetMaxCount());
            rstd_For(CharIndex, AppendedString.Count)
                Characters[Count++] = AppendedString[CharIndex];
            rstd_DebugOnly(InsertNullTerminator();)
        }
    };
    
    template<u32 LeftStringSize, u32 RightStringSize, class character> rstd_bool operator==
    (const string<LeftStringSize, character>& Lhs, const string<RightStringSize, character>& Rhs)
    {
        if(Lhs.Count != Rhs.Count)
            return false;
        
        rstd_For(CharIndex, Lhs.Count)
        {
            if((character)(Lhs[CharIndex]) != (character)(Rhs[CharIndex]))
                return false;
        }
        
        return true;
    }
    
    template<u32 LeftStringSize, u32 RightStringSize, class character> rstd_bool operator!=
    (const string<LeftStringSize, character>& Lhs, const string<RightStringSize, character>& Rhs)
    { return !(Lhs == Rhs); }
    
    // TODO: What if I want to compare string<255, char> to wchar*
    template<u32 LeftStringSize, class character> rstd_bool operator==
    (const string<LeftStringSize, character>& Lhs, const character* Rhs)
    {
        static_assert(0, "Use StringMatchesCString() instead!\n"
                      "This operator== is defined only to get the amiguity error message! If it wasn't defined implicit cast operator to character* would be used and you would compare adresses of pointers instead of strings!");
    }
    
    template<u32 LeftStringSize, class character> rstd_bool operator==
    (const character* Lhs, const string<LeftStringSize, character>& Rhs)
    {
        static_assert(0, "Use StringMatchesCString() instead!\n"
                      "This operator== is defined only to get the amiguity error message! If it wasn't defined implicit cast operator to character* would be used and you would compare adresses of pointers instead of strings!");
    }
    
    template<u32 LeftStringSize, class character> rstd_bool operator!=
    (const string<LeftStringSize, character>& Lhs, const character* Rhs)
    {
        static_assert(0, "Use !StringMatchesCString() instead!\n"
                      "This operator== is defined only to get the amiguity error message! If it wasn't defined implicit cast operator to character* would be used and you would compare adresses of pointers instead of strings!");
    }
    
    template<u32 RightStringSize, class character> rstd_bool operator!=
    (const character* Lhs, const string<RightStringSize, character>& Rhs)
    {
        static_assert(0, "Use !StringMatchesCString() instead!\n"
                      "This operator== is defined only to get the amiguity error message! If it wasn't defined implicit cast operator to character* would be used and you would compare adresses of pointers instead of strings!");
    }
    
    template<u32 LeftStringSize, class character> rstd_bool StringMatchesCString
    (const string<LeftStringSize, character>& Lhs, const character* Rhs)
    {
        u32 CharIndex = 0;
        for(;;)
        {
            if(CharIndex == Lhs.Count && *Rhs == 0)
                return true;
            
            if(CharIndex == Lhs.Count || *Rhs == 0)
                return false;
            
            if(Lhs.Characters[CharIndex] != *Rhs)
                return false;
            
            ++CharIndex;
            ++Rhs;
        }
    }
    
    
#define rstd_ForCString(_Char, _String) for(char* _Char = _String; *_Char != 0; ++_Char)
    
    template<class string_type> static auto MakeStringFittingAsMuchAsPossible
    (const char* CString)
    {
        string_type Res;
        while(CString && Res.Count < Res.GetMaxCount())
            Res += *CString++;
        return Res;
    }
    
    template<class string_type> static auto MakeStringUpTillCount
    (const char* CString, u32 CharacterCount)
    {
        string_type Res;
        while(CString && Res.Count < CharacterCount)
            Res += *CString++;
        return Res;
    }
    
    template<class string_type> static auto MakeStringTillCharacter
    (const char* CString, char EndCharacter)
    {
        string_type Res;
        while(*CString != EndCharacter)
        {
            AssertM(*CString, "Null terminator was encontered before EndCharacter");
            Res += *CString++;
        }
        return Res;
    }
    
    template<class string_type, class end_condition> static auto MakeStringTill
    (const char* CString, end_condition EndCondition)
    {
        string_type Res;
        while(!EndCondition(*CString))
        {
            rstd_AssertM(*CString, "Null terminator was encontered before EndCondition");
            Res += *CString++;
        }
        return Res;
    }
    
    // TODO: Compress MakeStringTillCharacterAndAdvanceCString functions
    template<class string_type> static auto MakeStringTillCharacterAndAdvanceCString
    (char** CStringPtr, char EndCharacter)
    {
        string_type Res;
        char* CString = *CStringPtr;
        while(*CString != EndCharacter)
        {
            rstd_AssertM(*CString, "Null terminator was encontered before EndCharacter");
            Res += *CString++;
        }
        *CStringPtr = CString;
        return Res;
    }
    
    template<class string_type, class comparison_fn> static auto MakeStringTillCharacterAndAdvanceCString
    (char** CStringPtr, comparison_fn Comparison)
    {
        string_type Res;
        char* CString = *CStringPtr;
        while(!Comparison(*CString))
        {
            rstd_AssertM(*CString, "Null terminator was encontered before EndCharacter");
            Res += *CString++;
        }
        *CStringPtr = CString;
        return Res;
    }
    
    static char DigitToChar
    (u32 Digit)
    { 
        rstd_RawAssert(Digit < 10);
        return '0' + (char)Digit;
    }
    
    static u32 GetDigitCount
    (u64 Int)
    {
        u32 Res = 1;
        while(Int /= 10)
            ++Res;
        return Res;
    }
    
    template<u32 string_size, class unsigned_integer> static auto UnsignedIntegerToString
    (unsigned_integer Int)
    {
        string<string_size> Res;
        
        u32 DigitCount = GetDigitCount((u64)Int);
        Res.Count = DigitCount;
        
        while(DigitCount)
        {
            Res[--DigitCount] = DigitToChar(Int % 10);
            Int /= 10;	
        }
        
        rstd_DebugOnly(Res.InsertNullTerminator());
        
        return Res;
    }
    
    template<u32 string_size, class signed_integer> static auto SignedIntegerToString
    (signed_integer Int)
    {
        string<string_size> Res;
        
        u32 StopIndex;
        u32 CharCount;
        if(Int < 0)
        {
            StopIndex = 1;
            Int = -Int;
            Res.Count = CharCount = GetDigitCount((u64)Int) + 1;
            Res[0] = '-';
        }
        else
        {
            StopIndex = 0;
            Res.Count = CharCount = GetDigitCount((u64)Int);
        }
        
        while(CharCount > StopIndex)
        {
            Res[--CharCount] = DigitToChar(Int % 10);
            Int /= 10;
        }
        
        rstd_DebugOnly(Res.InsertNullTerminator());
        
        return Res;
    }
    
    static auto ToString(u8 A)
    { return UnsignedIntegerToString<4>(A); }
    
    static auto ToString(u16 A)
    { return UnsignedIntegerToString<8>(A); }
    
    static auto ToString(u32 A)
    { return UnsignedIntegerToString<12>(A); }
    
    static auto ToString(u64 A)
    { return UnsignedIntegerToString<24>(A); }
    
    static auto ToString(i8 A)
    { return SignedIntegerToString<4>(A); }
    
    static auto ToString(i16 A)
    { return SignedIntegerToString<8>(A); }
    
    static auto ToString(i32 A)
    { return SignedIntegerToString<12>(A); }
    
    static auto ToString(i64 A)
    { return SignedIntegerToString<24>(A); }
    
    // TODO: ToString(void*), Use it in debug.cpp
    
    template<u32 string_size, class float_type> static string<string_size> FloatToString
    (float_type F, u32 Precision)
    {
        // TODO: Handle special numbers like inf, nan
        
        string<string_size> Res;
        
        f32 Rounding = 5.f / pow(10.f, (f32)Precision + 1.f); // TODO: Replace pow with fm function
        
        if(F < 0)
        {
            F *= -1;
            Res += '-';
        }
        
        u64 Integer = (u64)F;
        f64 Fraction = F - Integer + Rounding;
        
        if(Integer == 0)
            Res += '0';
        else
            Res += ToString(Integer);
        
        if(Precision > 0)
        {
            Res += '.';
            while(Fraction < 0.1f)
            {
                Res += '0';
                Fraction *= 10;
                --Precision;
            }
            Fraction *= pow(10, Precision); // TODO: Replace pow with fm function
            Res += ToString((u64)Fraction);
        }
        
        return Res;
    }
    
    // TODO: Check if sizes of string are appropriate
    static auto ToString(f32 F, u32 Precision = 5)
    { return FloatToString<32>(F, Precision); }
    
    static auto ToString(f64 F, u32 Precision = 5)
    { return FloatToString<64>(F, Precision); }
    
    template<class bool_type> static const char* BoolToString(bool_type Bool)
    { return Bool ? "true" : "false"; }
    
    static const char* ToString(rstd_bool Bool)
    { return BoolToString(Bool); }
    
    // NOTE: This is only to make Format<string> template craziness work
    // TODO: Maybe do that differently, this is too silly!
    static char ToString(char C)
    { return C; }
    
    static char* ToString(char* C)
    { return C; }
    
    static auto ToString(const char* C)
    { return C; }
    
    template<u32 size> static auto ToString(string<size> String)
    { return String; }
    
    template<class string_type> static void InternalFormat
    (string_type* Res, const char* Format)
    { *Res = Format; }
    
    template<class string_type, class arg> static void InternalFormat
    (string_type* Res, const char* Format, arg Arg)
    {
        rstd_DebugOnly(rstd_bool LastArgPeeledOff = false);
        rstd_RawAssert(*Format); // You passed more variadic arguments then you have % signs in Format<string>()
        while(*Format)
        {
            if(*Format == '%')
            {
                ++Format;
                if(*Format == '%')
                {
                    *Res += '%';
                    ++Format;
                }
                else
                {
                    *Res += ToString(Arg);
                    rstd_RawAssert(!LastArgPeeledOff); // You have more % signs then variadic arguments in Format<string>()
                    rstd_DebugOnly(LastArgPeeledOff = true;)
                }
            }
            else
            {
                *Res += *Format;
                ++Format;
            }
        }
    }
    
    template<class string_type, class arg, class... args> static void InternalFormat
    (string_type* Res, const char* Format, arg Arg, args... Args)
    {
        for(;; ++Format)
        {
            if(*Format == '%')
            {
                ++Format;
                if(*Format == '%')
                {
                    *Res += '%';
                }
                else
                {
                    *Res += ToString(Arg);
                    InternalFormat(Res, Format, Args...);
                    return;
                }
            }
            else
            {
                rstd_RawAssert(*Format); // You passed more variadic arguments then you have % signs in Format<string>()
                *Res += *Format;
            }
        }
    }
    
    template<class string_type, class... args> static string_type Format
    (const char* Format, args... Args)
    {
        string_type Res;
        InternalFormat(&Res, Format, Args...);
        return Res;
    }
    
    template<class type> char* TypeName()
    { return " YOU DIDN'T SPECIFY TypeName() "; }
    
    template<> char* TypeName<u64>()
    { return "u64"; }
    
    template<> char* TypeName<u32>()
    { return "u32"; }
    
    template<> char* TypeName<u16>()
    { return "u16"; }
    
    template<> char* TypeName<u8>()
    { return "u8"; }
    
    template<> char* TypeName<i64>()
    { return "i64"; }
    
    template<> char* TypeName<i32>()
    { return "i32"; }
    
    template<> char* TypeName<i16>()
    { return "i16"; }
    
    template<> char* TypeName<i8>()
    { return "i8"; }
    
    template<> char* TypeName<f32>()
    { return "f32"; }
    
    template<> char* TypeName<f64>()
    { return "f64"; }
    
    static char* GetNullTerminator
    (char* String)
    {
        char* C = String;
        while(*C)
            ++C;
        return C;
    }
    
    template<class character> static rstd_bool CharIsAlpha(character C)
    { return (C >= 'A' && C <= 'Z') || (C >= 'a' && C <= 'z'); }
    
    template<class character> static rstd_bool CharIsDigit(character C)
    { return C >= '0' && C <= '9'; }
    
    template<class character> static rstd_bool CharIsDigitOrPoint(character C)
    { return CharIsDigit(C) || C == '.'; }
    
    // TODO: The user of library could assume that StringsMatch does EntireStringsMatch, Maybe I should change names of these functions
    static rstd_bool StringsMatch
    (const char* A, const char* B)
    {
        for(;;)
        {
            if(*A == 0 || *B == 0)
                return true;
            
            if(*A != *B)
                return false;
            
            ++A;
            ++B;
        }
    }
    
    static rstd_bool StringsMatchUntilCharacter
    (const char* A, const char* B, char Character)
    {
        for(;;)
        {
            if(*A == Character || *B == Character)
                return *A == Character && *B == Character;
            
            if(*A == 0 || *B == 0)
                return false;
            
            if(*A != *B)
                return false;
            
            ++A;
            ++B;
        }
    }
    
    static rstd_bool EntireStringsMatch(const char* A, const char* B)
    { return StringsMatchUntilCharacter(A, B, 0); }
    
    static rstd_bool AdvanceIfStringsMatch
    (char** APtr, char* B)
    {
        char* A = *APtr;
        for(;;)
        {
            if(*B == 0)
                break;
            
            if(*A != *B)
                return false;
            
            ++A;
            ++B;
        }
        
        *APtr = A;
        return true;
    }
    
    static rstd_bool AdvanceUntilCharacter
    (char** StringPtr, char Character)
    {
        char* C = *StringPtr;
        for(;;)
        {
            if(*C == Character)
            {
                *StringPtr = C;
                return true;
            }
            else if(*C == 0)
            {
                return false;
            }
            
            ++C;
        }
    }
    
    static rstd_bool AdvanceAfterCharacter
    (char** StringPtr, char Character)
    {
        rstd_bool Res = AdvanceUntilCharacter(StringPtr, Character);
        ++(*StringPtr);
        return Res;
    }
    
    static rstd_bool AdvanceUntilNextLine(char** StringPtr)
    { return AdvanceAfterCharacter(StringPtr, '\n'); }
    
    static rstd_bool AdvanceUntilAlpha
    (char** StringPtr)
    {
        char* C = *StringPtr;
        for(;;)
        {
            if(CharIsAlpha(*C))
            {
                *StringPtr = C;
                return true;
            }
            else if(*C == 0)
            {
                return false;
            }
            
            ++C;
        }
    }
    
    static void CopyStringUntilChar
    (char* Dest, const char* Source, char EndChar)
    {
        while(*Source != EndChar)
        {
            rstd_Assert(*Source != 0);
            *Dest++ = *Source++;
        }
        *Dest = 0;
    }
    
    template<class string_type> static void CopyStringUntilChar
    (string_type* Dest, const char* Source, char EndChar)
    {
        while(*Source != EndChar)
        {
            rstd_Assert(*Source != 0);
            *Dest += *Source++;
        }
    }
    
    static void CopyString
    (char* Dest, const char* Source, u32 LengthWithNullChar)
    {
        u32 CharIndex = 0;
        u32 Length = LengthWithNullChar - 1;
        for(; CharIndex < Length; ++CharIndex)
            Dest[CharIndex] = Source[CharIndex];
        Dest[CharIndex + 1] = 0;
    }
    
    static rstd_bool StringsMatch
    (const char* A, const char* B, u32 Length)
    {
        rstd_For(CharIndex, Length)
        {
            if(A[CharIndex] != B[CharIndex])
                return false;
        }
        return true;
    }
    
    static u32 GetOffsetToChar
    (const char* String, char Char)
    {
        u32 Offset = 0;
        while(String[Offset++] != Char);
        return Offset;
    }
    
    static void JumpAfterChar
    (char** String, char Char)
    {
        while(**String != Char)
        (*String)++;
        (*String)++;
    }
    
    static void JumpToNextLine(char** String)
    { JumpAfterChar(String, '\n'); }
    
    static char* GetPtrAfter
    (char* String, char* StringToLookFor, char EndChar = -128)
    {
        while(*String && *String != EndChar)
        {
            if(*String == *StringToLookFor)
            {
                auto* A = String;
                auto* B = StringToLookFor;
                for(;;)
                {
                    ++A;
                    ++B;
                    
                    if(*B == 0)
                        return A;
                    
                    if(*A != *B)
                        break;
                }
            }
            
            ++String;
        }
        return nullptr;
    }
    
    static rstd_bool StringContains(char* String, char* StringToLookFor, char EndChar = -128)
    { return GetPtrAfter(String, StringToLookFor, EndChar); }
    
    static f32 ReadF32
    (char** StringPtr)
    {
        char* String = *StringPtr;
        
        f32 Res = 0;
        u32 CharIndex = 0;
        
        // parse sign
        f32 Sign;
        if(*String == '-')
        {
            Sign = -1;
            ++CharIndex;
        }
        else
        {
            Sign = 1;
        }
        
        rstd_Assert(CharIsDigit(String[CharIndex]));
        
        // extract integer part
        rstd_bool ThereIsPartAfterPoint;
        for(;;)
        {
            char C = String[CharIndex++];
            
            if(!CharIsDigitOrPoint(C))
            {
                ThereIsPartAfterPoint = false;
                break;
            }
            if(C == '.')
            {
                ThereIsPartAfterPoint = true;
                break;
            }
            
            Res = 10 * Res + C - '0';
        }
        
        if(ThereIsPartAfterPoint)
        {
            // extract part after point
            f32 DigitSignificance = 1;
            for(;;)
            {
                char C = String[CharIndex++];
                
                if(!CharIsDigit(C))
                    break;
                
                DigitSignificance *= 0.1f;
                Res += DigitSignificance * (C - '0');
            }
        }
        
        Res *= Sign;
        *StringPtr += CharIndex - 1;
        return Res;
    }
    
    static u32 ReadU32
    (char** StringPtr)
    {
        char* String = *StringPtr;
        rstd_Assert(CharIsDigit(*String));
        u32 Res = 0, CharIndex = 0;
        while(CharIsDigit(String[CharIndex]))
        {
            Res = Res * 10 + String[CharIndex] - '0';
            ++CharIndex;
        }
        *StringPtr += CharIndex;
        return Res;
    }
    
    static i32 ReadI32
    (char** StringPtr)
    {
        char* String = *StringPtr;
        rstd_Assert(CharIsDigit(*String) || *String == '-');
        i32 Sign = 1;
        if(*String == '-')
        {
            Sign = -1;
            ++(*StringPtr);
        }
        return (i32)ReadU32(StringPtr) * Sign;
    }
    
    static u16 ReadU16
    (char** StringPtr)
    {
        u32 Res = ReadU32(StringPtr);
        rstd_Assert(Res <= MaxU16);
        return (u16)Res;
    }
    
    static f32 StringToF32(char* String)
    { return ReadF32(&String); }
    
    static u32 StringToU32(char* String)
    { return ReadU32(&String); }
    
    static u16 StringToU16(char* String)
    { return ReadU16(&String); }
    
    static u8 StringToU8(char* String)
    { return (u8)StringToU32(String); }
    
    static rstd_bool StringToBool
    (char* String)
    {
        if(StringsMatch(String, "1"))
            return true;
        else if(StringsMatch(String, "0"))
            return false;
        
        rstd_InvalidCodePath;
        return false;
    }
    
    static u64 HashString
    (const char* String)
    {
        // NOTE: Murmur string hash, one byte at a time version
        
        u64 Hash = 525201411107845655;
        for(; *String; ++String)
        {
            Hash ^= *String;
            Hash *= 0x5bd1e9955bd1e995;
            Hash ^= Hash >> 47;
        }
        return Hash;
    }
    
    template<class string_type> static u64 HashString
    (const string_type& String)
    {
        // NOTE: Murmur string hash, one byte at a time version
        
        u64 Hash = 525201411107845655;
        for(u32 CharIndex = 0; CharIndex < String.Count; ++CharIndex)
        {
            Hash ^= String[CharIndex];
            Hash *= 0x5bd1e9955bd1e995;
            Hash ^= Hash >> 47;
        }
        return Hash;
    }
    
    //////////
    // TIME //
    //////////
    rstd_bool operator==
    (time A, time B)
    {
        if(A.Year == B.Year && A.Month == B.Month && A.Day == B.Day)
        {
            rstd_Assert(A.DayOfWeek == B.DayOfWeek);
            return A.Hour == B.Hour && A.Minute == B.Minute && A.Second == B.Second && A.Millisecond == B.Millisecond;
        }
        return false;
    }
    
    rstd_bool operator<=
    (time A, time B)
    {
        if(A.DayMonthYearPack < B.DayMonthYearPack)
            return true;
        else if(A.DayMonthYearPack > B.DayMonthYearPack)
            return false;
        else
            return A.MillisecondSecondMinuteHourPack <= B.MillisecondSecondMinuteHourPack;
    }
    
    rstd_bool operator>=
    (time A, time B)
    {
        if(A.DayMonthYearPack > B.DayMonthYearPack)
            return true;
        else if(A.DayMonthYearPack < B.DayMonthYearPack)
            return false;
        else
            return A.MillisecondSecondMinuteHourPack >= B.MillisecondSecondMinuteHourPack;
    }
    
    rstd_bool operator<(time A, time B)
    { return !(A >= B); }
    
    rstd_bool operator>(time A, time B)
    { return !(A <= B); }
    
    
    static string<12> ToString
    (month M)
    {
        switch(M)
        {
            case month::January: return "January";
            case month::February: return "February";
            case month::March: return "March";
            case month::April: return "April";
            case month::May: return "May";
            case month::June: return "June";
            case month::July: return "July";
            case month::August: return "August";
            case month::September: return "September";
            case month::October: return "October";
            case month::November: return "November";
            case month::December: return "December";
            default: return "ERROR";
        }
        return {};
    }
    
    static month ReadMonth
    (char** StringPtr)
    {
        if(AdvanceIfStringsMatch(StringPtr, "January"))
            return month::January;
        else if(AdvanceIfStringsMatch(StringPtr, "February"))
            return month::February;
        else if(AdvanceIfStringsMatch(StringPtr, "March"))
            return month::March;
        else if(AdvanceIfStringsMatch(StringPtr, "April"))
            return month::April;
        else if(AdvanceIfStringsMatch(StringPtr, "May"))
            return month::May;
        else if(AdvanceIfStringsMatch(StringPtr, "June"))
            return month::June;
        else if(AdvanceIfStringsMatch(StringPtr, "July"))
            return month::July;
        else if(AdvanceIfStringsMatch(StringPtr, "August"))
            return month::August;
        else if(AdvanceIfStringsMatch(StringPtr, "September"))
            return month::September;
        else if(AdvanceIfStringsMatch(StringPtr, "October"))
            return month::October;
        else if(AdvanceIfStringsMatch(StringPtr, "November"))
            return month::November;
        else if(AdvanceIfStringsMatch(StringPtr, "December"))
            return month::December;
        rstd_InvalidCodePath;
        return {};
    }
    
    static month StringToMonth(char* String)
    { return ReadMonth(&String); }
    
    static string<12> ToString
    (day_of_week D)
    {
        switch(D)
        {
            case day_of_week::Monday: return "Monday";
            case day_of_week::Tuesday: return "Tuesday";
            case day_of_week::Wednesday: return "Wednesday";
            case day_of_week::Thursday: return "Thursday";
            case day_of_week::Friday: return "Friday";
            case day_of_week::Saturday: return "Saturday";
            case day_of_week::Sunday: return "Sunday";
            return "ERROR";
        }
        return {};
    }
    
    static string<4> MinuteToReadableString(u8 Minute)
    { return Minute < 10 ? Format<string<4>>("0%", Minute) : ToString(Minute); }
    
    static string<> ToReadableString(time T)
    { return Format<string<>>("% | % % % | %:%", T.DayOfWeek, T.Day, T.Month, T.Year, T.Hour, MinuteToReadableString(T.Minute)); }
    
    static string<> ToString(time T)
    { return Format<string<>>("%.%.%.%_%.%.%.%", T.Year, (u32)T.Month, T.Day, (u32)T.DayOfWeek, T.Hour, T.Minute, T.Second, T.Millisecond); }
    
    static time ReadTime
    (char** StringPtr)
    {
        time T;
        T.Year = ReadU32(StringPtr);
        ++(*StringPtr);
        T.Month = (month)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Day = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.DayOfWeek = (day_of_week)ReadU32(StringPtr); // TODO
        ++(*StringPtr);
        T.Hour = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Minute = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Second = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Millisecond = (u16)ReadU32(StringPtr);
        return T;
    }
    
    static time StringToTime(char* String)
    { return ReadTime(&String); }
    
    
    struct calling_info
    {
        char* FilePath;
        char* Function;
        u32 Line;
    };
    
#define GetCallingInfo() calling_info{__FILE__, __FUNCTION__, __LINE__}
    
    
    using allocator_name = string<16>;
    
    struct memory_block
    {
        memory_block* Prev;
        u8* Base;
        size Used;
        size MaxHistoricalUsed;
        size Size;
    };
    
    struct arena
    {
        memory_block* MemoryBlock;
        size MinimalAllocationSize;
        u32 TempMemCount;
        rstd_DebugOnly(const char* DebugName;)
    };
    
    struct push_size_uninitialized_ex_res
    {
        u8* Memory;
        size GarbageBytes;
        rstd_DebugOnly(rstd_bool NewMemoryBlockWasAllocated;)
    };
    
    struct temporary_memory
    {
        arena* Arena;
        memory_block* MemBlockOnBeginTemporaryMemory;
        size ArenaUsedOnBeginTemporaryMemory;
        rstd_DebugOnly(u32 DebugId;)
    };
    
#define rstd_AllocateArenaZero(_Size, ...) \
InternalAllocateArenaZero(_Size, GetCallingInfo(), __VA_ARGS__)
    
#define rstd_SubArena(_MasterArena, _Size, ...) \
InternalSubArena(_MasterArena, _Size, GetCallingInfo(), __VA_ARGS__)
    
#define rstd_PushSizeUninitialized(_Arena, _Size) \
InternalPushSizeUninitialized(_Arena, _Size, GetCallingInfo())
    
#define rstd_PushSizeZero(_Arena, _Size) \
InternalPushSizeZero(_Arena, _Size, GetCallingInfo())
    
#define rstd_PushStructUninitialized(_Arena, _Type) \
(*(_Type*)(rstd_PushSizeUninitialized(_Arena, sizeof(_Type))))
    
#define rstd_PushStructZero(_Arena, _Type) \
(*(_Type*)(rstd_PushSizeZero(_Arena, sizeof(_Type))))
    
#define rstd_PushArrayUninitialized(_Arena, _Type, _ArrayCount) \
(_Type*)(rstd_PushSizeUninitialized(_Arena, sizeof(_Type) * _ArrayCount))
    
#define rstd_PushArrayZero(_Arena, _Type, _ArrayCount) \
(_Type*)(rstd_PushSizeZero(_Arena, sizeof(_Type) * _ArrayCount))
    
#define rstd_PushStringCopy(_Arena, _InitString) \
InternalPushStringCopy(_Arena, _InitString, GetCallingInfo())
    
    
    namespace MemoryDebug
    {
        
        enum class allocation_type
        {
            ArenaPushUninitialized,
            ArenaPushZero,
            ArenaPushStringCopy,
            GenAlloc,
        };
        
        static void DummyScopeMemoryGroup(){}
        
#if rstd_MemoryProfilerEnabled
#define rstd_MemoryProfilerFunctionSignature
#else
#define rstd_MemoryProfilerFunctionSignature {}
#define ScopeMemoryGroup(Name) DummyScopeMemoryGroup()
#endif
        
        static void Init() rstd_MemoryProfilerFunctionSignature;
        static void BeginMemoryGroup(const char*) rstd_MemoryProfilerFunctionSignature;
        static void EndMemoryGroup() rstd_MemoryProfilerFunctionSignature;
        static void NextCallMemoryGroup(const char*) rstd_MemoryProfilerFunctionSignature;
        static void RegisterArenaPush(arena, push_size_uninitialized_ex_res, size Size, allocation_type, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterGenAlloc(void* Memory, size Size, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterGenFree(void* Memory, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterCreateArena(arena&, const char* ArenaName, const char* MasterArenaName, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterDeallocateArena(arena, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterArenaAllocateNextMemoryBlock(arena) rstd_MemoryProfilerFunctionSignature;
        static void RegisterArenaDeallocateMemoryBlock(arena) rstd_MemoryProfilerFunctionSignature;
        static void RegisterBeginTemporaryMemory(temporary_memory TempMem) rstd_MemoryProfilerFunctionSignature;
        static void RegisterEndTemporaryMemory(temporary_memory TempMem) rstd_MemoryProfilerFunctionSignature;
        
    }
    
    template<class type> static type BytesToKilobytes
    (type Bytes)
    { 
        type Kilobytes = Bytes / 1024;
        return Kilobytes;
    }
    
    template<class type> static type BytesToMegabytes
    (type Bytes)
    { 
        type Megabytes = Bytes / 1024 / 1024;
        return Megabytes;
    }
    
    template<class type> static type KilobytesToBytes(type Kilobytes)
    { return Kilobytes * 1024; }
    
    template<class type> static type MegabytesToBytes(type Megabytes)
    { return Megabytes * 1024 * 1024; }
    
    constexpr size operator"" _B(unsigned long long Bytes)
    { return (size)Bytes; }
    
    constexpr size operator"" _KB(unsigned long long Kilobytes)
    { return (size)Kilobytes * 1024; }
    
    constexpr size operator"" _MB(unsigned long long Megabytes)
    { return (size)Megabytes * 1024 * 1024; }
    
    static constexpr size MemoryPageSize = 4_KB;
    
    static size Align(size Size, size Alignment)
    { return (Size + Alignment - 1) & ~(Alignment - 1); }
    
    constexpr u32 ConstAlign(u32 Size, u32 Alignment)
    { return (Size + Alignment - 1) & ~(Alignment - 1); }
    
    static void Zero(void* Ptr, size Size)
    { memset(Ptr, 0, Size); }
    
    template<class type> static void ZeroStruct(type& Instance)
    { Zero(&Instance, sizeof(type)); }
    
    static rstd_bool Equal(void* A, void* B, size Size)
    { return memcmp(A, B, Size) == 0; }
    
    template<class type> static rstd_bool Equal(type& A, type& B)
    { return Equal(&A, &B, sizeof(type)); }
    
    static rstd_bool IsMemoryInitializedToZero
    (void* Memory, u32 MemorySize)
    {
        u8* Mem = (u8*)Memory;
        rstd_For(Byte, MemorySize)
        {
            if(Mem[Byte] != 0)
                return false;
        }
        return true;
    }
    
    template<class type> static rstd_bool IsStructInitializedToZero(type& Instance)
    { return IsMemoryInitializedToZero(&Instance, sizeof(type)); }
    
    static string<> GetChoppedSizeText
    (u64 Size)
    {
        string<> Res;
        
        u64 Megabytes = BytesToMegabytes(Size);
        u64 Kilobytes = BytesToKilobytes(Size - MegabytesToBytes(Megabytes));
        u64 Bytes = Size - MegabytesToBytes(Megabytes) - KilobytesToBytes(Kilobytes);
        
        if(Megabytes && Kilobytes && Bytes)
            Res = Format<string<>>("% MB + % KB + % B", Megabytes, Kilobytes, Bytes);
        else if(Megabytes && Kilobytes)
            Res = Format<string<>>("% MB + % KB", Megabytes, Kilobytes);
        else if(Megabytes && Bytes)
            Res = Format<string<>>("% MB + % B", Megabytes, Bytes);
        else if(Kilobytes && Bytes)
            Res = Format<string<>>("% KB + % B", Kilobytes, Bytes);
        else if(Megabytes)
            Res = Format<string<>>("% MB", Megabytes);
        else if(Kilobytes)
            Res = Format<string<>>("% KB", Kilobytes);
        else
            Res = Format<string<>>("% B", Bytes);
        
        return Res;
    }
    
    static string<> GetChoppedSizeText(u32 Size)
    { return GetChoppedSizeText((u64)Size); }
    
    static void* InternalGenAlloc
    (u32 Size, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        void* Memory = malloc(Size);
        MemoryDebug::RegisterGenAlloc(Memory, Size, CallingInfo);
        return Memory;
    }
    
    static void InternalGenFree
    (void* Memory, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        rstd_Assert(Memory);
        MemoryDebug::RegisterGenFree(Memory, CallingInfo);
        free(Memory);
    }
    
#define GenAllocSize(_Size) InternalGenAlloc(_Size, GetCallingInfo())
#define GenAllocStruct(_Type) (_Type*)GenAllocSize(sizeof(_Type))
#define GenAllocArray(_Type, _Count) (_Type*)GenAllocSize(sizeof(_Type) * _Count)
#define GenFree(_Memory) InternalGenFree(_Memory, GetCallingInfo())
    
    void* PageAlloc(size Bytes);
    void PageFree(void* Memory);
    
    static push_size_uninitialized_ex_res PushSizeUninitializedEx
    (arena& Arena, size Size)
    {
        rstd_MemoryProfileFunction;
        
        push_size_uninitialized_ex_res Res = {};
        
        auto* MemBlock = Arena.MemoryBlock;
        size UsedBeforeAllocation = MemBlock->Used;
        size UsedAfterAllocation = UsedBeforeAllocation + Size;
        
        if(UsedAfterAllocation > MemBlock->Size)
        {
            rstd_Assert(Arena.MinimalAllocationSize >= MemoryPageSize);
            
            auto Max = [](size A, size B){ return A > B ? A : B; };
            size AllocationSize = Max(Size + (size)sizeof(memory_block), Arena.MinimalAllocationSize);
            u8* NewBase = (u8*)PageAlloc(AllocationSize);
            rstd_RAssert(NewBase, "OS Allocation call failed (probably your machine ran out of memory)");
            
            size NewMemBlockSize = AllocationSize - sizeof(memory_block);
            auto* NewMemBlock = (memory_block*)(NewBase + NewMemBlockSize);
            NewMemBlock->Prev = MemBlock;
            NewMemBlock->Base = NewBase;
            NewMemBlock->Used = Size;
            NewMemBlock->MaxHistoricalUsed = Size;
            NewMemBlock->Size = NewMemBlockSize;
            Arena.MemoryBlock = NewMemBlock;
            
            Res.Memory = NewMemBlock->Base;
            
            MemoryDebug::RegisterArenaAllocateNextMemoryBlock(Arena);
        }
        else
        {
            Res.Memory = MemBlock->Base + MemBlock->Used;
            
            if(UsedBeforeAllocation < MemBlock->MaxHistoricalUsed)
                Res.GarbageBytes = MemBlock->MaxHistoricalUsed - UsedBeforeAllocation;
            
            if(UsedAfterAllocation > MemBlock->MaxHistoricalUsed)
                MemBlock->MaxHistoricalUsed = UsedAfterAllocation;
            
            MemBlock->Used = UsedAfterAllocation;
        }
        
        return Res;
    }
    
    static u8* InternalPushSizeUninitialized
    (arena& Arena, size Size, calling_info CallingInfo)
    { 
        rstd_MemoryProfileFunction;
        
        auto Res = PushSizeUninitializedEx(Arena, Size);
        MemoryDebug::RegisterArenaPush(Arena, Res, Size,
                                       MemoryDebug::allocation_type::ArenaPushUninitialized,
                                       CallingInfo);
        return Res.Memory;
    }
    
    static u8* InternalPushSizeZero
    (arena& Arena, u32 Size, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        
        auto Res = PushSizeUninitializedEx(Arena, Size);
        if(Res.GarbageBytes)
            Zero(Res.Memory, Res.GarbageBytes);
        
        MemoryDebug::RegisterArenaPush(Arena, Res, Size,
                                       MemoryDebug::allocation_type::ArenaPushZero, CallingInfo);
        
        return Res.Memory;
    }
    
    static char* InternalPushStringCopy
    (arena& Arena, char* InitString, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        
        u32 LengthWithNullChar = (u32)strlen(InitString) + 1;
        auto Res = PushSizeUninitializedEx(Arena, LengthWithNullChar);
        char* String = (char*)Res.Memory;
        strcpy(String, InitString);
        
        MemoryDebug::RegisterArenaPush(Arena, Res, LengthWithNullChar,
                                       MemoryDebug::allocation_type::ArenaPushStringCopy, CallingInfo);
        
        return String;
    }
    
    template<class type> static type& PushStruct
    (arena& Arena, const type& Init)
    {
        auto& A = rstd_PushStructUninitialized(Arena, type);
        A = Init;
        return A;
    }
    
    static arena InternalAllocateArenaZero
    (size Size, calling_info CallingInfo, char* DebugName = nullptr)
    {
        arena Arena;
        
        Size += sizeof(memory_block);
        Size = Align(Size, MemoryPageSize);
        u8* Base = (u8*)PageAlloc(Size);
        Size -= sizeof(memory_block);
        
        auto* MemBlock = (memory_block*)(Base + Size);
        MemBlock->Base = Base;
        MemBlock->Size = Size;
        Arena.MemoryBlock = MemBlock;
        
        Arena.MinimalAllocationSize = 1_MB;
        Arena.TempMemCount = 0;
        
        MemoryDebug::RegisterCreateArena(Arena, DebugName, nullptr, CallingInfo);
        
        return Arena;
    }
    
    static arena InternalSubArena
    (arena& MasterArena, size Size, calling_info CallingInfo, const char* DebugName = nullptr)
    {
        rstd_Assert(MasterArena.TempMemCount == 0);
        
        arena SubArena;
        
        u8* Base = InternalPushSizeUninitialized(MasterArena,
                                                 Size + sizeof(memory_block), CallingInfo);
        
        memory_block* MemBlock = (memory_block*)(Base + Size);
        ZeroStruct(*MemBlock);
        MemBlock->Base = Base;
        MemBlock->Size = Size;
        SubArena.MemoryBlock = MemBlock;
        
        SubArena.MinimalAllocationSize = MasterArena.MinimalAllocationSize;
        SubArena.TempMemCount = 0;
        
        MemoryDebug::RegisterCreateArena(SubArena, DebugName, MasterArena.DebugName, CallingInfo);
        
        return SubArena;
    }
    
    static void Clear
    (arena& Arena)
    {
        rstd_AssertM(Arena.TempMemCount == 0, "You forgot to call EndTemporaryMemory()");
        
        while(Arena.MemoryBlock->Prev)
        {
            auto* PrevMemBlock = Arena.MemoryBlock->Prev;
            MemoryDebug::RegisterArenaDeallocateMemoryBlock(Arena);
            PageFree(Arena.MemoryBlock->Base);
            Arena.MemoryBlock = PrevMemBlock;
        }
        Arena.MemoryBlock->Used = 0;
    }
    
    // TODO: Can't we just store arena copy on stack?
    static void DeallocateArenaStoredInItself
    (arena& Arena)
    {
        auto* MemBlock = Arena.MemoryBlock;
        while(MemBlock)
        {
            auto* PrevMemBlock = MemBlock->Prev;
            MemoryDebug::RegisterArenaDeallocateMemoryBlock(Arena);
            // TODO: Don't try to deallocate first block if it comes from SubArena()
            PageFree(MemBlock);
            MemBlock = PrevMemBlock;
        }
    }
    
    static void DeallocateArena
    (arena& Arena)
    {
        DeallocateArenaStoredInItself(Arena);
        Arena.MemoryBlock = nullptr;
    }
    
    static rstd_bool AllocatedFromSingleOsAllocationCall(arena& Arena)
    { return !Arena.MemoryBlock->Prev; }
    
    static size GetUsed
    (arena& Arena)
    {
        size Bytes = 0;
        auto* MemBlock = Arena.MemoryBlock;
        while(MemBlock)
        {
            Bytes += MemBlock->Used;
            MemBlock = MemBlock->Prev;
        }
        return Bytes;
    }
    
    static size GetUsedWithNewBlockAllocationAlignment
    (arena& Arena)
    {
        size Bytes = 0;
        auto* MemBlock = Arena.MemoryBlock;
        if(MemBlock)
        {
            Bytes = MemBlock->Used;
            while(MemBlock->Prev)
            {
                MemBlock = MemBlock->Prev;
                Bytes += MemBlock->Size;
            }
        }
        return Bytes;
    }
    
    static size GetAllocatedBytes
    (arena& Arena)
    {
        size Bytes = 0;
        auto* MemBlock = Arena.MemoryBlock;
        while(MemBlock)
        {
            Bytes += MemBlock->Size;
            MemBlock = MemBlock->Prev;
        }
        return Bytes;
    }
    
    static size GetUnusedBytes(memory_block MemBlock)
    { return MemBlock.Size - MemBlock.Used; }
    
    static temporary_memory BeginTemporaryMemory
    (arena& Arena)
    {
        temporary_memory TempMem;
        TempMem.Arena = &Arena;
        TempMem.MemBlockOnBeginTemporaryMemory = Arena.MemoryBlock;
        TempMem.ArenaUsedOnBeginTemporaryMemory = Arena.MemoryBlock->Used;
        ++Arena.TempMemCount;
        rstd_DebugOnly(TempMem.DebugId = Arena.TempMemCount;)
            MemoryDebug::RegisterBeginTemporaryMemory(TempMem);
        return TempMem;
    }
    
    static void EndTemporaryMemory
    (temporary_memory TempMem)
    {
        rstd_AssertM(TempMem.DebugId == TempMem.Arena->TempMemCount, "Temporary memory that was created most recently has to be ended first");
        
        auto* Arena = TempMem.Arena;
        while(Arena->MemoryBlock != TempMem.MemBlockOnBeginTemporaryMemory)
        {
            auto* PrevMemBlock = Arena->MemoryBlock->Prev;
            PageFree(Arena->MemoryBlock->Base);
            Arena->MemoryBlock = PrevMemBlock;
        }
        
        TempMem.Arena->MemoryBlock->Used = TempMem.ArenaUsedOnBeginTemporaryMemory;
        --TempMem.Arena->TempMemCount;
        
        MemoryDebug::RegisterEndTemporaryMemory(TempMem);
    }
    
    struct scope_temporary_memory
    {	
        temporary_memory TempMem;
        
        scope_temporary_memory(arena& Arena)
        { TempMem = BeginTemporaryMemory(Arena); }
        
        ~scope_temporary_memory()
        { EndTemporaryMemory(TempMem); }
    };
    
#define ScopeTemporaryMemory(Arena) scope_temporary_memory ScopeTempMem##__LINE__(Arena)
    
    
    struct arena_revert_point
    {
        memory_block* MemBlock;
        size Used;
    };
    
    static arena_revert_point GetArenaRevertPoint(arena Arena)
    { return {Arena.MemoryBlock, Arena.MemoryBlock->Used}; }
    
    static void RevertArena
    (arena& Arena, arena_revert_point RevertPoint)
    {
        while(Arena.MemoryBlock != RevertPoint.MemBlock)
        {
            auto* PrevArenaMemBlock = Arena.MemoryBlock->Prev;
            PageFree(Arena.MemoryBlock->Base);
            Arena.MemoryBlock = PrevArenaMemBlock;
        }
        rstd_Assert(Arena.MemoryBlock);
        Arena.MemoryBlock->Used = RevertPoint.Used;
    }
    
    struct arena_ref
    {
        arena_ref()
            :ArenaPtr(nullptr)
            ,OwnsArena(false)
        {}
        
        arena* Ptr()
        { return OwnsArena ? &Arena : ArenaPtr; }
        
        arena* operator->()
        { return Ptr(); }
        
        arena& operator*()
        { return *Ptr(); }
        
        operator rstd_bool()
        { return ArenaPtr != nullptr; }
        
        union
        {
            arena Arena;
            arena* ArenaPtr;
        };
        rstd_bool OwnsArena;
    };
    
    static arena_ref ShareArena
    (arena& Arena)
    {
        arena_ref Ref;
        Ref.ArenaPtr = &Arena;
        Ref.OwnsArena = false;
        return Ref;
    }
    
    static arena_ref OwnArena
    (arena& Arena)
    {
        arena_ref Ref;
        Ref.Arena = Arena;
        Ref.OwnsArena = true;
        return Ref;
    }
    
    template<class dest_container, class source_container> void CopyElements
    (dest_container* Dest, source_container& Source)
    {
        Dest->Clear();
        for(auto& E : Source)
            Dest->Push(E);
    }
    
    ///////////
    // ARRAY //
    ///////////
    template<class type, u32 size>
        struct array
    {
        using iterator = type*;
        
        type Elements[size];
        
        iterator Begin()
        { return Elements; }
        
        iterator End()
        { return Elements + size; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        type& operator[]
        (u32 Index)
        {
            rstd_AssertM(Index < size, Format<string<>>("You tried to get element [%], but this array has only % elements", Index, size));
            return Elements[Index];
        }
        
        constexpr u32 GetCount()
        { return size; }
        
        void Zero()
        { Zero(Elements, size * sizeof(type)); }
        
        type& GetFirst()
        { return *Elements; }
        
        type& GetLast()
        { return *(Elements + size - 1); }
        
        template<class compare_type>               
type* FindEqual                            
(const compare_type& ThingToComare)        
{                                          
    for(auto& Element : *this)         
    {                                      
        if(Element == ThingToComare)               
            return &Element;               
    }                                      
    return nullptr;                        
}                                          

template<class comparison_fn>        
type* Find                           
(comparison_fn Comparison)           
{                                    
    for(auto& Element : *this)       
    {                                
        if(Comparison(Element))      
            return &Element;         
    }                                
    return nullptr;                  
}                                    

template<class comparison_fn> 
auto& FindWithAssert 
(comparison_fn Comparison) 
{ 
    auto* Found = Find(Comparison); 
    rstd_Assert(Found); 
    return *Found; 
} 

template<class compare_type> 
rstd_bool HasEqual 
(const compare_type& ThingToCompare) 
{ return (rstd_bool)FindEqual(ThingToCompare); } 

template<class comparison_fn> 
rstd_bool Has 
(comparison_fn Comparison) 
{ return (rstd_bool)Find(Comparison); } 

template<class compare_type> 
u32 FindIndexOfFirstEqual 
(const compare_type& ThingToCompare) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class comparison_fn> 
u32 FindIndexOfFirst 
(comparison_fn Comparison) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class compare_type> 
u32 HowManyEqualHas 
(const compare_type& ThingToCompare) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            ++Res; 
    } 
    return Res; 
} 

template<class comparison_fn> 
u32 HowManyHas 
(comparison_fn Comparison) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            ++Res; 
    } 
    return Res; 
} 

struct get_value_and_index_result 
{ 
    type* Value; 
    u32 Index; 
    
    operator rstd_bool() 
    { return Value; } 
    
    type* operator->() 
    { 	 
        rstd_Assert(Value); 
        return Value; 
    } 
}; 

template<class comparison_fn> 
get_value_and_index_result FindValueAndIndex 
(comparison_fn Comparison) 
{ 
    get_value_and_index_result Res = {}; 
    for(auto& E : *this) 
    { 
        if(Comparison(E)) 
        { 
            Res.Value = &E; 
            break; 
        } 
        ++Res.Index; 
    } 
    return Res; 
}

        rstd_bool HasPtr(const type* Ptr) 
{ return Ptr >= Elements && Ptr < Elements + Count; } 

u32 GetIndexFromPtr 
(const type* Ptr) 
{ 
    /* TODO: rstd_Assert(Is aligned to array elements) */ 
    rstd_Assert(HasPtr(Ptr)); 
    return (u32)((umm)(Ptr - Elements)); 
}

    };
    
    ////////////////////
    // PUSHABLE ARRAY //
    ////////////////////
    template<class type, u32 size>
        struct pushable_array
    {
        using iterator = type*;
        
        type Elements[size];
        u32 Count;
        
        pushable_array() 
        { Count = 0; }
        
        template<class... args>
            pushable_array
        (const args&&... InitElements)
        {
            Count = 0;
            Push(InitElements...);
        }
        
        iterator Begin()
        { return Elements; }
        
        iterator End()
        { return Elements + Count; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        type& operator[]
        (u32 Index)
        {
            rstd_AssertM(Index < Count, Format<string<>>("You tried to get element [%], but this pushable_array has only % elements", Index, Count));
            return Elements[Index];
        }
        
        constexpr u32 GetMaxCount()
        { return size; }
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        { Count = 0; }
        
        void Zero()
        {
            Zero(Elements, Count * sizeof(type));
            Count = 0;
        }
        
        rstd_bool Full()
        { return Count == size; }
        
        rstd_bool Empty()
        { return Count == 0; }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return *Elements; 
        }
        
        type& GetLast()
        { 
            rstd_Assert(!Empty());
            return Elements[Count - 1]; 
        }
        
        type& PushUninitialized()
        {
            rstd_Assert(!Full());
            ++Count;
            return Elements[Count - 1];
        }
        
        type& PushZero()
        {
            auto& Data = PushUninitialized();
            ZeroStruct(Data);
            return Data;
        }
        
        type* PushZeroIfNotFull()
        {
            if(!Full())
                return &PushZero();
            return nullptr;
        }
        
        type* PushUninitializedIfNotFull()
        {
            if(!Full())
                return &PushUninitialized();
            return nullptr;
        }
        
        void Pop()
        {
            rstd_Assert(!Empty());
            --Count;
        }
        
        void RemovePointerAsserts
        (type& E) 
        {
            rstd_Assert(!Empty());
            rstd_Assert(&E >= Elements);
            rstd_Assert(&E < Elements + Count);
        }
        
        type* Remove
        (type& E)
        {
            RemovePointerAsserts(E);
            if(&E < Elements + Count - 1)
                E = Elements[Count - 1];
            --Count;
            return &E - 1;
        }
        
        type* RemoveAndPersistOrder
        (type& E)
        {
            RemovePointerAsserts(E);
            if(&E < Elements + Count - 1)
                memcpy(&E, &E + 1, (size_t)(((char*)(Elements + Count) - (char*)(&E))));
            --Count;
            return &E - 1;
        }
        
        void Remove
        (u32 Index)
        {
            rstd_Assert(!Empty());
            rstd_Assert(Index >= 0);
            rstd_Assert(Index < Count);
            if(Index < Count - 1)
                Elements[Index] = Elements[Count - 1];
            --Count;
        }
        
        void PopFirst()
        { Remove(Elements); }
        
        void PopLast()
        {
            rstd_Assert(!Empty());
            --Count;
        }
        
        void PopFrontAndPersistOrder()
        { RemoveAndPersistOrder(Elements); }
        
        template<class comparison_fn>
            void Sort(comparison_fn Comparison)
        { std::sort(Elements, Elements + Count, Comparison); }
        
        template<class comparison_fn>
            void StableSort(comparison_fn Comparison)
        { std::stable_sort(Elements, Elements + Count, Comparison); }
        
        template<class compare_type>               
type* FindEqual                            
(const compare_type& ThingToComare)        
{                                          
    for(auto& Element : *this)         
    {                                      
        if(Element == ThingToComare)               
            return &Element;               
    }                                      
    return nullptr;                        
}                                          

template<class comparison_fn>        
type* Find                           
(comparison_fn Comparison)           
{                                    
    for(auto& Element : *this)       
    {                                
        if(Comparison(Element))      
            return &Element;         
    }                                
    return nullptr;                  
}                                    

template<class comparison_fn> 
auto& FindWithAssert 
(comparison_fn Comparison) 
{ 
    auto* Found = Find(Comparison); 
    rstd_Assert(Found); 
    return *Found; 
} 

template<class compare_type> 
rstd_bool HasEqual 
(const compare_type& ThingToCompare) 
{ return (rstd_bool)FindEqual(ThingToCompare); } 

template<class comparison_fn> 
rstd_bool Has 
(comparison_fn Comparison) 
{ return (rstd_bool)Find(Comparison); } 

template<class compare_type> 
u32 FindIndexOfFirstEqual 
(const compare_type& ThingToCompare) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class comparison_fn> 
u32 FindIndexOfFirst 
(comparison_fn Comparison) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class compare_type> 
u32 HowManyEqualHas 
(const compare_type& ThingToCompare) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            ++Res; 
    } 
    return Res; 
} 

template<class comparison_fn> 
u32 HowManyHas 
(comparison_fn Comparison) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            ++Res; 
    } 
    return Res; 
} 

struct get_value_and_index_result 
{ 
    type* Value; 
    u32 Index; 
    
    operator rstd_bool() 
    { return Value; } 
    
    type* operator->() 
    { 	 
        rstd_Assert(Value); 
        return Value; 
    } 
}; 

template<class comparison_fn> 
get_value_and_index_result FindValueAndIndex 
(comparison_fn Comparison) 
{ 
    get_value_and_index_result Res = {}; 
    for(auto& E : *this) 
    { 
        if(Comparison(E)) 
        { 
            Res.Value = &E; 
            break; 
        } 
        ++Res.Index; 
    } 
    return Res; 
}

        type& Push 
(const type& InitialData) 
{ 
    auto& Data = PushUninitialized(); 
    Data = InitialData; 
    return Data; 
} 

type& PushDefault() 
{ 
    type E; 
    return Push(E); 
} 

type* PushIfNotFull 
(const type& InitialData) 
{ 
    if(!Full()) 
        return &Push(InitialData); 
    return nullptr; 
} 

type* PushIfUnique 
(const type& InitialData) 
{ 
    if(!HasEqual(InitialData)) 
        return &Push(InitialData); 
    return nullptr; 
} 

template<class comparison_fn> 
type* PushIfUnique
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &Push(InitialData); 
    return nullptr; 
} 

type& GetIfExistsOrPushIfUnique 
(const type& ThingToCompareOrInitialData) 
{ 
    if(auto* Found = Find(ThingToCompareOrInitialData)) 
        return *Found; 
    return Push(ThingToCompareOrInitialData); 
} 

/* TODO(now): Do args need to be templated? */ 
template<class... args> 
void Push 
(const type& CurrentPushElement, const args&&... NextPushElements) 
{ 
    Push(CurrentPushElement); 
    Push(NextPushElements...); 
}

        template<class compare_type> 
rstd_bool RemoveFirstEqualTo 
(const compare_type& ThingToCompare) 
{ 
    if(auto* Found = FindEqual(ThingToCompare)) 
    { 
        Remove(*Found); 
        return true; 
    } 
    return false; 
} 

template<class compare_type> 
void RemoveFirstEqualToWithAssert 
(const compare_type& ThingToCompare) 
{ 
    bool ManagedToRemove = RemoveFirstEqualTo(ThingToCompare); 
    rstd_Assert(ManagedToRemove); 
} 

template<class comparison_fn> 
rstd_bool RemoveFirstIf 
(comparison_fn Comparison) 
{ 
    if(auto* Found = Find(Comparison)) 
    { 
        Remove(*Found); 
        return true; 
    } 
    return false; 
} 

template<class comparison_fn> 
void RemoveFirstIfWithAssert 
(comparison_fn Comparison) 
{ 
    bool ManagedToRemove = RemoveFirstIf(Comparison); 
    rstd_Assert(ManagedToRemove); 
} 

template<class comparison_fn> 
optional<type> RemoveFirstIfReturnCopy 
(comparison_fn Comparison) 
{ 
    if(auto* Found = Find(Comparison)) 
    { 
        Remove(*Found); 
        return {*Found}; 
    } 
    return {}; 
} 

template<class comparison_fn> 
u32 RemoveIf 
(comparison_fn Comparison) 
{ 
    u32 RemovedCount = 0; 
    for(auto It = Begin(); It != End(); ++It) 
    { 
        if(Comparison(*It)) 
        { 
            Remove(It); 
            ++RemovedCount; 
        } 
    } 
    return RemovedCount; 
} 

type GetAndPopFirst() 
{ 
    auto FirstCopy = GetFirst(); 
    PopFirst(); 
    return FirstCopy; 
} 

rstd_bool PopFirstIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return false; 
    } 
    else 
    { 
        PopFirst(); 
        return true; 
    } 
} 

optional<type> GetAndPopFirstIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return {}; 
    } 
    else 
    { 
        auto* First = &GetFirst(); 
        type FirstCopy = *First; 
        Remove(First); 
        return FirstCopy; 
    } 
} 

rstd_bool PopLastIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return false; 
    } 
    else 
    { 
        PopLast(); 
        return true; 
    } 
} 

type GetAndPopLast() 
{ 
    type LastCopy = GetLast(); 
    PopLast(); 
    return LastCopy; 
} 

optional<type> GetAndPopLastIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return {}; 
    } 
    else 
    { 
        auto* Last = &GetLast(); 
        type LastCopy = *Last; 
        Remove(Last); 
        return LastCopy; 
    } 
}

        rstd_bool HasPtr(const type* Ptr) 
{ return Ptr >= Elements && Ptr < Elements + Count; } 

u32 GetIndexFromPtr 
(const type* Ptr) 
{ 
    /* TODO: rstd_Assert(Is aligned to array elements) */ 
    rstd_Assert(HasPtr(Ptr)); 
    return (u32)((umm)(Ptr - Elements)); 
}

    };
    
    template<class type>
        struct doubly_linked_list
    {
        // NOTE: to be more precise, it's a doubly linked list with sentinel node
        
        struct node
        {
            type Data;
            node* Next;
            node* Prev;
        };
        
        static_assert(offsetof(node, Data) == 0, "we need Data to be first in node for Remove(type*)");
        
        arena_ref ArenaRef;
        node* Sentinel;
        node* Nodes;
        node* FreeNodes;
        
        rstd_bool Initialized()
        { return Nodes; }
        
        doubly_linked_list()
        { Nodes = nullptr; }
        
        doubly_linked_list
        (arena_ref ArenaRef)
        {
            this->ArenaRef = ArenaRef;
            Sentinel = &rstd_PushStructUninitialized(*ArenaRef, doubly_linked_list<type>::node);
            Nodes = Sentinel;
            Sentinel->Next = Sentinel;
            Sentinel->Prev = Sentinel;
            FreeNodes = nullptr;
        }
        
        struct iterator
        {
            node* Node;
            rstd_DebugOnly(node* DebugSentinel;)
                
                iterator& operator++() 
            {
                Node = Node->Next;
                return *this;
            }
            
            iterator operator++(int)
            {
                auto Res = *this;
                Node = Node->Next;
                return Res;
            }
            
            iterator& operator--() 
            { 
                Node = Node->Prev;
                return *this;
            }
            
            iterator operator--(int)
            {
                auto Res = *this;
                Node = Node->Prev;
                return Res;
            }
            
            iterator& operator+=
            (u32 Offset) 
            {
                while(Offset--)
                    Node = Node->Next;
                return *this;
            }
            
            iterator& operator-=
            (u32 Offset)
            {
                while(Offset--)
                    Node = Node->Prev;
                return *this;
            }
            
            iterator operator+
            (u32 Offset)
            {
                auto Res = *this;
                return Res += Offset;
            }
            
            iterator operator-
            (u32 Offset)
            {
                auto Res = *this;
                return Res -= Offset;
            }
            
            u32 operator-
            (iterator Rhs)
            {
                u32 Diff = 0;
                while(Node != Rhs.Node)	
                {
                    Rhs = Rhs.Prev;
                    ++Diff;
                    rstd_Assert(Rhs.Node != DebugSentinel);
                }
                return Diff;
            }
            
            // TODO: Should I remove Ptr() and leave just conversion operator?
            type* Ptr()
            { 
                rstd_Assert(Node != DebugSentinel);
                return &Node->Data;
            }
            
            operator type*()
            { return Ptr(); }
            
            type* operator->()
            { return Ptr(); }
            
            type& operator*()
            { return *Ptr(); }
            
            rstd_bool operator==(iterator Rhs)
            { return Node == Rhs.Node; }
            
            rstd_bool operator!=(iterator Rhs)
            { return Node != Rhs.Node; }
        };
        
        iterator Begin()
        {
            iterator It = {Sentinel->Next};
            rstd_DebugOnly(It.DebugSentinel = Sentinel);
            return It;
        }
        
        iterator End()
        {
            iterator It = {Sentinel};
            rstd_DebugOnly(It.DebugSentinel = Sentinel);
            return It;
        }
        
        internal_rstd_RestOfIteratorFunctions;
        
        rstd_bool Empty()
        { return Sentinel->Next == Sentinel; }
        
        type& operator[]
        (u32 DesiredIndex)
        {
            rstd_Assert(!Empty());
            
            u32 Index = 0;
            for(auto& E : *this)
            {
                if(Index == DesiredIndex)
                    return E;
                ++Index;
            }
            
            rstd_InvalidCodePathM(Format<string<>>("You tried to get element [%], but doubly_linked_list has only % elements", DesiredIndex, Index));
            return *(type*)(nullptr);
        }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return Sentinel->Next->Data;
        }
        
        type& GetLast()
        {
            rstd_Assert(!Empty());
            return Sentinel->Prev->Data;
        }
        
        void SanityCheck()
        {
#if rstd_Debug
            rstd_AssertM(Nodes, "You have to initialize list before using it!");
            rstd_Assert(Nodes == Sentinel);
            rstd_Assert(FreeNodes != Sentinel);
            rstd_Assert(Sentinel->Next && Sentinel->Prev);
            
#ifdef rstd_EnableDoublyLinkedListAdvancedSanityCheck	
            auto* Prev = Sentinel;
            auto* Current = Sentinel->Next;
            while(Current != Sentinel)
            {
                rstd_Assert(Current->Next);
                rstd_Assert(Current->Prev);
                rstd_Assert(Prev->Next == Current);
                rstd_Assert(Current->Prev == Prev);
                Prev = Current;
                Current = Current->Next;
            }
#endif
#endif
        }
        
        u32 GetCount()
        {
#ifdef rstd_GetCountEnabledInListsWithoutCounter
            SanityCheck();
            u32 Count = 0;
            for(auto& E : *this)
                ++Count;
            return Count;
#else
            static_assert(0,
                          "Use doubly_linked_list_with_counter instead of doubly_linked_list\n"
                          "to be able to GetCount()\n"
                          "You can also #define rstd_GetCountEnabledInListsWithoutCounter");
#endif
        }
        
        void PushNode
        (node* Node)
        {
            auto* LastNode = Sentinel->Prev;
            Sentinel->Prev = Node;
            LastNode->Next = Node;
            Node->Prev = LastNode;
            Node->Next = Sentinel;
            
            SanityCheck();
        }
        
        void PushNodeFront
        (node* Node)
        {
            auto* FirstNode = Sentinel->Next;
            Sentinel->Next = Node;
            FirstNode->Prev = Node;
            Node->Next = FirstNode;
            Node->Prev = Sentinel;
            
            SanityCheck();
        }
        
        node* PopFreeNode()
        {
            rstd_Assert(FreeNodes);
            node* Node = FreeNodes;
            FreeNodes = FreeNodes->Next;
            return Node;
        }
        
        node* AllocateZeroNode()
        {
            node* Node;
            if(FreeNodes)
            {
                Node = PopFreeNode();
                ZeroStruct(Node->Data);
            }
            else
            {
                Node = &rstd_PushStructZero(*ArenaRef, node);
            }
            return Node;
        }
        
        node* AllocateUninitializedNode()
        { return FreeNodes ? PopFreeNode() : &rstd_PushStructUninitialized(*ArenaRef, node); }
        
        type& PushZero()
        {
            node* Node = AllocateZeroNode();
            PushNode(Node);
            return Node->Data;
        }
        
        type& PushUninitialized()
        {
            node* Node = AllocateUninitializedNode();
            PushNode(Node);
            return Node->Data;
        }
        
        type& PushFrontZero()
        {
            node* Node = AllocateZeroNode();
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushFrontUninitialized()
        {
            node* Node = AllocateUninitializedNode();
            PushNodeFront(Node);
            return Node->Data;
        }
        
        void AddFreeNode
        (node* Node)
        {
            Node->Next = FreeNodes;
            FreeNodes = Node;
        }
        
        void Clear()
        {
            SanityCheck();
            
            iterator Next;
            for(auto It = Begin(); It != End(); It = Next)
            {
                Next = It + 1;
                AddFreeNode(It.Node);
            }
            
            Sentinel->Next = Sentinel;
            Sentinel->Prev = Sentinel;
        }
        
        void Remove
        (node* Node)
        {
            rstd_Assert(!Empty());
            
            Node->Prev->Next = Node->Next;
            Node->Next->Prev = Node->Prev;
            AddFreeNode(Node);
            
            SanityCheck();
        }
        
        iterator Remove
        (iterator It)
        {
            Remove(It.Node);
            return --It;
        }
        
        void Remove(type& E)
        { Remove((node*)&E); }
        
        void PopFirst()
        { Remove(GetFirst()); }
        
        void PopLast()
        { Remove(GetLast()); }
        
        template<class compare_type>               
type* FindEqual                            
(const compare_type& ThingToComare)        
{                                          
    for(auto& Element : *this)         
    {                                      
        if(Element == ThingToComare)               
            return &Element;               
    }                                      
    return nullptr;                        
}                                          

template<class comparison_fn>        
type* Find                           
(comparison_fn Comparison)           
{                                    
    for(auto& Element : *this)       
    {                                
        if(Comparison(Element))      
            return &Element;         
    }                                
    return nullptr;                  
}                                    

template<class comparison_fn> 
auto& FindWithAssert 
(comparison_fn Comparison) 
{ 
    auto* Found = Find(Comparison); 
    rstd_Assert(Found); 
    return *Found; 
} 

template<class compare_type> 
rstd_bool HasEqual 
(const compare_type& ThingToCompare) 
{ return (rstd_bool)FindEqual(ThingToCompare); } 

template<class comparison_fn> 
rstd_bool Has 
(comparison_fn Comparison) 
{ return (rstd_bool)Find(Comparison); } 

template<class compare_type> 
u32 FindIndexOfFirstEqual 
(const compare_type& ThingToCompare) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class comparison_fn> 
u32 FindIndexOfFirst 
(comparison_fn Comparison) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class compare_type> 
u32 HowManyEqualHas 
(const compare_type& ThingToCompare) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            ++Res; 
    } 
    return Res; 
} 

template<class comparison_fn> 
u32 HowManyHas 
(comparison_fn Comparison) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            ++Res; 
    } 
    return Res; 
} 

struct get_value_and_index_result 
{ 
    type* Value; 
    u32 Index; 
    
    operator rstd_bool() 
    { return Value; } 
    
    type* operator->() 
    { 	 
        rstd_Assert(Value); 
        return Value; 
    } 
}; 

template<class comparison_fn> 
get_value_and_index_result FindValueAndIndex 
(comparison_fn Comparison) 
{ 
    get_value_and_index_result Res = {}; 
    for(auto& E : *this) 
    { 
        if(Comparison(E)) 
        { 
            Res.Value = &E; 
            break; 
        } 
        ++Res.Index; 
    } 
    return Res; 
}

        type& Push 
(const type& InitialData) 
{ 
    auto& Data = PushUninitialized(); 
    Data = InitialData; 
    return Data; 
} 

type& PushDefault() 
{ 
    type E; 
    return Push(E); 
} 

type* PushIfNotFull 
(const type& InitialData) 
{ 
    if(!Full()) 
        return &Push(InitialData); 
    return nullptr; 
} 

type* PushIfUnique 
(const type& InitialData) 
{ 
    if(!HasEqual(InitialData)) 
        return &Push(InitialData); 
    return nullptr; 
} 

template<class comparison_fn> 
type* PushIfUnique
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &Push(InitialData); 
    return nullptr; 
} 

type& GetIfExistsOrPushIfUnique 
(const type& ThingToCompareOrInitialData) 
{ 
    if(auto* Found = Find(ThingToCompareOrInitialData)) 
        return *Found; 
    return Push(ThingToCompareOrInitialData); 
} 

/* TODO(now): Do args need to be templated? */ 
template<class... args> 
void Push 
(const type& CurrentPushElement, const args&&... NextPushElements) 
{ 
    Push(CurrentPushElement); 
    Push(NextPushElements...); 
}

        type& PushFront 
(const type& InitialData) 
{ 
    auto& Data = PushFrontUninitialized(); 
    Data = InitialData; 
    return Data; 
}

template<class comparison_fn> 
type* PushFrontIfUnique 
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &PushFront(InitialData); 
    return nullptr; 
} 

        template<class compare_type> 
rstd_bool RemoveFirstEqualTo 
(const compare_type& ThingToCompare) 
{ 
    if(auto* Found = FindEqual(ThingToCompare)) 
    { 
        Remove(*Found); 
        return true; 
    } 
    return false; 
} 

template<class compare_type> 
void RemoveFirstEqualToWithAssert 
(const compare_type& ThingToCompare) 
{ 
    bool ManagedToRemove = RemoveFirstEqualTo(ThingToCompare); 
    rstd_Assert(ManagedToRemove); 
} 

template<class comparison_fn> 
rstd_bool RemoveFirstIf 
(comparison_fn Comparison) 
{ 
    if(auto* Found = Find(Comparison)) 
    { 
        Remove(*Found); 
        return true; 
    } 
    return false; 
} 

template<class comparison_fn> 
void RemoveFirstIfWithAssert 
(comparison_fn Comparison) 
{ 
    bool ManagedToRemove = RemoveFirstIf(Comparison); 
    rstd_Assert(ManagedToRemove); 
} 

template<class comparison_fn> 
optional<type> RemoveFirstIfReturnCopy 
(comparison_fn Comparison) 
{ 
    if(auto* Found = Find(Comparison)) 
    { 
        Remove(*Found); 
        return {*Found}; 
    } 
    return {}; 
} 

template<class comparison_fn> 
u32 RemoveIf 
(comparison_fn Comparison) 
{ 
    u32 RemovedCount = 0; 
    for(auto It = Begin(); It != End(); ++It) 
    { 
        if(Comparison(*It)) 
        { 
            Remove(It); 
            ++RemovedCount; 
        } 
    } 
    return RemovedCount; 
} 

type GetAndPopFirst() 
{ 
    auto FirstCopy = GetFirst(); 
    PopFirst(); 
    return FirstCopy; 
} 

rstd_bool PopFirstIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return false; 
    } 
    else 
    { 
        PopFirst(); 
        return true; 
    } 
} 

optional<type> GetAndPopFirstIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return {}; 
    } 
    else 
    { 
        auto* First = &GetFirst(); 
        type FirstCopy = *First; 
        Remove(First); 
        return FirstCopy; 
    } 
} 

rstd_bool PopLastIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return false; 
    } 
    else 
    { 
        PopLast(); 
        return true; 
    } 
} 

type GetAndPopLast() 
{ 
    type LastCopy = GetLast(); 
    PopLast(); 
    return LastCopy; 
} 

optional<type> GetAndPopLastIfNotEmpty() 
{ 
    if(Empty()) 
    { 
        return {}; 
    } 
    else 
    { 
        auto* Last = &GetLast(); 
        type LastCopy = *Last; 
        Remove(Last); 
        return LastCopy; 
    } 
}

    };
    
    // TODO: Add memory debugging for singly_linked_list
    template<class type>
        struct singly_linked_list
    {
        struct node
        {
            type Data;
            node* Next;
        };
        
        struct iterator
        {
            node* Node;
            
            iterator& operator++() 
            { 
                Node = Node->Next;
                return *this;
            }
            
            iterator operator++(int)
            {
                auto Res = *this;
                Node = Node->Next;
                return Res;
            }
            
            type& operator*() 
            { return Node->Data; }
            
            type* Ptr()
            { return &Node->Data; }
            
            type* operator->() 
            { return Ptr(); }
            
            rstd_bool operator==(iterator Rhs)
            { return Node == Rhs.Node; }
            
            rstd_bool operator!=(iterator Rhs)
            { return Node != Rhs.Node; }
        };
        
        node* FirstNode;
        node* LastNode;
        arena_ref ArenaRef;
        
        void Clear()
        { FirstNode = LastNode = nullptr; }
        
        void Init
        (arena_ref ArenaRef)
        {	
            rstd_AssertM(!this->ArenaRef, "It's already initialized!");
            this->ArenaRef = ArenaRef;
            Clear();
        }
        
        singly_linked_list(arena_ref ArenaRef) 
        { Init(ArenaRef); }
        
        singly_linked_list()
            :ArenaRef() { Clear(); }
        
        iterator Begin()
        { return {FirstNode}; }
        
        iterator End()
        { return {nullptr}; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        rstd_bool Initialized()
        { return ArenaRef; }
        
        rstd_bool Empty()
        { return !FirstNode; }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return FirstNode->Data;
        }
        
        type& GetLast()
        {
            rstd_Assert(!Empty());
            return LastNode->Data;
        }
        
        u32 GetCount()
        { 
#ifdef rstd_GetCountEnabledInListsWithoutCounter
            u32 Count = 0;
            for(auto& E : *this)
                ++Count;
            return Count;
#else
            static_assert(0,
                          "Use singly_linked_list_with_counter instead of singly_linked_list\n"
                          "to be able to GetCount()\n"
                          "You can also #define rstd_GetCountEnabledInListsWithoutCounter");
#endif
        }
        
        void PushNode
        (node& Node)
        {
            if(LastNode)
                LastNode->Next = &Node;
            LastNode = &Node;
            if(!FirstNode)
                FirstNode = &Node;
        }
        
        void PushNodeFront
        (node& Node)
        {
            Node.Next = FirstNode;
            FirstNode = Node;
            if(!LastNode)
                LastNode = FirstNode;
        }
        
        type& PushZero()
        {
            rstd_Assert(ArenaRef);
            auto* Node = PushStructZero(ArenaRef.Ptr(), node);
            PushNode(Node);
            return Node->Data;
        }
        
        type& PushUninitialized()
        {
            rstd_Assert(ArenaRef);
            auto& Node = rstd_PushStructUninitialized(*ArenaRef, node);
            Node.Next = nullptr;
            PushNode(Node);
            return Node.Data;
        }
        
        type& PushFrontZero()
        {
            rstd_Assert(ArenaRef);
            auto* Node = PushStructZero(ArenaRef.Ptr(), node);
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushFrontUninitialized()
        {
            rstd_Assert(ArenaRef);
            auto* Node = PushStructUninitialized(ArenaRef.Ptr(), node);
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushAfter
        (node* NodeWhichWillBeBeforeThePushedOne)
        {
            rstd_Assert(ArenaRef);
            auto* PushedNode = PushStructUninitialized(ArenaRef.Ptr(), node);
            PushedNode->Next = NodeWhichWillBeBeforeThePushedOne->Next;
            if(!PushedNode->Next)
                LastNode = PushedNode;
            NodeWhichWillBeBeforeThePushedOne->Next = PushedNode;
            return PushedNode->Data;
        }
        
        type& PushAfter
        (node* NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            auto& Data = PushAfter(NodeWhichWillBeBeforeThePushedOne);
            Data = InitialData;
            return Data;
        }
        
        void PopFront()
        {
            rstd_Assert(!Empty());
            Nodes = Nodes->Next;
        }
        
        void Pop()
        {
            rstd_Assert(!Empty());
            if(FirstNode->Next)
            {
                auto* Node = FirstNode;
                while(Node->Next != LastNode)
                    ++Node;
                Node->Next = nullptr;	
                LastNode = Node;
            }
            else
            {
                FirstNode = nullptr;
            }
        }
        
        void Remove
        (type* ElementToRemove)
        {
            node* NodeToRemove = (node*)ElementToRemove;	
            
            if(NodeToRemove == Nodes)
            {
                Nodes = NodeToRemove->Next;
            }
            else
            {
                node* PrevNode = Nodes;
                for(auto* Node = Nodes->Next;; Node = Node->Next)
                {
                    rstd_AssertM(Node, "You're trying to remove thing that is not in this container");
                    if(Node == NodeToRemove)
                    {
                        PrevNode->Next = Node->Next;
                        break;
                    }
                    PrevNode = Node;
                }
            }
        }
        
        // NOTE: returns true if found element to remove
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                    {
                        PrevNode->Next = Node->Next;	
                        if(Node == LastNode)
                            LastNode = PrevNode;
                    }
                    else
                    {
                        FirstNode = Node->Next;
                    }
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                    {
                        PrevNode->Next = Node->Next;	
                        if(Node == LastNode)
                            LastNode = PrevNode;
                    }
                    else
                    {
                        FirstNode = Node->Next;
                    }
                    return {Node->Data};
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return {};
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            node* LastNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Node->Data == E)
                {
                    if(PrevNode)
                    {
                        PrevNode->Next = Node->Next;	
                        if(Node == LastNode)
                            LastNode = PrevNode;
                    }
                    else
                    {
                        FirstNode = Node->Next;
                    }
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        template<class compare_type>               
type* FindEqual                            
(const compare_type& ThingToComare)        
{                                          
    for(auto& Element : *this)         
    {                                      
        if(Element == ThingToComare)               
            return &Element;               
    }                                      
    return nullptr;                        
}                                          

template<class comparison_fn>        
type* Find                           
(comparison_fn Comparison)           
{                                    
    for(auto& Element : *this)       
    {                                
        if(Comparison(Element))      
            return &Element;         
    }                                
    return nullptr;                  
}                                    

template<class comparison_fn> 
auto& FindWithAssert 
(comparison_fn Comparison) 
{ 
    auto* Found = Find(Comparison); 
    rstd_Assert(Found); 
    return *Found; 
} 

template<class compare_type> 
rstd_bool HasEqual 
(const compare_type& ThingToCompare) 
{ return (rstd_bool)FindEqual(ThingToCompare); } 

template<class comparison_fn> 
rstd_bool Has 
(comparison_fn Comparison) 
{ return (rstd_bool)Find(Comparison); } 

template<class compare_type> 
u32 FindIndexOfFirstEqual 
(const compare_type& ThingToCompare) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class comparison_fn> 
u32 FindIndexOfFirst 
(comparison_fn Comparison) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class compare_type> 
u32 HowManyEqualHas 
(const compare_type& ThingToCompare) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            ++Res; 
    } 
    return Res; 
} 

template<class comparison_fn> 
u32 HowManyHas 
(comparison_fn Comparison) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            ++Res; 
    } 
    return Res; 
} 

struct get_value_and_index_result 
{ 
    type* Value; 
    u32 Index; 
    
    operator rstd_bool() 
    { return Value; } 
    
    type* operator->() 
    { 	 
        rstd_Assert(Value); 
        return Value; 
    } 
}; 

template<class comparison_fn> 
get_value_and_index_result FindValueAndIndex 
(comparison_fn Comparison) 
{ 
    get_value_and_index_result Res = {}; 
    for(auto& E : *this) 
    { 
        if(Comparison(E)) 
        { 
            Res.Value = &E; 
            break; 
        } 
        ++Res.Index; 
    } 
    return Res; 
}

        type& Push 
(const type& InitialData) 
{ 
    auto& Data = PushUninitialized(); 
    Data = InitialData; 
    return Data; 
} 

type& PushDefault() 
{ 
    type E; 
    return Push(E); 
} 

type* PushIfNotFull 
(const type& InitialData) 
{ 
    if(!Full()) 
        return &Push(InitialData); 
    return nullptr; 
} 

type* PushIfUnique 
(const type& InitialData) 
{ 
    if(!HasEqual(InitialData)) 
        return &Push(InitialData); 
    return nullptr; 
} 

template<class comparison_fn> 
type* PushIfUnique
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &Push(InitialData); 
    return nullptr; 
} 

type& GetIfExistsOrPushIfUnique 
(const type& ThingToCompareOrInitialData) 
{ 
    if(auto* Found = Find(ThingToCompareOrInitialData)) 
        return *Found; 
    return Push(ThingToCompareOrInitialData); 
} 

/* TODO(now): Do args need to be templated? */ 
template<class... args> 
void Push 
(const type& CurrentPushElement, const args&&... NextPushElements) 
{ 
    Push(CurrentPushElement); 
    Push(NextPushElements...); 
}

        type& PushFront 
(const type& InitialData) 
{ 
    auto& Data = PushFrontUninitialized(); 
    Data = InitialData; 
    return Data; 
}

template<class comparison_fn> 
type* PushFrontIfUnique 
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &PushFront(InitialData); 
    return nullptr; 
} 

    };
    
    // TODO: Add memory debugging for backward_singly_linked_list
    template<class type>
        struct backward_singly_linked_list
    {
        struct node
        {
            type Data;
            node* Next;
        };
        
        struct iterator
        {
            node* Node;
            
            iterator& operator++() 
            { 
                Node = Node->Next;
                return *this;
            }
            
            iterator operator++(int)
            {
                auto Res = *this;
                Node = Node->Next;
                return Res;
            }
            
            type& operator*() 
            { return Node->Data; }
            
            type* Ptr()
            { return &Node->Data; }
            
            type* operator->() 
            { return Ptr(); }
            
            rstd_bool operator==(iterator Rhs)
            { return Node == Rhs.Node; }
            
            rstd_bool operator!=(iterator Rhs)
            { return Node != Rhs.Node; }
        };
        
        node* Nodes;
        arena_ref ArenaRef;
        
        backward_singly_linked_list(arena_ref ArenaRef) 
            :ArenaRef(ArenaRef), Nodes(nullptr) {}
        
        backward_singly_linked_list()
            :ArenaRef(), Nodes(nullptr) {}
        
        iterator Begin()
        { return {Nodes}; }
        
        iterator End()
        { return {nullptr}; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        rstd_bool Initialized()
        { return ArenaRef; }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return Nodes->Data;
        }
        
        type& GetLast()
        {
            rstd_Assert(!Empty());
            node* Node = Nodes;
            while(Node && Node->Next)
                Node = Node->Next;
            return Node->Data;
        }
        
        type& operator[]
        (u32 DesiredIndex)
        {
            rstd_Assert(!Empty());
            
            u32 Index = 0;
            for(auto& E : *this)
            {
                if(Index == DesiredIndex)
                    return E;
                ++Index;
            }
            
            rstd_InvalidCodePathM(Format<string<>>("You tried to get element [%], but this backward_singly_linked_list has only % elements", DesiredIndex, Index));
            return *(type*)(nullptr);
        }
        
        void Clear()
        { Nodes = nullptr; }
        
        rstd_bool Empty()
        { return !Nodes; }
        
        u32 GetCount()
        { 
#ifdef rstd_GetCountEnabledInQuickBackwardListWithoutCounter
            u32 Count = 0;
            for(auto& E : *this)
                ++Count;
            return Count;
#else
            static_assert(0,
                          "Use backward_singly_linked_list_with_counter instead of backward_singly_linked_list\n"
                          "to be able to GetCount()\n"
                          "You can also #define rstd_GetCountEnabledInQuickBackwardListWithoutCounter");
#endif
        }
        
        void PushNode
        (node& Node)
        {
            Node.Next = Nodes;
            Nodes = &Node;
        }
        
        type& PushZero()
        {
            rstd_Assert(ArenaRef);
            auto& Node = rstd_PushStructZero(*ArenaRef, node);
            PushNode(Node);
            return Node.Data;
        }
        
        type& PushUninitialized()
        {
            rstd_Assert(ArenaRef);
            auto& Node = rstd_PushStructUninitialized(*ArenaRef, node);
            PushNode(Node);
            return Node.Data;
        }
        
        type& PushAfter
        (node& NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            rstd_Assert(ArenaRef);
            auto& PushedNode = rstd_PushStructUninitialized(*ArenaRef, node);
            PushedNode.Next = NodeWhichWillBeBeforeThePushedOne.Next;
            NodeWhichWillBeBeforeThePushedOne.Next = &PushedNode;
            PushedNode.Data = InitialData;
            return PushedNode.Data;
        }
        
        void Pop()
        {
            rstd_Assert(!Empty());
            Nodes = Nodes->Next; 
        }
        
        node* PopLastNode()
        {
            rstd_Assert(!Empty());
            node* Node = Nodes;
            while(Node && Node->Next->Next)
                Node = Node->Next;
            auto* NodeBeingPopped = Node->Next;
            Node->Next = nullptr;
            return NodeBeingPopped;
        }
        
        void PopLast()
        { PopLastNode(); }
        
        type* PopLastReturnPtr()
        { return &PopLastNode()->Data; }
        
        void Remove
        (type* ElementToRemove)
        {
            node* NodeToRemove = (node*)ElementToRemove;	
            
            if(NodeToRemove == Nodes)
            {
                Nodes = NodeToRemove->Next;
            }
            else
            {
                node* PrevNode = Nodes;
                for(auto* Node = Nodes->Next;; Node = Node->Next)
                {
                    rstd_AssertM(Node, "You're trying to remove thing that is not in this container");
                    if(Node == NodeToRemove)
                    {
                        PrevNode->Next = Node->Next;
                        break;
                    }
                    PrevNode = Node;
                }
            }
        }
        
        // NOTE: returns true if found element to remove
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;	
                    else
                        Nodes = Node->Next;
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        template<class comparison_fn>
            void RemoveFirstIfWithAssert
        (comparison_fn Comparison)
        {
            rstd_bool ManagedToRemove = RemoveFirstIf(Comparison);	
            rstd_AssertM(ManagedToRemove, "Element which meets the condition of removal wasn't found!");
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;	
                    else
                        Nodes = Node->Next;
                    return {Node->Data};
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return {};
        }
        
        template<class comparison_fn>
            type RemoveFirstIfWithAssertReturnCopy
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;	
                    else
                        Nodes = Node->Next;
                    return Node->Data;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            rstd_InvalidCodePathM("Element which meets the condition of removal wasn't found!");
            return {};
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Node->Data == E)
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;	
                    else
                        Nodes = Node->Next;
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        template<class compare_type>               
type* FindEqual                            
(const compare_type& ThingToComare)        
{                                          
    for(auto& Element : *this)         
    {                                      
        if(Element == ThingToComare)               
            return &Element;               
    }                                      
    return nullptr;                        
}                                          

template<class comparison_fn>        
type* Find                           
(comparison_fn Comparison)           
{                                    
    for(auto& Element : *this)       
    {                                
        if(Comparison(Element))      
            return &Element;         
    }                                
    return nullptr;                  
}                                    

template<class comparison_fn> 
auto& FindWithAssert 
(comparison_fn Comparison) 
{ 
    auto* Found = Find(Comparison); 
    rstd_Assert(Found); 
    return *Found; 
} 

template<class compare_type> 
rstd_bool HasEqual 
(const compare_type& ThingToCompare) 
{ return (rstd_bool)FindEqual(ThingToCompare); } 

template<class comparison_fn> 
rstd_bool Has 
(comparison_fn Comparison) 
{ return (rstd_bool)Find(Comparison); } 

template<class compare_type> 
u32 FindIndexOfFirstEqual 
(const compare_type& ThingToCompare) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class comparison_fn> 
u32 FindIndexOfFirst 
(comparison_fn Comparison) 
{ 
    u32 ElementIndex = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            return ElementIndex; 
        ++ElementIndex; 
    } 
    return InvalidU32; 
} 

template<class compare_type> 
u32 HowManyEqualHas 
(const compare_type& ThingToCompare) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Element == ThingToCompare) 
            ++Res; 
    } 
    return Res; 
} 

template<class comparison_fn> 
u32 HowManyHas 
(comparison_fn Comparison) 
{ 
    u32 Res = 0; 
    for(auto& Element : *this) 
    { 
        if(Comparison(Element)) 
            ++Res; 
    } 
    return Res; 
} 

struct get_value_and_index_result 
{ 
    type* Value; 
    u32 Index; 
    
    operator rstd_bool() 
    { return Value; } 
    
    type* operator->() 
    { 	 
        rstd_Assert(Value); 
        return Value; 
    } 
}; 

template<class comparison_fn> 
get_value_and_index_result FindValueAndIndex 
(comparison_fn Comparison) 
{ 
    get_value_and_index_result Res = {}; 
    for(auto& E : *this) 
    { 
        if(Comparison(E)) 
        { 
            Res.Value = &E; 
            break; 
        } 
        ++Res.Index; 
    } 
    return Res; 
}

        type& Push 
(const type& InitialData) 
{ 
    auto& Data = PushUninitialized(); 
    Data = InitialData; 
    return Data; 
} 

type& PushDefault() 
{ 
    type E; 
    return Push(E); 
} 

type* PushIfNotFull 
(const type& InitialData) 
{ 
    if(!Full()) 
        return &Push(InitialData); 
    return nullptr; 
} 

type* PushIfUnique 
(const type& InitialData) 
{ 
    if(!HasEqual(InitialData)) 
        return &Push(InitialData); 
    return nullptr; 
} 

template<class comparison_fn> 
type* PushIfUnique
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &Push(InitialData); 
    return nullptr; 
} 

type& GetIfExistsOrPushIfUnique 
(const type& ThingToCompareOrInitialData) 
{ 
    if(auto* Found = Find(ThingToCompareOrInitialData)) 
        return *Found; 
    return Push(ThingToCompareOrInitialData); 
} 

/* TODO(now): Do args need to be templated? */ 
template<class... args> 
void Push 
(const type& CurrentPushElement, const args&&... NextPushElements) 
{ 
    Push(CurrentPushElement); 
    Push(NextPushElements...); 
}

        type& PushFront 
(const type& InitialData) 
{ 
    auto& Data = PushFrontUninitialized(); 
    Data = InitialData; 
    return Data; 
}

template<class comparison_fn> 
type* PushFrontIfUnique 
(const type& InitialData, comparison_fn Comparison) 
{ 
    if(!Has(Comparison)) 
        return &PushFront(InitialData); 
    return nullptr; 
} 

    };
    
    template<class type>
        struct doubly_linked_list_with_counter : doubly_linked_list<type>
    {
        doubly_linked_list_with_counter(arena_ref ArenaRef)
            :doubly_linked_list(ArenaRef), Count(0) {}
        
        doubly_linked_list_with_counter()
            :doubly_linked_list(), Count(0) {}
        
        u32 Count;
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        {
            doubly_linked_list::Clear();
            Count = 0;
        }
        
        void PushNode
        (node* Node)
        {
            ++Count;
            doubly_linked_list::PushNode(Node);
        }
        
        void PushNodeFront
        (node* Node)
        {
            ++Count;
            doubly_linked_list::PushNodeFront(Node);
        }
        
        void PopFreeNode
        (node* Node)
        {
            --Count;
            doubly_linked_list::PopFreeNode(Node);
        }
        
        type& PushZero()
        {
            ++Count;
            return doubly_linked_list::PushZero();
        }
        
        type& PushUninitialized()
        {
            ++Count;
            return doubly_linked_list::PushUninitialized();
        }
        
        type& Push
        (const type& InitialData)
        {
            ++Count;
            return doubly_linked_list::Push(InitialData);
        }
        
        type& PushFrontZero()
        {
            ++Count;
            return doubly_linked_list::PushFrontZero();
        }
        
        type& PushFrontUninitialized()
        {
            ++Count;
            return doubly_linked_list::PushFrontUninitialized();
        }
        
        type& PushFront
        (const type& InitialData)
        {
            ++Count;
            return doubly_linked_list::PushFront(InitialData);
        }
        
        type* PushIfUnique
        (const type& InitialData)
        {
            ++Count;
            return doubly_linked_list::PushIfUnique(InitialData);
        }
        
        type& GetIfExistsOrPushIfUnique
        (const type& E)
        {
            auto* Found = Find(*this, E);
            if(Found)
                return *Found;
            return Push(E);
        }
        
        void Remove
        (node* Node)
        {
            doubly_linked_list::Remove(Node);
            --Count;
        }
        
        iterator Remove
        (iterator It)
        {
            iterator Res = doubly_linked_list::Remove(It);
            --Count;
            return Res;
        }
        
        void Remove
        (type& ElementToRemove)
        {
            --Count;
            doubly_linked_list::Remove(ElementToRemove);
        }
        
        void PopFirst()
        {
            doubly_linked_list::PopFirst();
            --Count;
        }
        
        rstd_bool PopFirstIfNotEmpty()
        {
            rstd_bool Res = Empty();
            if(Res)
            {
                PopFirst();
                --Count;
            }
            return Res;
        }
        
        type GetAndPopFirst()
        {
            --Count;
            return doubly_linked_list::GetAndPopFirst();
        }
        
        optional<type> GetAndPopFirstIfNotEmpty()
        {
            if(Empty())
            {
                return {};
            }
            else
            {
                auto* First = &GetFirst();
                type FirstCopy = *First;
                Remove(First);
                return FirstCopy;
            }
        }
        
        void PopLast()
        {
            doubly_linked_list::PopLast();
            --Count;
        }
        
        rstd_bool PopLastIfNotEmpty()
        {
            rstd_bool Res = Empty();
            if(Res)
            {
                PopLast();
                --Count;
            }
            return Res;
        }
        
        type GetAndPopLast()
        {
            --Count;
            return doubly_linked_list::GetAndPopLast();
        }
        
        optional<type> GetAndPopLastIfNotEmpty()
        {
            if(Empty())
            {
                return {};
            }
            else
            {
                auto* Last = &GetLast();
                type LastCopy = *Last;
                Remove(Last);
                return LastCopy;
            }
        }
        
        // NOTE: returns true if found element to remove
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            if(auto* Found = Find(Comparison))
            {
                Remove(*Found);
                return true;
            }
            return false;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            if(auto* Found = Find(Comparison))
            {
                Remove(*Found);
                return {*Found};
            }
            return {};
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            if(auto* Found = FindEqual(E))
            {
                Remove(*Found);
                return true;
            }
            return false;
        }
    };
    
    template<class type>
        struct singly_linked_list_with_counter : singly_linked_list<type>
    {
        singly_linked_list_with_counter(arena_ref ArenaRef)
            :singly_linked_list(ArenaRef), Count(0) {}
        
        singly_linked_list_with_counter()
            :singly_linked_list(), Count(0) {}
        
        u32 Count;
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        {
            singly_linked_list::Clear();
            Count = 0;
        }
        
        void PushNodeFront(node* Node)
        {
            ++Count;
            singly_linked_list::PushNodeFront(Node);
        }
        
        type& PushFrontZero()
        {
            ++Count;
            return singly_linked_list::PushFrontZero();
        }
        
        type& PushFrontUninitialized()
        {
            ++Count;
            return singly_linked_list::PushFrontUninitialized();
        }
        
        type& PushFront
        (const type& InitialData)
        {
            ++Count;
            return singly_linked_list::PushFront(InitialData);
        }
        
        void PushNode(node* Node)
        {
            ++Count;
            singly_linked_list::PushNode(Node);
        }
        
        type& PushZero()
        {
            ++Count;
            return singly_linked_list::PushZero();
        }
        
        type& PushUninitialized()
        {
            ++Count;
            return singly_linked_list::PushUninitialized();
        }
        
        type& Push
        (const type& InitialData)
        {
            ++Count;
            return singly_linked_list::Push(InitialData);
        }
        
        type& PushAfter
        (node* NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            ++Count;
            return singly_linked_list::PushAfter(NodeWhichWillBeBeforeThePushedOne, InitialData);
        }
        
        type* PushIfUnique
        (const type& InitialData)
        {
            ++Count;
            return singly_linked_list::PushIfUnique(InitialData);
        }
        
        type& GetIfExistsOrPushIfUnique
        (const type& E)
        {
            auto* Found = Find(this, E);
            if(Found)
                return *Found;
            return Push(E);
        }
        
        void PopFront()
        {
            --Count;
            singly_linked_list::PopFront();
        }
        
        void Pop()
        {
            --Count;
            singly_linked_list::Pop();
        }
        
        void Remove
        (type& ElementToRemove)
        {
            --Count;
            singly_linked_list::Remove(ElementToRemove);
        }
        
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            rstd_bool ManagedToRemove = singly_linked_list::RemoveFirstIf(Comparison);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            auto Res = singly_linked_list::RemoveFirstIfReturnCopy(Comparison);
            if(Res)
                --Count;
            return Res;
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            rstd_bool ManagedToRemove = singly_linked_list::RemoveFirstEqualTo(E);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
    };
    
    template<class type>
        struct backward_singly_linked_list_with_counter : backward_singly_linked_list<type>
    {
        backward_singly_linked_list_with_counter(arena_ref ArenaRef)
            :backward_singly_linked_list(ArenaRef), Count(0) {}
        
        backward_singly_linked_list_with_counter()
            :backward_singly_linked_list(), Count(0) {}
        
        u32 Count;
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        {
            backward_singly_linked_list::Clear();
            Count = 0;
        }
        
        void PushNode
        (node& Node)
        {
            ++Count;
            backward_singly_linked_list::PushNode(Node);
        }
        
        type& PushZero()
        {
            ++Count;
            return backward_singly_linked_list::PushZero();
        }
        
        type& PushUninitialized()
        {
            ++Count;
            return backward_singly_linked_list::PushUninitialized();
        }
        
        type& Push
        (const type& InitialData)
        {
            ++Count;
            return backward_singly_linked_list::Push(InitialData);
        }
        
        type& PushAfter
        (node& NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            ++Count;
            return backward_singly_linked_list::PushAfter(NodeWhichWillBeBeforeThePushedOne, InitialData);
        }
        
        type* PushIfUnique
        (const type& InitialData)
        {
            ++Count;
            return backward_singly_linked_list::PushIfUnique(InitialData);
        }
        
        type& GetIfExistsOrPushIfUnique
        (const type& E)
        {
            auto* Found = Find(*this, E);
            if(Found)
                return *Found;
            return Push(E);
        }
        
        void Pop()
        {
            --Count;
            backward_singly_linked_list::Pop();
        }
        
        void PopLast()
        {
            --Count;
            backward_singly_linked_list::PopLast();
        }
        
        node* PopLastNode()
        {
            --Count;
            return backward_singly_linked_list::PopLastNode();
        }
        
        type* PopLastReturnPtr()
        {
            --Count;
            return backward_singly_linked_list::PopLastReturnPtr()
        }
        
        void Remove
        (type* ElementToRemove)
        {
            --Count;
            backward_singly_linked_list::Remove(ElementToRemove);
        }
        
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            rstd_bool ManagedToRemove = backward_singly_linked_list::RemoveFirstIf(Comparison);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
        
        template<class comparison_fn>
            void RemoveFirstIfWithAssert
        (comparison_fn Comparison)
        {
            backward_singly_linked_list::RemoveFirstIfWithAssert(Comparison);	
            --Count;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            auto Res = backward_singly_linked_list::RemoveFirstIfReturnCopy(Comparison);
            if(Res)
                --Count;
            return Res;
        }
        
        template<class comparison_fn>
            type RemoveFirstIfWithAssertReturnCopy
        (comparison_fn Comparison)
        {
            auto Res = backward_singly_linked_list::RemoveFirstIfWithAssertReturnCopy(Comparison);
            --Count;
            return Res;
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            rstd_bool ManagedToRemove = backward_singly_linked_list::RemoveFirstEqualTo(E);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
    };
    
    
    /////////////////////
    // MULTI-THREADING //
    /////////////////////
    
#ifdef _WIN32
    static void WriteFence()
    {
#if rstd_MultiThreadingEnabled
        _WriteBarrier(); _mm_sfence();
#endif
    }
    
    static void ReadFence()
    {
#if rstd_MultiThreadingEnabled
        _ReadBarrier();
#endif
    }
    
    static void ReadWriteFence()
    {
#if rstd_MultiThreadingEnabled
        _ReadWriteBarrier();
#endif
    }
    
    u32 AtomicIncrement(volatile u32& A);
    u32 AtomicDecrement(volatile u32& A);
    
    i32 AtomicSetValueAndGetValueThatReallyIsInDestination
    (volatile i32 *Destination, i32 NewValue, i32 ValueThatShouldBeInDestination);
    
    i64 AtomicSetValueAndGetValueThatReallyIsInDestination
    (volatile i64 *Destination, i64 NewValue, i64 ValueThatShouldBeInDestination);
    
    static i32 AtomicSetValueAndGetValueThatReallyIsInDestination(volatile i32 *Destination, i32 NewValue)
    { return AtomicSetValueAndGetValueThatReallyIsInDestination(Destination, NewValue, *Destination); }
    
    // TODO: AtomicSetValue should be probably InterlockedExchange, Call InterlockedCompareExchange something else
    static rstd_bool AtomicSetValue
    (volatile i32 *Destination, i32 NewValue, i32 ValueThatShouldBeInDestination)
    {
#if rstd_MultiThreadingEnabled
        i32 ValueThatReallyIsInDestination = AtomicSetValueAndGetValueThatReallyIsInDestination(Destination, NewValue, ValueThatShouldBeInDestination);
        rstd_bool NewValueWasSet = ValueThatReallyIsInDestination == ValueThatShouldBeInDestination;
        return NewValueWasSet;
#endif
    }
    
    // TODO: Do cast with assetion or use different intrinsic
    static rstd_bool AtomicSetValue(volatile u32 *Destination, u32 NewValue, u32 ValueThatShouldBeInDestination)
    { return AtomicSetValue((i32*)Destination, (i32)NewValue, (i32)ValueThatShouldBeInDestination); }
    
    static rstd_bool AtomicSetValue(volatile u32 *Destination, u32 NewValue)
    { return AtomicSetValue(Destination, NewValue, *Destination); }
    
    static i64 AtomicSetValueAndGetValueThatReallyIsInDestination(volatile i64 *Destination, i64 NewValue)
    { return AtomicSetValueAndGetValueThatReallyIsInDestination(Destination, NewValue, *Destination); }
    
    static rstd_bool AtomicSetValue
    (volatile i64 *Destination, i64 NewValue, i64 ValueThatShouldBeInDestination)
    {
#if rstd_MultiThreadingEnabled
        i64 ValueThatReallyIsInDestination = AtomicSetValueAndGetValueThatReallyIsInDestination(Destination, NewValue, ValueThatShouldBeInDestination);
        rstd_bool NewValueWasSet = ValueThatReallyIsInDestination == ValueThatShouldBeInDestination;
        return NewValueWasSet;
#else
        return true;
#endif
    }
    
    // TODO: Do cast with assetion or use different intrinsic
    static rstd_bool AtomicSetValue(volatile u64 *Destination, u64 NewValue, u64 ValueThatShouldBeInDestination)
    { return AtomicSetValue((i64*)Destination, (i64)NewValue, (i64)ValueThatShouldBeInDestination); }
    
    static rstd_bool AtomicSetValue(volatile u64 *Destination, u64 NewValue)
    { return AtomicSetValue(Destination, NewValue, *Destination); }
    
    static u32 GetThreadID()
    {
        u8 *ThreadLocalStorage = (u8 *)__readgsqword(0x30);
        return *(u32*)(ThreadLocalStorage + 0x48);
    }
    
#endif
    
    struct mutex
    { volatile i32 Locked = 0; };
    
    static void Lock
    (volatile i32& Locked)
    {
#if rstd_MultiThreadingEnabled
        while(Locked || !AtomicSetValue(&Locked, 1, 0));
#endif
    }
    
    static void Lock(mutex& Mutex)
    { Lock(Mutex.Locked); }
    
    static rstd_bool TryLock
    (volatile i32& Locked)
    {
#if rstd_MultiThreadingEnabled
        return !Locked && AtomicSetValue(&Locked, 1, 0);
#else
        return true;
#endif
    }
    
    static rstd_bool TryLock(mutex& Mutex)
    { return TryLock(Mutex.Locked); }
    
    static void Unlock
    (volatile i32& Locked)
    {
#if rstd_MultiThreadingEnabled
        Locked = 0;
#endif
    }
    
    static void Unlock(mutex& Mutex)
    { Unlock(Mutex.Locked); }
    
#define ScopeLock(_Mutex) \
Lock(_Mutex); \
rstd_defer(Unlock(_Mutex)); \
    
    struct thread_pool;
    
    typedef void thread_pool_job_callback(void* Data);
    
    struct thread_pool_job
    {
        void* CallbackUserData;
        thread_pool_job_callback* Callback;
    };
    
    struct thread_pool_job_node : thread_pool_job
    { thread_pool_job* Next; };
    
    struct thread_pool_job_list
    {
        thread_pool_job_node* NextJobToTake;
        thread_pool_job_node* LastJobToTake;
        thread_pool_job_node* JobFreeList;
        arena Arena;
        mutex Mutex;
    };
    
    struct thread_pool
    {
        thread_pool_job_list JobList;
        void* SemaphoreHandle;
        u32 ThreadCount;
    };
    
    void Init(thread_pool& Pool, u32 ThreadCount, arena ArenaResponsibleOnlyForAllocatingJobs);
    void PushJob(thread_pool&, void* JobUserData, thread_pool_job_callback* JobCallback);
    template<class job_container> void PushJobs(thread_pool& Pool, job_container Jobs);
    
    ///////////
    // FILES // 
    ///////////
    constexpr char FilePathSlash = '\\';
    
    struct file_info // TODO: What else should I put in file_info?
    { char* Name; };
    
    struct file
    {
        void* PlatformFileHandle;
        
        operator rstd_bool()
        { return PlatformFileHandle; }
    };
    
    struct file_stream
    {
        file File;
        u64 Pos;
        
        operator rstd_bool()
        { return File; }
    };
    
    enum class io_mode
    {
        Read,
        Write,
        ReadWrite,
    };
    
    enum class file_error
    {
        NoError,
        UnknownError,
        FileIsMissing,
        DirectoryOnPathIsMissing,
        AccessToFileWasDenied,
    };
    
    template<class string> backward_singly_linked_list_with_counter<file_info> GetFileInfos(arena& Arena, string Path);
    template<class string> i32 RemoveFile(string& FilePath);
    i32 RenameFile(char* FilePath, char* NewFilePath);
    rstd_bool FileExists(char* Path);
    file OpenFile(char* Path, io_mode);
    rstd_bool Close(file&);
    u32 Write(file File, u64 Pos, void* Data, u32 Size);
    u32 Read(void* Dest, file File, u64 Pos, u32 Size);
    rstd_bool SetFileSize(file File, u32 Size);
    u32 GetFileSize(file File);
    char* ReadWholeFile(arena* Arena, file File);
    rstd_bool CreateDirectory(char* Path);
    rstd_bool CreateDirectory(wchar_t* Path);
    rstd_bool DeleteDirectory(char* Path);
    rstd_bool DeleteDirectory(wchar_t* Path);
    rstd_bool DeleteDirectoryWithAllContents(char* Path);
    
    static void CreateFile
    (char* FilePath)
    {
        auto File = OpenFile(FilePath, io_mode::Write);
        Close(File);
    }
    
    static char* GetRelativePath
    (char* FilePath, char* RootDirectory)
    {
        char* C = FilePath;
        while(*C)
        {
            if(*C == '\\')
            {
                ++C;
                // TODO: This could be faster if you didn't use StringsMatch
                //       You can create AdvanceIfStringDontMatch
                if(StringsMatch(C, RootDirectory))
                    return C;
            }
            ++C;
        }
        return nullptr;
    }
    
    static file_stream OpenFileStream
    (char* FilePath, io_mode Mode)
    {
        auto File = OpenFile(FilePath, Mode);
        if(Mode == io_mode::Write)
            SetFileSize(File, 0);
        return {File};
    }
    
    static rstd_bool Close(file_stream& Stream)
    { return Close(Stream.File); }
    
    template<class type> static u32 WriteStruct(file File, u64 Pos, type& Data, u32 Count = 1)
    { return Write(File, Pos, (void*)&Data, (u32)sizeof(type) * Count); }
    
    // TODO: Should I translate \n to \r\n here on windows???
    template<class... args> static u32 WriteString
    (file File, u64 Pos, const char* Fmt, args... Args)
    {
        auto String = Format<string<1024>>(Fmt, Args...);
        return Write(File, Pos, (void*)(char*)String, String.GetCount());
    }
    
    template<class type> static type Read
    (file File, u64 Pos)
    {
        type Res;
        Read((void*)&Res, File, Pos, (u32)sizeof(Res));
        return Res;
    }
    
    static u32 Write
    (file_stream& Stream, void* Data, u32 Size)
    {
        u32 WrittenBytes = Write(Stream.File, Stream.Pos, Data, Size);
        rstd_Assert(WrittenBytes == Size);
        Stream.Pos += Size;
        return WrittenBytes;
    }
    
    template<class type> static u32 WriteStruct(file_stream& Stream, type& Data, u32 Count = 1)
    { return Write(Stream, &Data, (u32)sizeof(type) * Count); }
    
    template<class... args> static u32 WriteString
    (file_stream& Stream, const char* Fmt, args... Args)
    {
        u32 WrittenBytes = WriteString(Stream.File, Stream.Pos, Fmt, Args...);
        Stream.Pos += WrittenBytes;
        return WrittenBytes;
    }
    
    static u32 Read
    (void* Dest, file_stream& Stream, u32 Size)
    {
        u32 ReadBytes = Read(Dest, Stream.File, Stream.Pos, Size);
        rstd_Assert(ReadBytes == Size); // TODO: Should this assertion be here? Maybe there should be second function which just returns failure?
        Stream.Pos += Size;
        return ReadBytes;
    }
    
    template<class type> static type Read
    (file_stream& Stream)
    {
        auto Res = Read<type>(Stream.File, Stream.Pos);
        Stream.Pos += sizeof(Res);
        return Res;
    }
    
    template<class type> static type& GetAndAdvance
    (char** DataPtr)
    {
        auto& Res = *(type*)(*DataPtr);
        *DataPtr += sizeof(type);
        return Res;
    }
}

#ifdef rstd_Implementation

#ifdef _WIN32

#pragma comment(lib, "shell32.lib")

#ifndef rstd_ExcludeDebugPrintingFunctions
	#pragma comment(lib, "user32.lib")
#endif

#include <windows.h>

namespace rstd
{
#ifndef rstd_ExcludeDebugPrintingFunctions
    ////////////////////
    // DEBUG PRINTING //
    ////////////////////
    u32 GetSystemErrorCode()
    { return GetLastError(); }
    
    void InternalPrintInDebugger(const char* Message)
    { OutputDebugStringA(Message); }
    
    void InternalWarningMessageBox(const char* Message)
    { MessageBoxA(0, Message, "Warning!", MB_ICONWARNING|MB_OK); }
    
    void ShowErrorMessageBox(const char* Message)
    { MessageBoxA(0, Message, "Error!", MB_ICONERROR|MB_OK); }
    
    void ShowErrorMessageBoxAndExitProcess
    (const char* Message)
    {
        ShowErrorMessageBox(Message);
        ExitProcess(0);
    }
#endif
    
    ///////////////////////
    // MEMORY ALLOCATION //
    ///////////////////////
    void* PageAlloc(size Bytes)
    { return VirtualAlloc(nullptr, Bytes, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); }
    
    void PageFree(void* Memory)
    { VirtualFree(Memory, 0, MEM_RELEASE); }
    
#ifdef rstd_ThreadPoolLogging
#if rstd_ThreadPoolLogging
#define ThreadPoolLog(_Str) InternalPrintInDebugger(_Str)
#else
#define ThreadPoolLog(_Str)
#endif
#else
#define ThreadPoolLog(_Str)
#endif
    
    /////////////////////
    // MULTI-THREADING //
    /////////////////////
    u32 AtomicIncrement
    (volatile u32& A)
    {
#if rstd_MultiThreadingEnabled
        return InterlockedIncrement(&A);
#endif
    }
    
    u32 AtomicDecrement
    (volatile u32& A)
    {
#if rstd_MultiThreadingEnabled
        return InterlockedDecrement(&A);
#endif
    }
    
    i32 AtomicSetValueAndGetValueThatReallyIsInDestination
    (volatile i32 *Destination, i32 NewValue, i32 ValueThatShouldBeInDestination)
    {
#if rstd_MultiThreadingEnabled
        i32 ValueThatReallyIsInDestination = InterlockedCompareExchange((volatile LONG*)Destination,
                                                                        NewValue, ValueThatShouldBeInDestination);
        return ValueThatReallyIsInDestination;
#endif
    }
    
    i64 AtomicSetValueAndGetValueThatReallyIsInDestination
    (volatile i64 *Destination, i64 NewValue, i64 ValueThatShouldBeInDestination)
    {
#if rstd_MultiThreadingEnabled
        i64 ValueThatReallyIsInDestination = InterlockedCompareExchange64(Destination, NewValue, ValueThatShouldBeInDestination);
        return ValueThatReallyIsInDestination;
#else
        *Destination = NewValue;
        return *Destination;
#endif
    }
    
    
#if rstd_MultiThreadingEnabled
    DWORD WINAPI ThreadProc
    (LPVOID ThreadPoolVoidPtr)
    {
#if rstd_ThreadPoolLogging
        u32 ThreadId = GetThreadID();
#endif
        ThreadPoolLog(Format<string<>>("Thread % starts\n", ThreadId));
        
        thread_pool& ThreadPool = *(thread_pool*)ThreadPoolVoidPtr;
        auto& List = ThreadPool.JobList;
        for(;;)
        {
            ThreadPoolLog(Format<string<>>("Thread % at the begining of loop\n", ThreadId));
            while(List.NextJobToTake)
            {
                if(TryLock(List.Mutex))
                {
                    ThreadPoolLog(Format<string<>>("Mutex is locked by thread: %\n", ThreadId));
                    ThreadPoolLog(Format<string<>>("Thread % took the job\n", ThreadId));
                    
                    auto* CurrentJob = List.NextJobToTake;
                    if(List.NextJobToTake == List.LastJobToTake)
                        List.NextJobToTake = List.LastJobToTake = nullptr;
                    else
                        List.NextJobToTake = List.LastJobToTake;
                    
                    Unlock(List.Mutex);
                    ThreadPoolLog(Format<string<>>("Mutex is unlocked by thread: %\n", ThreadId));
                    
                    ThreadPoolLog(Format<string<>>("Thread % About to call Callback\n", ThreadId));
                    CurrentJob->Callback(CurrentJob->CallbackUserData);
                }
            }
            ThreadPoolLog(Format<string<>>("Thread % going to sleep\n", ThreadId));
            WaitForSingleObjectEx(ThreadPool.SemaphoreHandle, INFINITE, FALSE);
            ThreadPoolLog(Format<string<>>("Thread % awakes\n", ThreadId));
        }
    }
#endif
    
    void Init
    (thread_pool& Pool, u32 ThreadCount, arena Arena)
    {
#if rstd_MultiThreadingEnabled
        Pool = {};
        
        Pool.JobList.Arena = Arena;
        Pool.SemaphoreHandle = CreateSemaphore(0, 0, ThreadCount, nullptr);
        Pool.ThreadCount = ThreadCount;
        
        for(u32 ThreadIndex = 0; ThreadIndex < ThreadCount; ++ThreadIndex)
        {
            DWORD ThreadId;
            auto ThreadHandle = CreateThread(0, 0, ThreadProc, &Pool, 0, &ThreadId);
            CloseHandle(ThreadHandle); // TODO: Support changing number of threads in runtime?
        }
#endif
    }
    
    void PushJob
    (thread_pool& Pool, void* JobUserData, thread_pool_job_callback* JobCallback)
    {
#if rstd_MultiThreadingEnabled
        auto& List = Pool.JobList;
        auto* JobNode = &rstd_PushStructZero(List.Arena, thread_pool_job_node);
        JobNode->CallbackUserData = JobUserData;
        JobNode->Callback = JobCallback;
        
        Lock(List.Mutex);
        ThreadPoolLog("Mutex is locked by PushJob\n");
        if(List.NextJobToTake)
        {
            List.LastJobToTake->Next = JobNode;
            List.LastJobToTake = JobNode;
        }
        else
        {
            List.NextJobToTake = List.LastJobToTake = JobNode;
        }
        Unlock(List.Mutex);
        ThreadPoolLog("Mutex is unlocked by PushJob\n");
        
        ReleaseSemaphore(Pool.SemaphoreHandle, 1, 0);
        ThreadPoolLog("Thread should be awoken now\n");
#else
        JobCallback(JobUserData);
#endif
    }
    
    template<class job_container>
        void PushJobs
    (thread_pool& Pool, job_container Jobs)
    {
#if rstd_MultiThreadingEnabled
        auto& List = Pool.JobList;
        
        u32 PushedJobCount = Jobs.GetCount();
        
        Lock(List.Mutex);
        ThreadPoolLog("Mutex is locked by PushJob\n");
        
        if(!List.NextJobToTake)
        {
            auto LastJob = Jobs.GetAndPopLast();
            auto* JobNode = rstd_PushStruct(&List.Arena, thread_pool_job_node{LastJob});
            List.NextJobToTake = List.LastJobToTake = JobNode;
        }
        
        for(auto Job : Jobs)
        {
            auto* JobNode = rstd_PushStruct(&List.Arena, thread_pool_job_node{Job});
            List.LastJobToTake->Next = JobNode;
            List.LastJobToTake = JobNode;
        }
        
        Unlock(List.Mutex);
        ThreadPoolLog("Mutex is unlocked by PushJob\n");
        
        u32 ThreadsToAwakeCount = PushedJobCount > Pool.ThreadCount ? Pool.ThreadCount : PushedJobCount;
        LONG WorkingThreadCount = 0;
        ThreadPoolLog(Format<string<>>("ReleaseSemaphore 1 - ThreadsToAwakeCount: %\n", ThreadsToAwakeCount));
        if(!ReleaseSemaphore(Pool.SemaphoreHandle, ThreadsToAwakeCount, &WorkingThreadCount))
        {
            ThreadsToAwakeCount = Pool.ThreadCount - WorkingThreadCount;
            ThreadPoolLog(Format<string<>>("ReleaseSemaphore 2 - ThreadsToAwakeCount: %, WorkingThreadCount: %\n", ThreadsToAwakeCount, WorkingThreadCount));
            ReleaseSemaphore(Pool.SemaphoreHandle, ThreadsToAwakeCount, nullptr);
        }
#else
        for(auto Job : Jobs)
            Job.Callback(Job.CallbackUserData);
#endif
    }
    
    /*
    TODO:
    fn CompleteAllJobs
    (thread_pool& Pool)
    {
        // Main thread should also take jobs
    }
    */
    
    ///////////
    // FILES //
    ///////////
#if rstd_FileDebugEnabled
    
    struct file_debug
    {
        struct open_file
        {
            string<> FilePath;
            void* PlatformFileHandle;
        };
        
        backward_singly_linked_list_with_counter<open_file> OpenFiles;
        mutex Mutex;
        
        file_debug()
        { OpenFiles = {OwnArena(rstd_AllocateArenaZero(1_MB))}; }
        
        void OnOpenFile
        (char* FilePath, void* PlatformFileHandle)
        {
            ScopeLock(Mutex);
            OpenFiles.Push({FilePath, PlatformFileHandle});
        }
        
        void OnCloseFile
        (void* PlatformFileHandle)
        {
            ScopeLock(Mutex);
            OpenFiles.RemoveFirstIfWithAssert(
                                              [=](auto& File){ return File.PlatformFileHandle == PlatformFileHandle; });
        }
    };
    
#else
    
    struct file_debug
    {
        void OnOpenFile(char* FilePath, void* PlatformFileHandle) {}
        void OnCloseFile(void* PlatformFileHandle) {}
    };
    
#endif
    
    static file_debug FileDebug;
    
    
    file_error GetLastFileError()
    {
        auto Error = GetLastError();
        switch(Error)
        {
            case ERROR_SUCCESS: return file_error::NoError;
            case ERROR_FILE_NOT_FOUND: return file_error::FileIsMissing;
            case ERROR_PATH_NOT_FOUND: return file_error::DirectoryOnPathIsMissing;
            case ERROR_INVALID_HANDLE: return file_error::DirectoryOnPathIsMissing; // TODO: Can't ERROR_INVALID_HANDLE be produced in other cases?
            case ERROR_ACCESS_DENIED: return file_error::AccessToFileWasDenied;
            default: return file_error::UnknownError;
        }
    }
    
    template<class string> backward_singly_linked_list_with_counter<file_info> GetFileInfos
    (arena& Arena, string Path)
    {
        backward_singly_linked_list_with_counter<file_info> FileInfos = ShareArena(Arena);
        
        // TODO: Support unicode file names
        
        Path += "/*";
        
        WIN32_FIND_DATA FindData;
        HANDLE FindHandle = FindFirstFileA(Path, &FindData); // name of first found thing is "." TODO: Make sure it's true
        if(FindHandle != INVALID_HANDLE_VALUE)
        {
            FindNextFileA(FindHandle, &FindData); // name of second found thing is ".." TODO: Make sure it's true
            
            while(FindNextFileA(FindHandle, &FindData))
            {
                file_info FileInfo;
                FileInfo.Name = rstd_PushStringCopy(Arena, FindData.cFileName);
                FileInfos.Push(FileInfo);
            }
            FindClose(FindHandle);
        }
        
        return FileInfos;
    }
    
    // TODO: Try to rename those to DeleteFile and MoveFile,
    //       You can use :: operator after you wrap this library in rlsr namespace
    //       Or you can do:
    //       namespace win32
    //       {
    //			#include <windows.h>
    //       }
    
#undef CopyFile // TODO: What to do about those stupid macros and repeated names (same name in rstd and win32)
#undef CreateDirectory
    
    // TODO: Make unicode version of the function in which path can be longer the MAX_PATH
    template<class string> i32 RemoveFile(string& FilePath)
    { return DeleteFileA((char*)FilePath); }
    
    i32 RenameFile(char* FilePath, char* NewFilePath)
    { return MoveFile(FilePath, NewFilePath); }
    
    const rstd_bool FailIfFileWithNewPathExists = true;
    const rstd_bool OverrideFileIfFileWithNewPathExists = false;
    
    rstd_bool CopyFile(char* ExistingFilePath, char* NewFilePath, rstd_bool FailOrOverride)
    { return (rstd_bool)CopyFileA(ExistingFilePath, NewFilePath, (BOOL)FailOrOverride); }
    
    rstd_bool FileExists
    (char* Path)
    {
        u32 Attrib = GetFileAttributes(Path);
        return Attrib != INVALID_FILE_ATTRIBUTES && !(Attrib & FILE_ATTRIBUTE_DIRECTORY);
    }
    
    // TODO: Make OpenFile api support control over dwShareMode, dwCreationDisposition, dwFlagsAndAttributes
    file OpenFile
    (char* FilePath, io_mode Mode)
    {
        file File = {};
        
        u32 DesiredAccess = 0;
        u32 CreationDisposition = 0;
        switch(Mode)
        {
            case io_mode::Read:
            {
                DesiredAccess = GENERIC_READ;
                CreationDisposition = OPEN_EXISTING;
            } break;
            
            case io_mode::Write:
            {
                DesiredAccess = GENERIC_WRITE;
                CreationDisposition = OPEN_ALWAYS;
            } break;
            
            case io_mode::ReadWrite:
            {
                DesiredAccess = GENERIC_READ | GENERIC_WRITE;
                CreationDisposition = OPEN_ALWAYS;
            } break;
            
            rstd_InvalidDefaultCase;
        }
        
        File.PlatformFileHandle = ::CreateFileA(FilePath, DesiredAccess,
                                                FILE_SHARE_READ, 0, CreationDisposition, 0, 0);
        if(File.PlatformFileHandle == INVALID_HANDLE_VALUE)
            File.PlatformFileHandle = nullptr;
        else
            FileDebug.OnOpenFile(FilePath, File.PlatformFileHandle);
        
        return File;
    }
    
    rstd_bool Close
    (file& File)
    { 
        rstd_bool Succeded = (rstd_bool)CloseHandle(File.PlatformFileHandle);
        if(Succeded)
            FileDebug.OnCloseFile(File.PlatformFileHandle);
        File.PlatformFileHandle = nullptr;
        return Succeded;
    }
    
    OVERLAPPED MakeOverlapped
    (u64 Pos)
    {
        OVERLAPPED Overlapped = {};
        Overlapped.Offset = (u32)Pos;
        Overlapped.OffsetHigh = (u32)(Pos >> 32);
        return Overlapped;
    }
    
    u32 Write
    (file File, u64 Pos, void* Data, u32 Size)
    {
        rstd_Assert(File);
        DWORD WrittenBytes;
        auto Overlapped = MakeOverlapped(Pos);
        WriteFile(File.PlatformFileHandle, Data, Size, &WrittenBytes, &Overlapped);
        // TODO: Check if GetLastError returned ERROR_IO_PENDING
        return WrittenBytes;
    }
    
    u32 Read
    (void* Dest, file File, u64 Pos, u32 Size)
    {
        rstd_Assert(File);
        DWORD ReadBytes;
        auto Overlapped = MakeOverlapped(Pos);
        ReadFile(File.PlatformFileHandle, Dest, Size, &ReadBytes, &Overlapped);
        // TODO: Check if GetLastError returned ERROR_IO_PENDING
        return ReadBytes;
    }
    
    // TODO: Add u64 version of this function
    rstd_bool SetFileSize
    (file File, u32 Size)
    {
        LARGE_INTEGER FilePointer = {};
        FilePointer.QuadPart = Size;
        auto FileHandle = File.PlatformFileHandle;
        auto Success = SetFilePointerEx(FileHandle, FilePointer, 0, FILE_BEGIN) && SetEndOfFile(FileHandle);
        return (rstd_bool)Success;
    }
    
    // TODO: Make this function work for size_t
    u32 GetFileSize
    (file File)
    { 
        LARGE_INTEGER Int;
        if(GetFileSizeEx(File.PlatformFileHandle, &Int))
            return (u32)Int.QuadPart;
        else
            return InvalidU32;
    }
    
    char* ReadWholeFile
    (arena& Arena, char* FilePath)
    {
        auto File = OpenFile(FilePath, io_mode::Read);
        rstd_defer(Close(File));
        
        u32 FileSize = GetFileSize(File);
        if(FileSize == 0 || FileSize == InvalidU32)
        {
            return nullptr;
        }
        else
        {
            void* Content = rstd_PushSizeUninitialized(Arena, FileSize + 1);
            Read(Content, File, (u32)0, FileSize);
            char* Res = (char*)Content;
            Res[FileSize] = 0;
            return Res;
        }
    }
    
    rstd_bool CreateDirectory(char* Path)
    { return ::CreateDirectoryA(Path, nullptr); }
    
    rstd_bool CreateDirectory(wchar_t* Path)
    { return CreateDirectoryW(Path, nullptr); }
    
    rstd_bool DeleteDirectory(char* Path)
    { return RemoveDirectoryA(Path); }
    
    // TODO: What's up with prepend "\\?\". Check the docs and test it!
    rstd_bool DeleteDirectory(wchar_t* Path)
    { return RemoveDirectoryW(Path); }
    
    // TODO: Paths longer then MAX_PATH could be a problem here. Should I use unicode versions of DeleteFile and DeleteDirectory?
    rstd_bool DeleteAllContentsOfDirectory
    (char* Path)
    {
        string<> SearchPath = Path;
        SearchPath += "/*";
        
        WIN32_FIND_DATA FindData;
        HANDLE FindHandle = FindFirstFileA(SearchPath, &FindData); // name of first found thing is "." TODO: Make sure it's true
        if(FindHandle == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        else
        {
            FindNextFileA(FindHandle, &FindData); // name of second found thing is ".." TODO: Make sure it's true
            
            while(FindNextFileA(FindHandle, &FindData))
            {
                string<> FilePath = Path;
                FilePath += "/";
                FilePath += FindData.cFileName;
                if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    DeleteAllContentsOfDirectory(FilePath);
                    DeleteDirectory(FilePath);
                }
                else
                {
                    DeleteFile(FilePath);
                }
            }
            FindClose(FindHandle);
            return true;
        }
    }
    
    rstd_bool DeleteDirectoryWithAllContents
    (char* Path)
    {
        rstd_bool Success = DeleteAllContentsOfDirectory(Path);
        if(Success)
            DeleteDirectory(Path);
        return Success;
    }
    
    //////////
    // TIME //
    //////////
    static time ConvertToTime
    (SYSTEMTIME& St)
    {
        time Time;
        Time.Year = (u32)St.wYear;
        Time.Month = (month)St.wMonth;
        Time.Day = (u8)St.wDay;
        Time.DayOfWeek = St.wDayOfWeek == 0 ? day_of_week::Sunday : (day_of_week)St.wDayOfWeek;
        Time.Hour = (u8)St.wHour;
        Time.Minute = (u8)St.wMinute;
        Time.Second = (u8)St.wSecond;
        Time.Millisecond = St.wMilliseconds;
        return Time;
    }
    
    time GetUtcTime()
    {
        SYSTEMTIME St;
        GetSystemTime(&St);
        return ConvertToTime(St);
    }
    
    time GetLocalTime()
    {
        SYSTEMTIME St;
        GetLocalTime(&St);
        return ConvertToTime(St);
    }
    
    u64 GetSystemTimeAsUnixEpoch()
    {
        union file_time 
        {
            FILETIME FileTime;
            u64 U64;
        };
        file_time Res;
        GetSystemTimeAsFileTime(&Res.FileTime);
        u64 DifferenceBetweenWindowsEpochAndUnixEpoch = 116444736000000000;
        Res.U64 -= DifferenceBetweenWindowsEpochAndUnixEpoch;
        return Res.U64;
    }
    
#endif // _WIN32
    
    //////////////////
    // MEMORY DEBUG //
    //////////////////
    namespace MemoryDebug
    {
        
#if rstd_MemoryProfilerEnabled
        
        struct statistics
        {
            u32
                Used, UsedPeak,
            Size, SizePeak,
            Wasted, Unused;
        };
        
        struct arena_debug_data
        {
            doubly_linked_list<memory_block> MemoryBlocks;
            const char* Name;
            const char* MasterArenaName;
            statistics Stats;
            calling_info CreationCallingInfo;
            rstd_bool HasNamelessName;
            rstd_bool TemporaryMemory;
        };
        
        struct allocation
        {
            calling_info CallingInfo;
            const char* AllocatorName;
            void* Memory;
            u32 Size;
            allocation_type Type;
        };
        
        struct deallocation
        {
            void* Memory;
            allocation_type Type;
        };
        
        struct memory_group
        {
            doubly_linked_list_with_counter<arena_debug_data> Arenas;
            doubly_linked_list<allocation> Allocations;
            doubly_linked_list<deallocation> Deallocations;
            const char* Name;
        };
        
        struct state
        {
            arena Arena;
            doubly_linked_list<memory_group> MemoryGroups;
            doubly_linked_list<allocator_name> NamelessAllocatorNames;
            statistics Stats;
            memory_group* OthersMemoryGroup;
            memory_group* CurrentMemoryGroup;
            u32 NextArenaDebugId;
            u32 NamelessAllocatorIndex;
            rstd_bool JustCalledNextAllocationMemoryGroup;
            rstd_bool RegisterAllocations;
        };
        
        global state State;
        
        struct scope_no_alloc_registration
        {
            scope_no_alloc_registration()
            { State.RegisterAllocations = false; }
            
            ~scope_no_alloc_registration()
            { State.RegisterAllocations = true; }
        };
        
#define ScopeNoAllocRegistration scope_no_alloc_registration ScopeNoAllocRegis
        
        memory_group* AddMemoryGroup
        (const char* Name)
        {
            ScopeNoAllocRegistration;
            auto& MemoryGroup = State.MemoryGroups.PushUninitialized();
            MemoryGroup.Name = Name;
            MemoryGroup.Arenas = ShareArena(State.Arena);
            MemoryGroup.Allocations = ShareArena(State.Arena);
            MemoryGroup.Deallocations = ShareArena(State.Arena);
            return &MemoryGroup;
        }
        
        void Init()
        {
            ScopeNoAllocRegistration;
            State.Arena = AllocateArenaZero(2_MB, "Memory debug");
            State.MemoryGroups = ShareArena(State.Arena);
            State.NamelessAllocatorNames = ShareArena(State.Arena);
            State.OthersMemoryGroup = AddMemoryGroup("Others");
            State.CurrentMemoryGroup = State.OthersMemoryGroup;
            State.RegisterAllocations = true;
        }
        
        void BeginMemoryGroup
        (const char* Name)
        {
            auto* MemoryGroup = Find(State.MemoryGroups, [=]
                                     (auto Group){ return strcmp(Group.Name, Name) == 0; });
            
            if(!MemoryGroup)
                MemoryGroup = AddMemoryGroup(Name);
            
            State.CurrentMemoryGroup = MemoryGroup;
        }
        
        void EndMemoryGroup()
        { State.CurrentMemoryGroup = State.OthersMemoryGroup; }
        
        void NextCallMemoryGroup
        (const char* Name)
        {
            BeginMemoryGroup(Name);
            State.JustCalledNextAllocationMemoryGroup = true;
        }
        
        struct scope_memory_group
        {
            scope_memory_group(char* Name)
            { BeginMemoryGroup(Name); }
            
            ~scope_memory_group()
            { EndMemoryGroup(); }
        };
        
#define ScopeMemoryGroup(_Name) scope_memory_group ScopeMemGroup##__LINE__(_Name)
        
        static void EndNextCallMemoryGroup()
        {
            if(State.JustCalledNextAllocationMemoryGroup)
            {
                State.JustCalledNextAllocationMemoryGroup = false;
                State.CurrentMemoryGroup = State.OthersMemoryGroup;
            }
        }
        
        static void AddStatistic
        (u32* Statistic, u32* MaxStatistic, u32 Delta)
        {
            *Statistic += Delta;
            if(*Statistic > *MaxStatistic)
                *MaxStatistic = *Statistic;
        }
        
        static void AddUsedAndSubtractUnused
        (statistics* Stats, u32 Delta)
        {
            AddStatistic(&Stats->Used, &Stats->UsedPeak, Delta);
            Stats->Unused -= Delta;
        }
        
        static void AddSizeAndUnused
        (statistics* Stats, u32 Delta)
        { 
            AddStatistic(&Stats->Size, &Stats->SizePeak, Delta);
            Stats->Unused += Delta;
        }
        
        static void SubtractUsedAndAddUnused
        (statistics* Stats, u32 Delta)
        { 
            Stats->Used -= Delta;
            Stats->Unused += Delta;
        }
        
        static void SubtractSizeAndUnused
        (statistics* Stats, u32 SizeDelta, u32 UnusedDelta)
        { 
            Stats->Size -= SizeDelta; 
            Stats->Unused -= UnusedDelta;
        }
        
        static void AlterStatsOnMemoryBlockAllocation
        (statistics* Stats, u32 NewMemoryBlockSize, u32 PrevMemoryBlockUnusedBytes)
        {
            AddSizeAndUnused(Stats, NewMemoryBlockSize);
            Stats->Unused -= PrevMemoryBlockUnusedBytes;
            Stats->Wasted += PrevMemoryBlockUnusedBytes;
        }
        
        static void AlterStatsOnMemoryBlockDeallocation
        (statistics* Stats, u32 MemoryBlockToDeallocateSize, u32 PrevMemoryBlockUnusedBytes)
        {
            SubtractSizeAndUnused(Stats, MemoryBlockToDeallocateSize, PrevMemoryBlockUnusedBytes);
            Stats->Unused += PrevMemoryBlockUnusedBytes;
            Stats->Wasted -= PrevMemoryBlockUnusedBytes;
        }
        
        void RegisterCreateArena
        (arena& Arena, const char* ArenaName, const char* MasterArenaName, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& MemGroup = *State.CurrentMemoryGroup;
            auto& ArenaDebug = MemGroup.Arenas.PushZero();
            
            ArenaDebug.MemoryBlocks = ShareArena(State.Arena);
            ArenaDebug.MemoryBlocks.Push(*Arena.MemoryBlock);
            
            u32 ArenaSize = Arena.MemoryBlock->Size;
            AddSizeAndUnused(&ArenaDebug.Stats, ArenaSize);
            AddSizeAndUnused(&State.Stats, ArenaSize);
            
            ArenaDebug.CreationCallingInfo = CallingInfo;
            
            if(ArenaName)
            {
                Arena.DebugName = ArenaDebug.Name = ArenaName;
            }
            else
            {
                u32 MemGroupArenaCount = MemGroup.Arenas.GetCount();
                auto& Name = State.NamelessAllocatorNames.PushUninitialized();
                Name = Format<allocator_name>("nameless %", MemGroupArenaCount);
                Arena.DebugName = ArenaDebug.Name = Name;
                ArenaDebug.HasNamelessName = true;
            }
            
            ArenaDebug.MasterArenaName = MasterArenaName;
            
            EndNextCallMemoryGroup();
        }
        
        static arena_debug_data& GetArenaDebug
        (arena Arena)
        {
            arena_debug_data* ArenaDebug = nullptr;
            
            for(auto& MemGroup : State.MemoryGroups)
            {
                ArenaDebug = Find(MemGroup.Arenas,
                                  [=](auto Ad){ return Ad.Name == Arena.DebugName; });
                
                if(ArenaDebug)
                    break;
            }
            
            rstd_Assert(ArenaDebug);
            return *ArenaDebug;
        }
        
        void RegisterArenaAllocateNextMemoryBlock
        (arena Arena)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto* NewMemBlock = Arena.MemoryBlock;
            auto& ArenaDebug = GetArenaDebug(Arena);
            ArenaDebug.MemoryBlocks.Push(*NewMemBlock);
            
            rstd_AssertM(NewMemBlock->Prev, "Pay attention to word 'Next' in function name");
            
            u32 PrevMemBlockUnused = GetUnusedBytes(*NewMemBlock->Prev);
            AlterStatsOnMemoryBlockAllocation(&ArenaDebug.Stats, NewMemBlock->Size, PrevMemBlockUnused);
            AlterStatsOnMemoryBlockAllocation(&State.Stats, NewMemBlock->Size, PrevMemBlockUnused);
        }
        
        void RegisterArenaDeallocateMemoryBlock
        (arena Arena)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& MemBlockToDeallocate = *Arena.MemoryBlock;
            auto& ArenaDebug = GetArenaDebug(Arena);
            ArenaDebug.MemoryBlocks.PopLast();
            
            auto* PrevMemBlock = MemBlockToDeallocate.Prev;
            u32 PrevMemBlockUnused = PrevMemBlock ? GetUnusedBytes(*PrevMemBlock) : 0;
            AlterStatsOnMemoryBlockDeallocation(&ArenaDebug.Stats, MemBlockToDeallocate.Size, PrevMemBlockUnused);
            AlterStatsOnMemoryBlockDeallocation(&State.Stats, MemBlockToDeallocate.Size, PrevMemBlockUnused);
        }
        
        void SetDebugTemporaryMemory
        (temporary_memory TempMem, rstd_bool Flag)
        {
            auto& ArenaDebug = GetArenaDebug(*TempMem.Arena);
            ArenaDebug.TemporaryMemory = Flag;
        }
        
        void RegisterBeginTemporaryMemory(temporary_memory TempMem)
        { SetDebugTemporaryMemory(TempMem, true); }
        
        void RegisterEndTemporaryMemory(temporary_memory TempMem)
        { SetDebugTemporaryMemory(TempMem, false); }
        
        void RegisterArenaPush
        (arena Arena, push_size_uninitialized_ex_res Res, size Size,
         allocation_type AllocationType, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& DebugArena = GetArenaDebug(Arena);
            
            if(DebugArena.TemporaryMemory)
                return;
            
            allocation Alloc = {CallingInfo, DebugArena.Name, Res.Memory, Size, AllocationType};
            auto& MemGroup = *State.CurrentMemoryGroup;
            MemGroup.Allocations.PushFront(Alloc);
            
            auto& DebugMemBlock = DebugArena.MemoryBlocks.GetLast();
            DebugMemBlock = *Arena.MemoryBlock;
            AddUsedAndSubtractUnused(&DebugArena.Stats, Size);
            AddUsedAndSubtractUnused(&State.Stats, Size);
            
            EndNextCallMemoryGroup();
        }
        
        void RegisterGenAlloc
        (void* Memory, size Size, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            allocation Alloc = {CallingInfo, "malloc", Memory, Size, allocation_type::GenAlloc};
            auto& MemGroup = *State.CurrentMemoryGroup;
            MemGroup.Allocations.PushFront(Alloc);
            AddUsedAndSubtractUnused(&State.Stats, Size);
            AddSizeAndUnused(&State.Stats, Size);
            EndNextCallMemoryGroup();
        }
        
        void RegisterGenFree
        (void* Memory, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& MemGroup = *State.CurrentMemoryGroup;
            
            auto AllocComparison = [=](auto Alloc){ return Alloc.Memory == Memory; };
            auto* FoundAlloc = Find(MemGroup.Allocations, AllocComparison);
            if(!FoundAlloc)
            {
                for(auto& Mg : State.MemoryGroups)
                {
                    if(&Mg != &MemGroup)	
                    {
                        FoundAlloc = Find(Mg.Allocations, AllocComparison);
                        if(FoundAlloc)
                            break;
                    }	
                }
            }
            
            if(FoundAlloc)
            {
                MemGroup.Allocations.Remove(FoundAlloc);
                MemGroup.Deallocations.Push({FoundAlloc->Memory, allocation_type::GenAlloc});
                SubtractUsedAndAddUnused(&State.Stats, FoundAlloc->Size);
                SubtractSizeAndUnused(&State.Stats, FoundAlloc->Size, 0);
            }
            else
            {
                auto DeallocComparison = [=](auto Dealloc){ return Dealloc.Memory == Memory; };
                auto* FoundFreedMemory = Find(MemGroup.Deallocations, DeallocComparison);
                if(!FoundFreedMemory)
                {
                    for(auto& Mg : State.MemoryGroups)
                    {
                        if(&Mg != &MemGroup)	
                        {
                            FoundFreedMemory = Find(Mg.Deallocations, DeallocComparison);
                            if(FoundFreedMemory)
                                break;
                        }	
                    }
                }
                
                rstd_InvalidCodePathM(FoundFreedMemory ?
                                      "Memory was already freed!" : "Memory was never allocated!");
            }
        }
        
#endif
        
    }
    
    
#endif
    
}
