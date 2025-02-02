//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Utils_Endian.cpp
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#include "AS3_Obj_Utils_Endian.h"
#include "../../AS3_VM.h"
#include "../../AS3_Marshalling.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{

//##protect##"methods"
//##protect##"methods"

namespace Classes { namespace fl_utils
{
    Endian::Endian(ClassTraits::Traits& t)
    : Class(t)
    , BIG_ENDIAN("bigEndian")
    , LITTLE_ENDIAN("littleEndian")
    {
//##protect##"class_::Endian::Endian()"
//##protect##"class_::Endian::Endian()"
    }
//##protect##"class_$methods"
//##protect##"class_$methods"

}} // namespace Classes


namespace ClassTraits { namespace fl_utils
{
    const MemberInfo Endian::mi[Endian::MemberInfoNum] = {
        {"BIG_ENDIAN", NULL, OFFSETOF(Classes::fl_utils::Endian, BIG_ENDIAN), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
        {"LITTLE_ENDIAN", NULL, OFFSETOF(Classes::fl_utils::Endian, LITTLE_ENDIAN), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
    };

    Endian::Endian(VM& vm)
    : Traits(vm, AS3::fl_utils::EndianCI)
    {
//##protect##"ClassTraits::Endian::Endian()"
//##protect##"ClassTraits::Endian::Endian()"
        MemoryHeap* mh = vm.GetMemoryHeap();

        Pickable<InstanceTraits::Traits> it(SF_HEAP_NEW_ID(mh, StatMV_VM_ITraits_Mem) InstanceTraits::fl::Object(vm, AS3::fl_utils::EndianCI));
        SetInstanceTraits(it);

        // There is no problem with Pickable not assigned to anything here. Class constructor takes care of this.
        Pickable<Class> cl(SF_HEAP_NEW_ID(mh, StatMV_VM_Class_Mem) Classes::fl_utils::Endian(*this));

    }

    Pickable<Traits> Endian::MakeClassTraits(VM& vm)
    {
        return Pickable<Traits>(SF_HEAP_NEW_ID(vm.GetMemoryHeap(), StatMV_VM_CTraits_Mem) Endian(vm));
    }
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

}} // namespace ClassTraits

namespace fl_utils
{
    const TypeInfo EndianTI = {
        TypeInfo::CompileTime | TypeInfo::Final,
        "Endian", "flash.utils", &fl::ObjectTI,
        TypeInfo::None
    };

    const ClassInfo EndianCI = {
        &EndianTI,
        ClassTraits::fl_utils::Endian::MakeClassTraits,
        0,
        ClassTraits::fl_utils::Endian::MemberInfoNum,
        0,
        0,
        NULL,
        ClassTraits::fl_utils::Endian::mi,
        NULL,
        NULL,
    };
}; // namespace fl_utils


}}} // namespace Scaleform { namespace GFx { namespace AS3

