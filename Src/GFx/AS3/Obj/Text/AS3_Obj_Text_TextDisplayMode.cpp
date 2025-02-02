//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Text_TextDisplayMode.cpp
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#include "AS3_Obj_Text_TextDisplayMode.h"
#include "../../AS3_VM.h"
#include "../../AS3_Marshalling.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{

//##protect##"methods"
//##protect##"methods"

namespace Classes { namespace fl_text
{
    TextDisplayMode::TextDisplayMode(ClassTraits::Traits& t)
    : Class(t)
    , CRT("crt")
    , DEFAULT("default")
    , LCD("lcd")
    {
//##protect##"class_::TextDisplayMode::TextDisplayMode()"
//##protect##"class_::TextDisplayMode::TextDisplayMode()"
    }
//##protect##"class_$methods"
//##protect##"class_$methods"

}} // namespace Classes


namespace ClassTraits { namespace fl_text
{
    const MemberInfo TextDisplayMode::mi[TextDisplayMode::MemberInfoNum] = {
        {"CRT", NULL, OFFSETOF(Classes::fl_text::TextDisplayMode, CRT), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
        {"DEFAULT", NULL, OFFSETOF(Classes::fl_text::TextDisplayMode, DEFAULT), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
        {"LCD", NULL, OFFSETOF(Classes::fl_text::TextDisplayMode, LCD), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
    };

    TextDisplayMode::TextDisplayMode(VM& vm)
    : Traits(vm, AS3::fl_text::TextDisplayModeCI)
    {
//##protect##"ClassTraits::TextDisplayMode::TextDisplayMode()"
//##protect##"ClassTraits::TextDisplayMode::TextDisplayMode()"
        MemoryHeap* mh = vm.GetMemoryHeap();

        Pickable<InstanceTraits::Traits> it(SF_HEAP_NEW_ID(mh, StatMV_VM_ITraits_Mem) InstanceTraits::fl::Object(vm, AS3::fl_text::TextDisplayModeCI));
        SetInstanceTraits(it);

        // There is no problem with Pickable not assigned to anything here. Class constructor takes care of this.
        Pickable<Class> cl(SF_HEAP_NEW_ID(mh, StatMV_VM_Class_Mem) Classes::fl_text::TextDisplayMode(*this));

    }

    Pickable<Traits> TextDisplayMode::MakeClassTraits(VM& vm)
    {
        return Pickable<Traits>(SF_HEAP_NEW_ID(vm.GetMemoryHeap(), StatMV_VM_CTraits_Mem) TextDisplayMode(vm));
    }
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

}} // namespace ClassTraits

namespace fl_text
{
    const TypeInfo TextDisplayModeTI = {
        TypeInfo::CompileTime | TypeInfo::Final,
        "TextDisplayMode", "flash.text", &fl::ObjectTI,
        TypeInfo::None
    };

    const ClassInfo TextDisplayModeCI = {
        &TextDisplayModeTI,
        ClassTraits::fl_text::TextDisplayMode::MakeClassTraits,
        0,
        ClassTraits::fl_text::TextDisplayMode::MemberInfoNum,
        0,
        0,
        NULL,
        ClassTraits::fl_text::TextDisplayMode::mi,
        NULL,
        NULL,
    };
}; // namespace fl_text


}}} // namespace Scaleform { namespace GFx { namespace AS3

