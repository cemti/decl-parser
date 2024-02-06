#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
    bool EnumType::Empty::get()
    {
        return Enumerators->Count == 0;
    }

    String^ EnumType::Keyword::get()
    {
        return "enum";
    }
    
    EnumType::EnumType() : EnumType(nullptr) { }

    EnumType::EnumType(String^ name) : EnumType(name, false) { }

    EnumType::EnumType(String^ name, bool complete) : NamedType(name, complete)
    {
        Enumerators = gcnew Enums;
    }

    int EnumType::SizeOf(DataModel)
    {
        return 4;
    }
}