#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace System::Collections::Generic;

namespace DeclParser
{
    bool FunctionType::HasParameters::get()
    {
        if (Parameters->Count == 0)
            return false;

        auto fType = dynamic_cast<FundamentalType^>(Parameters[0].Declaration->Type);
        return fType == nullptr || fType->Type != FundamentalType::DataType::Void;
    }

    FunctionType::FunctionType() : FunctionType(nullptr, nullptr) { }

    FunctionType::FunctionType(BaseType^ decayTo, IList<NamedDeclaration>^ parameters) : CompositeType(decayTo)
    {
        Parameters = parameters;
    }
    
    void FunctionType::SetQualifier(TypeQualifiers qualifiers)
    {
        if (qualifiers != TypeQualifiers::None)
            throw gcnew ArgumentException("Function doesn't accept any qualifier.");
    }
}