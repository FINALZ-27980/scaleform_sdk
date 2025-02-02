//##protect##"disclaimer"
/**************************************************************************

Filename    :   AS3_Obj_Utils_ByteArray.h
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
//##protect##"disclaimer"

#ifndef INC_AS3_Obj_Utils_ByteArray_H
#define INC_AS3_Obj_Utils_ByteArray_H

#include "../AS3_Obj_Object.h"
//##protect##"includes"
//##protect##"includes"


namespace Scaleform { namespace GFx { namespace AS3 
{
// Forward declaration.
namespace fl_utils
{
    extern const TypeInfo ByteArrayTI;
    extern const ClassInfo ByteArrayCI;
    extern const TypeInfo IDataInputTI;
    extern const ClassInfo IDataInputCI;
    extern const TypeInfo IDataOutputTI;
    extern const ClassInfo IDataOutputCI;
} // namespace fl_utils
namespace fl
{
    extern const TypeInfo uintTI;
    extern const ClassInfo uintCI;
    extern const TypeInfo StringTI;
    extern const ClassInfo StringCI;
    extern const TypeInfo BooleanTI;
    extern const ClassInfo BooleanCI;
    extern const TypeInfo int_TI;
    extern const ClassInfo int_CI;
    extern const TypeInfo NumberTI;
    extern const ClassInfo NumberCI;
} // namespace fl

namespace ClassTraits { namespace fl_utils
{
    class ByteArray;
}}

namespace InstanceTraits { namespace fl_utils
{
    class ByteArray;
}}

namespace Classes { namespace fl_utils
{
    class ByteArray;
}}

//##protect##"forward_declaration"
//##protect##"forward_declaration"

namespace Instances { namespace fl_utils
{
    class ByteArray : public Instances::fl::Object
    {
#ifndef SF_OS_WII
        template <class TR> friend Pickable<typename TR::InstanceType> InstanceTraits::MakeInstance(TR& tr);
#endif
        friend class InstanceTraits::fl_utils::ByteArray;
        
    public:
        typedef ByteArray SelfType;
        typedef Classes::fl_utils::ByteArray ClassType;
        typedef InstanceTraits::fl_utils::ByteArray TraitsType;
        static const TypeInfo& GetTypeInfo() { return AS3::fl_utils::ByteArrayTI; }

#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "Instances::fl_utils::ByteArray"; }
#endif
#ifdef SF_OS_WII
    public:
#else
    protected:
#endif
        ByteArray(InstanceTraits::Traits& t);

//##protect##"instance$methods"
    public:
        enum EncodingType { encAMF0 = 0, encAMF3 = 3 };
        enum EndianType { endianBig = 0, endianLittle = 1 };

    protected:
        ByteArray(InstanceTraits::Traits& t, EncodingType enc);

        void ThrowEOFError();
        void ThrowMemoryError();

        // Return true in case of an error.
        CheckResult EOFError();

        UInt32 GetAvailableNum() const { return static_cast<UInt32>(Data.GetSize() - Position); }
        void Resize(UInt32 size);

        static EndianType GetNativeEndian()
        {
#if (SF_BYTE_ORDER == SF_LITTLE_ENDIAN)
            return endianLittle;
#else
            return endianBig;
#endif
        }

        static EndianType GetDefaultEndian()
        {
            return endianBig;
        }

        EndianType GetEndian() const
        {
            return Endian;
        }

        template <typename T>
        void AdjustByteOrder(T& v) const
        {
            if (GetEndian() != GetNativeEndian())
                v = Alg::ByteUtil::SwapOrder(v);
        }

        static bool IsUTF8BOM(const char* str)
        {
            return (UInt8)str[0] == 0xEF && (UInt8)str[1] == 0xBB && (UInt8)str[2] == 0xBF;
        }
        static bool IsUTF16BE(const char* str)
        {
            return (UInt8)str[0] == 0xFE && (UInt8)str[1] == 0xFF;
        }
        static bool IsUTF16LE(const char* str)
        {
            return (UInt8)str[0] == 0xFF && (UInt8)str[1] == 0xFE;
        }

        // Return true on success.
        CheckResult Read(void* dest, UInt32 buff_size);
		// Return true if we can read num bytes.
		CheckResult CanRead(UInt32 num)
		{
			return (Position + num <= Data.GetSize());
		}

        void Write(const void* src, UInt32 buff_size);
        void Write(bool v);
        void Write(UInt8 v);
        void Write(SInt8 v);
        void Write(UInt16 v)
        {
            AdjustByteOrder(v);
            Write((void*)&v, sizeof(v));
        }
        void Write(SInt16 v)
        {
            AdjustByteOrder(v);
            Write((void*)&v, sizeof(v));
        }

        UInt16 ReadU16();
        SInt16 ReadS16() { return static_cast<SInt16>(ReadU16()); }
        // !!! It doesn't check for EOF.
        // Return false in case of an error.
        CheckResult ReadUTFBytes(ASString& result, UInt32 len);

    public:
        // Array-like access.
        UInt8 Get(UInt32 ind) const;
        void Set(UInt32 ind, UInt8 v);

        // sets content of bytearray
        void Get(void *dest, UPInt destSz);
        void Set(const void* data, UPInt sz);
        UInt32 GetLength() const { return Length; }
        const void* GetDataPtr() const { return Data.GetDataPtr(); }

        virtual CheckResult SetProperty(const Multiname& prop_name, const Value& value);
        virtual CheckResult GetProperty(const Multiname& prop_name, Value& value);
        virtual void GetDynamicProperty(AbsoluteIndex ind, Value& value);

//##protect##"instance$methods"

    public:
        // AS3 API methods.
        enum MethodID {
            mid_bytesAvailableGet, 
            mid_endianGet, 
            mid_endianSet, 
            mid_lengthGet, 
            mid_lengthSet, 
            mid_objectEncodingGet, 
            mid_objectEncodingSet, 
            mid_positionGet, 
            mid_positionSet, 
            mid_clear, 
            mid_compress, 
            mid_deflate, 
            mid_inflate, 
            mid_readBoolean, 
            mid_readByte, 
            mid_readBytes, 
            mid_readDouble, 
            mid_readFloat, 
            mid_readInt, 
            mid_readMultiByte, 
            mid_readObject, 
            mid_readShort, 
            mid_readUnsignedByte, 
            mid_readUnsignedInt, 
            mid_readUnsignedShort, 
            mid_readUTF, 
            mid_readUTFBytes, 
            mid_toString, 
            mid_uncompress, 
            mid_writeBoolean, 
            mid_writeByte, 
            mid_writeBytes, 
            mid_writeDouble, 
            mid_writeFloat, 
            mid_writeInt, 
            mid_writeMultiByte, 
            mid_writeObject, 
            mid_writeShort, 
            mid_writeUnsignedInt, 
            mid_writeUTF, 
            mid_writeUTFBytes, 
        };
        void bytesAvailableGet(UInt32& result);
        void endianGet(ASString& result);
        void endianSet(const Value& result, const ASString& value);
        void lengthGet(UInt32& result);
        void lengthSet(const Value& result, UInt32 value);
        void objectEncodingGet(UInt32& result);
        void objectEncodingSet(const Value& result, UInt32 value);
        void positionGet(UInt32& result);
        void positionSet(const Value& result, UInt32 value);
        void clear(const Value& result);
        void compress(Value& result, unsigned argc, const Value* const argv);
        void deflate(const Value& result);
        void inflate(const Value& result);
        void readBoolean(bool& result);
        void readByte(SInt32& result);
        void readBytes(const Value& result, Instances::fl_utils::ByteArray* bytes, UInt32 offset = 0, UInt32 length = 0);
        void readDouble(Value::Number& result);
        void readFloat(Value::Number& result);
        void readInt(SInt32& result);
        void readMultiByte(ASString& result, UInt32 length, const ASString& charSet);
        void readObject(Value& result);
        void readShort(SInt32& result);
        void readUnsignedByte(UInt32& result);
        void readUnsignedInt(UInt32& result);
        void readUnsignedShort(UInt32& result);
        void readUTF(ASString& result);
        void readUTFBytes(ASString& result, UInt32 length);
        void toString(ASString& result);
        void uncompress(Value& result, unsigned argc, const Value* const argv);
        void writeBoolean(const Value& result, bool value);
        void writeByte(const Value& result, SInt32 value);
        void writeBytes(const Value& result, Instances::fl_utils::ByteArray* bytes, UInt32 offset = 0, UInt32 length = 0);
        void writeDouble(const Value& result, Value::Number value);
        void writeFloat(const Value& result, Value::Number value);
        void writeInt(const Value& result, SInt32 value);
        void writeMultiByte(const Value& result, const ASString& value, const ASString& charSet);
        void writeObject(const Value& result, const Value& object);
        void writeShort(const Value& result, SInt32 value);
        void writeUnsignedInt(const Value& result, UInt32 value);
        void writeUTF(const Value& result, const ASString& value);
        void writeUTFBytes(const Value& result, const Value& value);

        // C++ friendly wrappers for AS3 methods.
        UInt32 bytesAvailableGet()
        {
            UInt32 result;
            bytesAvailableGet(result);
            return result;
        }
        ASString endianGet()
        {
            ASString result(GetStringManager().CreateEmptyString());
            endianGet(result);
            return result;
        }
        void endianSet(const ASString& value)
        {
            endianSet(Value::GetUndefined(), value);
        }
        UInt32 lengthGet()
        {
            UInt32 result;
            lengthGet(result);
            return result;
        }
        void lengthSet(UInt32 value)
        {
            lengthSet(Value::GetUndefined(), value);
        }
        UInt32 objectEncodingGet()
        {
            UInt32 result;
            objectEncodingGet(result);
            return result;
        }
        void objectEncodingSet(UInt32 value)
        {
            objectEncodingSet(Value::GetUndefined(), value);
        }
        UInt32 positionGet()
        {
            UInt32 result;
            positionGet(result);
            return result;
        }
        void positionSet(UInt32 value)
        {
            positionSet(Value::GetUndefined(), value);
        }
        void clear()
        {
            clear(Value::GetUndefined());
        }
        bool readBoolean()
        {
            bool result;
            readBoolean(result);
            return result;
        }
        SInt32 readByte()
        {
            SInt32 result;
            readByte(result);
            return result;
        }
        void readBytes(Instances::fl_utils::ByteArray* bytes, UInt32 offset = 0, UInt32 length = 0)
        {
            readBytes(Value::GetUndefined(), bytes, offset, length);
        }
        Value::Number readDouble()
        {
            Value::Number result;
            readDouble(result);
            return result;
        }
        Value::Number readFloat()
        {
            Value::Number result;
            readFloat(result);
            return result;
        }
        SInt32 readInt()
        {
            SInt32 result;
            readInt(result);
            return result;
        }
        ASString readMultiByte(UInt32 length, const ASString& charSet)
        {
            ASString result(GetStringManager().CreateEmptyString());
            readMultiByte(result, length, charSet);
            return result;
        }
        Value readObject()
        {
            Value result;
            readObject(result);
            return result;
        }
        SInt32 readShort()
        {
            SInt32 result;
            readShort(result);
            return result;
        }
        UInt32 readUnsignedByte()
        {
            UInt32 result;
            readUnsignedByte(result);
            return result;
        }
        UInt32 readUnsignedInt()
        {
            UInt32 result;
            readUnsignedInt(result);
            return result;
        }
        UInt32 readUnsignedShort()
        {
            UInt32 result;
            readUnsignedShort(result);
            return result;
        }
        ASString readUTF()
        {
            ASString result(GetStringManager().CreateEmptyString());
            readUTF(result);
            return result;
        }
        ASString readUTFBytes(UInt32 length)
        {
            ASString result(GetStringManager().CreateEmptyString());
            readUTFBytes(result, length);
            return result;
        }
        ASString toString()
        {
            ASString result(GetStringManager().CreateEmptyString());
            toString(result);
            return result;
        }
        void writeBoolean(bool value)
        {
            writeBoolean(Value::GetUndefined(), value);
        }
        void writeByte(SInt32 value)
        {
            writeByte(Value::GetUndefined(), value);
        }
        void writeBytes(Instances::fl_utils::ByteArray* bytes, UInt32 offset = 0, UInt32 length = 0)
        {
            writeBytes(Value::GetUndefined(), bytes, offset, length);
        }
        void writeDouble(Value::Number value)
        {
            writeDouble(Value::GetUndefined(), value);
        }
        void writeFloat(Value::Number value)
        {
            writeFloat(Value::GetUndefined(), value);
        }
        void writeInt(SInt32 value)
        {
            writeInt(Value::GetUndefined(), value);
        }
        void writeMultiByte(const ASString& value, const ASString& charSet)
        {
            writeMultiByte(Value::GetUndefined(), value, charSet);
        }
        void writeObject(const Value& object)
        {
            writeObject(Value::GetUndefined(), object);
        }
        void writeShort(SInt32 value)
        {
            writeShort(Value::GetUndefined(), value);
        }
        void writeUnsignedInt(UInt32 value)
        {
            writeUnsignedInt(Value::GetUndefined(), value);
        }
        void writeUTF(const ASString& value)
        {
            writeUTF(Value::GetUndefined(), value);
        }
        void writeUTFBytes(const Value& value)
        {
            writeUTFBytes(Value::GetUndefined(), value);
        }

//##protect##"instance$data"
    protected:
		static const char* ASCII_Names[];
		static const char* UTF8_Names[];
		static const char* UTF16_Names[];
		// No ISO support for the time being.
		//static const char* ISO_Names[];

        EncodingType Encoding:3; // One bit for sign.
        EndianType Endian:2; // One bit for sign.
        UInt32 Position;
        // Length is different from Data.GetSize().
        // Data doesn't get truncated when Length get set to zero.
        UInt32 Length;
        ArrayLH_POD<UInt8> Data;
//##protect##"instance$data"

    };
}} // namespace Instances

namespace InstanceTraits { namespace fl_utils
{
    class ByteArray : public CTraits
    {
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "InstanceTraits::ByteArray"; }
#endif
    public:
        typedef Instances::fl_utils::ByteArray InstanceType;

    public:
        ByteArray(VM& vm, const ClassInfo& ci);

    public: 
        static Pickable<Instances::fl_utils::ByteArray> MakeInstance(ByteArray& t)
        {
            return Pickable<Instances::fl_utils::ByteArray>(new(t.Alloc()) Instances::fl_utils::ByteArray(t));
        }
        SPtr<Instances::fl_utils::ByteArray> MakeInstanceS(ByteArray& t)
        {
            return MakeInstance(t);
        }

        virtual void MakeObject(Value& result, Traits& t);

        enum { ThunkInfoNum = 41 };
        static const ThunkInfo ti[ThunkInfoNum];
//##protect##"instance_traits$methods"
//##protect##"instance_traits$methods"

//##protect##"instance_traits$data"
//##protect##"instance_traits$data"

    };
    
}}
    
namespace ClassTraits { namespace fl_utils
{
    class ByteArray : public Traits
    {
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "ClassTraits::ByteArray"; }
#endif
    public:
        typedef Classes::fl_utils::ByteArray ClassType;

    public:
        ByteArray(VM& vm);
        static Pickable<Traits> MakeClassTraits(VM& vm);
        enum { ThunkInfoNum = 2 };
        static const ThunkInfo ti[ThunkInfoNum];
//##protect##"ClassTraits$methods"
//##protect##"ClassTraits$methods"

    };
}}

namespace Classes { namespace fl_utils
{
    class ByteArray : public Class
    {
        friend class ClassTraits::fl_utils::ByteArray;
        static const TypeInfo& GetTypeInfo() { return AS3::fl_utils::ByteArrayTI; }
        
#ifdef GFX_AS3_VERBOSE
    private:
        virtual const char* GetAS3ObjectType() const { return "Classes::ByteArray"; }
#endif
    public:
        typedef ByteArray SelfType;
        typedef ByteArray ClassType;
        
    private:
        ByteArray(ClassTraits::Traits& t);
       
    private:
        SelfType& GetSelf()
        {
            return *this;
        }

//##protect##"class_$methods"
    public:
        Instances::fl_utils::ByteArray::EncodingType GetDefEncoding() const { return DefEncoding; }
//##protect##"class_$methods"

    public:
        // AS3 API methods.
        enum MethodID {
            mid_defaultObjectEncodingSet, 
            mid_defaultObjectEncodingGet, 
        };
        void defaultObjectEncodingSet(const Value& result, UInt32 value);
        void defaultObjectEncodingGet(UInt32& result);

        // C++ friendly wrappers for AS3 methods.
        void defaultObjectEncodingSet(UInt32 value)
        {
            defaultObjectEncodingSet(Value::GetUndefined(), value);
        }
        UInt32 defaultObjectEncodingGet()
        {
            UInt32 result;
            defaultObjectEncodingGet(result);
            return result;
        }

//##protect##"class_$data"
        Instances::fl_utils::ByteArray::EncodingType DefEncoding;
//##protect##"class_$data"

    };
}}

//##protect##"methods"
//##protect##"methods"

}}} // namespace Scaleform { namespace GFx { namespace AS3

#endif

