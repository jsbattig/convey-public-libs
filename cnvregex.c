#include "cnvregex.h"
#include <assert.h>

typedef char* (__cdecl *RegExpr_Create_t)(void ** _this);
typedef char* (__cdecl *RegExpr_Free_t)(void* _this);
typedef char* (__cdecl *RegExpr_SetInputString_t)(void* _this, char* AInputString);
typedef char* (__cdecl *RegExpr_SetExpression_t)(void* _this, char* AExpression);
typedef char* (__cdecl *RegExpr_Exec_t)(void* _this, BOOL* AMatch);
typedef char* (__cdecl *RegExpr_ExecNext_t)(void* _this, BOOL* AMore);
typedef void (__cdecl *OverrideMemAllocator_t)(GetMem_t pGetMem, ReallocMem_t pReallocMem, FreeMem_t pFreeMem);
typedef void (__cdecl *ResetMemAllocator_t)();

static HMODULE RegExLib = 0;

static RegExpr_Create_t RegExpr_CreateFn = NULL;
static RegExpr_Free_t RegExpr_FreeFn = NULL;
static RegExpr_SetInputString_t RegExpr_SetInputStringFn = NULL;
static RegExpr_SetExpression_t RegExpr_SetExpressionFn = NULL;
static RegExpr_Exec_t RegExpr_ExecFn = NULL;
static RegExpr_ExecNext_t RegExpr_ExecNextFn = NULL;
static OverrideMemAllocator_t OverrideMemAllocatorFn = NULL;
static ResetMemAllocator_t ResetMemAllocatorFn = NULL;

#define INIT_DLL_FUNCTION(FNNAME) \
  FNNAME##Fn = (FNNAME##_t)GetProcAddress(RegExLib, #FNNAME); \
  assert(FNNAME##Fn)

#define DONE_DLL_FUNCTION(FNNAME) FNNAME##Fn = NULL 

void InitCnvRegEx() {
  RegExLib = LoadLibraryA("cnvregex.dll");

  INIT_DLL_FUNCTION(RegExpr_Create);
  INIT_DLL_FUNCTION(RegExpr_Free);
  INIT_DLL_FUNCTION(RegExpr_SetInputString);
  INIT_DLL_FUNCTION(RegExpr_SetExpression);
  INIT_DLL_FUNCTION(RegExpr_Exec);
  INIT_DLL_FUNCTION(RegExpr_ExecNext);
  INIT_DLL_FUNCTION(OverrideMemAllocator);
  INIT_DLL_FUNCTION(ResetMemAllocator);
}

void DoneCnvRegEx() {
  DONE_DLL_FUNCTION(RegExpr_Create);
  DONE_DLL_FUNCTION(RegExpr_Free);
  DONE_DLL_FUNCTION(RegExpr_SetInputString);
  DONE_DLL_FUNCTION(RegExpr_SetExpression);
  DONE_DLL_FUNCTION(RegExpr_Exec);
  DONE_DLL_FUNCTION(RegExpr_ExecNext);
  DONE_DLL_FUNCTION(OverrideMemAllocator);
  DONE_DLL_FUNCTION(ResetMemAllocator);

  FreeLibrary(RegExLib);
}

void OverrideMemAllocator(GetMem_t pGetMem, ReallocMem_t pReallocMem, FreeMem_t pFreeMem) {
  assert(OverrideMemAllocatorFn);
  OverrideMemAllocatorFn(pGetMem, pReallocMem, pFreeMem);
}

void ResetMemAllocator() {
  assert(ResetMemAllocatorFn);
  ResetMemAllocatorFn();
}

char* RegExpr_Create(void ** _this) {
  assert(RegExpr_CreateFn);
  return RegExpr_CreateFn(_this);
}

char* RegExpr_Free(void* _this) {
  assert(RegExpr_FreeFn);
  return RegExpr_FreeFn(_this);
}

char* RegExpr_SetInputString(void* _this, char* AInputString) {
  assert(RegExpr_SetInputStringFn);
  return RegExpr_SetInputStringFn(_this, AInputString);
}

char* RegExpr_SetExpression(void* _this, char* AExpression) {
  assert(RegExpr_SetExpressionFn);
  return RegExpr_SetExpressionFn(_this, AExpression);
}

char* RegExpr_Exec(void* _this, BOOL* AMatch) {
  assert(RegExpr_ExecFn);
  return RegExpr_ExecFn(_this, AMatch);
}

char* RegExpr_ExecNext(void* _this, BOOL* AMore) {
  assert(RegExpr_ExecNextFn);
  return RegExpr_ExecNextFn(_this, AMore);
}