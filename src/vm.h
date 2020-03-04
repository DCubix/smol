#ifndef VM_H
#define VM_H

#include <stdint.h>

typedef struct Object_t {
	enum {
		OT_NIL = 0,
		OT_NUMBER,
		OT_BOOL,
		OT_STRING
	} type;
	union {
		void* p;
		double n;
		int b;
	};
} Object;

enum InstructionType {
	IT_NOP = 0
};

typedef struct Instruction_t {
	uint8_t type : 8; 
	union {
		uint32_t value : 24;
		struct { uint8_t a, b, c };
		struct { uint8_t ax; uint16_t bx; };
	};
} Instruction;

typedef struct SmolVM_t {
	Instruction* program;
} SmolVM;

#endif // VM_H