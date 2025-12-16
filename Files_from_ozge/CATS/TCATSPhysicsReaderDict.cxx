// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TCATSPhysicsReaderDict
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
#include "TCATSPhysicsReader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TCATSPhysicsReader(void *p = nullptr);
   static void *newArray_TCATSPhysicsReader(Long_t size, void *p);
   static void delete_TCATSPhysicsReader(void *p);
   static void deleteArray_TCATSPhysicsReader(void *p);
   static void destruct_TCATSPhysicsReader(void *p);
   static void streamer_TCATSPhysicsReader(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TCATSPhysicsReader*)
   {
      ::TCATSPhysicsReader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TCATSPhysicsReader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TCATSPhysicsReader", ::TCATSPhysicsReader::Class_Version(), "TCATSPhysicsReader.h", 43,
                  typeid(::TCATSPhysicsReader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TCATSPhysicsReader::Dictionary, isa_proxy, 16,
                  sizeof(::TCATSPhysicsReader) );
      instance.SetNew(&new_TCATSPhysicsReader);
      instance.SetNewArray(&newArray_TCATSPhysicsReader);
      instance.SetDelete(&delete_TCATSPhysicsReader);
      instance.SetDeleteArray(&deleteArray_TCATSPhysicsReader);
      instance.SetDestructor(&destruct_TCATSPhysicsReader);
      instance.SetStreamerFunc(&streamer_TCATSPhysicsReader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TCATSPhysicsReader*)
   {
      return GenerateInitInstanceLocal(static_cast<::TCATSPhysicsReader*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TCATSPhysicsReader*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TCATSPhysicsReader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TCATSPhysicsReader::Class_Name()
{
   return "TCATSPhysicsReader";
}

//______________________________________________________________________________
const char *TCATSPhysicsReader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TCATSPhysicsReader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TCATSPhysicsReader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TCATSPhysicsReader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TCATSPhysicsReader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TCATSPhysicsReader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TCATSPhysicsReader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TCATSPhysicsReader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TCATSPhysicsReader::Streamer(TBuffer &R__b)
{
   // Stream an object of class TCATSPhysicsReader.

   //This works around a msvc bug and should be harmless on other platforms
   typedef NPL::VTreeReader baseClass0;
   baseClass0::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TCATSPhysicsReader(void *p) {
      return  p ? new(p) ::TCATSPhysicsReader : new ::TCATSPhysicsReader;
   }
   static void *newArray_TCATSPhysicsReader(Long_t nElements, void *p) {
      return p ? new(p) ::TCATSPhysicsReader[nElements] : new ::TCATSPhysicsReader[nElements];
   }
   // Wrapper around operator delete
   static void delete_TCATSPhysicsReader(void *p) {
      delete (static_cast<::TCATSPhysicsReader*>(p));
   }
   static void deleteArray_TCATSPhysicsReader(void *p) {
      delete [] (static_cast<::TCATSPhysicsReader*>(p));
   }
   static void destruct_TCATSPhysicsReader(void *p) {
      typedef ::TCATSPhysicsReader current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TCATSPhysicsReader(TBuffer &buf, void *obj) {
      ((::TCATSPhysicsReader*)obj)->::TCATSPhysicsReader::Streamer(buf);
   }
} // end of namespace ROOT for class ::TCATSPhysicsReader

namespace {
  void TriggerDictionaryInitialization_TCATSPhysicsReaderDict_Impl() {
    static const char* headers[] = {
"TCATSPhysicsReader.h",
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
#line 1 "TCATSPhysicsReaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TCATSPhysicsReader.h")))  TCATSPhysicsReader;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TCATSPhysicsReaderDict dictionary payload"

#ifndef NPMULTITHREADING
  #define NPMULTITHREADING 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TCATSPhysicsReader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TCATSPhysicsReader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TCATSPhysicsReaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TCATSPhysicsReaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TCATSPhysicsReaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TCATSPhysicsReaderDict() {
  TriggerDictionaryInitialization_TCATSPhysicsReaderDict_Impl();
}
