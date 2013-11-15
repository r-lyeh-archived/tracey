#include "google.hpp"

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
 * Table of relevant information about how to decode the ModR/M byte.
 * Based on information in the IA-32 Intel® Architecture
 * Software Developer’s Manual Volume 2: Instruction Set Reference.
 */

//#include "mini_disassembler.h"
//#include "mini_disassembler_types.h"

namespace sidestep {

const ModrmEntry MiniDisassembler::s_ia16_modrm_map_[] = {
// mod == 00
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, false, OS_ZERO },
  /* r/m == 101 */ { false, false, OS_ZERO },
  /* r/m == 110 */ { true, false, OS_WORD },
  /* r/m == 111 */ { false, false, OS_ZERO },
// mod == 01
  /* r/m == 000 */ { true, false, OS_BYTE },
  /* r/m == 001 */ { true, false, OS_BYTE },
  /* r/m == 010 */ { true, false, OS_BYTE },
  /* r/m == 011 */ { true, false, OS_BYTE },
  /* r/m == 100 */ { true, false, OS_BYTE },
  /* r/m == 101 */ { true, false, OS_BYTE },
  /* r/m == 110 */ { true, false, OS_BYTE },
  /* r/m == 111 */ { true, false, OS_BYTE },
// mod == 10
  /* r/m == 000 */ { true, false, OS_WORD },
  /* r/m == 001 */ { true, false, OS_WORD },
  /* r/m == 010 */ { true, false, OS_WORD },
  /* r/m == 011 */ { true, false, OS_WORD },
  /* r/m == 100 */ { true, false, OS_WORD },
  /* r/m == 101 */ { true, false, OS_WORD },
  /* r/m == 110 */ { true, false, OS_WORD },
  /* r/m == 111 */ { true, false, OS_WORD },
// mod == 11
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, false, OS_ZERO },
  /* r/m == 101 */ { false, false, OS_ZERO },
  /* r/m == 110 */ { false, false, OS_ZERO },
  /* r/m == 111 */ { false, false, OS_ZERO }
};

const ModrmEntry MiniDisassembler::s_ia32_modrm_map_[] = {
// mod == 00
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, true, OS_ZERO },
  /* r/m == 101 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 110 */ { false, false, OS_ZERO },
  /* r/m == 111 */ { false, false, OS_ZERO },
// mod == 01
  /* r/m == 000 */ { true, false, OS_BYTE },
  /* r/m == 001 */ { true, false, OS_BYTE },
  /* r/m == 010 */ { true, false, OS_BYTE },
  /* r/m == 011 */ { true, false, OS_BYTE },
  /* r/m == 100 */ { true, true, OS_BYTE },
  /* r/m == 101 */ { true, false, OS_BYTE },
  /* r/m == 110 */ { true, false, OS_BYTE },
  /* r/m == 111 */ { true, false, OS_BYTE },
// mod == 10
  /* r/m == 000 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 001 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 010 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 011 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 100 */ { true, true, OS_DOUBLE_WORD },
  /* r/m == 101 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 110 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 111 */ { true, false, OS_DOUBLE_WORD },
// mod == 11
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, false, OS_ZERO },
  /* r/m == 101 */ { false, false, OS_ZERO },
  /* r/m == 110 */ { false, false, OS_ZERO },
  /* r/m == 111 */ { false, false, OS_ZERO },
};

};  // namespace sidestep
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
 * Opcode decoding maps.  Based on the IA-32 Intel® Architecture
 * Software Developer’s Manual Volume 2: Instruction Set Reference.  Idea
 * for how to lay out the tables in memory taken from the implementation
 * in the Bastard disassembly environment.
 */

//#include "mini_disassembler.h"

namespace sidestep {

/*
* This is the first table to be searched; the first field of each
* Opcode in the table is either 0 to indicate you're in the
* right table, or an index to the correct table, in the global
* map g_pentiumOpcodeMap
*/
const Opcode s_first_opcode_byte[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF */ { 1, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x10 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x11 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x12 */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x13 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x14 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x15 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x16 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x17 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x18 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x19 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1A */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1B */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1C */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1D */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1E */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1F */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x20 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x21 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x22 */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x23 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x24 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x25 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x26 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x27 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "daa", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x28 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x29 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2A */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2B */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2C */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2D */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2E */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2F */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "das", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x30 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x31 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x32 */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x33 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x34 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x35 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x36 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x37 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "aaa", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x38 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x39 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3A */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3B */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3C */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3D */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3E */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3F */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "aas", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
#ifdef _M_X64
  /* REX Prefixes in 64-bit mode. */
  /* 0x40 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x41 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x42 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x43 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x44 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x45 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x46 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x47 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x48 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x49 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4A */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4B */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4C */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4D */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4E */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4F */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
#else
  /* 0x40 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x41 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x42 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x43 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x44 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x45 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x46 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x47 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x48 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x49 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4A */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4B */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4C */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4D */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4E */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4F */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
#endif
  /* 0x50 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x51 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x52 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x53 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x54 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x55 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x56 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x57 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x58 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x59 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5A */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5B */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5C */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5D */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5E */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5F */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x60 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "pushad", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x61 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "popad", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x62 */ { 0, IT_GENERIC, AM_G | OT_V, AM_M | OT_A, AM_NOT_USED, "bound", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x63 */ { 0, IT_GENERIC, AM_E | OT_W, AM_G | OT_W, AM_NOT_USED, "arpl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x64 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x65 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x66 */ { 0, IT_PREFIX_OPERAND, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x67 */ { 0, IT_PREFIX_ADDRESS, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x68 */ { 0, IT_GENERIC, AM_I | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x69 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_I | OT_V, "imul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6A */ { 0, IT_GENERIC, AM_I | OT_B, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6B */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_I |  OT_B, "imul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6C */ { 0, IT_GENERIC, AM_Y | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "insb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6D */ { 0, IT_GENERIC, AM_Y | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "insd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6E */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_X | OT_B, AM_NOT_USED, "outsb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6F */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_X | OT_V, AM_NOT_USED, "outsb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x70 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x71 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jno", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x72 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x73 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jnc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x74 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x75 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jnz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x76 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jbe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x77 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "ja", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x78 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "js", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x79 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jns", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7A */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jpe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7B */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jpo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7C */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7D */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jge", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7E */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jle", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7F */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x80 */ { 2, IT_REFERENCE, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x81 */ { 3, IT_REFERENCE, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x82 */ { 4, IT_REFERENCE, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x83 */ { 5, IT_REFERENCE, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x84 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x85 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x86 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x87 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x88 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x89 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8A */ { 0, IT_GENERIC, AM_G | OT_B, AM_E | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8B */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8C */ { 0, IT_GENERIC, AM_E | OT_W, AM_S | OT_W, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8D */ { 0, IT_GENERIC, AM_G | OT_V, AM_M | OT_ADDRESS_MODE_M, AM_NOT_USED, "lea", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8E */ { 0, IT_GENERIC, AM_S | OT_W, AM_E | OT_W, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8F */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x90 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "nop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x91 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x92 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x93 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x94 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x95 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x96 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x97 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "xchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x98 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "cwde", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x99 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "cdq", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9A */ { 0, IT_JUMP, AM_A | OT_P, AM_NOT_USED, AM_NOT_USED, "callf", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9B */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "wait", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9C */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "pushfd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9D */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "popfd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9E */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "sahf", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9F */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "lahf", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA0 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_O | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA1 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_O | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA2 */ { 0, IT_GENERIC, AM_O | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA3 */ { 0, IT_GENERIC, AM_O | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA4 */ { 0, IT_GENERIC, AM_X | OT_B, AM_Y | OT_B, AM_NOT_USED, "movsb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA5 */ { 0, IT_GENERIC, AM_X | OT_V, AM_Y | OT_V, AM_NOT_USED, "movsd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA6 */ { 0, IT_GENERIC, AM_X | OT_B, AM_Y | OT_B, AM_NOT_USED, "cmpsb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA7 */ { 0, IT_GENERIC, AM_X | OT_V, AM_Y | OT_V, AM_NOT_USED, "cmpsd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA8 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA9 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAA */ { 0, IT_GENERIC, AM_Y | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "stosb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAB */ { 0, IT_GENERIC, AM_Y | OT_V, AM_REGISTER | OT_V, AM_NOT_USED, "stosd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAC */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_X| OT_B, AM_NOT_USED, "lodsb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAD */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_X| OT_V, AM_NOT_USED, "lodsd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAE */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_Y | OT_B, AM_NOT_USED, "scasb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAF */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_Y | OT_V, AM_NOT_USED, "scasd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB0 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB1 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB2 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB3 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB4 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB5 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB6 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB7 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
#ifdef _M_X64
  /* 0xB8 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB9 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBA */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBB */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBC */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBD */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBE */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBF */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V | IOS_64, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
#else
  /* 0xB8 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB9 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBA */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBB */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBC */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBD */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBE */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBF */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
#endif
  /* 0xC0 */ { 6, IT_REFERENCE, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC1 */ { 7, IT_REFERENCE, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC2 */ { 0, IT_RETURN, AM_I | OT_W, AM_NOT_USED, AM_NOT_USED, "ret", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC3 */ { 0, IT_RETURN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "ret", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC4 */ { 0, IT_GENERIC, AM_G | OT_V, AM_M | OT_P, AM_NOT_USED, "les", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC5 */ { 0, IT_GENERIC, AM_G | OT_V, AM_M | OT_P, AM_NOT_USED, "lds", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC6 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC8 */ { 0, IT_GENERIC, AM_I | OT_W, AM_I | OT_B, AM_NOT_USED, "enter", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC9 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "leave", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCA */ { 0, IT_RETURN, AM_I | OT_W, AM_NOT_USED, AM_NOT_USED, "retf", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCB */ { 0, IT_RETURN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "retf", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCC */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "int3", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCD */ { 0, IT_GENERIC, AM_I | OT_B, AM_NOT_USED, AM_NOT_USED, "int", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCE */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "into", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCF */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "iret", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD0 */ { 8, IT_REFERENCE, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD1 */ { 9, IT_REFERENCE, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD2 */ { 10, IT_REFERENCE, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD3 */ { 11, IT_REFERENCE, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD4 */ { 0, IT_GENERIC, AM_I | OT_B, AM_NOT_USED, AM_NOT_USED, "aam", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD5 */ { 0, IT_GENERIC, AM_I | OT_B, AM_NOT_USED, AM_NOT_USED, "aad", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD6 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD7 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "xlat", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },

  // The following 8 lines would be references to the FPU tables, but we currently
  // do not support the FPU instructions in this disassembler.

  /* 0xD8 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD9 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xDA */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xDB */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xDC */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xDD */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xDE */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xDF */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },


  /* 0xE0 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "loopnz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE1 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "loopz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE2 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "loop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE3 */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jcxz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE4 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "in", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE5 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_I | OT_B, AM_NOT_USED, "in", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE6 */ { 0, IT_GENERIC, AM_I | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "out", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE7 */ { 0, IT_GENERIC, AM_I | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "out", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE8 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "call", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE9 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xEA */ { 0, IT_JUMP, AM_A | OT_P, AM_NOT_USED, AM_NOT_USED, "jmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xEB */ { 0, IT_JUMP, AM_J | OT_B, AM_NOT_USED, AM_NOT_USED, "jmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xEC */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_REGISTER | OT_W, AM_NOT_USED, "in", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xED */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_REGISTER | OT_W, AM_NOT_USED, "in", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xEE */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_REGISTER | OT_B, AM_NOT_USED, "out", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xEF */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_REGISTER | OT_V, AM_NOT_USED, "out", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF0 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "lock:", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF1 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF2 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "repne:", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF3 */ { 0, IT_PREFIX, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "rep:", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF4 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "hlt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF5 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "cmc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF6 */ { 12, IT_REFERENCE, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF7 */ { 13, IT_REFERENCE, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF8 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "clc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF9 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "stc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xFA */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "cli", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xFB */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "sti", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xFC */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "cld", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xFD */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "std", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xFE */ { 14, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xFF */ { 15, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f[] = {
  /* 0x0 */ { 16, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 17, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_W, AM_NOT_USED, "lar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_W, AM_NOT_USED, "lsl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "clts", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "invd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "wbinvd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "ud2", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xE */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x10 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "movups", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "movsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "movss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "movupd" } },
  /* 0x11 */ { 0, IT_GENERIC, AM_W | OT_PS, AM_V | OT_PS, AM_NOT_USED, "movups", true,
    /* F2h */ { 0, IT_GENERIC, AM_W | OT_SD, AM_V | OT_SD, AM_NOT_USED, "movsd" },
    /* F3h */ { 0, IT_GENERIC, AM_W | OT_SS, AM_V | OT_SS, AM_NOT_USED, "movss" },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_PD, AM_V | OT_PD, AM_NOT_USED, "movupd" } },
  /* 0x12 */ { 0, IT_GENERIC, AM_W | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movlps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movhlps" },  // only one of ...
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movhlps" },  // ...these two is correct, Intel doesn't specify which
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_W | OT_S, AM_NOT_USED, "movlpd" } },
  /* 0x13 */ { 0, IT_GENERIC, AM_V | OT_Q, AM_W | OT_Q, AM_NOT_USED, "movlps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_W | OT_Q, AM_NOT_USED, "movlpd" } },
  /* 0x14 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_Q, AM_NOT_USED, "unpcklps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_Q, AM_NOT_USED, "unpcklpd" } },
  /* 0x15 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_Q, AM_NOT_USED, "unpckhps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_Q, AM_NOT_USED, "unpckhpd" } },
  /* 0x16 */ { 0, IT_GENERIC, AM_V | OT_Q, AM_W | OT_Q, AM_NOT_USED, "movhps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movlhps" },  // only one of...
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movlhps" },  // ...these two is correct, Intel doesn't specify which
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_W | OT_Q, AM_NOT_USED, "movhpd" } },
  /* 0x17 */ { 0, IT_GENERIC, AM_W | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movhps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movhpd" } },
  /* 0x18 */ { 18, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x19 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1A */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1B */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1C */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1D */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1E */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1F */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x20 */ { 0, IT_GENERIC, AM_R | OT_D, AM_C | OT_D, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x21 */ { 0, IT_GENERIC, AM_R | OT_D, AM_D | OT_D, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x22 */ { 0, IT_GENERIC, AM_C | OT_D, AM_R | OT_D, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x23 */ { 0, IT_GENERIC, AM_D | OT_D, AM_R | OT_D, AM_NOT_USED, "mov", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x24 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x25 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x26 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x27 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x28 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "movaps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "movapd" } },
  /* 0x29 */ { 0, IT_GENERIC, AM_W | OT_PS, AM_V | OT_PS, AM_NOT_USED, "movaps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_PD, AM_V | OT_PD, AM_NOT_USED, "movapd" } },
  /* 0x2A */ { 0, IT_GENERIC, AM_V | OT_PS, AM_Q | OT_Q, AM_NOT_USED, "cvtpi2ps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_E | OT_D, AM_NOT_USED, "cvtsi2sd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_E | OT_D, AM_NOT_USED, "cvtsi2ss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_Q | OT_DQ, AM_NOT_USED, "cvtpi2pd" } },
  /* 0x2B */ { 0, IT_GENERIC, AM_W | OT_PS, AM_V | OT_PS, AM_NOT_USED, "movntps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_PD, AM_V | OT_PD, AM_NOT_USED, "movntpd" } },
  /* 0x2C */ { 0, IT_GENERIC, AM_Q | OT_Q, AM_W | OT_PS, AM_NOT_USED, "cvttps2pi", true,
    /* F2h */ { 0, IT_GENERIC, AM_G | OT_D, AM_W | OT_SD, AM_NOT_USED, "cvttsd2si" },
    /* F3h */ { 0, IT_GENERIC, AM_G | OT_D, AM_W | OT_SS, AM_NOT_USED, "cvttss2si" },
    /* 66h */ { 0, IT_GENERIC, AM_Q | OT_DQ, AM_W | OT_PD, AM_NOT_USED, "cvttpd2pi" } },
  /* 0x2D */ { 0, IT_GENERIC, AM_Q | OT_Q, AM_W | OT_PS, AM_NOT_USED, "cvtps2pi", true,
    /* F2h */ { 0, IT_GENERIC, AM_G | OT_D, AM_W | OT_SD, AM_NOT_USED, "cvtsd2si" },
    /* F3h */ { 0, IT_GENERIC, AM_G | OT_D, AM_W | OT_SS, AM_NOT_USED, "cvtss2si" },
    /* 66h */ { 0, IT_GENERIC, AM_Q | OT_DQ, AM_W | OT_PD, AM_NOT_USED, "cvtpd2pi" } },
  /* 0x2E */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "ucomiss", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "ucomisd" } },
  /* 0x2F */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_SS, AM_NOT_USED, "comiss", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "comisd" } },
  /* 0x30 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "wrmsr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x31 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "rdtsc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x32 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "rdmsr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x33 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "rdpmc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x34 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "sysenter", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x35 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "sysexit", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x36 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x37 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x38 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x39 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3A */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3B */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3C */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "movnti", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3D */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3E */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3F */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x40 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x41 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovno", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x42 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x43 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovnc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x44 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x45 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovnz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x46 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovbe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x47 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmova", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x48 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovs", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x49 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovns", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4A */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovpe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4B */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovpo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4C */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4D */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovge", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4E */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovle", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4F */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "cmovg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x50 */ { 0, IT_GENERIC, AM_E | OT_D, AM_V | OT_PS, AM_NOT_USED, "movmskps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_E | OT_D, AM_V | OT_PD, AM_NOT_USED, "movmskpd" } },
  /* 0x51 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "sqrtps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "sqrtsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "sqrtss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "sqrtpd" } },
  /* 0x52 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "rsqrtps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "rsqrtss" },
    /* 66h */ { 0 } },
  /* 0x53 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "rcpps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "rcpss" },
    /* 66h */ { 0 } },
  /* 0x54 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "andps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "andpd" } },
  /* 0x55 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "andnps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "andnpd" } },
  /* 0x56 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "orps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "orpd" } },
  /* 0x57 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "xorps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "xorpd" } },
  /* 0x58 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "addps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "addsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "addss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "addpd" } },
  /* 0x59 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "mulps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "mulsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "mulss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "mulpd" } },
  /* 0x5A */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PS, AM_NOT_USED, "cvtps2pd", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "cvtsd2ss" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "cvtss2sd" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PD, AM_NOT_USED, "cvtpd2ps" } },
  /* 0x5B */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_DQ, AM_NOT_USED, "cvtdq2ps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_PS, AM_NOT_USED, "cvttps2dq" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_PS, AM_NOT_USED, "cvtps2dq" } },
  /* 0x5C */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "subps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "subsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "subss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "subpd" } },
  /* 0x5D */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "minps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "minsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "minss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "minpd" } },
  /* 0x5E */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "divps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "divsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "divss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "divpd" } },
  /* 0x5F */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_NOT_USED, "maxps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_NOT_USED, "maxsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W | OT_SS, AM_NOT_USED, "maxss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_NOT_USED, "maxpd" } },
  /* 0x60 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "punpcklbw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "punpcklbw" } },
  /* 0x61 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "punpcklwd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "punpcklwd" } },
  /* 0x62 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "punpckldq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "punpckldq" } },
  /* 0x63 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "packsswb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "packsswb" } },
  /* 0x64 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "pcmpgtb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pcmpgtb" } },
  /* 0x65 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "pcmpgtw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pcmpgtw" } },
  /* 0x66 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "pcmpgtd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pcmpgtd" } },
  /* 0x67 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "packuswb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "packuswb" } },
  /* 0x68 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "punpckhbw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_Q | OT_DQ, AM_NOT_USED, "punpckhbw" } },
  /* 0x69 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "punpckhwd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_Q | OT_DQ, AM_NOT_USED, "punpckhwd" } },
  /* 0x6A */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "punpckhdq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_Q | OT_DQ, AM_NOT_USED, "punpckhdq" } },
  /* 0x6B */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "packssdw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_Q | OT_DQ, AM_NOT_USED, "packssdw" } },
  /* 0x6C */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "not used without prefix", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "punpcklqdq" } },
  /* 0x6D */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "not used without prefix", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "punpcklqdq" } },
  /* 0x6E */ { 0, IT_GENERIC, AM_P | OT_D, AM_E | OT_D, AM_NOT_USED, "movd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_E | OT_D, AM_NOT_USED, "movd" } },
  /* 0x6F */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_D, AM_NOT_USED, "movq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "movdqu" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "movdqa" } },
  /* 0x70 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_I |  OT_B, "pshuf", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_I | OT_B, "pshuflw" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_I | OT_B, "pshufhw" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_I | OT_B, "pshufd" } },
  /* 0x71 */ { 19, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x72 */ { 20, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x73 */ { 21, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x74 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pcmpeqb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pcmpeqb" } },
  /* 0x75 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pcmpeqw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pcmpeqw" } },
  /* 0x76 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pcmpeqd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pcmpeqd" } },
  /* 0x77 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "emms", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },

  // The following six opcodes are escapes into the MMX stuff, which this disassembler does not support.
  /* 0x78 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x79 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7A */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7B */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7C */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7D */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },

  /* 0x7E */ { 0, IT_GENERIC, AM_E | OT_D, AM_P | OT_D, AM_NOT_USED, "movd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_Q, AM_W | OT_Q, AM_NOT_USED, "movq" },
    /* 66h */ { 0, IT_GENERIC, AM_E | OT_D, AM_V | OT_DQ, AM_NOT_USED, "movd" } },
  /* 0x7F */ { 0, IT_GENERIC, AM_Q | OT_Q, AM_P | OT_Q, AM_NOT_USED, "movq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_V | OT_DQ, AM_NOT_USED, "movdqu" },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_V | OT_DQ, AM_NOT_USED, "movdqa" } },
  /* 0x80 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x81 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jno", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x82 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x83 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jnc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x84 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x85 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jnz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x86 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jbe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x87 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "ja", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x88 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "js", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x89 */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jns", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8A */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jpe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8B */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jpo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8C */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8D */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jge", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8E */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jle", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x8F */ { 0, IT_JUMP, AM_J | OT_V, AM_NOT_USED, AM_NOT_USED, "jg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x90 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "seto", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x91 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setno", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x92 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x93 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setnc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x94 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x95 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setnz", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x96 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setbe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x97 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "seta", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x98 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "sets", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x99 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setns", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9A */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setpe", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9B */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setpo", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9C */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9D */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setge", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9E */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setle", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x9F */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "setg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA0 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA1 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA2 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "cpuid", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "bt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_I | OT_B, "shld", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_I | OT_B | AM_REGISTER, "shld", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA6 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA7 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA8 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xA9 */ { 0, IT_GENERIC, AM_REGISTER | OT_W, AM_NOT_USED, AM_NOT_USED, "pop", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAA */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "rsm", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAB */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "bts", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAC */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_I | OT_B, "shrd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAD */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_I | OT_B | AM_REGISTER, "shrd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAE */ { 22, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xAF */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "imul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "cmpxchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "cmpxchg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB2 */ { 0, IT_GENERIC, AM_M | OT_P, AM_NOT_USED, AM_NOT_USED, "lss", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "btr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB4 */ { 0, IT_GENERIC, AM_M | OT_P, AM_NOT_USED, AM_NOT_USED, "lfs", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB5 */ { 0, IT_GENERIC, AM_M | OT_P, AM_NOT_USED, AM_NOT_USED, "lgs", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB6 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_B, AM_NOT_USED, "movzx", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB7 */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_W, AM_NOT_USED, "movzx", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB8 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xB9 */ { 0, IT_UNKNOWN, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "ud1", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBA */ { 23, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBB */ { 0, IT_GENERIC, AM_E | OT_V, AM_G | OT_V, AM_NOT_USED, "btc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBC */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "bsf", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBD */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_V, AM_NOT_USED, "bsr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBE */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_B, AM_NOT_USED, "movsx", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xBF */ { 0, IT_GENERIC, AM_G | OT_V, AM_E | OT_W, AM_NOT_USED, "movsx", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_G | OT_B, AM_NOT_USED, "xadd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "xadd", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC2 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_I | OT_B, "cmpps", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_SD, AM_W | OT_SD, AM_I | OT_B, "cmpsd" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_SS, AM_W  | OT_SS, AM_I | OT_B, "cmpss" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_I | OT_B, "cmppd" } },
  /* 0xC3 */ { 0, IT_GENERIC, AM_E | OT_D, AM_G | OT_D, AM_NOT_USED, "movnti", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC4 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_E | OT_D, AM_I | OT_B, "pinsrw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_E | OT_D, AM_I | OT_B, "pinsrw" } },
  /* 0xC5 */ { 0, IT_GENERIC, AM_G | OT_D, AM_P | OT_Q, AM_I | OT_B, "pextrw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_G | OT_D, AM_V | OT_DQ, AM_I | OT_B, "pextrw" } },
  /* 0xC6 */ { 0, IT_GENERIC, AM_V | OT_PS, AM_W | OT_PS, AM_I | OT_B, "shufps", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_PD, AM_I | OT_B, "shufpd" } },
  /* 0xC7 */ { 24, IT_REFERENCE, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC8 */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xC9 */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCA */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCB */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCC */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCD */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCE */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xCF */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "bswap", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xD1 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psrlw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psrlw" } },
  /* 0xD2 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psrld", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psrld" } },
  /* 0xD3 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psrlq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psrlq" } },
  /* 0xD4 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddq" } },
  /* 0xD5 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmullw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmullw" } },
  /* 0xD6 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "unused without prefix", true,
    /* F2h */ { 0, IT_GENERIC, AM_P | OT_Q, AM_W | OT_Q, AM_NOT_USED, "movdq2q" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_Q | OT_Q, AM_NOT_USED, "movq2dq" },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movq" } },
  /* 0xD7 */ { 0, IT_GENERIC, AM_G | OT_D, AM_P | OT_Q, AM_NOT_USED, "pmovmskb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_G | OT_D, AM_V | OT_DQ, AM_NOT_USED, "pmovmskb" } },
  /* 0xD8 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubusb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubusb" } },
  /* 0xD9 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubusw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubusw" } },
  /* 0xDA */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pminub", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pminub" } },
  /* 0xDB */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pand", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pand" } },
  /* 0xDC */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddusb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddusb" } },
  /* 0xDD */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddusw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddusw" } },
  /* 0xDE */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmaxub", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmaxub" } },
  /* 0xDF */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pandn", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pandn" } },
  /* 0xE0 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pavgb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pavgb" } },
  /* 0xE1 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psraw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psrqw" } },
  /* 0xE2 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psrad", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psrad" } },
  /* 0xE3 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pavgw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pavgw" } },
  /* 0xE4 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmulhuw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmulhuw" } },
  /* 0xE5 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmulhuw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmulhw" } },
  /* 0xE6 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "not used without prefix", true,
    /* F2h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_PD, AM_NOT_USED, "cvtpd2dq" },
    /* F3h */ { 0, IT_GENERIC, AM_V | OT_PD, AM_W | OT_DQ, AM_NOT_USED, "cvtdq2pd" },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_PD, AM_NOT_USED, "cvttpd2dq" } },
  /* 0xE7 */ { 0, IT_GENERIC, AM_W | OT_Q, AM_V | OT_Q, AM_NOT_USED, "movntq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_V | OT_DQ, AM_NOT_USED, "movntdq" } },
  /* 0xE8 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubsb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubsb" } },
  /* 0xE9 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubsw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubsw" } },
  /* 0xEA */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pminsw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pminsw" } },
  /* 0xEB */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "por", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "por" } },
  /* 0xEC */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddsb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddsb" } },
  /* 0xED */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddsw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddsw" } },
  /* 0xEE */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmaxsw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmaxsw" } },
  /* 0xEF */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pxor", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pxor" } },
  /* 0xF0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0xF1 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psllw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psllw" } },
  /* 0xF2 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pslld", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pslld" } },
  /* 0xF3 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psllq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psllq" } },
  /* 0xF4 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmuludq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmuludq" } },
  /* 0xF5 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "pmaddwd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "pmaddwd" } },
  /* 0xF6 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psadbw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psadbw" } },
  /* 0xF7 */ { 0, IT_GENERIC, AM_P | OT_PI, AM_Q | OT_PI, AM_NOT_USED, "maskmovq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "maskmovdqu" } },
  /* 0xF8 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubb" } },
  /* 0xF9 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubw" } },
  /* 0xFA */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubd" } },
  /* 0xFB */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "psubq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "psubq" } },
  /* 0xFC */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddb", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddb" } },
  /* 0xFD */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddw" } },
  /* 0xFE */ { 0, IT_GENERIC, AM_P | OT_Q, AM_Q | OT_Q, AM_NOT_USED, "paddd", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_V | OT_DQ, AM_W | OT_DQ, AM_NOT_USED, "paddd" } },
  /* 0xFF */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f00[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "sldt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "str", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "lldt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "ltr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "verr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "verw", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f01[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_M | OT_S, AM_NOT_USED, AM_NOT_USED, "sgdt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_M | OT_S, AM_NOT_USED, AM_NOT_USED, "sidt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_M | OT_S, AM_NOT_USED, AM_NOT_USED, "lgdt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_M | OT_S, AM_NOT_USED, AM_NOT_USED, "lidt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "smsw", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_W, AM_NOT_USED, AM_NOT_USED, "lmsw", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_M | OT_B, AM_NOT_USED, AM_NOT_USED, "invlpg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f18[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_M | OT_ADDRESS_MODE_M, AM_NOT_USED, AM_NOT_USED, "prefetch", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "prefetch", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "prefetch", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_REGISTER | OT_D, AM_NOT_USED, AM_NOT_USED, "prefetch", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f71[] = {
  /* 0x0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psrlw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psrlw" } },
  /* 0x3 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psraw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psraw" } },
  /* 0x5 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psllw", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_P | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psllw" } },
  /* 0x7 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f72[] = {
  /* 0x0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psrld", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psrld" } },
  /* 0x3 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psrad", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psrad" } },
  /* 0x5 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "pslld", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "pslld" } },
  /* 0x7 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0f73[] = {
  /* 0x0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psrlq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psrlq" } },
  /* 0x3 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_P | OT_Q, AM_I | OT_B, AM_NOT_USED, "psllq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "psllq" } },
  /* 0x7 */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "pslldq", true,
    /* F2h */ { 0 },
    /* F3h */ { 0 },
    /* 66h */ { 0, IT_GENERIC, AM_W | OT_DQ, AM_I | OT_B, AM_NOT_USED, "pslldq" } },
};

const Opcode s_opcode_byte_after_0fae[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "fxsave", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "fxrstor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "ldmxcsr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "stmxcsr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "lfence", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "mfence", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, "clflush/sfence", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
};

const Opcode s_opcode_byte_after_0fba[] = {
  /* 0x0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "bt", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "bts", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "btr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "btc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_0fc7[] = {
  /* 0x0 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_M | OT_Q, AM_NOT_USED, AM_NOT_USED, "cmpxch8b", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_80[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_81[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_82[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_83[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "add", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "or", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "adc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "sbb", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "and", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "sub", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "xor", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "cmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_c0[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "rol", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "ror", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "rcl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "rcr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "shl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "shr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "sal", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "sar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_c1[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "rol", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "ror", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "rcl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "rcr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "shl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "shr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "sal", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_B, AM_NOT_USED, "sar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_d0[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "rol", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "ror", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "rcl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "rcr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "shl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "shr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "sal", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_B, AM_IMPLICIT, AM_NOT_USED, "sar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_d1[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "rol", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "ror", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "rcl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "rcr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "shl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "shr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "sal", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_IMPLICIT, AM_NOT_USED, "sar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_d2[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "rol", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "ror", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "rcl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "rcr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "shl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "shr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "sal", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_B, AM_REGISTER | OT_B, AM_NOT_USED, "sar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_d3[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "rol", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "ror", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "rcl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "rcr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "shl", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "shr", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "sal", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_E | OT_V, AM_REGISTER | OT_B, AM_NOT_USED, "sar", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_f6[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_B, AM_I | OT_B, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "not", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "neg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, OT_B | AM_REGISTER, AM_E | OT_B, AM_NOT_USED, "mul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, OT_B | AM_REGISTER, AM_E | OT_B, AM_NOT_USED, "imul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_E | OT_B, AM_NOT_USED, "div", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_REGISTER | OT_B, AM_E | OT_B, AM_NOT_USED, "idiv", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_f7[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_I | OT_V, AM_NOT_USED, "test", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "not", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "neg", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_E | OT_V, AM_NOT_USED, "mul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_E | OT_V, AM_NOT_USED, "imul", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_E | OT_V, AM_NOT_USED, "div", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_GENERIC, AM_REGISTER | OT_V, AM_E | OT_V, AM_NOT_USED, "idiv", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_fe[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_B, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

const Opcode s_opcode_byte_after_ff[] = {
  /* 0x0 */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "inc", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x1 */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "dec", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x2 */ { 0, IT_JUMP, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "call", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x3 */ { 0, IT_JUMP, AM_E | OT_P, AM_NOT_USED, AM_NOT_USED, "call", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x4 */ { 0, IT_JUMP, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "jmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x5 */ { 0, IT_JUMP, AM_E | OT_P, AM_NOT_USED, AM_NOT_USED, "jmp", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x6 */ { 0, IT_GENERIC, AM_E | OT_V, AM_NOT_USED, AM_NOT_USED, "push", false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } },
  /* 0x7 */ { 0, IT_UNUSED, AM_NOT_USED, AM_NOT_USED, AM_NOT_USED, 0, false, /* F2h */ { 0 }, /* F3h */ { 0 }, /* 66h */ { 0 } }
};

/*
* A table of all the other tables, containing some extra information, e.g.
* how to mask out the byte we're looking at.
*/
const OpcodeTable MiniDisassembler::s_ia32_opcode_map_[]={
  // One-byte opcodes and jumps to larger
  /*  0 */ {s_first_opcode_byte, 0, 0xff, 0, 0xff},
  // Two-byte opcodes (second byte)
  /*  1 */ {s_opcode_byte_after_0f, 0, 0xff, 0, 0xff},
  // Start of tables for opcodes using ModR/M bits as extension
  /*  2 */ {s_opcode_byte_after_80, 3, 0x07, 0, 0x07},
  /*  3 */ {s_opcode_byte_after_81, 3, 0x07, 0, 0x07},
  /*  4 */ {s_opcode_byte_after_82, 3, 0x07, 0, 0x07},
  /*  5 */ {s_opcode_byte_after_83, 3, 0x07, 0, 0x07},
  /*  6 */ {s_opcode_byte_after_c0, 3, 0x07, 0, 0x07},
  /*  7 */ {s_opcode_byte_after_c1, 3, 0x07, 0, 0x07},
  /*  8 */ {s_opcode_byte_after_d0, 3, 0x07, 0, 0x07},
  /*  9 */ {s_opcode_byte_after_d1, 3, 0x07, 0, 0x07},
  /* 10 */ {s_opcode_byte_after_d2, 3, 0x07, 0, 0x07},
  /* 11 */ {s_opcode_byte_after_d3, 3, 0x07, 0, 0x07},
  /* 12 */ {s_opcode_byte_after_f6, 3, 0x07, 0, 0x07},
  /* 13 */ {s_opcode_byte_after_f7, 3, 0x07, 0, 0x07},
  /* 14 */ {s_opcode_byte_after_fe, 3, 0x07, 0, 0x01},
  /* 15 */ {s_opcode_byte_after_ff, 3, 0x07, 0, 0x07},
  /* 16 */ {s_opcode_byte_after_0f00, 3, 0x07, 0, 0x07},
  /* 17 */ {s_opcode_byte_after_0f01, 3, 0x07, 0, 0x07},
  /* 18 */ {s_opcode_byte_after_0f18, 3, 0x07, 0, 0x07},
  /* 19 */ {s_opcode_byte_after_0f71, 3, 0x07, 0, 0x07},
  /* 20 */ {s_opcode_byte_after_0f72, 3, 0x07, 0, 0x07},
  /* 21 */ {s_opcode_byte_after_0f73, 3, 0x07, 0, 0x07},
  /* 22 */ {s_opcode_byte_after_0fae, 3, 0x07, 0, 0x07},
  /* 23 */ {s_opcode_byte_after_0fba, 3, 0x07, 0, 0x07},
  /* 24 */ {s_opcode_byte_after_0fc7, 3, 0x07, 0, 0x01}
};

};  // namespace sidestep// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
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
 * Implementation of MiniDisassembler.
 */

//#include "mini_disassembler.h"

namespace sidestep {

MiniDisassembler::MiniDisassembler(bool operand_default_is_32_bits,
                                   bool address_default_is_32_bits)
    : operand_default_is_32_bits_(operand_default_is_32_bits),
      address_default_is_32_bits_(address_default_is_32_bits) {
  Initialize();
}

MiniDisassembler::MiniDisassembler()
    : operand_default_is_32_bits_(true),
      address_default_is_32_bits_(true) {
  Initialize();
}

InstructionType MiniDisassembler::Disassemble(
    unsigned char* start_byte,
    unsigned int& instruction_bytes) {
  // Clean up any state from previous invocations.
  Initialize();

  // Start by processing any prefixes.
  unsigned char* current_byte = start_byte;
  unsigned int size = 0;
  InstructionType instruction_type = ProcessPrefixes(current_byte, size);

  if (IT_UNKNOWN == instruction_type)
    return instruction_type;

  current_byte += size;
  size = 0;

  // Invariant: We have stripped all prefixes, and the operand_is_32_bits_
  // and address_is_32_bits_ flags are correctly set.

  instruction_type = ProcessOpcode(current_byte, 0, size);

  // Check for error processing instruction
  if ((IT_UNKNOWN == instruction_type_) || (IT_UNUSED == instruction_type_)) {
    return IT_UNKNOWN;
  }

  current_byte += size;

  // Invariant: operand_bytes_ indicates the total size of operands
  // specified by the opcode and/or ModR/M byte and/or SIB byte.
  // pCurrentByte points to the first byte after the ModR/M byte, or after
  // the SIB byte if it is present (i.e. the first byte of any operands
  // encoded in the instruction).

  // We get the total length of any prefixes, the opcode, and the ModR/M and
  // SIB bytes if present, by taking the difference of the original starting
  // address and the current byte (which points to the first byte of the
  // operands if present, or to the first byte of the next instruction if
  // they are not).  Adding the count of bytes in the operands encoded in
  // the instruction gives us the full length of the instruction in bytes.
  instruction_bytes += operand_bytes_ + (current_byte - start_byte);

  // Return the instruction type, which was set by ProcessOpcode().
  return instruction_type_;
}

void MiniDisassembler::Initialize() {
  operand_is_32_bits_ = operand_default_is_32_bits_;
  address_is_32_bits_ = address_default_is_32_bits_;
#ifdef _M_X64
  operand_default_support_64_bits_ = true;
#else
  operand_default_support_64_bits_ = false;
#endif
  operand_is_64_bits_ = false;
  operand_bytes_ = 0;
  have_modrm_ = false;
  should_decode_modrm_ = false;
  instruction_type_ = IT_UNKNOWN;
  got_f2_prefix_ = false;
  got_f3_prefix_ = false;
  got_66_prefix_ = false;
}

InstructionType MiniDisassembler::ProcessPrefixes(unsigned char* start_byte,
                                                  unsigned int& size) {
  InstructionType instruction_type = IT_GENERIC;
  const Opcode& opcode = s_ia32_opcode_map_[0].table_[*start_byte];

  switch (opcode.type_) {
    case IT_PREFIX_ADDRESS:
      address_is_32_bits_ = !address_default_is_32_bits_;
      goto nochangeoperand;
    case IT_PREFIX_OPERAND:
      operand_is_32_bits_ = !operand_default_is_32_bits_;
      nochangeoperand:
    case IT_PREFIX:

      if (0xF2 == (*start_byte))
        got_f2_prefix_ = true;
      else if (0xF3 == (*start_byte))
        got_f3_prefix_ = true;
      else if (0x66 == (*start_byte))
        got_66_prefix_ = true;
      else if (operand_default_support_64_bits_ && (*start_byte) & 0x48)
        operand_is_64_bits_ = true;

      instruction_type = opcode.type_;
      size ++;
      // we got a prefix, so add one and check next byte
      ProcessPrefixes(start_byte + 1, size);
    default:
      break;   // not a prefix byte
  }

  return instruction_type;
}

InstructionType MiniDisassembler::ProcessOpcode(unsigned char* start_byte,
                                                unsigned int table_index,
                                                unsigned int& size) {
  const OpcodeTable& table = s_ia32_opcode_map_[table_index];   // Get our table
  unsigned char current_byte = (*start_byte) >> table.shift_;
  current_byte = current_byte & table.mask_;  // Mask out the bits we will use

  // Check whether the byte we have is inside the table we have.
  if (current_byte < table.min_lim_ || current_byte > table.max_lim_) {
    instruction_type_ = IT_UNKNOWN;
    return instruction_type_;
  }

  const Opcode& opcode = table.table_[current_byte];
  if (IT_UNUSED == opcode.type_) {
    // This instruction is not used by the IA-32 ISA, so we indicate
    // this to the user.  Probably means that we were pointed to
    // a byte in memory that was not the start of an instruction.
    instruction_type_ = IT_UNUSED;
    return instruction_type_;
  } else if (IT_REFERENCE == opcode.type_) {
    // We are looking at an opcode that has more bytes (or is continued
    // in the ModR/M byte).  Recursively find the opcode definition in
    // the table for the opcode's next byte.
    size++;
    ProcessOpcode(start_byte + 1, opcode.table_index_, size);
    return instruction_type_;
  }

  const SpecificOpcode* specific_opcode = (SpecificOpcode*)&opcode;
  if (opcode.is_prefix_dependent_) {
    if (got_f2_prefix_ && opcode.opcode_if_f2_prefix_.mnemonic_ != 0) {
      specific_opcode = &opcode.opcode_if_f2_prefix_;
    } else if (got_f3_prefix_ && opcode.opcode_if_f3_prefix_.mnemonic_ != 0) {
      specific_opcode = &opcode.opcode_if_f3_prefix_;
    } else if (got_66_prefix_ && opcode.opcode_if_66_prefix_.mnemonic_ != 0) {
      specific_opcode = &opcode.opcode_if_66_prefix_;
    }
  }

  // Inv: The opcode type is known.
  instruction_type_ = specific_opcode->type_;

  // Let's process the operand types to see if we have any immediate
  // operands, and/or a ModR/M byte.

  ProcessOperand(specific_opcode->flag_dest_);
  ProcessOperand(specific_opcode->flag_source_);
  ProcessOperand(specific_opcode->flag_aux_);

  // Inv: We have processed the opcode and incremented operand_bytes_
  // by the number of bytes of any operands specified by the opcode
  // that are stored in the instruction (not registers etc.).  Now
  // we need to return the total number of bytes for the opcode and
  // for the ModR/M or SIB bytes if they are present.

  if (table.mask_ != 0xff) {
    if (have_modrm_) {
      // we're looking at a ModR/M byte so we're not going to
      // count that into the opcode size
      ProcessModrm(start_byte, size);
      return IT_GENERIC;
    } else {
      // need to count the ModR/M byte even if it's just being
      // used for opcode extension
      size++;
      return IT_GENERIC;
    }
  } else {
    if (have_modrm_) {
      // The ModR/M byte is the next byte.
      size++;
      ProcessModrm(start_byte + 1, size);
      return IT_GENERIC;
    } else {
      size++;
      return IT_GENERIC;
    }
  }
}

bool MiniDisassembler::ProcessOperand(int flag_operand) {
  bool succeeded = true;
  if (AM_NOT_USED == flag_operand)
    return succeeded;

  // Decide what to do based on the addressing mode.
  switch (flag_operand & AM_MASK) {
    // No ModR/M byte indicated by these addressing modes, and no
    // additional (e.g. immediate) parameters.
    case AM_A: // Direct address
    case AM_F: // EFLAGS register
    case AM_X: // Memory addressed by the DS:SI register pair
    case AM_Y: // Memory addressed by the ES:DI register pair
    case AM_IMPLICIT: // Parameter is implicit, occupies no space in
                       // instruction
      break;

    // There is a ModR/M byte but it does not necessarily need
    // to be decoded.
    case AM_C: // reg field of ModR/M selects a control register
    case AM_D: // reg field of ModR/M selects a debug register
    case AM_G: // reg field of ModR/M selects a general register
    case AM_P: // reg field of ModR/M selects an MMX register
    case AM_R: // mod field of ModR/M may refer only to a general register
    case AM_S: // reg field of ModR/M selects a segment register
    case AM_T: // reg field of ModR/M selects a test register
    case AM_V: // reg field of ModR/M selects a 128-bit XMM register
      have_modrm_ = true;
      break;

    // In these addressing modes, there is a ModR/M byte and it needs to be
    // decoded. No other (e.g. immediate) params than indicated in ModR/M.
    case AM_E: // Operand is either a general-purpose register or memory,
                 // specified by ModR/M byte
    case AM_M: // ModR/M byte will refer only to memory
    case AM_Q: // Operand is either an MMX register or memory (complex
                 // evaluation), specified by ModR/M byte
    case AM_W: // Operand is either a 128-bit XMM register or memory (complex
                 // eval), specified by ModR/M byte
      have_modrm_ = true;
      should_decode_modrm_ = true;
      break;

    // These addressing modes specify an immediate or an offset value
    // directly, so we need to look at the operand type to see how many
    // bytes.
    case AM_I: // Immediate data.
    case AM_J: // Jump to offset.
    case AM_O: // Operand is at offset.
      switch (flag_operand & OT_MASK) {
        case OT_B: // Byte regardless of operand-size attribute.
          operand_bytes_ += OS_BYTE;
          break;
        case OT_C: // Byte or word, depending on operand-size attribute.
          if (operand_is_32_bits_)
            operand_bytes_ += OS_WORD;
          else
            operand_bytes_ += OS_BYTE;
          break;
        case OT_D: // Doubleword, regardless of operand-size attribute.
          operand_bytes_ += OS_DOUBLE_WORD;
          break;
        case OT_DQ: // Double-quadword, regardless of operand-size attribute.
          operand_bytes_ += OS_DOUBLE_QUAD_WORD;
          break;
        case OT_P: // 32-bit or 48-bit pointer, depending on operand-size
                     // attribute.
          if (operand_is_32_bits_)
            operand_bytes_ += OS_48_BIT_POINTER;
          else
            operand_bytes_ += OS_32_BIT_POINTER;
          break;
        case OT_PS: // 128-bit packed single-precision floating-point data.
          operand_bytes_ += OS_128_BIT_PACKED_SINGLE_PRECISION_FLOATING;
          break;
        case OT_Q: // Quadword, regardless of operand-size attribute.
          operand_bytes_ += OS_QUAD_WORD;
          break;
        case OT_S: // 6-byte pseudo-descriptor.
          operand_bytes_ += OS_PSEUDO_DESCRIPTOR;
          break;
        case OT_SD: // Scalar Double-Precision Floating-Point Value
        case OT_PD: // Unaligned packed double-precision floating point value
          operand_bytes_ += OS_DOUBLE_PRECISION_FLOATING;
          break;
        case OT_SS:
          // Scalar element of a 128-bit packed single-precision
          // floating data.
          // We simply return enItUnknown since we don't have to support
          // floating point
          succeeded = false;
          break;
        case OT_V: // Word, doubleword or quadword, depending on operand-size
                   // attribute.
          if (operand_is_64_bits_ && flag_operand & AM_I &&
              flag_operand & IOS_64)
            operand_bytes_ += OS_QUAD_WORD;
          else if (operand_is_32_bits_)
            operand_bytes_ += OS_DOUBLE_WORD;
          else
            operand_bytes_ += OS_WORD;
          break;
        case OT_W: // Word, regardless of operand-size attribute.
          operand_bytes_ += OS_WORD;
          break;

        // Can safely ignore these.
        case OT_A: // Two one-word operands in memory or two double-word
                     // operands in memory
        case OT_PI: // Quadword MMX technology register (e.g. mm0)
        case OT_SI: // Doubleword integer register (e.g., eax)
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return succeeded;
}

bool MiniDisassembler::ProcessModrm(unsigned char* start_byte,
                                    unsigned int& size) {
  // If we don't need to decode, we just return the size of the ModR/M
  // byte (there is never a SIB byte in this case).
  if (!should_decode_modrm_) {
    size++;
    return true;
  }

  // We never care about the reg field, only the combination of the mod
  // and r/m fields, so let's start by packing those fields together into
  // 5 bits.
  unsigned char modrm = (*start_byte);
  unsigned char mod = modrm & 0xC0; // mask out top two bits to get mod field
  modrm = modrm & 0x07; // mask out bottom 3 bits to get r/m field
  mod = mod >> 3; // shift the mod field to the right place
  modrm = mod | modrm; // combine the r/m and mod fields as discussed
  mod = mod >> 3; // shift the mod field to bits 2..0

  // Invariant: modrm contains the mod field in bits 4..3 and the r/m field
  // in bits 2..0, and mod contains the mod field in bits 2..0

  const ModrmEntry* modrm_entry = 0;
  if (address_is_32_bits_)
    modrm_entry = &s_ia32_modrm_map_[modrm];
  else
    modrm_entry = &s_ia16_modrm_map_[modrm];

  // Invariant: modrm_entry points to information that we need to decode
  // the ModR/M byte.

  // Add to the count of operand bytes, if the ModR/M byte indicates
  // that some operands are encoded in the instruction.
  if (modrm_entry->is_encoded_in_instruction_)
    operand_bytes_ += modrm_entry->operand_size_;

  // Process the SIB byte if necessary, and return the count
  // of ModR/M and SIB bytes.
  if (modrm_entry->use_sib_byte_) {
    size++;
    return ProcessSib(start_byte + 1, mod, size);
  } else {
    size++;
    return true;
  }
}

bool MiniDisassembler::ProcessSib(unsigned char* start_byte,
                                  unsigned char mod,
                                  unsigned int& size) {
  // get the mod field from the 2..0 bits of the SIB byte
  unsigned char sib_base = (*start_byte) & 0x07;
  if (0x05 == sib_base) {
    switch (mod) {
    case 0x00: // mod == 00
    case 0x02: // mod == 10
      operand_bytes_ += OS_DOUBLE_WORD;
      break;
    case 0x01: // mod == 01
      operand_bytes_ += OS_BYTE;
      break;
    case 0x03: // mod == 11
      // According to the IA-32 docs, there does not seem to be a disp
      // value for this value of mod
    default:
      break;
    }
  }

  size++;
  return true;
}

};  // namespace sidestep// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "preamble_patcher.h"
//#include "memory_hook.h"
//#include "mini_disassembler.h"

// compatibility shims
//#include "base.h"

// Definitions of assembly statements we need
#define ASM_JMP32REL 0xE9
#define ASM_INT3 0xCC

namespace sidestep {

SideStepError PreamblePatcher::RawPatchWithStubAndProtections(
    void* target_function, void *replacement_function,
    unsigned char* preamble_stub, unsigned long stub_size,
    unsigned long* bytes_needed) {
  // We need to be able to write to a process-local copy of the first
  // MAX_PREAMBLE_STUB_SIZE bytes of target_function. We may be giving execute
  // privilege to something that doesn't have it, but that's the price to pay
  // for tools.
  DWORD old_target_function_protect = 0;
  BOOL succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),
                                    MAX_PREAMBLE_STUB_SIZE,
                                    PAGE_EXECUTE_READWRITE,
                                    &old_target_function_protect);
  if (!succeeded) {
    ASSERT(false, "Failed to make page containing target function "
                   "copy-on-write.");
    return SIDESTEP_ACCESS_DENIED;
  }

  SideStepError error_code = RawPatchWithStub(target_function,
                                              replacement_function,
                                              preamble_stub,
                                              stub_size,
                                              bytes_needed);
  if (SIDESTEP_SUCCESS != error_code) {
    ASSERT1(false);
    return error_code;
  }

  // Restore the protection of the first MAX_PREAMBLE_STUB_SIZE bytes of
  // pTargetFunction to what they were before we started goofing around.
  succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),
                               MAX_PREAMBLE_STUB_SIZE,
                               old_target_function_protect,
                               &old_target_function_protect);
  if (!succeeded) {
    ASSERT(false, "Failed to restore protection to target function.");
    // We must not return an error here because the function has actually
    // been patched, and returning an error would likely cause our client
    // code not to unpatch it.  So we just keep going.
  }

  // Flush the instruction cache to make sure the processor doesn't execute the
  // old version of the instructions (before our patch).
  //
  // FlushInstructionCache is actually a no-op at least on single-processor
  // XP machines.  I'm not sure why this is so, but it is, yet I want to keep
  // the call to the API here for correctness in case there is a difference in
  // some variants of Windows/hardware.
  succeeded = ::FlushInstructionCache(::GetCurrentProcess(),
                                      target_function,
                                      MAX_PREAMBLE_STUB_SIZE);
  if (!succeeded) {
    ASSERT(false, "Failed to flush instruction cache.");
    // We must not return an error here because the function has actually
    // been patched, and returning an error would likely cause our client
    // code not to unpatch it.  So we just keep going.
  }

  return SIDESTEP_SUCCESS;
}

SideStepError PreamblePatcher::RawPatch(void* target_function,
                                        void* replacement_function,
                                        void** original_function_stub) {
  if (!target_function || !replacement_function || !original_function_stub ||
      (*original_function_stub) || target_function == replacement_function) {
    ASSERT(false, "Preconditions not met");
    return SIDESTEP_INVALID_PARAMETER;
  }

  // @see MAX_PREAMBLE_STUB_SIZE for an explanation of how we arrives at
  // this size
  enum {
		sizeof_allocation = MAX_PREAMBLE_STUB_SIZE,
		num_allocations = 32
  };
  static int patched = 0;
  static unsigned char buffer[num_allocations][sizeof_allocation];
  unsigned char* preamble_stub = buffer[patched++];
  if (!preamble_stub) {
    ASSERT(false, "Unable to allocate preamble-stub.");
    return SIDESTEP_INSUFFICIENT_BUFFER;
  }

  // Change the protection of the newly allocated preamble stub to
  // PAGE_EXECUTE_READWRITE. This is required to work with DEP (Data
  // Execution Prevention) which will cause an exception if code is executed
  // from a page on which you do not have read access.
  DWORD old_stub_protect = 0;
  BOOL succeeded = VirtualProtect(preamble_stub, MAX_PREAMBLE_STUB_SIZE,
                             PAGE_EXECUTE_READWRITE, &old_stub_protect);
  if (!succeeded) {
    ASSERT(false, "Failed to make page preamble stub read-write-execute.");
    delete[] preamble_stub;
    return SIDESTEP_ACCESS_DENIED;
  }

  SideStepError error_code = RawPatchWithStubAndProtections(target_function,
                                              replacement_function,
                                              preamble_stub,
                                              MAX_PREAMBLE_STUB_SIZE,
                                              NULL);
  if (SIDESTEP_SUCCESS != error_code) {
    ASSERT1(false);
    delete[] preamble_stub;
    return error_code;
  }

  *original_function_stub = reinterpret_cast<void*>(preamble_stub);

  // NOTE: For hooking malloc/free, we don't want to use streams which
  // allocate.  Basically, we've hooked malloc, but not necessarily
  // hooked free yet.  To do anything which uses the heap could crash
  // with a mismatched malloc/free!
  //VLOG(1) << "PreamblePatcher::RawPatch successfully patched 0x"
  //        << target_function;

  return SIDESTEP_SUCCESS;
}

SideStepError PreamblePatcher::Unpatch(void* target_function,
                                       void* replacement_function,
                                       void* original_function_stub) {
  ASSERT1(target_function && original_function_stub);
  if (!target_function || !original_function_stub) {
    return SIDESTEP_INVALID_PARAMETER;
  }

  // We disassemble the preamble of the _stub_ to see how many bytes we
  // originally copied to the stub.
  MiniDisassembler disassembler;
  unsigned int preamble_bytes = 0;
  while (preamble_bytes < 5) {
    InstructionType instruction_type = disassembler.Disassemble(
        reinterpret_cast<unsigned char*>(original_function_stub) +
        preamble_bytes, preamble_bytes);
    if (IT_GENERIC != instruction_type) {
      ASSERT(false, "Should only have generic instructions in stub!!");
      return SIDESTEP_UNSUPPORTED_INSTRUCTION;
    }
  }

  // Before unpatching, target_function should be a JMP to
  // replacement_function.  If it's not, then either it's an error, or
  // we're falling into the case where the original instruction was a
  // JMP, and we patched the jumped_to address rather than the JMP
  // itself.  (For instance, if malloc() is just a JMP to __malloc(),
  // we patched __malloc() and not malloc().)
  unsigned char* target = reinterpret_cast<unsigned char*>(target_function);
  while (1) {    // we stop when target is a JMP to replacement_function
    if (target[0] != ASM_JMP32REL) {
      ASSERT(false, "target_function does not look like it was patched.");
      return SIDESTEP_INVALID_PARAMETER;
    }
    int relative_offset;   // Windows guarantees int is 4 bytes
    ASSERT1(sizeof(relative_offset) == 4);
    memcpy(reinterpret_cast<void*>(&relative_offset),
           reinterpret_cast<void*>(target + 1), 4);
    unsigned char* jump_to = target + 5 + relative_offset;
    if (jump_to == replacement_function)
      break;
    target = jump_to;      // follow the jmp
  }

  // We need to be able to write to a process-local copy of the first
  // MAX_PREAMBLE_STUB_SIZE bytes of target_function. We may be giving execute
  // privilege to something that doesn't have it, but that's the price to pay
  // for tools.
  DWORD old_target_function_protect = 0;
  BOOL succeeded = ::VirtualProtect(reinterpret_cast<void*>(target),
                                    MAX_PREAMBLE_STUB_SIZE,
                                    PAGE_EXECUTE_READWRITE,
                                    &old_target_function_protect);
  if (!succeeded) {
    ASSERT(false, "Failed to make page containing target function "
                   "copy-on-write.");
    return SIDESTEP_ACCESS_DENIED;
  }

  // Replace the first few bytes of the original function with the bytes we
  // previously moved to the preamble stub.
  memcpy(reinterpret_cast<void*>(target),
         original_function_stub, preamble_bytes);

  // Stub is now useless so delete it.
  // [csilvers: Commented out for perftools because it causes big problems
  //  when we're unpatching malloc.  We just let this live on as a leak.]
  //delete original_function_stub;

  // Restore the protection of the first MAX_PREAMBLE_STUB_SIZE bytes of
  // target to what they were before we started goofing around.
  succeeded = ::VirtualProtect(reinterpret_cast<void*>(target),
                               MAX_PREAMBLE_STUB_SIZE,
                               old_target_function_protect,
                               &old_target_function_protect);

  // Flush the instruction cache to make sure the processor doesn't execute the
  // old version of the instructions (before our patch).
  //
  // See comment on FlushInstructionCache elsewhere in this file.
  succeeded = ::FlushInstructionCache(::GetCurrentProcess(),
                                      target,
                                      MAX_PREAMBLE_STUB_SIZE);
  if (!succeeded) {
    ASSERT(false, "Failed to flush instruction cache.");
    return SIDESTEP_UNEXPECTED;
  }

/*
  VLOG(1) << "PreamblePatcher::Unpatch successfully unpatched 0x"
          << target_function;
          */
  return SIDESTEP_SUCCESS;
}

};  // namespace sidestep
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/*
 * Implementation of PreamblePatcher
 */

//#include "preamble_patcher.h"

//#include "mini_disassembler.h"

// Definitions of assembly statements we need
#define ASM_JMP32REL 0xE9
#define ASM_INT3 0xCC

namespace sidestep {

SideStepError PreamblePatcher::RawPatchWithStub(
    void* target_function,
    void *replacement_function,
    unsigned char* preamble_stub,
    unsigned long stub_size,
    unsigned long* bytes_needed) {
  if ((NULL == target_function) ||
      (NULL == replacement_function) ||
      (NULL == preamble_stub)) {
    ASSERT(false, "Invalid parameters - either pTargetFunction or "
                  "pReplacementFunction or pPreambleStub were NULL.");
    return SIDESTEP_INVALID_PARAMETER;
  }

  // TODO(V7:joi) Siggi and I just had a discussion and decided that both
  // patching and unpatching are actually unsafe.  We also discussed a
  // method of making it safe, which is to freeze all other threads in the
  // process, check their thread context to see if their eip is currently
  // inside the block of instructions we need to copy to the stub, and if so
  // wait a bit and try again, then unfreeze all threads once we've patched.
  // Not implementing this for now since we're only using SideStep for unit
  // testing, but if we ever use it for production code this is what we
  // should do.
  //
  // NOTE: Stoyan suggests we can write 8 or even 10 bytes atomically using
  // FPU instructions, and on newer processors we could use cmpxchg8b or
  // cmpxchg16b. So it might be possible to do the patching/unpatching
  // atomically and avoid having to freeze other threads.  Note though, that
  // doing it atomically does not help if one of the other threads happens
  // to have its eip in the middle of the bytes you change while you change
  // them.
  unsigned char* target = reinterpret_cast<unsigned char*>(target_function);

  // First, deal with a special case that we see with functions that
  // point into an IAT table (including functions linked statically
  // into the application): these function already starts with
  // ASM_JMP32REL.  For instance, malloc() might be implemented as a
  // JMP to __malloc().  In that case, we replace the destination of
  // the JMP (__malloc), rather than the JMP itself (malloc).  This
  // way we get the correct behavior no matter how malloc gets called.
  if (target[0] == ASM_JMP32REL) {
    // target[1-4] holds the place the jmp goes to, but it's
    // relative to the next instruction.
    int relative_offset;   // Windows guarantees int is 4 bytes
    ASSERT1(sizeof(relative_offset) == 4);
    memcpy(reinterpret_cast<void*>(&relative_offset),
           reinterpret_cast<void*>(target + 1), 4);
    // I'd like to just say "target = target + 5 + relative_offset" here, but
    // I can't, because the new target will need to have its protections set.
    return RawPatchWithStubAndProtections(target + 5 + relative_offset,
                                          replacement_function, preamble_stub,
                                          stub_size, bytes_needed);
  }

  // Let's disassemble the preamble of the target function to see if we can
  // patch, and to see how much of the preamble we need to take.  We need 5
  // bytes for our jmp instruction, so let's find the minimum number of
  // instructions to get 5 bytes.
  MiniDisassembler disassembler;
  unsigned int preamble_bytes = 0;
  while (preamble_bytes < 5) {
    InstructionType instruction_type =
      disassembler.Disassemble(target + preamble_bytes, preamble_bytes);
    if (IT_JUMP == instruction_type) {
      ASSERT(false, "Unable to patch because there is a jump instruction "
                     "in the first 5 bytes.");
      return SIDESTEP_JUMP_INSTRUCTION;
    } else if (IT_RETURN == instruction_type) {
      ASSERT(false, "Unable to patch because function is too short");
      return SIDESTEP_FUNCTION_TOO_SMALL;
    } else if (IT_GENERIC != instruction_type) {
      ASSERT(false, "Disassembler encountered unsupported instruction "
                    "(either unused or unknown)");
      return SIDESTEP_UNSUPPORTED_INSTRUCTION;
    }
  }

  if (NULL != bytes_needed)
    *bytes_needed = preamble_bytes + 5;

  // Inv: cbPreamble is the number of bytes (at least 5) that we need to take
  // from the preamble to have whole instructions that are 5 bytes or more
  // in size total. The size of the stub required is cbPreamble + size of
  // jmp (5)
  if (preamble_bytes + 5 > stub_size) {
    ASSERT1(false);
    return SIDESTEP_INSUFFICIENT_BUFFER;
  }

  // First, copy the preamble that we will overwrite.
  memcpy(reinterpret_cast<void*>(preamble_stub),
         reinterpret_cast<void*>(target), preamble_bytes);

  // Now, make a jmp instruction to the rest of the target function (minus the
  // preamble bytes we moved into the stub) and copy it into our preamble-stub.
  // find address to jump to, relative to next address after jmp instruction
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244)
#endif
  int relative_offset_to_target_rest
    = ((reinterpret_cast<unsigned char*>(target) + preamble_bytes) -
        (preamble_stub + preamble_bytes + 5));
#ifdef _MSC_VER
#pragma warning(pop)
#endif
  // jmp (Jump near, relative, displacement relative to next instruction)
  preamble_stub[preamble_bytes] = ASM_JMP32REL;
  // copy the address
  memcpy(reinterpret_cast<void*>(preamble_stub + preamble_bytes + 1),
    reinterpret_cast<void*>(&relative_offset_to_target_rest), 4);

  // Inv: preamble_stub points to assembly code that will execute the
  // original function by first executing the first cbPreamble bytes of the
  // preamble, then jumping to the rest of the function.

  // Overwrite the first 5 bytes of the target function with a jump to our
  // replacement function.
  // (Jump near, relative, displacement relative to next instruction)
  target[0] = ASM_JMP32REL;

  // Find offset from instruction after jmp, to the replacement function.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244)
#endif
  int offset_to_replacement_function =
    reinterpret_cast<unsigned char*>(replacement_function) -
    reinterpret_cast<unsigned char*>(target) - 5;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
  // complete the jmp instruction
  memcpy(reinterpret_cast<void*>(target + 1),
         reinterpret_cast<void*>(&offset_to_replacement_function), 4);
  // Set any remaining bytes that were moved to the preamble-stub to INT3 so
  // as not to cause confusion (otherwise you might see some strange
  // instructions if you look at the disassembly, or even invalid
  // instructions). Also, by doing this, we will break into the debugger if
  // some code calls into this portion of the code.  If this happens, it
  // means that this function cannot be patched using this patcher without
  // further thought.
  if (preamble_bytes > 5) {
    memset(reinterpret_cast<void*>(target + 5), ASM_INT3, preamble_bytes - 5);
  }

  // Inv: The memory pointed to by target_function now points to a relative
  // jump instruction that jumps over to the preamble_stub.  The preamble
  // stub contains the first stub_size bytes of the original target
  // function's preamble code, followed by a relative jump back to the next
  // instruction after the first cbPreamble bytes.

  return SIDESTEP_SUCCESS;
}

};  // namespace sidestep// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Static class for hooking Win32 API routines.

// Some notes about how to hook Memory Allocation Routines in Windows.
//
// For our purposes we do not hook the libc routines.  There are two
// reasons for this.  First, the libc routines all go through HeapAlloc
// anyway.  So, it's redundant to log both HeapAlloc and malloc.
// Second, it can be tricky to hook in both static and dynamic linkages
// of libc.

//#include <windows.h>
//#include <map>

//#include "base.h"
//#include "memory_hook.h"
//#include "preamble_patcher.h"

//#include "tracey.hpp"

// Calls GetProcAddress, but casts to the correct type.
#define GET_PROC_ADDRESS(hmodule, name) \
  ( (Type_##name)(::GetProcAddress(hmodule, #name)) )

// Macro to declare Patch functions.
#define DECLARE_PATCH(name) Patch<Type_##name> patch_##name

// Macro to install Patch functions.
#define INSTALL_PATCH(name)  do {                                       \
  patch_##name.set_original(GET_PROC_ADDRESS(hkernel32, ##name));       \
  patch_##name.Install(&Perftools_##name);                              \
} while (0)

// Macro to install Patch functions.
#define INSTALL_NTDLLPATCH(name)  do {                                  \
  patch_##name.set_original(GET_PROC_ADDRESS(hntdll, ##name));          \
  patch_##name.Install(&Perftools_##name);                              \
} while (0)

// Macro to uninstall Patch functions.
#define UNINSTALL_PATCH(name) patch_##name.Uninstall();



// Windows APIs to be hooked

// HeapAlloc routines
typedef HANDLE (WINAPI *Type_HeapCreate)(DWORD flOptions,
                                         SIZE_T dwInitialSize,
                                         SIZE_T dwMaximumSize);
typedef BOOL (WINAPI *Type_HeapDestroy)(HANDLE hHeap);
typedef LPVOID (WINAPI *Type_HeapAlloc)(HANDLE hHeap, DWORD dwFlags,
                                        DWORD_PTR dwBytes);
typedef LPVOID (WINAPI *Type_HeapReAlloc)(HANDLE hHeap, DWORD dwFlags,
                                          LPVOID lpMem, SIZE_T dwBytes);
typedef BOOL (WINAPI *Type_HeapFree)(HANDLE hHeap, DWORD dwFlags,
                                     LPVOID lpMem);

// GlobalAlloc routines
typedef HGLOBAL (WINAPI *Type_GlobalAlloc)(UINT uFlags, SIZE_T dwBytes);
typedef HGLOBAL (WINAPI *Type_GlobalReAlloc)(HGLOBAL hMem, SIZE_T dwBytes,
                                             UINT uFlags);
typedef HGLOBAL (WINAPI *Type_GlobalFree)(HGLOBAL hMem);

// LocalAlloc routines
typedef HLOCAL (WINAPI *Type_LocalAlloc)(UINT uFlags, SIZE_T uBytes);
typedef HLOCAL (WINAPI *Type_LocalReAlloc)(HLOCAL hMem, SIZE_T uBytes,
                                           UINT uFlags);
typedef HLOCAL (WINAPI *Type_LocalFree)(HLOCAL hMem);

// A Windows-API equivalent of mmap and munmap, for "anonymous regions"
typedef LPVOID (WINAPI *Type_VirtualAllocEx)(HANDLE process, LPVOID address,
                                             SIZE_T size, DWORD type,
                                             DWORD protect);
typedef BOOL (WINAPI *Type_VirtualFreeEx)(HANDLE process, LPVOID address,
                                          SIZE_T size, DWORD type);

// A Windows-API equivalent of mmap and munmap, for actual files
typedef LPVOID (WINAPI *Type_MapViewOfFile)(HANDLE hFileMappingObject,
                                            DWORD dwDesiredAccess,
                                            DWORD dwFileOffsetHigh,
                                            DWORD dwFileOffsetLow,
                                            SIZE_T dwNumberOfBytesToMap);
typedef LPVOID (WINAPI *Type_MapViewOfFileEx)(HANDLE hFileMappingObject,
                                              DWORD dwDesiredAccess,
                                              DWORD dwFileOffsetHigh,
                                              DWORD dwFileOffsetLow,
                                              SIZE_T dwNumberOfBytesToMap,
                                              LPVOID lpBaseAddress);
typedef BOOL (WINAPI *Type_UnmapViewOfFile)(LPVOID lpBaseAddress);

typedef DWORD (WINAPI *Type_NtUnmapViewOfSection)(HANDLE process,
                                                  LPVOID lpBaseAddress);


// Patch is a template for keeping the pointer to the original
// hooked routine, the function to call when hooked, and the
// stub routine which is patched.
template<class T>
class Patch {
 public:
  // Constructor.  Does not hook the function yet.
  Patch<T>()
    : original_function_(NULL),
      patch_function_(NULL),
      stub_function_(NULL) {
  }

  // Destructor.  Unhooks the function if it has been hooked.
  ~Patch<T>() {
    Uninstall();
  }

  // Patches original function with func.
  // Must have called set_original to set the original function.
  void Install(T func) {
	  patch_function_ = func;
    CHECK(patch_function_ != NULL);
    CHECK(original_function_ != NULL);
    CHECK(stub_function_ == NULL);
    CHECK(sidestep::SIDESTEP_SUCCESS ==
          sidestep::PreamblePatcher::Patch(original_function_,
                                           patch_function_, &stub_function_));
  }

  // Un-patches the function.
  void Uninstall() {
    if (stub_function_)
      sidestep::PreamblePatcher::Unpatch(original_function_,
                                         patch_function_, stub_function_);
    stub_function_ = NULL;
  }

  // Set the function to be patched.
  void set_original(T original) { original_function_ = original; }

  // Get the original function being patched.
  T original() { return original_function_; }

  // Get the patched function.  (e.g. the replacement function)
  T patched() { return patch_function_; }

  // Access to the stub for calling the original function
  // while it is patched.
  T operator()() {
    DCHECK(stub_function_);
    return stub_function_;
  }

 private:
  // The function that we plan to patch.
  T original_function_;
  // The function to replace the original with.
  T patch_function_;
  // To unpatch, we also need to keep around a "stub" that points to the
  // pre-patched Windows function.
  T stub_function_;
};


// All Windows memory-allocation routines call through to one of these.
DECLARE_PATCH(HeapCreate);
DECLARE_PATCH(HeapDestroy);
DECLARE_PATCH(HeapAlloc);
DECLARE_PATCH(HeapReAlloc);
DECLARE_PATCH(HeapFree);
DECLARE_PATCH(VirtualAllocEx);
DECLARE_PATCH(VirtualFreeEx);
DECLARE_PATCH(MapViewOfFile);
DECLARE_PATCH(MapViewOfFileEx);
DECLARE_PATCH(UnmapViewOfFile);
DECLARE_PATCH(GlobalAlloc);
DECLARE_PATCH(GlobalReAlloc);
DECLARE_PATCH(GlobalFree);
DECLARE_PATCH(LocalAlloc);
DECLARE_PATCH(LocalReAlloc);
DECLARE_PATCH(LocalFree);
DECLARE_PATCH(NtUnmapViewOfSection);

// -----------------------------------------
// Our replacement functions.

static HANDLE WINAPI Perftools_HeapCreate(DWORD flOptions,
                                          SIZE_T dwInitialSize,
                                          SIZE_T dwMaximumSize) {
if (dwInitialSize > 4096)
    dwInitialSize = 4096;
  return patch_HeapCreate()(flOptions, dwInitialSize, dwMaximumSize);
}

static BOOL WINAPI Perftools_HeapDestroy(HANDLE hHeap) {
	return patch_HeapDestroy()(hHeap);
}

static LPVOID WINAPI Perftools_HeapAlloc(HANDLE hHeap, DWORD dwFlags,
                                         DWORD_PTR dwBytes) {
	LPVOID rv = patch_HeapAlloc()(hHeap, dwFlags, dwBytes);
  MemoryHook::trace(hHeap, reinterpret_cast<int32>(rv), dwBytes);
  return rv;
}

static BOOL WINAPI Perftools_HeapFree(HANDLE hHeap, DWORD dwFlags,
                                      LPVOID lpMem) {
	size_t size = 0;
  if (lpMem != 0) {
    size = HeapSize(hHeap, 0, lpMem);  // Will crash if lpMem is 0.
    // Note: size could be 0; HeapAlloc does allocate 0 length buffers.
  }
  MemoryHook::untrace(hHeap, reinterpret_cast<int32>(lpMem), size);
  return patch_HeapFree()(hHeap, dwFlags, lpMem);
}

static LPVOID WINAPI Perftools_HeapReAlloc(HANDLE hHeap, DWORD dwFlags,
                                           LPVOID lpMem, SIZE_T dwBytes) {
  // Don't call realloc, but instead do a free/malloc.  The problem is that
  // the builtin realloc may either expand a buffer, or it may simply
  // just call free/malloc.  If so, we will already have tracked the new
  // block via Perftools_HeapAlloc.
  LPVOID rv = HeapAlloc(hHeap, dwFlags, dwBytes);
  DCHECK_EQ((HEAP_REALLOC_IN_PLACE_ONLY & dwFlags), 0);

  // If there was an old buffer, now copy the data to the new buffer.
  if (lpMem != 0) {
    size_t size = HeapSize(hHeap, 0, lpMem);
    if (size > dwBytes)
      size = dwBytes;
    memcpy(rv, lpMem, size );
    // Note: size could be 0; HeapAlloc does allocate 0 length buffers.
    HeapFree(hHeap, dwFlags, lpMem);
  }
  return rv;
}

static LPVOID WINAPI Perftools_VirtualAllocEx(HANDLE process, LPVOID address,
                                              SIZE_T size, DWORD type,
                                              DWORD protect) {
	bool already_committed = false;
  if (address != NULL) {
    MEMORY_BASIC_INFORMATION info;
    CHECK(VirtualQuery(address, &info, sizeof(info)));
    if (info.State & MEM_COMMIT) {
      already_committed = true;
      CHECK(size >= info.RegionSize);
    }
  }
  bool reserving = (address == NULL) || (type & MEM_RESERVE);
  bool committing = !already_committed && (type & MEM_COMMIT);


  LPVOID result = patch_VirtualAllocEx()(process, address, size, type,
                                         protect);
  MEMORY_BASIC_INFORMATION info;
  CHECK(VirtualQuery(result, &info, sizeof(info)));
  size = info.RegionSize;

  if (committing)
    MemoryHook::trace(0, reinterpret_cast<int32>(result), size);

  return result;
}

static BOOL WINAPI Perftools_VirtualFreeEx(HANDLE process, LPVOID address,
                                           SIZE_T size, DWORD type) {
	int chunk_size = size;
  MEMORY_BASIC_INFORMATION info;
  CHECK(VirtualQuery(address, &info, sizeof(info)));
  if (chunk_size == 0)
    chunk_size = info.RegionSize;
  bool decommit = (info.State & MEM_COMMIT) > 0;

  if (decommit)
      MemoryHook::untrace(0, reinterpret_cast<int32>(address),
                                     chunk_size);

  return patch_VirtualFreeEx()(process, address, size, type);
}

namespace {
  struct maps {
    std::mutex known_maps_lock;
    std::map<void*, int> known_maps;
  };
  maps *get() {
		  static char placement[sizeof(maps)];
    static maps *ptr = new ((maps*)placement) maps();
    return ptr;
  }
}

static LPVOID WINAPI Perftools_MapViewOfFileEx(HANDLE hFileMappingObject,
                                               DWORD dwDesiredAccess,
                                               DWORD dwFileOffsetHigh,
                                               DWORD dwFileOffsetLow,
                                               SIZE_T dwNumberOfBytesToMap,
                                               LPVOID lpBaseAddress) {
	// For this function pair, you always deallocate the full block of
  // data that you allocate, so NewHook/DeleteHook is the right API.
  LPVOID result = patch_MapViewOfFileEx()(hFileMappingObject, dwDesiredAccess,
                                           dwFileOffsetHigh, dwFileOffsetLow,
                                           dwNumberOfBytesToMap, lpBaseAddress);
	bool ok = 0;
    MEMORY_BASIC_INFORMATION info;
  {
    std::mutex &known_maps_lock = get()->known_maps_lock;
    std::map<void*, int> &known_maps = get()->known_maps;

    std::lock_guard<std::mutex> lock(known_maps_lock);
    if (known_maps.find(result) == known_maps.end()) {
      CHECK(VirtualQuery(result, &info, sizeof(info)));
      // TODO(mbelshe):  THIS map uses the standard heap!!!!
      known_maps[result] = 1;
	  ok = 1;
    } else {
      known_maps[result] = known_maps[result] + 1;
    }
  }
  if( ok )
        MemoryHook::trace(0, reinterpret_cast<int32>(result),
                                  info.RegionSize);

  return result;
}

static LPVOID WINAPI Perftools_MapViewOfFile(HANDLE hFileMappingObject,
                                               DWORD dwDesiredAccess,
                                               DWORD dwFileOffsetHigh,
                                               DWORD dwFileOffsetLow,
                                               SIZE_T dwNumberOfBytesToMap) {
	return Perftools_MapViewOfFileEx(hFileMappingObject, dwDesiredAccess,
                                   dwFileOffsetHigh, dwFileOffsetLow,
                                   dwNumberOfBytesToMap, 0);
}

static BOOL WINAPI Perftools_UnmapViewOfFile(LPVOID lpBaseAddress) {
	// This will call into NtUnmapViewOfSection().
  return patch_UnmapViewOfFile()(lpBaseAddress);
}

static DWORD WINAPI Perftools_NtUnmapViewOfSection(HANDLE process,
                                                   LPVOID lpBaseAddress) {
	// Some windows APIs call directly into this routine rather
  // than calling UnmapViewOfFile.  If we didn't trap this function,
  // then we appear to have bogus leaks.
	bool ok = 0;
    MEMORY_BASIC_INFORMATION info;
  {
    std::mutex &known_maps_lock = get()->known_maps_lock;
    std::map<void*, int> &known_maps = get()->known_maps;

    std::lock_guard<std::mutex> lock(known_maps_lock);
    CHECK(VirtualQuery(lpBaseAddress, &info, sizeof(info)));
    if (known_maps.find(lpBaseAddress) != known_maps.end()) {
      if (known_maps[lpBaseAddress] == 1) {
		  ok = 1;
        known_maps.erase(lpBaseAddress);
      } else {
        known_maps[lpBaseAddress] = known_maps[lpBaseAddress] - 1;
      }
    }
  }
	if( ok )
  MemoryHook::untrace(0, reinterpret_cast<int32>(lpBaseAddress),
                                       info.RegionSize);

  return patch_NtUnmapViewOfSection()(process, lpBaseAddress);
}

static HGLOBAL WINAPI Perftools_GlobalAlloc(UINT uFlags, SIZE_T dwBytes) {
	// GlobalAlloc is built atop HeapAlloc anyway.  So we don't track these.
  // GlobalAlloc will internally call into HeapAlloc and we track there.

  // Force all memory to be fixed.
  uFlags &= ~GMEM_MOVEABLE;
  HGLOBAL rv = patch_GlobalAlloc()(uFlags, dwBytes);
  return rv;
}

static HGLOBAL WINAPI Perftools_GlobalFree(HGLOBAL hMem) {
	return patch_GlobalFree()(hMem);
}

static HGLOBAL WINAPI Perftools_GlobalReAlloc(HGLOBAL hMem, SIZE_T dwBytes,
                                              UINT uFlags) {
	// TODO(jar): [The following looks like a copy/paste typo from LocalRealloc.]
  // GlobalDiscard is a macro which calls LocalReAlloc with size 0.
  if (dwBytes == 0) {
    return patch_GlobalReAlloc()(hMem, dwBytes, uFlags);
  }

  HGLOBAL rv = Perftools_GlobalAlloc(uFlags, dwBytes);
  if (hMem != 0) {
    size_t size = GlobalSize(hMem);
    if (size > dwBytes)
      size = dwBytes;
    // Note: size could be 0; HeapAlloc does allocate 0 length buffers.
    memcpy(rv, hMem, size);
    Perftools_GlobalFree(hMem);
  }

  return rv;
}

static HLOCAL WINAPI Perftools_LocalAlloc(UINT uFlags, SIZE_T dwBytes) {
	// LocalAlloc is built atop HeapAlloc anyway.  So we don't track these.
  // LocalAlloc will internally call into HeapAlloc and we track there.

  // Force all memory to be fixed.
  uFlags &= ~LMEM_MOVEABLE;
  HLOCAL rv = patch_LocalAlloc()(uFlags, dwBytes);
  return rv;
}

static HLOCAL WINAPI Perftools_LocalFree(HLOCAL hMem) {
	return patch_LocalFree()(hMem);
}

static HLOCAL WINAPI Perftools_LocalReAlloc(HLOCAL hMem, SIZE_T dwBytes,
                                            UINT uFlags) {
	// LocalDiscard is a macro which calls LocalReAlloc with size 0.
  if (dwBytes == 0) {
    return patch_LocalReAlloc()(hMem, dwBytes, uFlags);
  }

  HGLOBAL rv = Perftools_LocalAlloc(uFlags, dwBytes);
  if (hMem != 0) {
    size_t size = LocalSize(hMem);
    if (size > dwBytes)
      size = dwBytes;
    // Note: size could be 0; HeapAlloc does allocate 0 length buffers.
    memcpy(rv, hMem, size);
    Perftools_LocalFree(hMem);
  }

  return rv;
}

// ------------------------------------------

static MemoryHook *global_hook_ = 0;

  MemoryHook::MemoryHook() {

    // Enable memory tracing.  When memory is 'hooked',
    // MemoryWatchers which have registered will be called
    // as memory is allocated and deallocated.
    DCHECK(!global_hook_);
    if (!global_hook_) {

      heap_ = HeapCreate(0, 0, 0);

      // Luckily, Patch() doesn't call malloc or windows alloc routines
      // itself -- though it does call new (we can use PatchWithStub to
      // get around that, and will need to if we need to patch new).

      HMODULE hkernel32 = ::GetModuleHandleA("kernel32");
      CHECK(hkernel32 != NULL);

      HMODULE hntdll = ::GetModuleHandleA("ntdll");
      CHECK(hntdll != NULL);

      // Now that we've found all the functions, patch them
      INSTALL_PATCH(HeapCreate);
      INSTALL_PATCH(HeapDestroy);
      INSTALL_PATCH(HeapReAlloc);
      INSTALL_PATCH(HeapFree);
      INSTALL_PATCH(VirtualAllocEx);
      INSTALL_PATCH(VirtualFreeEx);
      INSTALL_PATCH(MapViewOfFileEx);
      INSTALL_PATCH(MapViewOfFile);
      INSTALL_PATCH(UnmapViewOfFile);
      INSTALL_NTDLLPATCH(NtUnmapViewOfSection);
      INSTALL_PATCH(GlobalAlloc);
      INSTALL_PATCH(GlobalReAlloc);
      INSTALL_PATCH(GlobalFree);
      INSTALL_PATCH(LocalAlloc);
      INSTALL_PATCH(LocalReAlloc);
      INSTALL_PATCH(LocalFree);

  	  INSTALL_PATCH(HeapAlloc);

      // We are finally completely hooked.
      global_hook_ = this;
	}

  }

  MemoryHook::~MemoryHook() {
		// tracey::disable();
    return;

    if (global_hook_) {
      // We need to go back to the system malloc/etc at global destruct time,
      // so objects that were constructed before tcmalloc, using the system
      // malloc, can destroy themselves using the system free.  This depends
      // on DLLs unloading in the reverse order in which they load!
      //
      // We also go back to the default HeapAlloc/etc, just for consistency.
      // Who knows, it may help avoid weird bugs in some situations.
      UNINSTALL_PATCH(HeapCreate);
      UNINSTALL_PATCH(HeapDestroy);
      UNINSTALL_PATCH(HeapAlloc);
      UNINSTALL_PATCH(HeapReAlloc);
      UNINSTALL_PATCH(HeapFree);
      UNINSTALL_PATCH(VirtualAllocEx);
      UNINSTALL_PATCH(VirtualFreeEx);
      UNINSTALL_PATCH(MapViewOfFile);
      UNINSTALL_PATCH(MapViewOfFileEx);
      UNINSTALL_PATCH(UnmapViewOfFile);
      UNINSTALL_PATCH(NtUnmapViewOfSection);
      UNINSTALL_PATCH(GlobalAlloc);
      UNINSTALL_PATCH(GlobalReAlloc);
      UNINSTALL_PATCH(GlobalFree);
      UNINSTALL_PATCH(LocalAlloc);
      UNINSTALL_PATCH(LocalReAlloc);
      UNINSTALL_PATCH(LocalFree);

      // It's a bit dangerous to ever close this heap; MemoryWatchers may have
      // used this heap for their tracking data.  Closing the heap while any
      // MemoryWatchers still exist is pretty dangerous.
      DCHECK(heap_ != NULL);
      HeapDestroy(heap_);
      heap_ = NULL;

      // We are finally completely unhooked.
      global_hook_ = 0;
    }
  }

void* MemoryHook::Alloc(size_t size) {
	return patch_HeapAlloc()(global_hook_->heap_, 0, size);
}
void MemoryHook::Free(void* ptr) {
	patch_HeapFree()(global_hook_->heap_, 0, ptr);
}

  // MemoryObserver interface.
  void MemoryHook::trace(HANDLE heap, int32 id, int32 size) {
		  // Don't notify about allocations to our internal heap.
    if (heap == global_hook_->heap_)
      return;

		tracey::watch((void*)id, size);
  }

  void MemoryHook::untrace(HANDLE heap, int32 id, int32 size) {
		  // Don't notify about allocations to our internal heap.
    if (heap == global_hook_->heap_)
      return;

    tracey::forget( (void*)id );
  }


