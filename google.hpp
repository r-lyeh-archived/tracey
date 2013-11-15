#pragma once

/*
#include <windows.h>
#include <map>
#include <mutex>
#include "tracey.hpp"
#include <iostream>
#include <cassert>
*/

#define CHECK(...) do { if(!(__VA_ARGS__)) assert( !#__VA_ARGS__ ); } while(0)
#define DCHECK CHECK
#define DCHECK_EQ(A,B) DCHECK( (A) == (B) )
#define int32 int
#define VLOG(x) ( std::cout )
#define PERFTOOLS_DLL_DECL

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Static class for hooking Win32 API routines.  For now,
// we only add one watcher at a time.
//
// TODO(mbelshe):  Support multiple watchers.

#ifndef MEMORY_WATCHER_MEMORY_HOOK_
#define MEMORY_WATCHER_MEMORY_HOOK_

//#include "base.h"

/*
// MemoryHook provides a private heap for allocating
// unwatched memory.
struct MemoryHook {

  HANDLE heap_;

   MemoryHook();
  ~MemoryHook();

  // MemoryObserver interface.
  static void trace(HANDLE heap, int32 id, int32 size);
  static void untrace(HANDLE heap, int32 id, int32 size);
  // Safe allocations
  static void* Alloc(size_t size);
  static void Free(void* ptr);
};
*/

#endif  // MEMORY_WATCHER_MEMORY_HOOK_
// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
/* Copyright (c) 2007, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Joi Sigurdsson
 *
 * Several simple types used by the disassembler and some of the patching
 * mechanisms.
 */

#ifndef GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_TYPES_H_
#define GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_TYPES_H_

namespace sidestep {

// Categories of instructions that we care about
enum InstructionType {
  // This opcode is not used
  IT_UNUSED,
  // This disassembler does not recognize this opcode (error)
  IT_UNKNOWN,
  // This is not an instruction but a reference to another table
  IT_REFERENCE,
  // This byte is a prefix byte that we can ignore
  IT_PREFIX,
  // This is a prefix byte that switches to the nondefault address size
  IT_PREFIX_ADDRESS,
  // This is a prefix byte that switches to the nondefault operand size
  IT_PREFIX_OPERAND,
  // A jump or call instruction
  IT_JUMP,
  // A return instruction
  IT_RETURN,
  // Any other type of instruction (in this case we don't care what it is)
  IT_GENERIC,
};

// Lists IA-32 operand sizes in multiples of 8 bits
enum OperandSize {
  OS_ZERO = 0,
  OS_BYTE = 1,
  OS_WORD = 2,
  OS_DOUBLE_WORD = 4,
  OS_QUAD_WORD = 8,
  OS_DOUBLE_QUAD_WORD = 16,
  OS_32_BIT_POINTER = 32/8,
  OS_48_BIT_POINTER = 48/8,
  OS_SINGLE_PRECISION_FLOATING = 32/8,
  OS_DOUBLE_PRECISION_FLOATING = 64/8,
  OS_DOUBLE_EXTENDED_PRECISION_FLOATING = 80/8,
  OS_128_BIT_PACKED_SINGLE_PRECISION_FLOATING = 128/8,
  OS_PSEUDO_DESCRIPTOR = 6
};

// Operand addressing methods from the IA-32 manual.  The enAmMask value
// is a mask for the rest.  The other enumeration values are named for the
// names given to the addressing methods in the manual, e.g. enAm_D is for
// the D addressing method.
//
// The reason we use a full 4 bytes and a mask, is that we need to combine
// these flags with the enOperandType to store the details
// on the operand in a single integer.
enum AddressingMethod {
  AM_NOT_USED = 0,        // This operand is not used for this instruction
  AM_MASK = 0x00FF0000,  // Mask for the rest of the values in this enumeration
  AM_A = 0x00010000,    // A addressing type
  AM_C = 0x00020000,    // C addressing type
  AM_D = 0x00030000,    // D addressing type
  AM_E = 0x00040000,    // E addressing type
  AM_F = 0x00050000,    // F addressing type
  AM_G = 0x00060000,    // G addressing type
  AM_I = 0x00070000,    // I addressing type
  AM_J = 0x00080000,    // J addressing type
  AM_M = 0x00090000,    // M addressing type
  AM_O = 0x000A0000,    // O addressing type
  AM_P = 0x000B0000,    // P addressing type
  AM_Q = 0x000C0000,    // Q addressing type
  AM_R = 0x000D0000,    // R addressing type
  AM_S = 0x000E0000,    // S addressing type
  AM_T = 0x000F0000,    // T addressing type
  AM_V = 0x00100000,    // V addressing type
  AM_W = 0x00110000,    // W addressing type
  AM_X = 0x00120000,    // X addressing type
  AM_Y = 0x00130000,    // Y addressing type
  AM_REGISTER = 0x00140000,  // Specific register is always used as this op
  AM_IMPLICIT = 0x00150000,  // An implicit, fixed value is used
};

// Operand types from the IA-32 manual. The enOtMask value is
// a mask for the rest. The rest of the values are named for the
// names given to these operand types in the manual, e.g. enOt_ps
// is for the ps operand type in the manual.
//
// The reason we use a full 4 bytes and a mask, is that we need
// to combine these flags with the enAddressingMethod to store the details
// on the operand in a single integer.
enum OperandType {
  OT_MASK = 0xFF000000,
  OT_A = 0x01000000,
  OT_B = 0x02000000,
  OT_C = 0x03000000,
  OT_D = 0x04000000,
  OT_DQ = 0x05000000,
  OT_P = 0x06000000,
  OT_PI = 0x07000000,
  OT_PS = 0x08000000,  // actually unsupported for (we don't know its size)
  OT_Q = 0x09000000,
  OT_S = 0x0A000000,
  OT_SS = 0x0B000000,
  OT_SI = 0x0C000000,
  OT_V = 0x0D000000,
  OT_W = 0x0E000000,
  OT_SD = 0x0F000000,  // scalar double-precision floating-point value
  OT_PD = 0x10000000,  // double-precision floating point
  // dummy "operand type" for address mode M - which doesn't specify
  // operand type
  OT_ADDRESS_MODE_M = 0x80000000
};

// Flag that indicates if an immediate operand is 64-bits.
//
// The Intel 64 and IA-32 Architecture Software Developer's Manual currently
// defines MOV as the only instruction supporting a 64-bit immediate operand.
enum ImmediateOperandSize {
  IOS_MASK = 0x0000F000,
  IOS_DEFAULT = 0x0,
  IOS_64 = 0x00001000
};

// Everything that's in an Opcode (see below) except the three
// alternative opcode structs for different prefixes.
struct SpecificOpcode {
  // Index to continuation table, or 0 if this is the last
  // byte in the opcode.
  int table_index_;

  // The opcode type
  InstructionType type_;

  // Description of the type of the dest, src and aux operands,
  // put together from enOperandType, enAddressingMethod and
  // enImmediateOperandSize flags.
  int flag_dest_;
  int flag_source_;
  int flag_aux_;

  // We indicate the mnemonic for debugging purposes
  const char* mnemonic_;
};

// The information we keep in our tables about each of the different
// valid instructions recognized by the IA-32 architecture.
struct Opcode {
  // Index to continuation table, or 0 if this is the last
  // byte in the opcode.
  int table_index_;

  // The opcode type
  InstructionType type_;

  // Description of the type of the dest, src and aux operands,
  // put together from an enOperandType flag and an enAddressingMethod
  // flag.
  int flag_dest_;
  int flag_source_;
  int flag_aux_;

  // We indicate the mnemonic for debugging purposes
  const char* mnemonic_;

  // Alternative opcode info if certain prefixes are specified.
  // In most cases, all of these are zeroed-out.  Only used if
  // bPrefixDependent is true.
  bool is_prefix_dependent_;
  SpecificOpcode opcode_if_f2_prefix_;
  SpecificOpcode opcode_if_f3_prefix_;
  SpecificOpcode opcode_if_66_prefix_;
};

// Information about each table entry.
struct OpcodeTable {
  // Table of instruction entries
  const Opcode* table_;
  // How many bytes left to shift ModR/M byte <b>before</b> applying mask
  unsigned char shift_;
  // Mask to apply to byte being looked at before comparing to table
  unsigned char mask_;
  // Minimum/maximum indexes in table.
  unsigned char min_lim_;
  unsigned char max_lim_;
};

// Information about each entry in table used to decode ModR/M byte.
struct ModrmEntry {
  // Is the operand encoded as bytes in the instruction (rather than
  // if it's e.g. a register in which case it's just encoded in the
  // ModR/M byte)
  bool is_encoded_in_instruction_;

  // Is there a SIB byte?  In this case we always need to decode it.
  bool use_sib_byte_;

  // What is the size of the operand (only important if it's encoded
  // in the instruction)?
  OperandSize operand_size_;
};

};  // namespace sidestep

#endif  // GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_TYPES_H_// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
/* Copyright (c) 2007, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Joi Sigurdsson
 *
 * Definition of MiniDisassembler.
 */

#ifndef GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_H_
#define GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_H_

//#include <windows.h>

//#include "base.h"
//#include "mini_disassembler_types.h"

// compatibility shim
#define SIDESTEP_ASSERT(cond)  RAW_DCHECK(cond, #cond)
#define SIDESTEP_LOG(msg)      RAW_VLOG(1, msg)

namespace sidestep {

// This small disassembler is very limited
// in its functionality, and in fact does only the bare minimum required by the
// preamble patching utility.  It may be useful for other purposes, however.
//
// The limitations include at least the following:
//  -# No support for coprocessor opcodes, MMX, etc.
//  -# No machine-readable identification of opcodes or decoding of
//     assembly parameters. The name of the opcode (as a string) is given,
//     however, to aid debugging.
//
// You may ask what this little disassembler actually does, then?  The answer is
// that it does the following, which is exactly what the patching utility needs:
//  -# Indicates if opcode is a jump (any kind) or a return (any kind)
//     because this is important for the patching utility to determine if
//     a function is too short or there are jumps too early in it for it
//     to be preamble patched.
//  -# The opcode length is always calculated, so that the patching utility
//     can figure out where the next instruction starts, and whether it
//     already has enough instructions to replace with the absolute jump
//     to the patching code.
//
// The usage is quite simple; just create a MiniDisassembler and use its
// Disassemble() method.
//
// If you would like to extend this disassembler, please refer to the
// IA-32 Intel® Architecture Software Developer’s Manual Volume 2:
// Instruction Set Reference for information about operand decoding
// etc.
class PERFTOOLS_DLL_DECL MiniDisassembler {
 public:

  // Creates a new instance and sets defaults.
  //
  // @param operand_default_32_bits If true, the default operand size is
  // set to 32 bits, which is the default under Win32. Otherwise it is 16 bits.
  // @param address_default_32_bits If true, the default address size is
  // set to 32 bits, which is the default under Win32. Otherwise it is 16 bits.
  MiniDisassembler(bool operand_default_32_bits,
                   bool address_default_32_bits);

  // Equivalent to MiniDisassembler(true, true);
  MiniDisassembler();

  // Attempts to disassemble a single instruction starting from the
  // address in memory it is pointed to.
  //
  // @param start Address where disassembly should start.
  // @param instruction_bytes Variable that will be <b>incremented</b> by
  // the length in bytes of the instruction.
  // @return enItJump, enItReturn or enItGeneric on success.  enItUnknown
  // if unable to disassemble, enItUnused if this seems to be an unused
  // opcode. In the last two (error) cases, cbInstruction will be set
  // to 0xffffffff.
  //
  // @post This instance of the disassembler is ready to be used again,
  // with unchanged defaults from creation time.
  InstructionType Disassemble(unsigned char* start, unsigned int& instruction_bytes);

 private:

  // Makes the disassembler ready for reuse.
  void Initialize();

  // Sets the flags for address and operand sizes.
  // @return Number of prefix bytes.
  InstructionType ProcessPrefixes(unsigned char* start, unsigned int& size);

  // Sets the flag for whether we have ModR/M, and increments
  // operand_bytes_ if any are specifies by the opcode directly.
  // @return Number of opcode bytes.
  InstructionType ProcessOpcode(unsigned char* start,
                                unsigned int table,
                                unsigned int& size);

  // Checks the type of the supplied operand.  Increments
  // operand_bytes_ if it directly indicates an immediate etc.
  // operand.  Asserts have_modrm_ if the operand specifies
  // a ModR/M byte.
  bool ProcessOperand(int flag_operand);

  // Increments operand_bytes_ by size specified by ModR/M and
  // by SIB if present.
  // @return 0 in case of error, 1 if there is just a ModR/M byte,
  // 2 if there is a ModR/M byte and a SIB byte.
  bool ProcessModrm(unsigned char* start, unsigned int& size);

  // Processes the SIB byte that it is pointed to.
  // @param start Pointer to the SIB byte.
  // @param mod The mod field from the ModR/M byte.
  // @return 1 to indicate success (indicates 1 SIB byte)
  bool ProcessSib(unsigned char* start, unsigned char mod, unsigned int& size);

  // The instruction type we have decoded from the opcode.
  InstructionType instruction_type_;

  // Counts the number of bytes that is occupied by operands in
  // the current instruction (note: we don't care about how large
  // operands stored in registers etc. are).
  unsigned int operand_bytes_;

  // True iff there is a ModR/M byte in this instruction.
  bool have_modrm_;

  // True iff we need to decode the ModR/M byte (sometimes it just
  // points to a register, we can tell by the addressing mode).
  bool should_decode_modrm_;

  // Current operand size is 32 bits if true, 16 bits if false.
  bool operand_is_32_bits_;

  // Default operand size is 32 bits if true, 16 bits if false.
  bool operand_default_is_32_bits_;

  // Current address size is 32 bits if true, 16 bits if false.
  bool address_is_32_bits_;

  // Default address size is 32 bits if true, 16 bits if false.
  bool address_default_is_32_bits_;

  // Determines if 64 bit operands are supported (x64).
  bool operand_default_support_64_bits_;

  // Current operand size is 64 bits if true, 32 bits if false.
  bool operand_is_64_bits_;

  // Huge big opcode table based on the IA-32 manual, defined
  // in Ia32OpcodeMap.cc
  static const OpcodeTable s_ia32_opcode_map_[];

  // Somewhat smaller table to help with decoding ModR/M bytes
  // when 16-bit addressing mode is being used.  Defined in
  // Ia32ModrmMap.cc
  static const ModrmEntry s_ia16_modrm_map_[];

  // Somewhat smaller table to help with decoding ModR/M bytes
  // when 32-bit addressing mode is being used.  Defined in
  // Ia32ModrmMap.cc
  static const ModrmEntry s_ia32_modrm_map_[];

  // Indicators of whether we got certain prefixes that certain
  // silly Intel instructions depend on in nonstandard ways for
  // their behaviors.
  bool got_f2_prefix_, got_f3_prefix_, got_66_prefix_;
};

};  // namespace sidestep

#endif  // GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_H_
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/*
 * Definition of PreamblePatcher
 */

#ifndef MEMORY_WATCHER_PREAMBLE_PATCHER_H__
#define MEMORY_WATCHER_PREAMBLE_PATCHER_H__

//#include <windows.h>

// compatibility shim
//#include "base.h"
#define ASSERT(cond, msg)  DCHECK(cond)
#define ASSERT1(cond)      DCHECK(cond)

// Maximum size of the preamble stub. We overwrite at least the first 5
// bytes of the function. Considering the worst case scenario, we need 4
// bytes + the max instruction size + 5 more bytes for our jump back to
// the original code. With that in mind, 32 is a good number :)
#define MAX_PREAMBLE_STUB_SIZE    (32)

namespace sidestep {

// Possible results of patching/unpatching
enum SideStepError {
  SIDESTEP_SUCCESS = 0,
  SIDESTEP_INVALID_PARAMETER,
  SIDESTEP_INSUFFICIENT_BUFFER,
  SIDESTEP_JUMP_INSTRUCTION,
  SIDESTEP_FUNCTION_TOO_SMALL,
  SIDESTEP_UNSUPPORTED_INSTRUCTION,
  SIDESTEP_NO_SUCH_MODULE,
  SIDESTEP_NO_SUCH_FUNCTION,
  SIDESTEP_ACCESS_DENIED,
  SIDESTEP_UNEXPECTED,
};

#define SIDESTEP_TO_HRESULT(error) \
  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, error)

// Implements a patching mechanism that overwrites the first few bytes of
// a function preamble with a jump to our hook function, which is then
// able to call the original function via a specially-made preamble-stub
// that imitates the action of the original preamble.
//
// NOTE:  This patching mechanism should currently only be used for
// non-production code, e.g. unit tests, because it is not threadsafe.
// See the TODO in preamble_patcher_with_stub.cc for instructions on what
// we need to do before using it in production code; it's fairly simple
// but unnecessary for now since we only intend to use it in unit tests.
//
// To patch a function, use either of the typesafe Patch() methods.  You
// can unpatch a function using Unpatch().
//
// Typical usage goes something like this:
// @code
// typedef int (*MyTypesafeFuncPtr)(int x);
// MyTypesafeFuncPtr original_func_stub;
// int MyTypesafeFunc(int x) { return x + 1; }
// int HookMyTypesafeFunc(int x) { return 1 + original_func_stub(x); }
//
// void MyPatchInitializingFunction() {
//   original_func_stub = PreamblePatcher::Patch(
//              MyTypesafeFunc, HookMyTypesafeFunc);
//   if (!original_func_stub) {
//     // ... error handling ...
//   }
//
//   // ... continue - you have patched the function successfully ...
// }
// @endcode
//
// Note that there are a number of ways that this method of patching can
// fail.  The most common are:
//    - If there is a jump (jxx) instruction in the first 5 bytes of
//    the function being patched, we cannot patch it because in the
//    current implementation we do not know how to rewrite relative
//    jumps after relocating them to the preamble-stub.  Note that
//    if you really really need to patch a function like this, it
//    would be possible to add this functionality (but at some cost).
//    - If there is a return (ret) instruction in the first 5 bytes
//    we cannot patch the function because it may not be long enough
//    for the jmp instruction we use to inject our patch.
//    - If there is another thread currently executing within the bytes
//    that are copied to the preamble stub, it will crash in an undefined
//    way.
//
// If you get any other error than the above, you're either pointing the
// patcher at an invalid instruction (e.g. into the middle of a multi-
// byte instruction, or not at memory containing executable instructions)
// or, there may be a bug in the disassembler we use to find
// instruction boundaries.
//
// NOTE:  In optimized builds, when you have very trivial functions that
// the compiler can reason do not have side effects, the compiler may
// reuse the result of calling the function with a given parameter, which
// may mean if you patch the function in between your patch will never get
// invoked.  See preamble_patcher_test.cc for an example.
class PreamblePatcher {
 public:

  // This is a typesafe version of RawPatch(), identical in all other
  // ways than it takes a template parameter indicating the type of the
  // function being patched.
  //
  // @param T The type of the function you are patching. Usually
  // you will establish this type using a typedef, as in the following
  // example:
  // @code
  // typedef BOOL (WINAPI *MessageBoxPtr)(HWND, LPCTSTR, LPCTSTR, UINT);
  // MessageBoxPtr original = NULL;
  // PreamblePatcher::Patch(MessageBox, Hook_MessageBox, &original);
  // @endcode
  template <class T>
  static SideStepError Patch(T target_function,
                               T replacement_function,
                               T* original_function_stub) {
    // NOTE: casting from a function to a pointer is contra the C++
    //       spec.  It's not safe on IA64, but is on i386.  We use
    //       a C-style cast here to emphasize this is not legal C++.
    return RawPatch((void*)(target_function),
                    (void*)(replacement_function),
                    (void**)(original_function_stub));
  }

  // Patches a named function imported from the named module using
  // preamble patching.  Uses RawPatch() to do the actual patching
  // work.
  //
  // @param T The type of the function you are patching.  Must
  // exactly match the function you specify using module_name and
  // function_name.
  //
  // @param module_name The name of the module from which the function
  // is being imported.  Note that the patch will fail if this module
  // has not already been loaded into the current process.
  //
  // @param function_name The name of the function you wish to patch.
  //
  // @param replacement_function Your replacement function which
  // will be called whenever code tries to call the original function.
  //
  // @param original_function_stub Pointer to memory that should receive a
  // pointer that can be used (e.g. in the replacement function) to call the
  // original function, or NULL to indicate failure.
  //
  // @return One of the EnSideStepError error codes; only SIDESTEP_SUCCESS
  // indicates success.
  template <class T>
  static SideStepError Patch(LPCTSTR module_name,
                               LPCSTR function_name,
                               T replacement_function,
                               T* original_function_stub) {
    ASSERT1(module_name && function_name);
    if (!module_name || !function_name) {
      ASSERT(false,
             "You must specify a module name and function name.");
      return SIDESTEP_INVALID_PARAMETER;
    }
    HMODULE module = ::GetModuleHandle(module_name);
    ASSERT1(module != NULL);
    if (!module) {
      ASSERT(false, "Invalid module name.");
      return SIDESTEP_NO_SUCH_MODULE;
    }
    FARPROC existing_function = ::GetProcAddress(module, function_name);
    if (!existing_function) {
      return SIDESTEP_NO_SUCH_FUNCTION;
    }
    // NOTE: casting from a function to a pointer is contra the C++
    //       spec.  It's not safe on IA64, but is on i386.  We use
    //       a C-style cast here to emphasize this is not legal C++.
    return RawPatchWithStub((void*)existing_function, (void*)replacement_function,
                    (void**)(original_function_stub));
  }

  // Patches a function by overwriting its first few bytes with
  // a jump to a different function.  This is the "worker" function
  // for each of the typesafe Patch() functions.  In most cases,
  // it is preferable to use the Patch() functions rather than
  // this one as they do more checking at compile time.
  //
  // @param target_function A pointer to the function that should be
  // patched.
  //
  // @param replacement_function A pointer to the function that should
  // replace the target function.  The replacement function must have
  // exactly the same calling convention and parameters as the original
  // function.
  //
  // @param original_function_stub Pointer to memory that should receive a
  // pointer that can be used (e.g. in the replacement function) to call the
  // original function, or NULL to indicate failure.
  //
  // @param original_function_stub Pointer to memory that should receive a
  // pointer that can be used (e.g. in the replacement function) to call the
  // original function, or NULL to indicate failure.
  //
  // @return One of the EnSideStepError error codes; only SIDESTEP_SUCCESS
  // indicates success.
  //
  // @note The preamble-stub (the memory pointed to by
  // *original_function_stub) is allocated on the heap, and (in
  // production binaries) never destroyed, resulting in a memory leak.  This
  // will be the case until we implement safe unpatching of a method.
  // However, it is quite difficult to unpatch a method (because other
  // threads in the process may be using it) so we are leaving it for now.
  // See however UnsafeUnpatch, which can be used for binaries where you
  // know only one thread is running, e.g. unit tests.
  static SideStepError RawPatch(void* target_function,
                                  void* replacement_function,
                                  void** original_function_stub);

  // Unpatches target_function and deletes the stub that previously could be
  // used to call the original version of the function.
  //
  // DELETES the stub that is passed to the function.
  //
  // @param target_function Pointer to the target function which was
  // previously patched, i.e. a pointer which value should match the value
  // of the symbol prior to patching it.
  //
  // @param replacement_function Pointer to the function target_function
  // was patched to.
  //
  // @param original_function_stub Pointer to the stub returned when
  // patching, that could be used to call the original version of the
  // patched function.  This function will also delete the stub, which after
  // unpatching is useless.
  //
  // If your original call was
  //    origptr = Patch(VirtualAlloc, MyVirtualAlloc)
  // then to undo it you would call
  //    Unpatch(VirtualAlloc, MyVirtualAlloc, origptr);
  //
  // @return One of the EnSideStepError error codes; only SIDESTEP_SUCCESS
  // indicates success.
  static SideStepError Unpatch(void* target_function,
                               void* replacement_function,
                               void* original_function_stub);

 private:

  // Patches a function by overwriting its first few bytes with
  // a jump to a different function.  This is similar to the RawPatch
  // function except that it uses the stub allocated by the caller
  // instead of allocating it.
  //
  // We call VirtualProtect to make the
  // target function writable at least for the duration of the call.
  //
  // @param target_function A pointer to the function that should be
  // patched.
  //
  // @param replacement_function A pointer to the function that should
  // replace the target function.  The replacement function must have
  // exactly the same calling convention and parameters as the original
  // function.
  //
  // @param preamble_stub A pointer to a buffer where the preamble stub
  // should be copied. The size of the buffer should be sufficient to
  // hold the preamble bytes.
  //
  // @param stub_size Size in bytes of the buffer allocated for the
  // preamble_stub
  //
  // @param bytes_needed Pointer to a variable that receives the minimum
  // number of bytes required for the stub.  Can be set to NULL if you're
  // not interested.
  //
  // @return An error code indicating the result of patching.
  static SideStepError RawPatchWithStubAndProtections(void* target_function,
                                          void *replacement_function,
                                          unsigned char* preamble_stub,
                                          unsigned long stub_size,
                                          unsigned long* bytes_needed);

  // A helper function used by RawPatchWithStubAndProtections -- it does
  // everything but the VirtualProtect wsork.  Defined in
  // preamble_patcher_with_stub.cc.
  static SideStepError RawPatchWithStub(void* target_function,
                                          void *replacement_function,
                                          unsigned char* preamble_stub,
                                          unsigned long stub_size,
                                          unsigned long* bytes_needed);
};

};  // namespace sidestep

#endif  // MEMORY_WATCHER_PREAMBLE_PATCHER_H__

