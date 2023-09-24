
build/pluginC.elf:     file format elf32-littlearm


Disassembly of section .text:

d0000000 <_Z11init_pluginPi>:
d0000000:	e92d4070 	push	{r4, r5, r6, lr}
d0000004:	e1a04000 	mov	r4, r0
d0000008:	e59f5094 	ldr	r5, [pc, #148]	; d00000a4 <_Z11init_pluginPi+0xa4>
d000000c:	eb000043 	bl	d0000120 <_Z12statics_initv>
d0000010:	e59f3090 	ldr	r3, [pc, #144]	; d00000a8 <_Z11init_pluginPi+0xa8>
d0000014:	e08f5005 	add	r5, pc, r5
d0000018:	e59f108c 	ldr	r1, [pc, #140]	; d00000ac <_Z11init_pluginPi+0xac>
d000001c:	e1a02005 	mov	r2, r5
d0000020:	e7953003 	ldr	r3, [r5, r3]
d0000024:	e5933000 	ldr	r3, [r3]
d0000028:	e5843000 	str	r3, [r4]
d000002c:	e7951001 	ldr	r1, [r5, r1]
d0000030:	e5910000 	ldr	r0, [r1]
d0000034:	e0833000 	add	r3, r3, r0
d0000038:	e5843000 	str	r3, [r4]
d000003c:	e5910004 	ldr	r0, [r1, #4]
d0000040:	e0833000 	add	r3, r3, r0
d0000044:	e5843000 	str	r3, [r4]
d0000048:	e5910008 	ldr	r0, [r1, #8]
d000004c:	e0833000 	add	r3, r3, r0
d0000050:	e5843000 	str	r3, [r4]
d0000054:	e591100c 	ldr	r1, [r1, #12]
d0000058:	e59f0050 	ldr	r0, [pc, #80]	; d00000b0 <_Z11init_pluginPi+0xb0>
d000005c:	e0833001 	add	r3, r3, r1
d0000060:	e283306e 	add	r3, r3, #110	; 0x6e
d0000064:	e5843000 	str	r3, [r4]
d0000068:	e7952000 	ldr	r2, [r5, r0]
d000006c:	e5921000 	ldr	r1, [r2]
d0000070:	e0833001 	add	r3, r3, r1
d0000074:	e5843000 	str	r3, [r4]
d0000078:	e5921004 	ldr	r1, [r2, #4]
d000007c:	e0833001 	add	r3, r3, r1
d0000080:	e5843000 	str	r3, [r4]
d0000084:	e5921008 	ldr	r1, [r2, #8]
d0000088:	e0833001 	add	r3, r3, r1
d000008c:	e5843000 	str	r3, [r4]
d0000090:	e592200c 	ldr	r2, [r2, #12]
d0000094:	e2822054 	add	r2, r2, #84	; 0x54
d0000098:	e0823003 	add	r3, r2, r3
d000009c:	e5843000 	str	r3, [r4]
d00000a0:	e8bd8070 	pop	{r4, r5, r6, pc}
d00000a4:	00000fe4 	andeq	r0, r0, r4, ror #31
d00000a8:	00000018 	andeq	r0, r0, r8, lsl r0
d00000ac:	00000004 	andeq	r0, r0, r4
d00000b0:	00000008 	andeq	r0, r0, r8

d00000b4 <_GLOBAL__sub_I_pluginC.cc>:
d00000b4:	e92d4010 	push	{r4, lr}
d00000b8:	e3a00003 	mov	r0, #3
d00000bc:	e59f4048 	ldr	r4, [pc, #72]	; d000010c <_GLOBAL__sub_I_pluginC.cc+0x58>
d00000c0:	eb000013 	bl	d0000114 <_Z14calc_somethingj>
d00000c4:	e59f2044 	ldr	r2, [pc, #68]	; d0000110 <_GLOBAL__sub_I_pluginC.cc+0x5c>
d00000c8:	e1a01000 	mov	r1, r0
d00000cc:	e08f4004 	add	r4, pc, r4
d00000d0:	e3a00006 	mov	r0, #6
d00000d4:	e1a03004 	mov	r3, r4
d00000d8:	e7944002 	ldr	r4, [r4, r2]
d00000dc:	e5841000 	str	r1, [r4]
d00000e0:	eb00000b 	bl	d0000114 <_Z14calc_somethingj>
d00000e4:	e1a03000 	mov	r3, r0
d00000e8:	e3a00009 	mov	r0, #9
d00000ec:	e5843004 	str	r3, [r4, #4]
d00000f0:	eb000007 	bl	d0000114 <_Z14calc_somethingj>
d00000f4:	e1a03000 	mov	r3, r0
d00000f8:	e3a0000c 	mov	r0, #12
d00000fc:	e5843008 	str	r3, [r4, #8]
d0000100:	eb000003 	bl	d0000114 <_Z14calc_somethingj>
d0000104:	e584000c 	str	r0, [r4, #12]
d0000108:	e8bd8010 	pop	{r4, pc}
d000010c:	00000f2c 	andeq	r0, r0, ip, lsr #30
d0000110:	00000008 	andeq	r0, r0, r8

d0000114 <_Z14calc_somethingj>:
d0000114:	e3003101 	movw	r3, #257	; 0x101
d0000118:	e0000093 	mul	r0, r3, r0
d000011c:	e12fff1e 	bx	lr

d0000120 <_Z12statics_initv>:
d0000120:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
d0000124:	e59f707c 	ldr	r7, [pc, #124]	; d00001a8 <_Z12statics_initv+0x88>
d0000128:	e59f207c 	ldr	r2, [pc, #124]	; d00001ac <_Z12statics_initv+0x8c>
d000012c:	e08f7007 	add	r7, pc, r7
d0000130:	e59f3078 	ldr	r3, [pc, #120]	; d00001b0 <_Z12statics_initv+0x90>
d0000134:	e7972002 	ldr	r2, [r7, r2]
d0000138:	e7975003 	ldr	r5, [r7, r3]
d000013c:	e1520005 	cmp	r2, r5
d0000140:	e0426005 	sub	r6, r2, r5
d0000144:	0a000007 	beq	d0000168 <_Z12statics_initv+0x48>
d0000148:	e1a06146 	asr	r6, r6, #2
d000014c:	e2455004 	sub	r5, r5, #4
d0000150:	e3a04000 	mov	r4, #0
d0000154:	e5b53004 	ldr	r3, [r5, #4]!
d0000158:	e2844001 	add	r4, r4, #1
d000015c:	e12fff33 	blx	r3
d0000160:	e1560004 	cmp	r6, r4
d0000164:	8afffffa 	bhi	d0000154 <_Z12statics_initv+0x34>
d0000168:	e59f2044 	ldr	r2, [pc, #68]	; d00001b4 <_Z12statics_initv+0x94>
d000016c:	e59f3044 	ldr	r3, [pc, #68]	; d00001b8 <_Z12statics_initv+0x98>
d0000170:	e7972002 	ldr	r2, [r7, r2]
d0000174:	e7975003 	ldr	r5, [r7, r3]
d0000178:	e0426005 	sub	r6, r2, r5
d000017c:	e1520005 	cmp	r2, r5
d0000180:	e1a06146 	asr	r6, r6, #2
d0000184:	08bd81f0 	popeq	{r4, r5, r6, r7, r8, pc}
d0000188:	e2455004 	sub	r5, r5, #4
d000018c:	e3a04000 	mov	r4, #0
d0000190:	e5b53004 	ldr	r3, [r5, #4]!
d0000194:	e2844001 	add	r4, r4, #1
d0000198:	e12fff33 	blx	r3
d000019c:	e1560004 	cmp	r6, r4
d00001a0:	8afffffa 	bhi	d0000190 <_Z12statics_initv+0x70>
d00001a4:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
d00001a8:	00000ecc 	andeq	r0, r0, ip, asr #29
d00001ac:	00000010 	andeq	r0, r0, r0, lsl r0
d00001b0:	0000001c 	andeq	r0, r0, ip, lsl r0
d00001b4:	0000000c 	andeq	r0, r0, ip
d00001b8:	00000014 	andeq	r0, r0, r4, lsl r0
d00001bc:	00000000 	andeq	r0, r0, r0

Disassembly of section .init_array:

d0001000 <__init_array_start>:
d0001000:	d00000b4 	strhle	r0, [r0], -r4
d0001004:	d00001c0 	andle	r0, r0, r0, asr #3
d0001008:	d00001e0 	andle	r0, r0, r0, ror #3
d000100c:	d0001020 	andle	r1, r0, r0, lsr #32
d0001010:	d0001000 	andle	r1, r0, r0
d0001014:	d0001000 	andle	r1, r0, r0
d0001018:	d00001f0 	strdle	r0, [r0], -r0	; <UNPREDICTABLE>
d000101c:	d0001000 	andle	r1, r0, r0

Disassembly of section .data:

d00001c0 <init_data>:
d00001c0:	00000001 	andeq	r0, r0, r1
d00001c4:	00000002 	andeq	r0, r0, r2
d00001c8:	00000003 	andeq	r0, r0, r3
d00001cc:	00000004 	andeq	r0, r0, r4

Disassembly of section .got.plt:

d00001d0 <_GLOBAL_OFFSET_TABLE_>:
	...

Disassembly of section .bss:

d00001dc <init_runtime-0x4>:
d00001dc:	00000000 	andeq	r0, r0, r0

d00001e0 <init_runtime>:
	...

d00001f0 <zero_data>:
	...

d00001f8 <_bss_end>:
	...

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00003441 	andeq	r3, r0, r1, asr #8
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	0000002a 	andeq	r0, r0, sl, lsr #32
  10:	412d3705 			; <UNDEFINED> instruction: 0x412d3705
  14:	070a0600 	streq	r0, [sl, -r0, lsl #12]
  18:	09010841 	stmdbeq	r1, {r0, r6, fp}
  1c:	0c050a02 			; <UNDEFINED> instruction: 0x0c050a02
  20:	17041202 	strne	r1, [r4, -r2, lsl #4]
  24:	19011803 	stmdbne	r1, {r0, r1, fp, ip}
  28:	1c011a01 			; <UNDEFINED> instruction: 0x1c011a01
  2c:	2a021e01 	bcs	87838 <__HEAP_SIZE+0x47838>
  30:	44022c01 	strmi	r2, [r2], #-3073	; 0xfffff3ff
  34:	Address 0x0000000000000034 is out of bounds.


Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <__HEAP_SIZE+0x1090d24>
   4:	72412820 	subvc	r2, r1, #32, 16	; 0x200000
   8:	4e47206d 	cdpmi	0, 4, cr2, cr7, cr13, {3}
   c:	6f542055 	svcvs	0x00542055
  10:	68636c6f 	stmdavs	r3!, {r0, r1, r2, r3, r5, r6, sl, fp, sp, lr}^
  14:	206e6961 	rsbcs	r6, lr, r1, ror #18
  18:	322e3231 	eorcc	r3, lr, #268435459	; 0x10000003
  1c:	75422820 	strbvc	r2, [r2, #-2080]	; 0xfffff7e0
  20:	20646c69 	rsbcs	r6, r4, r9, ror #24
  24:	2d6d7261 	sfmcs	f7, 2, [sp, #-388]!	; 0xfffffe7c
  28:	322e3231 	eorcc	r3, lr, #268435459	; 0x10000003
  2c:	20292934 	eorcs	r2, r9, r4, lsr r9
  30:	322e3231 	eorcc	r3, lr, #268435459	; 0x10000003
  34:	3220312e 	eorcc	r3, r0, #-2147483637	; 0x8000000b
  38:	31323230 	teqcc	r2, r0, lsr r2
  3c:	00353032 	eorseq	r3, r5, r2, lsr r0
