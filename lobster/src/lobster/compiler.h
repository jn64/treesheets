// Copyright 2014 Wouter van Oortmerssen. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LOBSTER_COMPILER
#define LOBSTER_COMPILER

#include "lobster/natreg.h"

namespace lobster {

enum { RUNTIME_NO_ASSERT, RUNTIME_ASSERT, RUNTIME_ASSERT_PLUS };

extern void Compile(NativeRegistry &natreg, string_view fn, string_view stringsource,
                    string &bytecode, string *parsedump, string *pakfile,
                    bool return_value, int runtime_checks);

extern string RunTCC(NativeRegistry &nfr,
                     string_view bytecode_buffer,
                     string_view fn,
                     const char *object_name /* save instead of run if non-null */,
                     vector<string> &&program_args,
                     TraceMode trace,
                     bool compile_only,
                     string &error);

extern bool LoadPakDir(const char *lpak);
extern bool LoadByteCode(string &bytecode);
extern void RegisterBuiltin(NativeRegistry &natreg, const char *name,
                            void (* regfun)(NativeRegistry &));
extern void RegisterCoreLanguageBuiltins(NativeRegistry &natreg);

extern FileLoader EnginePreInit(NativeRegistry &nfr);
extern "C" int RunCompiledCodeMain(int argc, const char * const *argv, const uint8_t *bytecodefb,
                                   size_t static_size, const lobster::fun_base_t *vtables);

extern void DumpBuiltinNames(NativeRegistry &nfr);
extern void DumpBuiltinDoc(NativeRegistry &nfr);

}  // namespace lobster

#endif  // LOBSTER_COMPILER
