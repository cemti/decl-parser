#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
    bool FunctionType::HasNoParams::get()
    {
        if (Parameters->Count == 0)
            return true;

        auto fType = dynamic_cast<FundamentalType^>(Parameters[0].Decl->Type);
        return fType && fType->Type == FundamentalType::Types::__identifier(void);
    }

    FunctionType::FunctionType() : FunctionType(nullptr, nullptr) { }

    FunctionType::FunctionType(BaseType^ decaysTo, Declarations^ params) : CompositeType(decaysTo)
    {
        Parameters = params;
    }
    
    void FunctionType::SetQualifier(Qualifiers q)
    {
        if (bool(q))
            throw gcnew ArgumentException("Function doesn't accept any qualifier.");
    }
}