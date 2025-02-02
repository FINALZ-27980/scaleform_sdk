//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Desktop_NativeDragManager.h
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#ifndef INC_AS3_Obj_Desktop_NativeDragManager_H
#define INC_AS3_Obj_Desktop_NativeDragManager_H

#include "../AS3_Obj_Object.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{
// Forward declaration.
namespace fl_desktop
{
    extern const TypeInfo NativeDragManagerTI;
    extern const ClassInfo NativeDragManagerCI;
} // namespace fl_desktop

namespace ClassTraits { namespace fl_desktop
{
    class NativeDragManager;
}}

namespace InstanceTraits { namespace fl_desktop
{
    class NativeDragManager;
}}

namespace Classes { namespace fl_desktop
{
    class NativeDragManager;
}}

//##protect##"forward_declaration"
namespace Instances 
{
    class InteractiveObject;
    class Clipboard;
    class BitmapData;
    class Point;
    class NativeDragOptions;
}
//##protect##"forward_declaration"
    
namespace ClassTraits { namespace fl_desktop
{
    class NativeDragManager : public Traits
    {
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "ClassTraits::NativeDragManager"; }
#endif
    public:
        typedef Classes::fl_desktop::NativeDragManager ClassType;

    public:
        NativeDragManager(VM& vm);
        static Pickable<Traits> MakeClassTraits(VM& vm);
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

    };
}}

namespace Classes { namespace fl_desktop
{
    class NativeDragManager : public Class
    {
        friend class ClassTraits::fl_desktop::NativeDragManager;
        static const TypeInfo& GetTypeInfo() { return AS3::fl_desktop::NativeDragManagerTI; }
        
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "Classes::NativeDragManager"; }
#endif
    public:
        typedef NativeDragManager SelfType;
        typedef NativeDragManager ClassType;
        
    private:
        NativeDragManager(ClassTraits::Traits& t);
       
    private:
        SelfType& GetSelf()
        {
            return *this;
        }

//##protect##"class_$methods"
//##protect##"class_$methods"

    public:

//##protect##"class_$data"
//##protect##"class_$data"

    };
}}

//##protect##"methods"
//##protect##"methods"

}}} // namespace Scaleform { namespace GFx { namespace AS3

#endif

