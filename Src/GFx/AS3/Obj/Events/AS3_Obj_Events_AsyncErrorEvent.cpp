//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Events_AsyncErrorEvent.cpp
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#include "AS3_Obj_Events_AsyncErrorEvent.h"
#include "../../AS3_VM.h"
#include "../../AS3_Marshalling.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{

//##protect##"methods"
//##protect##"methods"

// Values of default arguments.
namespace Impl
{

} // namespace Impl

namespace Instances { namespace fl_events
{
    AsyncErrorEvent::AsyncErrorEvent(InstanceTraits::Traits& t)
    : Instances::fl_events::ErrorEvent(t)
    , error()
//##protect##"instance::AsyncErrorEvent::AsyncErrorEvent()$data"
//##protect##"instance::AsyncErrorEvent::AsyncErrorEvent()$data"
    {
//##protect##"instance::AsyncErrorEvent::AsyncErrorEvent()$code"
//##protect##"instance::AsyncErrorEvent::AsyncErrorEvent()$code"
    }


//##protect##"instance$methods"
    AsyncErrorEvent::~AsyncErrorEvent()
    {
    }
//##protect##"instance$methods"

}} // namespace Instances

namespace InstanceTraits { namespace fl_events
{
    const ThunkInfo AsyncErrorEvent::ti[AsyncErrorEvent::ThunkInfoNum] = {
        {ThunkInfo::EmptyFunc, &AS3::fl_events::EventTI, "clone", NULL, Abc::NS_Public, CT_Method, 0, 0},
        {ThunkInfo::EmptyFunc, &AS3::fl::StringTI, "toString", NULL, Abc::NS_Public, CT_Method, 0, 0},
    };
    const MemberInfo AsyncErrorEvent::mi[AsyncErrorEvent::MemberInfoNum] = {
        {"error", NULL, OFFSETOF(Instances::fl_events::AsyncErrorEvent, error), Abc::NS_Public, SlotInfo::BT_ObjectCpp, 0},
    };


    AsyncErrorEvent::AsyncErrorEvent(VM& vm, const ClassInfo& ci)
    : CTraits(vm, ci)
    {
//##protect##"InstanceTraits::AsyncErrorEvent::AsyncErrorEvent()"
//##protect##"InstanceTraits::AsyncErrorEvent::AsyncErrorEvent()"
        SetMemSize(sizeof(Instances::fl_events::AsyncErrorEvent));

    }

    void AsyncErrorEvent::MakeObject(Value& result, Traits& t)
    {
        result = MakeInstance(static_cast<AsyncErrorEvent&>(t));
    }

//##protect##"instance_traits$methods"
//##protect##"instance_traits$methods"

}} // namespace InstanceTraits

namespace Classes { namespace fl_events
{
    AsyncErrorEvent::AsyncErrorEvent(ClassTraits::Traits& t)
    : Class(t)
    , ASYNC_ERROR("asyncError")
    {
//##protect##"class_::AsyncErrorEvent::AsyncErrorEvent()"
//##protect##"class_::AsyncErrorEvent::AsyncErrorEvent()"
    }
//##protect##"class_$methods"
//##protect##"class_$methods"

}} // namespace Classes


namespace ClassTraits { namespace fl_events
{
    const MemberInfo AsyncErrorEvent::mi[AsyncErrorEvent::MemberInfoNum] = {
        {"ASYNC_ERROR", NULL, OFFSETOF(Classes::fl_events::AsyncErrorEvent, ASYNC_ERROR), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
    };

    AsyncErrorEvent::AsyncErrorEvent(VM& vm)
    : Traits(vm, AS3::fl_events::AsyncErrorEventCI)
    {
//##protect##"ClassTraits::AsyncErrorEvent::AsyncErrorEvent()"
//##protect##"ClassTraits::AsyncErrorEvent::AsyncErrorEvent()"
        MemoryHeap* mh = vm.GetMemoryHeap();

        Pickable<InstanceTraits::Traits> it(SF_HEAP_NEW_ID(mh, StatMV_VM_ITraits_Mem) InstanceTraits::fl_events::AsyncErrorEvent(vm, AS3::fl_events::AsyncErrorEventCI));
        SetInstanceTraits(it);

        // There is no problem with Pickable not assigned to anything here. Class constructor takes care of this.
        Pickable<Class> cl(SF_HEAP_NEW_ID(mh, StatMV_VM_Class_Mem) Classes::fl_events::AsyncErrorEvent(*this));

    }

    Pickable<Traits> AsyncErrorEvent::MakeClassTraits(VM& vm)
    {
        return Pickable<Traits>(SF_HEAP_NEW_ID(vm.GetMemoryHeap(), StatMV_VM_CTraits_Mem) AsyncErrorEvent(vm));
    }
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

}} // namespace ClassTraits

namespace fl_events
{
    const TypeInfo AsyncErrorEventTI = {
        TypeInfo::CompileTime | TypeInfo::NotImplemented,
        "AsyncErrorEvent", "flash.events", &fl_events::ErrorEventTI,
        TypeInfo::None
    };

    const ClassInfo AsyncErrorEventCI = {
        &AsyncErrorEventTI,
        ClassTraits::fl_events::AsyncErrorEvent::MakeClassTraits,
        0,
        ClassTraits::fl_events::AsyncErrorEvent::MemberInfoNum,
        InstanceTraits::fl_events::AsyncErrorEvent::ThunkInfoNum,
        InstanceTraits::fl_events::AsyncErrorEvent::MemberInfoNum,
        NULL,
        ClassTraits::fl_events::AsyncErrorEvent::mi,
        InstanceTraits::fl_events::AsyncErrorEvent::ti,
        InstanceTraits::fl_events::AsyncErrorEvent::mi,
    };
}; // namespace fl_events


}}} // namespace Scaleform { namespace GFx { namespace AS3

