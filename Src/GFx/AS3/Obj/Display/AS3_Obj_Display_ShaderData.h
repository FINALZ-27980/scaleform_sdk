//##protect##"disclaimer"
/**********************************************************************

Filename    :   AS3_Obj_Display_ShaderData.h
Content     :   
Created     :   Sep, 2011
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**********************************************************************/
//##protect##"disclaimer"

#ifndef INC_AS3_Obj_Display_ShaderData_H
#define INC_AS3_Obj_Display_ShaderData_H

#include "../AS3_Obj_Object.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{
// Forward declaration.
namespace fl_display
{
    extern const TypeInfo ShaderDataTI;
    extern const ClassInfo ShaderDataCI;
} // namespace fl_display

namespace ClassTraits { namespace fl_display
{
    class ShaderData;
}}

namespace InstanceTraits { namespace fl_display
{
    class ShaderData;
}}

namespace Classes { namespace fl_display
{
    class ShaderData;
}}

//##protect##"forward_declaration"
//##protect##"forward_declaration"
    
namespace ClassTraits { namespace fl_display
{
    class ShaderData : public Traits
    {
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "ClassTraits::ShaderData"; }
#endif
    public:
        typedef Classes::fl_display::ShaderData ClassType;

    public:
        ShaderData(VM& vm);
        static Pickable<Traits> MakeClassTraits(VM& vm);
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

    };
}}
//##protect##"methods"
//##protect##"methods"

}}} // namespace Scaleform { namespace GFx { namespace AS3

#endif

