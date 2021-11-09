#define rstd_Implementation
#define rstd_Debug 1
#include "rstd.h"
#include "../rstd_remove_macro_prefix.h"
using namespace rstd;

#define fn static auto

struct parsing_context
{
    char* Char;
    u32 Line;
};

fn IsNextLineOrEndOfFile(char C)
{ return C == 0 || C == '\n'; }

fn IsThereCharInTheSameLine
(char* C, char EndingChar)
{
    while(*C != EndingChar)
    {
        if(IsNextLineOrEndOfFile(*C))
            return false;
        ++C;
    }
    return true;
}

fn AdvanceToInTheSameLine
(parsing_context& C, char EndingChar)
{
    while(*C.Char != EndingChar)
    {
        if(IsNextLineOrEndOfFile(*C.Char))
            return false;
        ++C.Char;
    }
    return true;
}

fn AdvanceToNextLine
(parsing_context& C)
{
    for(;;)
    {
        if(*C.Char == 0)
            return false;
        
        if(*C.Char == '\n')
        {
            ++C.Char;
            ++C.Line;
            return true;
        }
        
        ++C.Char;
    }
}

fn Advance
(parsing_context& C)
{
    if(*C.Char == '\n')
        ++C.Line;
    ++C.Char;
}

int main()
{
    parsing_context C;
    C.Line = 1;
    
    arena Arena = rstd_AllocateArenaZero(10_MB);
    C.Char = ReadWholeFile(Arena, "source.cpp");
    if(!C.Char)
    {
        printf(Format<string<>>("Couldn't open source.cpp"));
        return 0;
    }
    
    struct to_copy
    {
        char* Name;
        char* Code;
        u32 CodeSize;
    };
    backward_singly_linked_list<to_copy> CodeToCopy = ShareArena(Arena);
    
    char* CharOnMetaCode;
    for(;;)
    {
        if(!(*C.Char))
        {
            printf("ERROR: Source doesn't have META CODE");
            return 0;
        }
        
        CharOnMetaCode = C.Char;
        if(AdvanceIfStringsMatch(&C.Char, "META CODE"))
            break;
        
        char* CharOnMetaCopy = C.Char;
        if(AdvanceIfStringsMatch(&C.Char, "META COPY("))
        {
            if(!CodeToCopy.Empty())
            {
                auto& PrevToCopy = CodeToCopy.GetFirst();
                PrevToCopy.CodeSize = (u32)(CharOnMetaCopy - PrevToCopy.Code);
            }
            
            auto& ToCopy = CodeToCopy.PushZero();
            ToCopy.Name = C.Char;
            
            if(!AdvanceToInTheSameLine(C, ')'))
            {
                printf(Format<string<>>("Error at line %:\nName of META COPY has to be closed with ')'", C.Line));
                return 0;
            }
            
            if(!AdvanceToNextLine(C))
            {
                printf(Format<string<>>("ERROR: Source doesn't have META CODE", C.Line));
                return 0;
            }
            
            ToCopy.Code = C.Char;
        }
        
        Advance(C);
    }
    
    if(!CodeToCopy.Empty())
    {
        auto& PrevToCopy = CodeToCopy.GetFirst();
        PrevToCopy.CodeSize = (u32)(CharOnMetaCode - PrevToCopy.Code);
    }
    
    auto OutputFileStream = OpenFileStream("../rstd.h", io_mode::Write);
    rstd_defer(Close(OutputFileStream));
    if(!OutputFileStream)
    {
        printf("Error: Could not open rstd.h to write");
        return 0;
    }
    
    char* ChunkBegin = C.Char;
    while(*C.Char)
    {
        char* CharAtMetaPaste = C.Char;
        if(AdvanceIfStringsMatch(&C.Char, "META PASTE("))
        {
            Write(OutputFileStream, ChunkBegin, (u32)(CharAtMetaPaste - ChunkBegin));
            
            if(!IsThereCharInTheSameLine(C.Char, ')'))
            {
                printf(Format<string<>>("Error at line %:\nName of META PASTE has to be closed with ')'"), C.Line);
                return 0;
            }
            
            auto* FoundToCopy = CodeToCopy.Find([=](auto& ToCopy){ return StringsMatchUntilCharacter(C.Char, ToCopy.Name, ')'); });
            if(FoundToCopy)
            {
                Write(OutputFileStream, FoundToCopy->Code, FoundToCopy->CodeSize);
                AdvanceToNextLine(C);
                ChunkBegin = C.Char;
            }
            else
            {
                string<> MetaPasteName;
                while(*C.Char != ')')
                {
                    MetaPasteName += *C.Char;
                    ++C.Char;
                }
                
                printf(Format<string<>>("Error at line %:\nMETA PASTE(%) didn't find corresponding META COPY()",
                                        C.Line, MetaPasteName));
                return 0;
            }
        }
        
        Advance(C);
    }
    
    Write(OutputFileStream, ChunkBegin, (u32)(C.Char - ChunkBegin));
}
