//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Net_IDynamicPropertyWriter.h
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#ifndef INC_AS3_Obj_Net_IDynamicPropertyWriter_H
#define INC_AS3_Obj_Net_IDynamicPropertyWriter_H

#include "../AS3_Obj_Object.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{
// Forward declaration.
namespace fl_net
{
    extern const TypeInfo IDynamicPropertyWriterTI;
    extern const ClassInfo IDynamicPropertyWriterCI;
} // namespace fl_net

namespace ClassTraits { namespace fl_net
{
    class IDynamicPropertyWriter;
}}

namespace InstanceTraits { namespace fl_net
{
    class IDynamicPropertyWriter;
}}

namespace Classes { namespace fl_net
{
    class IDynamicPropertyWriter;
}}

//##protect##"forward_declaration"
    namespace Instances
    {
        class IDynamicPropertyOutput;
    }
//##protect##"forward_declaration"
    
namespace ClassTraits { namespace fl_net
{
    class IDynamicPropertyWriter : public Traits
    {
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "ClassTraits::IDynamicPropertyWriter"; }
#endif
    public:
        typedef Classes::fl_net::IDynamicPropertyWriter ClassType;

    public:
        IDynamicPropertyWriter(VM& vm);
        static Pickable<Traits> MakeClassTraits(VM& vm);
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

    };
}}
//##protect##"methods"
//##protect##"methods"

}}} // namespace Scaleform { namespace GFx { namespace AS3

#endif

