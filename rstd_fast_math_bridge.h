
namespace rstd
{
    // TODO: This function is not a good idea, Delete it
    static u32 Index2D(u32 Width, fm::v2u Pos)
    { return Width * Pos.Y + Pos.X; }
    
    template_fn ToString
    (fm::v2_base<type> V)
    {
        string<128> Res;
        if(IsValid(V))
        {
            Res += ToString(V.X);
            Res += ',';
            Res += ToString(V.Y);
        }
        else
        {
            Res = "Invalid";
        }
        return Res;
    }
    
    template_fn ToString
    (fm::v4_base<type> V)
    {
        string<> Res;
        if(IsValid(V))
        {
            Res += ToString(V.X);
            Res += ',';
            Res += ToString(V.Y);
            Res += ',';
            Res += ToString(V.Z);
            Res += ',';
            Res += ToString(V.W);
        }
        else
        {
            Res = "Invalid";
        }
        return Res;
    }
    
    template_fn ToString
    (fm::rect2_base<type> Rect)
    {
        string<255> Res;
        if(IsValid(Rect))
        {
            Res += ToString(Rect.Min);
            Res += ',';
            Res += ToString(Rect.Max);
        }
        else
        {
            Res = "Invalid";
        }
        return Res;
    }
    
    fn ReadV2
    (char** StringPtr)
    {
        fm::v2 Res;
        if(AdvanceIfStringsMatch(StringPtr, "Invalid"))
        {
            Res = InvalidV2;
        }
        else
        {
            Res.X = ReadF32(StringPtr);
            ++(*StringPtr);
            Res.Y = ReadF32(StringPtr);
        }
        return Res;
    }
    
    fn ReadV2u
    (char** StringPtr)
    {
        fm::v2u Res;
        if(AdvanceIfStringsMatch(StringPtr, "Invalid"))
        {
            Res = InvalidV2u;
        }
        else
        {
            Res.X = ReadU32(StringPtr);
            ++(*StringPtr);
            Res.Y = ReadU32(StringPtr);
        }
        return Res;
    }
    
    fn ReadV2i
    (char** StringPtr)
    {
        fm::v2i Res;
        if(AdvanceIfStringsMatch(StringPtr, "Invalid"))
        {
            Res = InvalidV2i;
        }
        else
        {
            Res.X = ReadI32(StringPtr);
            ++(*StringPtr);
            Res.Y = ReadI32(StringPtr);
        }
        return Res;
    }
    
    fn ReadV4
    (char** StringPtr)
    {
        fm::v4 Res;
        if(AdvanceIfStringsMatch(StringPtr, "Invalid"))
        {
            Res = InvalidV4;
        }
        else
        {
            Res.X = ReadF32(StringPtr);
            ++(*StringPtr);
            Res.Y = ReadF32(StringPtr);
            ++(*StringPtr);
            Res.Z = ReadF32(StringPtr);
            ++(*StringPtr);
            Res.W = ReadF32(StringPtr);
        }
        return Res;
    }
    
    fn ReadV4u
    (char** StringPtr)
    {
        fm::v4u Res;
        if(AdvanceIfStringsMatch(StringPtr, "Invalid"))
        {
            Res = InvalidV4u;
        }
        else
        {
            Res.X = ReadU32(StringPtr);
            ++(*StringPtr);
            Res.Y = ReadU32(StringPtr);
            ++(*StringPtr);
            Res.Z = ReadU32(StringPtr);
            ++(*StringPtr);
            Res.W = ReadU32(StringPtr);
        }
        return Res;
    }
    
    fn ReadRect2
    (char** StringPtr)
    {
        fm::rect2 Res;
        if(AdvanceIfStringsMatch(StringPtr, "Invalid"))
        {
            Res = InvalidRect2;
        }
        else
        {
            Res.Min = ReadV2(StringPtr);
            ++(*StringPtr);
            Res.Max = ReadV2(StringPtr);
        }
        return Res;
    }
    
    fn StringToV2(char* String)
    { return ReadV2(&String); }
    
    fn StringToV2u(char* String)
    { return ReadV2u(&String); }
    
    fn StringToV2i(char* String)
    { return ReadV2i(&String); }
    
    fn StringToV4(char* String)
    { return ReadV4(&String); }
    
    fn StringToV4u(char* String)
    { return ReadV4u(&String); }
    
    fn StringToRect2(char* String)
    { return ReadRect2(&String); }
    
    template<> char* TypeName<v2>()
    { return "v2"; }
    
    template<> char* TypeName<v2d>()
    { return "v2d"; }
    
    template<> char* TypeName<v2i>()
    { return "v2i"; }
    
    template<> char* TypeName<v2u>()
    { return "v2u"; }
    
    template<> char* TypeName<v2i16>()
    { return "v2i16"; }
    
    template<> char* TypeName<v2u16>()
    { return "v2u16"; }
    
    template<> char* TypeName<rect2>()
    { return "rect2"; }
}
