// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TCATSDataDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "TCATSData.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TCATSData(void *p = nullptr);
   static void *newArray_TCATSData(Long_t size, void *p);
   static void delete_TCATSData(void *p);
   static void deleteArray_TCATSData(void *p);
   static void destruct_TCATSData(void *p);
   static void streamer_TCATSData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TCATSData*)
   {
      ::TCATSData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TCATSData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TCATSData", ::TCATSData::Class_Version(), "TCATSData.h", 30,
                  typeid(::TCATSData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TCATSData::Dictionary, isa_proxy, 16,
                  sizeof(::TCATSData) );
      instance.SetNew(&new_TCATSData);
      instance.SetNewArray(&newArray_TCATSData);
      instance.SetDelete(&delete_TCATSData);
      instance.SetDeleteArray(&deleteArray_TCATSData);
      instance.SetDestructor(&destruct_TCATSData);
      instance.SetStreamerFunc(&streamer_TCATSData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TCATSData*)
   {
      return GenerateInitInstanceLocal(static_cast<::TCATSData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TCATSData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TCATSData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TCATSData::Class_Name()
{
   return "TCATSData";
}

//______________________________________________________________________________
const char *TCATSData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TCATSData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TCATSData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TCATSData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TCATSData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TCATSData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TCATSData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TCATSData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TCATSData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TCATSData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<UShort_t> &R__stl =  fCATS_DetX;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_StripX;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_ChargeX;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_DetY;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_StripY;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_ChargeY;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_DetQ;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_Charge;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, TCATSData::IsA());
   } else {
      R__c = R__b.WriteVersion(TCATSData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<UShort_t> &R__stl =  fCATS_DetX;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_StripX;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_ChargeX;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_DetY;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_StripY;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_ChargeY;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_DetQ;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UShort_t> &R__stl =  fCATS_Charge;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UShort_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TCATSData(void *p) {
      return  p ? new(p) ::TCATSData : new ::TCATSData;
   }
   static void *newArray_TCATSData(Long_t nElements, void *p) {
      return p ? new(p) ::TCATSData[nElements] : new ::TCATSData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TCATSData(void *p) {
      delete (static_cast<::TCATSData*>(p));
   }
   static void deleteArray_TCATSData(void *p) {
      delete [] (static_cast<::TCATSData*>(p));
   }
   static void destruct_TCATSData(void *p) {
      typedef ::TCATSData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TCATSData(TBuffer &buf, void *obj) {
      ((::TCATSData*)obj)->::TCATSData::Streamer(buf);
   }
} // end of namespace ROOT for class ::TCATSData

namespace ROOT {
   static TClass *vectorlEunsignedsPshortgR_Dictionary();
   static void vectorlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPshortgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPshortgR(void *p);
   static void deleteArray_vectorlEunsignedsPshortgR(void *p);
   static void destruct_vectorlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned short>*)
   {
      vector<unsigned short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned short>", -2, "vector", 389,
                  typeid(vector<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned short>) );
      instance.SetNew(&new_vectorlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPshortgR);
      instance.SetDelete(&delete_vectorlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned short> >()));

      ::ROOT::AddClassAlternate("vector<unsigned short>","std::vector<unsigned short, std::allocator<unsigned short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<unsigned short>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<unsigned short>*>(nullptr))->GetClass();
      vectorlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short> : new vector<unsigned short>;
   }
   static void *newArray_vectorlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short>[nElements] : new vector<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPshortgR(void *p) {
      delete (static_cast<vector<unsigned short>*>(p));
   }
   static void deleteArray_vectorlEunsignedsPshortgR(void *p) {
      delete [] (static_cast<vector<unsigned short>*>(p));
   }
   static void destruct_vectorlEunsignedsPshortgR(void *p) {
      typedef vector<unsigned short> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned short>

namespace {
  void TriggerDictionaryInitialization_TCATSDataDict_Impl() {
    static const char* headers[] = {
"TCATSData.h",
nullptr
    };
    static const char* includePaths[] = {
"../Core",
"../Physics",
"../../Core",
"../../Physics",
"../TrackReconstruction",
"../../TrackReconstruction",
"/space/newopt/root/include/",
"/home/aktas/Softs/nptool/NPLib/Detectors/CATS/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TCATSDataDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TCATSData.h")))  TCATSData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TCATSDataDict dictionary payload"

#ifndef NPMULTITHREADING
  #define NPMULTITHREADING 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TCATSData.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TCATSData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TCATSDataDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TCATSDataDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TCATSDataDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TCATSDataDict() {
  TriggerDictionaryInitialization_TCATSDataDict_Impl();
}
