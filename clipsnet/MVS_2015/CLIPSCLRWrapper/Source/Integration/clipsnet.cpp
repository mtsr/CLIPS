#include "clipscpp.h"
#include <msclr\gcroot.h>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::IO;
using namespace CLIPS;

namespace CLIPSNET
  {
   class CLIPSCPPRouterBridge;

   /*###################*/
   /* Utility functions */
   /*###################*/

   String ^ CharStarToString(
     const char *theString)
     {
	  if (theString == NULL)
	    { return nullptr; }

	  return gcnew String(theString,0, (int) strlen(theString), UTF8Encoding::UTF8);
	 }

   std::string StringToCPPString( String ^theString )
     {
      array<Byte>^ encodedBytes = Encoding::UTF8->GetBytes(theString);
      pin_ptr<Byte> pinnedBytes = &encodedBytes[0];
      return std::string( (char *) pinnedBytes );
     }

   /*##################################*/
   /* PrimitiveValue Class declaration */
   /*##################################*/
 
   public ref class PrimitiveValue abstract
     {
      protected:
	    PrimitiveValue();
		~PrimitiveValue();

      public:
		 virtual String^ ToString() override;
	 };

   /*#############################*/
   /* VoidValue Class declaration */
   /*#############################*/

   public ref class VoidValue : PrimitiveValue
     {
      public:
	    VoidValue();
		~VoidValue();
		virtual String^ ToString() override;
	 };

   /*###############################*/
   /* LexemeValue Class declaration */
   /*###############################*/

   public ref class LexemeValue abstract : PrimitiveValue
     {
	  private:
	    String ^ value;

      protected:
	    LexemeValue();
	    LexemeValue(String ^ theString);
		~LexemeValue();

      public:
	     String ^ GetLexemeValue();
		 virtual String^ ToString() override;
	 };

   /*###############################*/
   /* StringValue Class declaration */
   /*###############################*/

   public ref class StringValue : LexemeValue
     {
	  public:
	    StringValue();
	    StringValue(String ^ theString);
		~StringValue();

      public:
	     String ^ GetStringValue();
		 virtual String^ ToString() override;
	 };

   /*###############################*/
   /* SymbolValue Class declaration */
   /*###############################*/

   public ref class SymbolValue : LexemeValue
     {
	  public:
	    SymbolValue();
	    SymbolValue(String ^ theString);
		~SymbolValue();

      public:
	     String ^ GetSymbolValue();
		 virtual String^ ToString() override;
	 };

   /*#####################################*/
   /* InstanceNameValue Class declaration */
   /*#####################################*/

   public ref class InstanceNameValue : LexemeValue
     {
	  public:
	    InstanceNameValue();
	    InstanceNameValue(String ^ theString);
		~InstanceNameValue();

      public:
	     String ^ GetInstanceNameValue();
		 virtual String^ ToString() override;
	 };

   /*###############################*/
   /* NumberValue Class declaration */
   /*###############################*/

   public ref class NumberValue abstract : PrimitiveValue
     {
	  private:
	    double dValue;
	    long long lValue;

	  public:
	    NumberValue();
	    NumberValue(double theDouble);
	    NumberValue(long long theLong);
		~NumberValue();
        double NumberValue::GetFloatValue();
        long long NumberValue::GetIntegerValue();
        static operator long long ( NumberValue ^ val ) 
	      { return val->lValue; }
        static operator double ( NumberValue ^ val ) 
	      { return val->dValue; }
	 };

   /*################################*/
   /* IntegerValue Class declaration */
   /*################################*/

   public ref class IntegerValue : NumberValue
     {
	  public:
	    IntegerValue();
	    IntegerValue(long long theLong);
	    IntegerValue(double theDouble);
		~IntegerValue();

      public:
		 virtual String^ ToString() override;
	 };

   /*###############################*/
   /* FloatValue Class declaration */
   /*###############################*/

   public ref class FloatValue : NumberValue
     {
	  public:
	    FloatValue();
	    FloatValue(long long theLong);
	    FloatValue(double theDouble);
		~FloatValue();

      public:
		 virtual String^ ToString() override;
	 };

   /*###################################*/
   /* MultifieldValue Class declaration */
   /*###################################*/

   public ref class MultifieldValue : PrimitiveValue
     {
	  private:
	    List<PrimitiveValue ^> ^ listValue;

	  public:
	    MultifieldValue();
	    MultifieldValue(List<PrimitiveValue ^> ^theList);
		~MultifieldValue();
        property int Count
		  {
		   int get() { return listValue->Count; };
		  }

        property PrimitiveValue ^ default[int] 
		  {
           PrimitiveValue ^ get(int index) 
		     { return listValue[index]; }
		  }

      public:
		 virtual String^ ToString() override;
	 };

   /*####################################*/
   /* FactAddressValue Class declaration */
   /*####################################*/

   public ref class FactAddressValue : PrimitiveValue
     {
	  private:
		//Environment ^ owner;
		//void *m_factAddress;
		CLIPS::FactAddressValue *m_factAddressValue;

      protected:
		!FactAddressValue();

	  public:
	    FactAddressValue();
	    FactAddressValue(CLIPS::FactAddressValue *);
		~FactAddressValue();
		long long GetFactIndex();
		PrimitiveValue ^ GetFactSlot(String ^);

      public:
		 virtual String^ ToString() override;
	 };

   /*########################################*/
   /* InstanceAddressValue Class declaration */
   /*########################################*/

   public ref class InstanceAddressValue : PrimitiveValue
     {
	  private:
		CLIPS::InstanceAddressValue *m_instanceAddressValue;

      protected:
		!InstanceAddressValue();

	  public:
	    InstanceAddressValue();
	    InstanceAddressValue(CLIPS::InstanceAddressValue *);
		~InstanceAddressValue();
		String ^ GetInstanceName();
		PrimitiveValue ^ DirectGetSlot(String ^);

      public:
		 virtual String^ ToString() override;
	 };

   /*##########################*/
   /* Router class declaration */
   /*##########################*/

   public ref class Router
     {
      public:
        Router();
        ~Router();
		virtual bool Query(String ^ logicalName);
		virtual void Print(String ^ logicalName, String ^ printString);
		virtual int Getc(String ^ logicalName);
		virtual int Ungetc(String ^ logicalName,int theChar);
		CLIPSCPPRouterBridge * RouterBridge();

      protected:
        !Router();

      private:
        CLIPSCPPRouterBridge *m_RouterBridge;
	 };

   /*###################*/
   /* Environment class */
   /*###################*/

   public ref class Environment 
     {
      public:
        Environment();
        ~Environment();

        void CommandLoop();
        long long Run();
        long long Run(long long);
        void Reset();
        bool Build(String ^);
        int Load(String ^);
        void LoadFromString(String ^);
        bool LoadFromResource(String ^,String ^);
        PrimitiveValue ^ Eval(String ^);
        void AddRouter(String ^,int ,Router ^);
        FactAddressValue ^ AssertString(String ^);
        size_t InputBufferCount();
        virtual String^ ToString() override;

      protected:
        ! Environment();

      private:
        CLIPSCPPEnv *m_Env;
     };

   /*############################*/
   /* DataObjectUtilityFunctions */
   /*############################*/

   PrimitiveValue ^ SingleFieldToPrimitiveValue(
     CLIPSType theType,
	 Value *theValue)
     {
	  std::string *theCPPString;
	  const char *theCString;
	  CLIPS::StringValue *theStringValue;
	  CLIPS::SymbolValue *theSymbolValue;
	  CLIPS::InstanceNameValue *theInstanceNameValue;
	  CLIPS::IntegerValue *theIntegerValue;
	  CLIPS::FloatValue *theFloatValue;
	  CLIPS::FactAddressValue *theFactAddressValue;
	  CLIPS::InstanceAddressValue *theInstanceAddressValue;
	  PrimitiveValue ^ rv = nullptr;

	  switch (theType)
	    {
		 case STRING_TYPE:
		   theStringValue = (CLIPS::StringValue *) theValue;
		   theCPPString = theStringValue->GetStringValue();
		   theCString = theCPPString->c_str();
		   rv = gcnew StringValue(gcnew String(theCString));
		   break;

		 case SYMBOL_TYPE:
		   theSymbolValue = (CLIPS::SymbolValue *) theValue;
		   theCPPString = theSymbolValue->GetSymbolValue();
		   theCString = theCPPString->c_str();
		   rv = gcnew SymbolValue(gcnew String(theCString));
		   break;

		 case INSTANCE_NAME_TYPE:
		   theInstanceNameValue = (CLIPS::InstanceNameValue *) theValue;
		   theCPPString = theInstanceNameValue->GetInstanceNameValue();
		   theCString = theCPPString->c_str();
		   rv = gcnew InstanceNameValue(gcnew String(theCString));
		   break;

		 case INTEGER_TYPE:
		   theIntegerValue = (CLIPS::IntegerValue *) theValue;
		   rv = gcnew IntegerValue(theIntegerValue->GetIntegerValue());
		   break;

		 case FLOAT_TYPE:
		   theFloatValue = (CLIPS::FloatValue *) theValue;
		   rv = gcnew FloatValue(theFloatValue->GetFloatValue());
		   break;

		 case FACT_ADDRESS_TYPE:
		   theFactAddressValue = (CLIPS::FactAddressValue *) theValue;
		   rv = gcnew FactAddressValue(theFactAddressValue);
		   break;

		 case INSTANCE_ADDRESS_TYPE:
		   theInstanceAddressValue = (CLIPS::InstanceAddressValue *) theValue;
		   rv = gcnew InstanceAddressValue(theInstanceAddressValue);
		   break;

		 case VOID_TYPE:
		   rv = gcnew VoidValue();
		   break;
		}

	  return rv;
	 }

   PrimitiveValue ^ DataObjectToPrimitiveValue (DataObject theDO)
     {
	  CLIPSType theType = theDO.GetCLIPSType();
	  List<PrimitiveValue ^> ^ theList;
	  CLIPS::MultifieldValue *theMultifieldValue;
	  std::vector<Value *> *theMultifield;
	  Value *theValue;
	  PrimitiveValue ^ rv = nullptr;

	  switch (theType)
	    {
		 case STRING_TYPE:
		 case SYMBOL_TYPE:
		 case INSTANCE_NAME_TYPE:
		 case INTEGER_TYPE:
		 case FLOAT_TYPE:
		 case FACT_ADDRESS_TYPE:
		 case INSTANCE_ADDRESS_TYPE:
		 case VOID_TYPE:
		   rv = SingleFieldToPrimitiveValue(theType,theDO.GetCLIPSValue());
		   break;

		 case MULTIFIELD_TYPE:
		   theList = gcnew List<PrimitiveValue ^>;
		   theMultifieldValue = (CLIPS::MultifieldValue *) theDO.GetCLIPSValue();
		   theMultifield = theMultifieldValue->GetMultifieldValue();	
		   for (std::vector<Value *>::size_type i = 0; i != theMultifield->size(); i++) 
		      {
			   theValue = theMultifield->at(i);
			   theList->Add(SingleFieldToPrimitiveValue(theValue->GetCLIPSType(),theValue));
              }
		   rv = gcnew MultifieldValue(theList);
		   break;
		}

	  return rv;
	 }

   /*##############*/
   /* Data Objects */
   /*##############*/

   /**************************/
   /* PrimitiveValue Methods */
   /**************************/

   PrimitiveValue::PrimitiveValue()
     {
	 }

   PrimitiveValue::~PrimitiveValue()
     {
	 }
	 
   String ^ PrimitiveValue::ToString()
     {
	  return gcnew String(""); 
	 }

   /*************/
   /* VoidValue */
   /*************/

   VoidValue::VoidValue() 
     {
	 }

   VoidValue::~VoidValue()
     {
	 }

   String ^ VoidValue::ToString()
     {
	  return gcnew String(""); 
	 }

   /***************/
   /* LexemeValue */
   /***************/

   LexemeValue::LexemeValue() : value ( nullptr)
     {
	 }

   LexemeValue::LexemeValue(String ^ theString) : value (theString)
     {
	 }

   LexemeValue::~LexemeValue()
     {
	 }

   String ^ LexemeValue::GetLexemeValue()
     { return this->value; }

   String ^ LexemeValue::ToString()
     {
	  if (value != nullptr)
	    { return value->ToString(); }
		
	  return gcnew String(""); 
	 }

   /***************/
   /* StringValue */
   /***************/

   StringValue::StringValue() : LexemeValue(gcnew String(""))
     {
	 }

   StringValue::StringValue(String ^ theString) : LexemeValue (theString)
     {
	 }

   StringValue::~StringValue()
     {
	 }

   String ^ StringValue::GetStringValue()
     { return this->GetLexemeValue(); }

   String ^ StringValue::ToString()
     {
	  return gcnew String("\"" + LexemeValue::ToString() +"\""); 
	 }

   /***************/
   /* SymbolValue */
   /***************/

   SymbolValue::SymbolValue() : LexemeValue(gcnew String(""))
     {
	 }

   SymbolValue::SymbolValue(String ^ theString) : LexemeValue (theString)
     {
	 }

   SymbolValue::~SymbolValue()
     {
	 }

   String ^ SymbolValue::GetSymbolValue()
     { return this->GetLexemeValue(); }

   String ^ SymbolValue::ToString()
     {
	  return gcnew String(LexemeValue::ToString()); 
	 }

   /*********************/
   /* InstanceNameValue */
   /*********************/

   InstanceNameValue::InstanceNameValue() : LexemeValue(gcnew String(""))
     {
	 }

   InstanceNameValue::InstanceNameValue(String ^ theString) : LexemeValue (theString)
     {
	 }

   InstanceNameValue::~InstanceNameValue()
     {
	 }

   String ^ InstanceNameValue::GetInstanceNameValue()
     { return this->GetLexemeValue(); }

   String ^ InstanceNameValue::ToString()
     {
	  return gcnew String("[" + LexemeValue::ToString() + "]"); 
	 }

   /***************/
   /* NumberValue */
   /***************/

   NumberValue::NumberValue() 
     {
	  dValue = 0.0;
	  lValue = 0;
	 }

   NumberValue::NumberValue(double theDouble)
     {
	  dValue = theDouble;
	  lValue = (long long) theDouble;
	 }

   NumberValue::NumberValue(long long theLong)
     {
	  dValue = (double) theLong;
	  lValue = theLong;
	 }

   NumberValue::~NumberValue()
     {
	 }

   double NumberValue::GetFloatValue()
     { return this->dValue; }

   long long NumberValue::GetIntegerValue()
     { return this->lValue; }

   /****************/
   /* IntegerValue */
   /****************/

   IntegerValue::IntegerValue() : NumberValue()
     {
	 }

   IntegerValue::IntegerValue(double theDouble) : NumberValue (theDouble)
     {
	 }

   IntegerValue::IntegerValue(long long theLong) : NumberValue (theLong)
     {
	 }

   IntegerValue::~IntegerValue()
     {
	 }
 
   String ^ IntegerValue::ToString()
     {
	  return this->GetIntegerValue().ToString();
	 }

   /**************/
   /* FloatValue */
   /**************/

   FloatValue::FloatValue() : NumberValue()
     {
	 }

   FloatValue::FloatValue(double theDouble) : NumberValue (theDouble)
     {
	 }

   FloatValue::FloatValue(long long theLong) : NumberValue (theLong)
     {
	 }

   FloatValue::~FloatValue()
     {
	 }
 
   String ^ FloatValue::ToString()
     {
	  return this->GetFloatValue().ToString();
	 }

   /*******************/
   /* MultifieldValue */
   /*******************/

   MultifieldValue::MultifieldValue() 
     {
	  this->listValue = nullptr;
	 }
 
   MultifieldValue::MultifieldValue(List<PrimitiveValue ^> ^ theList)
     {
	  this->listValue = theList;
	 }

   MultifieldValue::~MultifieldValue()
     {
	 }

   String ^ MultifieldValue::ToString()
     {
      bool first = true;
	  String ^ theString;

	  theString = gcnew String("(");

	  for (int i = 0; i < this->listValue->Count; i++)
	    {
		 if (first)
		   { first = false; }
	     else
	 	   { theString = theString->Concat(theString," "); }
		 theString = theString->Concat(theString,this->listValue[i]);
		}
	  
	  theString = theString->Concat(theString,")");

	  return theString;
	 }

   /********************/
   /* FactAddressValue */
   /********************/

   FactAddressValue::FactAddressValue(
     CLIPS::FactAddressValue *theFactAddressValue) 
     {
	  m_factAddressValue = theFactAddressValue->clone();
	 }

   FactAddressValue::FactAddressValue() 
     {
	  m_factAddressValue = NULL;
	 }

   FactAddressValue::~FactAddressValue()
     {
	  this->!FactAddressValue(); 
	 }

   FactAddressValue::!FactAddressValue()
     {
	  if (m_factAddressValue != NULL)
	    { delete m_factAddressValue; }
	 }
 
   long long FactAddressValue::GetFactIndex()
     {
	  if (m_factAddressValue != NULL)
	    { return m_factAddressValue->GetFactIndex(); }

	  return 0;
	 }

   PrimitiveValue ^ FactAddressValue::GetFactSlot(
     String ^ slotName)
     {
	  array<Byte>^ ebSlotName = Encoding::UTF8->GetBytes(slotName);
      pin_ptr<Byte> pbSlotName = &ebSlotName[0];

      return DataObjectToPrimitiveValue(m_factAddressValue->GetFactSlot((char *) pbSlotName));
	 }

   String ^ FactAddressValue::ToString()
     {
	  return gcnew String("<Fact-" + this->GetFactIndex() + ">");
	 }

   /************************/
   /* InstanceAddressValue */
   /************************/

   InstanceAddressValue::InstanceAddressValue(
     CLIPS::InstanceAddressValue *theInstanceAddressValue) 
     {
	  m_instanceAddressValue = theInstanceAddressValue->clone();
	 }

   InstanceAddressValue::InstanceAddressValue() 
     {
	  m_instanceAddressValue = NULL;
	 }

   InstanceAddressValue::~InstanceAddressValue()
     {
	  this->!InstanceAddressValue(); 
	 }

   InstanceAddressValue::!InstanceAddressValue()
     {
	  if (m_instanceAddressValue != NULL)
	    { delete m_instanceAddressValue; }
	 }

  String ^ InstanceAddressValue::GetInstanceName()
     {
	  return gcnew String(m_instanceAddressValue->GetInstanceName());
	 }

   PrimitiveValue ^ InstanceAddressValue::DirectGetSlot(
     String ^ slotName)
     {
	  array<Byte>^ ebSlotName = Encoding::UTF8->GetBytes(slotName);
      pin_ptr<Byte> pbSlotName = &ebSlotName[0];

      return DataObjectToPrimitiveValue(m_instanceAddressValue->DirectGetSlot((char *) pbSlotName));
	 }

  String ^ InstanceAddressValue::ToString()
     {
	  return gcnew String("<Instance-" + this->GetInstanceName() + ">");
	 }

   /*##################################*/
   /* CLIPSCPPRouterBridge declaration */
   /*##################################*/

   class CLIPSCPPRouterBridge : private CLIPSCPPRouter
     {
	  public:
         CLIPSCPPRouterBridge();
         CLIPSCPPRouterBridge(msclr::gcroot<Router^>);
         ~CLIPSCPPRouterBridge();

		 int Query(CLIPSCPPEnv *,const char *);
		 int Print(CLIPSCPPEnv *,const char *,const char *);
		 int Getc(CLIPSCPPEnv *,const char *);
		 int Ungetc(CLIPSCPPEnv *,int,const char *);

      private:
         msclr::gcroot<Router^> m_Router;
	 };

   /*####################################*/
   /* CLIPSCPPRouterBridge class methods */
   /*####################################*/

   CLIPSCPPRouterBridge::CLIPSCPPRouterBridge() {}
   
   CLIPSCPPRouterBridge::CLIPSCPPRouterBridge(msclr::gcroot<Router^> the_Router) 
     { m_Router = the_Router; }
   
   CLIPSCPPRouterBridge::~CLIPSCPPRouterBridge() {}

   int CLIPSCPPRouterBridge::Query(
      CLIPSCPPEnv *theCPPEnv,
      const char *logicalName)
      {
       String ^ cliLogicalName = CharStarToString(logicalName);

       return m_Router->Query(cliLogicalName);
      }

   int CLIPSCPPRouterBridge::Print(
      CLIPSCPPEnv *theCPPEnv,
      const char *logicalName,
	  const char *printString)
      {
       String ^ cliLogicalName = CharStarToString(logicalName);
       String ^ cliPrintString = CharStarToString(printString);

       m_Router->Print(cliLogicalName,cliPrintString);

	   return true;
      }

   int CLIPSCPPRouterBridge::Getc(
      CLIPSCPPEnv *theCPPEnv,
      const char *logicalName)
      {
       String ^ cliLogicalName = CharStarToString(logicalName);

       return m_Router->Getc(cliLogicalName);
      }

   int CLIPSCPPRouterBridge::Ungetc(
      CLIPSCPPEnv *theCPPEnv,
	  int theChar,
      const char *logicalName)
      {
       String ^ cliLogicalName = CharStarToString(logicalName);

       return m_Router->Ungetc(cliLogicalName,theChar);
      }

   /*######################*/
   /* Router class methods */
   /*######################*/

   Router::Router() 
     {
	  m_RouterBridge = new CLIPSCPPRouterBridge(this);
     }

   Router::~Router()
     { this->!Router(); }

   bool Router::Query(String ^ logicalName)
     {
	  return false; 
	 }

   void Router::Print(String ^ logicalName,String ^printString)
     {
	 }

   int Router::Getc(String ^ logicalName)
     {
	  return -1;
	 }

   int Router::Ungetc(String ^ logicalName,int theChar)
     {
	  return 0;
	 }

   CLIPSCPPRouterBridge * Router::RouterBridge()
	 { return m_RouterBridge; }

   Router::!Router()
	 { delete m_RouterBridge; }
	 
   /*###########################*/
   /* Environment class methods */
   /*###########################*/

   Environment::Environment() : m_Env( new CLIPSCPPEnv ) {}
   
   Environment::~Environment()
     {
      this->!Environment(); 
     }

   void Environment::CommandLoop()
     { return m_Env->CommandLoop(); }

   long long Environment::Run()
     { return m_Env->Run(-1LL); }

   long long Environment::Run(
     long long runLimit)
     { return m_Env->Run(runLimit); }

   void Environment::Reset()
     { return m_Env->Reset(); }

   int Environment::Load(
     String ^ fileName)
     {
	  array<Byte>^ ebFileName = Encoding::UTF8->GetBytes(fileName);
      pin_ptr<Byte> pbFileName = &ebFileName[0];

	  return m_Env->Load((char *) pbFileName);
     }

   void Environment::LoadFromString(
     String ^ loadString)
     {
	  array<Byte>^ ebLoadString = Encoding::UTF8->GetBytes(loadString);
      pin_ptr<Byte> pbLoadString = &ebLoadString[0];

	  m_Env->LoadFromString((char *) pbLoadString);
     }

   bool Environment::LoadFromResource(
     String ^ assemblyName,
     String ^ resourceName)
     {
	  Assembly ^ assembly = System::Reflection::Assembly::Load(assemblyName);
	  Stream ^ stream = assembly->GetManifestResourceStream(resourceName);
	  if (stream == nullptr) return false;
	  StreamReader ^ reader = gcnew StreamReader(stream);
	  String ^ resourceContent = reader->ReadToEnd();

      this->LoadFromString(resourceContent);
	  return true;
     }

   bool Environment::Build(
     String ^ buildString)
     {
	  array<Byte>^ ebBuildString = Encoding::UTF8->GetBytes(buildString);
      pin_ptr<Byte> pbBuildString = &ebBuildString[0];

	  return m_Env->Build((char *) pbBuildString);
     }

   PrimitiveValue ^ Environment::Eval(
     String ^ evalString)
     {
	  array<Byte>^ ebEvalString = Encoding::UTF8->GetBytes(evalString);
      pin_ptr<Byte> pbEvalString = &ebEvalString[0];
	  PrimitiveValue ^ rv;

      rv = DataObjectToPrimitiveValue(m_Env->Eval((char *) pbEvalString));

	  return rv;
     }

   FactAddressValue ^ Environment::AssertString(
     String ^ factString)
     {
	  array<Byte>^ ebFactString = Encoding::UTF8->GetBytes(factString);
      pin_ptr<Byte> pbFactString = &ebFactString[0];
	  CLIPS::FactAddressValue *frv;

	  frv = m_Env->AssertString((char *) pbFactString);
	  if (frv == NULL) return (nullptr);
      
	  return gcnew FactAddressValue(frv);
     }

   void Environment::AddRouter(
	 String ^ routerName,
	 int priority,
	 Router ^ theRouter)
     { 
	  array<Byte>^ ebRouterName = Encoding::UTF8->GetBytes(routerName);
      pin_ptr<Byte> pbRouterName = &ebRouterName[0];

	  m_Env->AddRouter((char *) pbRouterName,priority,(CLIPS::CLIPSCPPRouter *) theRouter->RouterBridge());
     }

   size_t Environment::InputBufferCount()
     { 
      return m_Env->InputBufferCount();
     }   
     
   String ^ Environment::ToString()
     {
      long long envLong = (long long) m_Env;

      return gcnew String("<Environment-" + envLong + ">");
     }

   Environment::! Environment()
     { delete m_Env; }

  };