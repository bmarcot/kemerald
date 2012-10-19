#ifndef __IDT_H__
#define __IDT_H__

#include "segment.h"

#define IDT_N				256

#define IDT_DESC_FLAG_PRESENT		(1 << 15)
#define IDT_DESC_FIELD_IDT_GATE		(0xe << 8)

#define IDT_ADD_DESCRIPTOR(_index_)					\
  _idt_add_descriptor(_index_,						\
		      (uint32_t) isr_stage0_ ## _index_,		\
		      SEG_SELECTOR_RING0_CODE,				\
		      IDT_DESC_FLAG_PRESENT | IDT_DESC_FIELD_IDT_GATE);

/* #define IDT_ADD_DESCRIPTOR(_index_, _type_)				\ */
/*   _idt_add_descriptor(_index_,						\ */
/* 		      (uint32_t) isr_stage0_ ## _type_ ## _index_,	\ */
/* 		      SEG_SELECTOR_RING0_CODE,				\ */
/* 		      IDT_DESC_FLAG_PRESENT | IDT_DESC_FIELD_IDT_GATE); */


struct		_idt_idtr_s
{
  uint16_t	limit;
  uint32_t	base;
} __attribute__ ((packed));
typedef struct _idt_idtr_s idt_idtr_s;


int32_t		idt_init(void);
static void	_idt_add_descriptor(uint8_t, uint32_t, uint16_t, uint16_t);
static void	_idt_load_selector(void);

/**
 * Kemerald Project - IDT descriptors
 * Auto-generated on:	Fri Sep  3 14:52:24     2010
 * Author:		bmarcot
 * Plateform:		CYGWIN_NT-5.1
 * Arch:		i686
 *
 */

#define IDT_ADD_DESCRIPTORS()			\
  IDT_ADD_DESCRIPTOR(0);			\
  IDT_ADD_DESCRIPTOR(1);			\
  IDT_ADD_DESCRIPTOR(2);			\
  IDT_ADD_DESCRIPTOR(3);			\
  IDT_ADD_DESCRIPTOR(4);			\
  IDT_ADD_DESCRIPTOR(5);			\
  IDT_ADD_DESCRIPTOR(6);			\
  IDT_ADD_DESCRIPTOR(7);			\
  IDT_ADD_DESCRIPTOR(8);			\
  IDT_ADD_DESCRIPTOR(9);			\
  IDT_ADD_DESCRIPTOR(10);			\
  IDT_ADD_DESCRIPTOR(11);			\
  IDT_ADD_DESCRIPTOR(12);			\
  IDT_ADD_DESCRIPTOR(13);			\
  IDT_ADD_DESCRIPTOR(14);			\
  IDT_ADD_DESCRIPTOR(15);			\
  IDT_ADD_DESCRIPTOR(16);			\
  IDT_ADD_DESCRIPTOR(17);			\
  IDT_ADD_DESCRIPTOR(18);			\
  IDT_ADD_DESCRIPTOR(19);			\
  IDT_ADD_DESCRIPTOR(20);			\
  IDT_ADD_DESCRIPTOR(21);			\
  IDT_ADD_DESCRIPTOR(22);			\
  IDT_ADD_DESCRIPTOR(23);			\
  IDT_ADD_DESCRIPTOR(24);			\
  IDT_ADD_DESCRIPTOR(25);			\
  IDT_ADD_DESCRIPTOR(26);			\
  IDT_ADD_DESCRIPTOR(27);			\
  IDT_ADD_DESCRIPTOR(28);			\
  IDT_ADD_DESCRIPTOR(29);			\
  IDT_ADD_DESCRIPTOR(30);			\
  IDT_ADD_DESCRIPTOR(31);			\
  IDT_ADD_DESCRIPTOR(32);			\
  IDT_ADD_DESCRIPTOR(33);			\
  IDT_ADD_DESCRIPTOR(34);			\
  IDT_ADD_DESCRIPTOR(35);			\
  IDT_ADD_DESCRIPTOR(36);			\
  IDT_ADD_DESCRIPTOR(37);			\
  IDT_ADD_DESCRIPTOR(38);			\
  IDT_ADD_DESCRIPTOR(39);			\
  IDT_ADD_DESCRIPTOR(40);			\
  IDT_ADD_DESCRIPTOR(41);			\
  IDT_ADD_DESCRIPTOR(42);			\
  IDT_ADD_DESCRIPTOR(43);			\
  IDT_ADD_DESCRIPTOR(44);			\
  IDT_ADD_DESCRIPTOR(45);			\
  IDT_ADD_DESCRIPTOR(46);			\
  IDT_ADD_DESCRIPTOR(47);			\
  IDT_ADD_DESCRIPTOR(48);			\
  IDT_ADD_DESCRIPTOR(49);			\
  IDT_ADD_DESCRIPTOR(50);			\
  IDT_ADD_DESCRIPTOR(51);			\
  IDT_ADD_DESCRIPTOR(52);			\
  IDT_ADD_DESCRIPTOR(53);			\
  IDT_ADD_DESCRIPTOR(54);			\
  IDT_ADD_DESCRIPTOR(55);			\
  IDT_ADD_DESCRIPTOR(56);			\
  IDT_ADD_DESCRIPTOR(57);			\
  IDT_ADD_DESCRIPTOR(58);			\
  IDT_ADD_DESCRIPTOR(59);			\
  IDT_ADD_DESCRIPTOR(60);			\
  IDT_ADD_DESCRIPTOR(61);			\
  IDT_ADD_DESCRIPTOR(62);			\
  IDT_ADD_DESCRIPTOR(63);			\
  IDT_ADD_DESCRIPTOR(64);			\
  IDT_ADD_DESCRIPTOR(65);			\
  IDT_ADD_DESCRIPTOR(66);			\
  IDT_ADD_DESCRIPTOR(67);			\
  IDT_ADD_DESCRIPTOR(68);			\
  IDT_ADD_DESCRIPTOR(69);			\
  IDT_ADD_DESCRIPTOR(70);			\
  IDT_ADD_DESCRIPTOR(71);			\
  IDT_ADD_DESCRIPTOR(72);			\
  IDT_ADD_DESCRIPTOR(73);			\
  IDT_ADD_DESCRIPTOR(74);			\
  IDT_ADD_DESCRIPTOR(75);			\
  IDT_ADD_DESCRIPTOR(76);			\
  IDT_ADD_DESCRIPTOR(77);			\
  IDT_ADD_DESCRIPTOR(78);			\
  IDT_ADD_DESCRIPTOR(79);			\
  IDT_ADD_DESCRIPTOR(80);			\
  IDT_ADD_DESCRIPTOR(81);			\
  IDT_ADD_DESCRIPTOR(82);			\
  IDT_ADD_DESCRIPTOR(83);			\
  IDT_ADD_DESCRIPTOR(84);			\
  IDT_ADD_DESCRIPTOR(85);			\
  IDT_ADD_DESCRIPTOR(86);			\
  IDT_ADD_DESCRIPTOR(87);			\
  IDT_ADD_DESCRIPTOR(88);			\
  IDT_ADD_DESCRIPTOR(89);			\
  IDT_ADD_DESCRIPTOR(90);			\
  IDT_ADD_DESCRIPTOR(91);			\
  IDT_ADD_DESCRIPTOR(92);			\
  IDT_ADD_DESCRIPTOR(93);			\
  IDT_ADD_DESCRIPTOR(94);			\
  IDT_ADD_DESCRIPTOR(95);			\
  IDT_ADD_DESCRIPTOR(96);			\
  IDT_ADD_DESCRIPTOR(97);			\
  IDT_ADD_DESCRIPTOR(98);			\
  IDT_ADD_DESCRIPTOR(99);			\
  IDT_ADD_DESCRIPTOR(100);			\
  IDT_ADD_DESCRIPTOR(101);			\
  IDT_ADD_DESCRIPTOR(102);			\
  IDT_ADD_DESCRIPTOR(103);			\
  IDT_ADD_DESCRIPTOR(104);			\
  IDT_ADD_DESCRIPTOR(105);			\
  IDT_ADD_DESCRIPTOR(106);			\
  IDT_ADD_DESCRIPTOR(107);			\
  IDT_ADD_DESCRIPTOR(108);			\
  IDT_ADD_DESCRIPTOR(109);			\
  IDT_ADD_DESCRIPTOR(110);			\
  IDT_ADD_DESCRIPTOR(111);			\
  IDT_ADD_DESCRIPTOR(112);			\
  IDT_ADD_DESCRIPTOR(113);			\
  IDT_ADD_DESCRIPTOR(114);			\
  IDT_ADD_DESCRIPTOR(115);			\
  IDT_ADD_DESCRIPTOR(116);			\
  IDT_ADD_DESCRIPTOR(117);			\
  IDT_ADD_DESCRIPTOR(118);			\
  IDT_ADD_DESCRIPTOR(119);			\
  IDT_ADD_DESCRIPTOR(120);			\
  IDT_ADD_DESCRIPTOR(121);			\
  IDT_ADD_DESCRIPTOR(122);			\
  IDT_ADD_DESCRIPTOR(123);			\
  IDT_ADD_DESCRIPTOR(124);			\
  IDT_ADD_DESCRIPTOR(125);			\
  IDT_ADD_DESCRIPTOR(126);			\
  IDT_ADD_DESCRIPTOR(127);			\
  IDT_ADD_DESCRIPTOR(128);			\
  IDT_ADD_DESCRIPTOR(129);			\
  IDT_ADD_DESCRIPTOR(130);			\
  IDT_ADD_DESCRIPTOR(131);			\
  IDT_ADD_DESCRIPTOR(132);			\
  IDT_ADD_DESCRIPTOR(133);			\
  IDT_ADD_DESCRIPTOR(134);			\
  IDT_ADD_DESCRIPTOR(135);			\
  IDT_ADD_DESCRIPTOR(136);			\
  IDT_ADD_DESCRIPTOR(137);			\
  IDT_ADD_DESCRIPTOR(138);			\
  IDT_ADD_DESCRIPTOR(139);			\
  IDT_ADD_DESCRIPTOR(140);			\
  IDT_ADD_DESCRIPTOR(141);			\
  IDT_ADD_DESCRIPTOR(142);			\
  IDT_ADD_DESCRIPTOR(143);			\
  IDT_ADD_DESCRIPTOR(144);			\
  IDT_ADD_DESCRIPTOR(145);			\
  IDT_ADD_DESCRIPTOR(146);			\
  IDT_ADD_DESCRIPTOR(147);			\
  IDT_ADD_DESCRIPTOR(148);			\
  IDT_ADD_DESCRIPTOR(149);			\
  IDT_ADD_DESCRIPTOR(150);			\
  IDT_ADD_DESCRIPTOR(151);			\
  IDT_ADD_DESCRIPTOR(152);			\
  IDT_ADD_DESCRIPTOR(153);			\
  IDT_ADD_DESCRIPTOR(154);			\
  IDT_ADD_DESCRIPTOR(155);			\
  IDT_ADD_DESCRIPTOR(156);			\
  IDT_ADD_DESCRIPTOR(157);			\
  IDT_ADD_DESCRIPTOR(158);			\
  IDT_ADD_DESCRIPTOR(159);			\
  IDT_ADD_DESCRIPTOR(160);			\
  IDT_ADD_DESCRIPTOR(161);			\
  IDT_ADD_DESCRIPTOR(162);			\
  IDT_ADD_DESCRIPTOR(163);			\
  IDT_ADD_DESCRIPTOR(164);			\
  IDT_ADD_DESCRIPTOR(165);			\
  IDT_ADD_DESCRIPTOR(166);			\
  IDT_ADD_DESCRIPTOR(167);			\
  IDT_ADD_DESCRIPTOR(168);			\
  IDT_ADD_DESCRIPTOR(169);			\
  IDT_ADD_DESCRIPTOR(170);			\
  IDT_ADD_DESCRIPTOR(171);			\
  IDT_ADD_DESCRIPTOR(172);			\
  IDT_ADD_DESCRIPTOR(173);			\
  IDT_ADD_DESCRIPTOR(174);			\
  IDT_ADD_DESCRIPTOR(175);			\
  IDT_ADD_DESCRIPTOR(176);			\
  IDT_ADD_DESCRIPTOR(177);			\
  IDT_ADD_DESCRIPTOR(178);			\
  IDT_ADD_DESCRIPTOR(179);			\
  IDT_ADD_DESCRIPTOR(180);			\
  IDT_ADD_DESCRIPTOR(181);			\
  IDT_ADD_DESCRIPTOR(182);			\
  IDT_ADD_DESCRIPTOR(183);			\
  IDT_ADD_DESCRIPTOR(184);			\
  IDT_ADD_DESCRIPTOR(185);			\
  IDT_ADD_DESCRIPTOR(186);			\
  IDT_ADD_DESCRIPTOR(187);			\
  IDT_ADD_DESCRIPTOR(188);			\
  IDT_ADD_DESCRIPTOR(189);			\
  IDT_ADD_DESCRIPTOR(190);			\
  IDT_ADD_DESCRIPTOR(191);			\
  IDT_ADD_DESCRIPTOR(192);			\
  IDT_ADD_DESCRIPTOR(193);			\
  IDT_ADD_DESCRIPTOR(194);			\
  IDT_ADD_DESCRIPTOR(195);			\
  IDT_ADD_DESCRIPTOR(196);			\
  IDT_ADD_DESCRIPTOR(197);			\
  IDT_ADD_DESCRIPTOR(198);			\
  IDT_ADD_DESCRIPTOR(199);			\
  IDT_ADD_DESCRIPTOR(200);			\
  IDT_ADD_DESCRIPTOR(201);			\
  IDT_ADD_DESCRIPTOR(202);			\
  IDT_ADD_DESCRIPTOR(203);			\
  IDT_ADD_DESCRIPTOR(204);			\
  IDT_ADD_DESCRIPTOR(205);			\
  IDT_ADD_DESCRIPTOR(206);			\
  IDT_ADD_DESCRIPTOR(207);			\
  IDT_ADD_DESCRIPTOR(208);			\
  IDT_ADD_DESCRIPTOR(209);			\
  IDT_ADD_DESCRIPTOR(210);			\
  IDT_ADD_DESCRIPTOR(211);			\
  IDT_ADD_DESCRIPTOR(212);			\
  IDT_ADD_DESCRIPTOR(213);			\
  IDT_ADD_DESCRIPTOR(214);			\
  IDT_ADD_DESCRIPTOR(215);			\
  IDT_ADD_DESCRIPTOR(216);			\
  IDT_ADD_DESCRIPTOR(217);			\
  IDT_ADD_DESCRIPTOR(218);			\
  IDT_ADD_DESCRIPTOR(219);			\
  IDT_ADD_DESCRIPTOR(220);			\
  IDT_ADD_DESCRIPTOR(221);			\
  IDT_ADD_DESCRIPTOR(222);			\
  IDT_ADD_DESCRIPTOR(223);			\
  IDT_ADD_DESCRIPTOR(224);			\
  IDT_ADD_DESCRIPTOR(225);			\
  IDT_ADD_DESCRIPTOR(226);			\
  IDT_ADD_DESCRIPTOR(227);			\
  IDT_ADD_DESCRIPTOR(228);			\
  IDT_ADD_DESCRIPTOR(229);			\
  IDT_ADD_DESCRIPTOR(230);			\
  IDT_ADD_DESCRIPTOR(231);			\
  IDT_ADD_DESCRIPTOR(232);			\
  IDT_ADD_DESCRIPTOR(233);			\
  IDT_ADD_DESCRIPTOR(234);			\
  IDT_ADD_DESCRIPTOR(235);			\
  IDT_ADD_DESCRIPTOR(236);			\
  IDT_ADD_DESCRIPTOR(237);			\
  IDT_ADD_DESCRIPTOR(238);			\
  IDT_ADD_DESCRIPTOR(239);			\
  IDT_ADD_DESCRIPTOR(240);			\
  IDT_ADD_DESCRIPTOR(241);			\
  IDT_ADD_DESCRIPTOR(242);			\
  IDT_ADD_DESCRIPTOR(243);			\
  IDT_ADD_DESCRIPTOR(244);			\
  IDT_ADD_DESCRIPTOR(245);			\
  IDT_ADD_DESCRIPTOR(246);			\
  IDT_ADD_DESCRIPTOR(247);			\
  IDT_ADD_DESCRIPTOR(248);			\
  IDT_ADD_DESCRIPTOR(249);			\
  IDT_ADD_DESCRIPTOR(250);			\
  IDT_ADD_DESCRIPTOR(251);			\
  IDT_ADD_DESCRIPTOR(252);			\
  IDT_ADD_DESCRIPTOR(253);			\
  IDT_ADD_DESCRIPTOR(254);			\
  IDT_ADD_DESCRIPTOR(255);

extern void	isr_stage0_0(void);
extern void	isr_stage0_1(void);
extern void	isr_stage0_2(void);
extern void	isr_stage0_3(void);
extern void	isr_stage0_4(void);
extern void	isr_stage0_5(void);
extern void	isr_stage0_6(void);
extern void	isr_stage0_7(void);
extern void	isr_stage0_8(void);
extern void	isr_stage0_9(void);
extern void	isr_stage0_10(void);
extern void	isr_stage0_11(void);
extern void	isr_stage0_12(void);
extern void	isr_stage0_13(void);
extern void	isr_stage0_14(void);
extern void	isr_stage0_15(void);
extern void	isr_stage0_16(void);
extern void	isr_stage0_17(void);
extern void	isr_stage0_18(void);
extern void	isr_stage0_19(void);
extern void	isr_stage0_20(void);
extern void	isr_stage0_21(void);
extern void	isr_stage0_22(void);
extern void	isr_stage0_23(void);
extern void	isr_stage0_24(void);
extern void	isr_stage0_25(void);
extern void	isr_stage0_26(void);
extern void	isr_stage0_27(void);
extern void	isr_stage0_28(void);
extern void	isr_stage0_29(void);
extern void	isr_stage0_30(void);
extern void	isr_stage0_31(void);
extern void	isr_stage0_32(void);
extern void	isr_stage0_33(void);
extern void	isr_stage0_34(void);
extern void	isr_stage0_35(void);
extern void	isr_stage0_36(void);
extern void	isr_stage0_37(void);
extern void	isr_stage0_38(void);
extern void	isr_stage0_39(void);
extern void	isr_stage0_40(void);
extern void	isr_stage0_41(void);
extern void	isr_stage0_42(void);
extern void	isr_stage0_43(void);
extern void	isr_stage0_44(void);
extern void	isr_stage0_45(void);
extern void	isr_stage0_46(void);
extern void	isr_stage0_47(void);
extern void	isr_stage0_48(void);
extern void	isr_stage0_49(void);
extern void	isr_stage0_50(void);
extern void	isr_stage0_51(void);
extern void	isr_stage0_52(void);
extern void	isr_stage0_53(void);
extern void	isr_stage0_54(void);
extern void	isr_stage0_55(void);
extern void	isr_stage0_56(void);
extern void	isr_stage0_57(void);
extern void	isr_stage0_58(void);
extern void	isr_stage0_59(void);
extern void	isr_stage0_60(void);
extern void	isr_stage0_61(void);
extern void	isr_stage0_62(void);
extern void	isr_stage0_63(void);
extern void	isr_stage0_64(void);
extern void	isr_stage0_65(void);
extern void	isr_stage0_66(void);
extern void	isr_stage0_67(void);
extern void	isr_stage0_68(void);
extern void	isr_stage0_69(void);
extern void	isr_stage0_70(void);
extern void	isr_stage0_71(void);
extern void	isr_stage0_72(void);
extern void	isr_stage0_73(void);
extern void	isr_stage0_74(void);
extern void	isr_stage0_75(void);
extern void	isr_stage0_76(void);
extern void	isr_stage0_77(void);
extern void	isr_stage0_78(void);
extern void	isr_stage0_79(void);
extern void	isr_stage0_80(void);
extern void	isr_stage0_81(void);
extern void	isr_stage0_82(void);
extern void	isr_stage0_83(void);
extern void	isr_stage0_84(void);
extern void	isr_stage0_85(void);
extern void	isr_stage0_86(void);
extern void	isr_stage0_87(void);
extern void	isr_stage0_88(void);
extern void	isr_stage0_89(void);
extern void	isr_stage0_90(void);
extern void	isr_stage0_91(void);
extern void	isr_stage0_92(void);
extern void	isr_stage0_93(void);
extern void	isr_stage0_94(void);
extern void	isr_stage0_95(void);
extern void	isr_stage0_96(void);
extern void	isr_stage0_97(void);
extern void	isr_stage0_98(void);
extern void	isr_stage0_99(void);
extern void	isr_stage0_100(void);
extern void	isr_stage0_101(void);
extern void	isr_stage0_102(void);
extern void	isr_stage0_103(void);
extern void	isr_stage0_104(void);
extern void	isr_stage0_105(void);
extern void	isr_stage0_106(void);
extern void	isr_stage0_107(void);
extern void	isr_stage0_108(void);
extern void	isr_stage0_109(void);
extern void	isr_stage0_110(void);
extern void	isr_stage0_111(void);
extern void	isr_stage0_112(void);
extern void	isr_stage0_113(void);
extern void	isr_stage0_114(void);
extern void	isr_stage0_115(void);
extern void	isr_stage0_116(void);
extern void	isr_stage0_117(void);
extern void	isr_stage0_118(void);
extern void	isr_stage0_119(void);
extern void	isr_stage0_120(void);
extern void	isr_stage0_121(void);
extern void	isr_stage0_122(void);
extern void	isr_stage0_123(void);
extern void	isr_stage0_124(void);
extern void	isr_stage0_125(void);
extern void	isr_stage0_126(void);
extern void	isr_stage0_127(void);
extern void	isr_stage0_128(void);
extern void	isr_stage0_129(void);
extern void	isr_stage0_130(void);
extern void	isr_stage0_131(void);
extern void	isr_stage0_132(void);
extern void	isr_stage0_133(void);
extern void	isr_stage0_134(void);
extern void	isr_stage0_135(void);
extern void	isr_stage0_136(void);
extern void	isr_stage0_137(void);
extern void	isr_stage0_138(void);
extern void	isr_stage0_139(void);
extern void	isr_stage0_140(void);
extern void	isr_stage0_141(void);
extern void	isr_stage0_142(void);
extern void	isr_stage0_143(void);
extern void	isr_stage0_144(void);
extern void	isr_stage0_145(void);
extern void	isr_stage0_146(void);
extern void	isr_stage0_147(void);
extern void	isr_stage0_148(void);
extern void	isr_stage0_149(void);
extern void	isr_stage0_150(void);
extern void	isr_stage0_151(void);
extern void	isr_stage0_152(void);
extern void	isr_stage0_153(void);
extern void	isr_stage0_154(void);
extern void	isr_stage0_155(void);
extern void	isr_stage0_156(void);
extern void	isr_stage0_157(void);
extern void	isr_stage0_158(void);
extern void	isr_stage0_159(void);
extern void	isr_stage0_160(void);
extern void	isr_stage0_161(void);
extern void	isr_stage0_162(void);
extern void	isr_stage0_163(void);
extern void	isr_stage0_164(void);
extern void	isr_stage0_165(void);
extern void	isr_stage0_166(void);
extern void	isr_stage0_167(void);
extern void	isr_stage0_168(void);
extern void	isr_stage0_169(void);
extern void	isr_stage0_170(void);
extern void	isr_stage0_171(void);
extern void	isr_stage0_172(void);
extern void	isr_stage0_173(void);
extern void	isr_stage0_174(void);
extern void	isr_stage0_175(void);
extern void	isr_stage0_176(void);
extern void	isr_stage0_177(void);
extern void	isr_stage0_178(void);
extern void	isr_stage0_179(void);
extern void	isr_stage0_180(void);
extern void	isr_stage0_181(void);
extern void	isr_stage0_182(void);
extern void	isr_stage0_183(void);
extern void	isr_stage0_184(void);
extern void	isr_stage0_185(void);
extern void	isr_stage0_186(void);
extern void	isr_stage0_187(void);
extern void	isr_stage0_188(void);
extern void	isr_stage0_189(void);
extern void	isr_stage0_190(void);
extern void	isr_stage0_191(void);
extern void	isr_stage0_192(void);
extern void	isr_stage0_193(void);
extern void	isr_stage0_194(void);
extern void	isr_stage0_195(void);
extern void	isr_stage0_196(void);
extern void	isr_stage0_197(void);
extern void	isr_stage0_198(void);
extern void	isr_stage0_199(void);
extern void	isr_stage0_200(void);
extern void	isr_stage0_201(void);
extern void	isr_stage0_202(void);
extern void	isr_stage0_203(void);
extern void	isr_stage0_204(void);
extern void	isr_stage0_205(void);
extern void	isr_stage0_206(void);
extern void	isr_stage0_207(void);
extern void	isr_stage0_208(void);
extern void	isr_stage0_209(void);
extern void	isr_stage0_210(void);
extern void	isr_stage0_211(void);
extern void	isr_stage0_212(void);
extern void	isr_stage0_213(void);
extern void	isr_stage0_214(void);
extern void	isr_stage0_215(void);
extern void	isr_stage0_216(void);
extern void	isr_stage0_217(void);
extern void	isr_stage0_218(void);
extern void	isr_stage0_219(void);
extern void	isr_stage0_220(void);
extern void	isr_stage0_221(void);
extern void	isr_stage0_222(void);
extern void	isr_stage0_223(void);
extern void	isr_stage0_224(void);
extern void	isr_stage0_225(void);
extern void	isr_stage0_226(void);
extern void	isr_stage0_227(void);
extern void	isr_stage0_228(void);
extern void	isr_stage0_229(void);
extern void	isr_stage0_230(void);
extern void	isr_stage0_231(void);
extern void	isr_stage0_232(void);
extern void	isr_stage0_233(void);
extern void	isr_stage0_234(void);
extern void	isr_stage0_235(void);
extern void	isr_stage0_236(void);
extern void	isr_stage0_237(void);
extern void	isr_stage0_238(void);
extern void	isr_stage0_239(void);
extern void	isr_stage0_240(void);
extern void	isr_stage0_241(void);
extern void	isr_stage0_242(void);
extern void	isr_stage0_243(void);
extern void	isr_stage0_244(void);
extern void	isr_stage0_245(void);
extern void	isr_stage0_246(void);
extern void	isr_stage0_247(void);
extern void	isr_stage0_248(void);
extern void	isr_stage0_249(void);
extern void	isr_stage0_250(void);
extern void	isr_stage0_251(void);
extern void	isr_stage0_252(void);
extern void	isr_stage0_253(void);
extern void	isr_stage0_254(void);
extern void	isr_stage0_255(void);

/**
 * End of generated code
 *
 */

#endif /* !__IDT_H__ */
