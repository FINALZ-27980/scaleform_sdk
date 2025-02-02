//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Events_TimerEvent.cpp
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#include "AS3_Obj_Events_TimerEvent.h"
#include "../../AS3_VM.h"
#include "../../AS3_Marshalling.h"
//##protect##"includes"
#include "../../AS3_MovieRoot.h"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{

//##protect##"methods"
//##protect##"methods"

// Values of default arguments.
namespace Impl
{

} // namespace Impl
typedef ThunkFunc0<Instances::fl_events::TimerEvent, Instances::fl_events::TimerEvent::mid_clone, SPtr<Instances::fl_events::Event> > TFunc_Instances_TimerEvent_clone;
typedef ThunkFunc0<Instances::fl_events::TimerEvent, Instances::fl_events::TimerEvent::mid_toString, ASString> TFunc_Instances_TimerEvent_toString;
typedef ThunkFunc0<Instances::fl_events::TimerEvent, Instances::fl_events::TimerEvent::mid_updateAfterEvent, const Value> TFunc_Instances_TimerEvent_updateAfterEvent;

template <> const TFunc_Instances_TimerEvent_clone::TMethod TFunc_Instances_TimerEvent_clone::Method = &Instances::fl_events::TimerEvent::clone;
template <> const TFunc_Instances_TimerEvent_toString::TMethod TFunc_Instances_TimerEvent_toString::Method = &Instances::fl_events::TimerEvent::toString;
template <> const TFunc_Instances_TimerEvent_updateAfterEvent::TMethod TFunc_Instances_TimerEvent_updateAfterEvent::Method = &Instances::fl_events::TimerEvent::updateAfterEvent;

namespace Instances { namespace fl_events
{
    TimerEvent::TimerEvent(InstanceTraits::Traits& t)
    : Instances::fl_events::Event(t)
//##protect##"instance::TimerEvent::TimerEvent()$data"
//##protect##"instance::TimerEvent::TimerEvent()$data"
    {
//##protect##"instance::TimerEvent::TimerEvent()$code"
//##protect##"instance::TimerEvent::TimerEvent()$code"
    }

    void TimerEvent::clone(SPtr<Instances::fl_events::Event>& result)
    {
//##protect##"instance::TimerEvent::clone()"
        SF_UNUSED1(result);
        // GetEventClass returns a correct event class for the clone
        Event::clone(result);
//##protect##"instance::TimerEvent::clone()"
    }
    void TimerEvent::toString(ASString& result)
    {
//##protect##"instance::TimerEvent::toString()"
        SF_UNUSED1(result);
        // GetEventName will return a correct name for the event
        Event::toString(result);
//##protect##"instance::TimerEvent::toString()"
    }
    void TimerEvent::updateAfterEvent(const Value& result)
    {
//##protect##"instance::TimerEvent::updateAfterEvent()"
        SF_UNUSED1(result);
        WARN_NOT_IMPLEMENTED("TimerEvent::updateAfterEvent()");
//##protect##"instance::TimerEvent::updateAfterEvent()"
    }

    SPtr<Instances::fl_events::Event> TimerEvent::clone()
    {
        SPtr<Instances::fl_events::Event> result;
        clone(result);
        return result;
    }
//##protect##"instance$methods"
    AS3::Object*     TimerEvent::GetEventClass() const 
    { 
        return static_cast<ASVM&>(GetVM()).TimerEventClass.GetPtr(); 
    }
//##protect##"instance$methods"

}} // namespace Instances

namespace InstanceTraits { namespace fl_events
{
    const ThunkInfo TimerEvent::ti[TimerEvent::ThunkInfoNum] = {
        {TFunc_Instances_TimerEvent_clone::Func, &AS3::fl_events::EventTI, "clone", NULL, Abc::NS_Public, CT_Method, 0, 0},
        {TFunc_Instances_TimerEvent_toString::Func, &AS3::fl::StringTI, "toString", NULL, Abc::NS_Public, CT_Method, 0, 0},
        {TFunc_Instances_TimerEvent_updateAfterEvent::Func, NULL, "updateAfterEvent", NULL, Abc::NS_Public, CT_Method, 0, 0},
    };

    TimerEvent::TimerEvent(VM& vm, const ClassInfo& ci)
    : CTraits(vm, ci)
    {
//##protect##"InstanceTraits::TimerEvent::TimerEvent()"
//##protect##"InstanceTraits::TimerEvent::TimerEvent()"
        SetMemSize(sizeof(Instances::fl_events::TimerEvent));

    }

    void TimerEvent::MakeObject(Value& result, Traits& t)
    {
        result = MakeInstance(static_cast<TimerEvent&>(t));
    }

//##protect##"instance_traits$methods"
//##protect##"instance_traits$methods"

}} // namespace InstanceTraits

namespace Classes { namespace fl_events
{
    TimerEvent::TimerEvent(ClassTraits::Traits& t)
    : Class(t)
    , TIMER("timer")
    , TIMER_COMPLETE("timerComplete")
    {
//##protect##"class_::TimerEvent::TimerEvent()"
//##protect##"class_::TimerEvent::TimerEvent()"
    }
//##protect##"class_$methods"
//##protect##"class_$methods"

}} // namespace Classes


namespace ClassTraits { namespace fl_events
{
    const MemberInfo TimerEvent::mi[TimerEvent::MemberInfoNum] = {
        {"TIMER", NULL, OFFSETOF(Classes::fl_events::TimerEvent, TIMER), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
        {"TIMER_COMPLETE", NULL, OFFSETOF(Classes::fl_events::TimerEvent, TIMER_COMPLETE), Abc::NS_Public, SlotInfo::BT_ConstChar, 1},
    };

    TimerEvent::TimerEvent(VM& vm)
    : Traits(vm, AS3::fl_events::TimerEventCI)
    {
//##protect##"ClassTraits::TimerEvent::TimerEvent()"
//##protect##"ClassTraits::TimerEvent::TimerEvent()"
        MemoryHeap* mh = vm.GetMemoryHeap();

        Pickable<InstanceTraits::Traits> it(SF_HEAP_NEW_ID(mh, StatMV_VM_ITraits_Mem) InstanceTraits::fl_events::TimerEvent(vm, AS3::fl_events::TimerEventCI));
        SetInstanceTraits(it);

        // There is no problem with Pickable not assigned to anything here. Class constructor takes care of this.
        Pickable<Class> cl(SF_HEAP_NEW_ID(mh, StatMV_VM_Class_Mem) Classes::fl_events::TimerEvent(*this));

    }

    Pickable<Traits> TimerEvent::MakeClassTraits(VM& vm)
    {
        return Pickable<Traits>(SF_HEAP_NEW_ID(vm.GetMemoryHeap(), StatMV_VM_CTraits_Mem) TimerEvent(vm));
    }
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

}} // namespace ClassTraits

namespace fl_events
{
    const TypeInfo TimerEventTI = {
        TypeInfo::CompileTime,
        "TimerEvent", "flash.events", &fl_events::EventTI,
        TypeInfo::None
    };

    const ClassInfo TimerEventCI = {
        &TimerEventTI,
        ClassTraits::fl_events::TimerEvent::MakeClassTraits,
        0,
        ClassTraits::fl_events::TimerEvent::MemberInfoNum,
        InstanceTraits::fl_events::TimerEvent::ThunkInfoNum,
        0,
        NULL,
        ClassTraits::fl_events::TimerEvent::mi,
        InstanceTraits::fl_events::TimerEvent::ti,
        NULL,
    };
}; // namespace fl_events


}}} // namespace Scaleform { namespace GFx { namespace AS3

