#ifndef BINVECMUL_H_
#define BINVECMUL_H_

#define vADD(C,A,B)    {C = _mm_xor_si128(A,B);}
#define vSUB(C,A,B)    vADD(C,A,B)
#define vMULT(C,A,B)   {C = _mm_clmulepi64_si128(A,B,0);}
#define vSFTLB(C,A,B)   {C= _mm_slli_si128(A,B);}
#define vSFTRB(C,A,B)   {C= _mm_srli_si128(A,B);}
#define vAND(C,A,B)    {C= _mm_and_si128(A,B);}


extern void bincopy(gfe1x *c, gfe1x *a);
extern void gfe1xnSq(gfe1x *c, gfe1x *a, int n);
extern void gfe1xMult(gfe1x *c, gfe1x *a, gfe1x *b);
extern void gfe1xMultConst(gfe1x *c, gfe1x *a, vec b);
extern void gfe1xAdd(gfe1x *c, gfe1x *a, gfe1x *b);
extern void ladderStepF(gfe1x *x1, gfe1x *z1, gfe1x *x2, gfe1x *z2);
extern void ladderStepV(gfe1x *x1, gfe1x *z1, gfe1x *x2, gfe1x *z2, gfe1x *vbx);
#define gfe1xSq(x,y) gfe1xnSq(x,y,1)


//vec t0,t1,t2;
//vec t00,t11,t22,t33;
//vec f1g0,f0f2,f1f3,g0g2,g1g3;
//vec temp; 

//#define vMULT2(c0,c1,c2,f0,f1,g0,g1){ \
	vMULT(c0,f0,g0); \
	vMULT(c2,f1,g1); \
	vMULT(c1,f0,g1); \
	vMULT(f1g0,f1,g0); \
	vADD(c1,c1,f1g0); \
}

//#define vMULT4(c0,c1,c2,c3,c4,c5,c6,f0,f1,f2,f3,g0,g1,g2,g3){ \
	vMULT2(c0,c1,c2,f0,f1,g0,g1); \
	vMULT2(c4,c5,c6,f2,f3,g2,g3); \
	\
	vADD(f0f2,f0,f2); vADD(f1f3,f1,f3);\
	vADD(g0g2,g0,g2); vADD(g1g3,g1,g3);\
	vMULT2(t0,t1,t2,f0f2,f1f3,g0g2,g1g3); \
	\
	vSUB(t0,t0,c0); vSUB(t0,t0,c4); \
	vSUB(t1,t1,c1); vSUB(t1,t1,c5); \
	vSUB(t2,t2,c2); vSUB(t2,t2,c6); \
	\
	vADD(c2,c2,t0); vADD(c4,c4,t2);\
	c3 = t1; \
}

//#define vSq4(c0,c1,c2,c3,c4,c5,c6,f0,f1,f2,f3){ \
	vMULT(c0,f0,f0); \
	vMULT(c2,f1,f1); \
	vMULT(c4,f2,f2); \
	vMULT(c6,f3,f3); \
	c1 = zero; \
	c3 = zero; \
	c5 = zero; \
}

//#define exapnd(t){ \
	vSFTRB(temp,t[0],8); vAND(t[0],t[0],vMask64);   vADD(t[1],t[1],temp); \
	vSFTRB(temp,t[1],8); vAND(t[1],t[1],vMask64);   vADD(t[2],t[2],temp); \
	vSFTRB(temp,t[2],8); vAND(t[2],t[2],vMask64);   vADD(t[3],t[3],temp); \
	vSFTRB(temp,t[3],8); vAND(t[3],t[3],vMask64);   vADD(t[4],t[4],temp); \
	vSFTRB(temp,t[4],8); vAND(t[4],t[4],vMask64);   vADD(t[5],t[5],temp); \
	vSFTRB(temp,t[5],8); vAND(t[5],t[5],vMask64);   vADD(t[6],t[6],temp); \
	vSFTRB(temp,t[6],8); vAND(t[6],t[6],vMask64);   t[7]=temp; \
}	

//#define reduce(t){ \
	vec	temp; \
	\
	vMULT(t00,t[4],residue2); vMULT(t11,t[5],residue2);  vMULT(t22,t[6],residue2);  vMULT(t33,t[7],residue2);  \
	vADD(t[0],t[0],t00); vADD(t[1],t[1],t11); vADD(t[2],t[2],t22); vADD(t[3],t[3],t33); \
	\
	vSFTRB(temp,t[0],8); vAND(t[0],t[0],vMask64);   vADD(t[1],t[1],temp); \
	vSFTRB(temp,t[1],8); vAND(t[1],t[1],vMask64);   vADD(t[2],t[2],temp); \
	vSFTRB(temp,t[2],8); vAND(t[2],t[2],vMask64);   vADD(t[3],t[3],temp); \
	vSFTRB(t00,t[3],7); temp = _mm_srli_epi64(t00, 3); vAND(t[3],t[3],vMask59);  \
	vMULT(temp,temp,residue1); vADD(t[0],t[0],temp); \
}	


/*void gfe1xMult(gfe1x *c, gfe1x *a, gfe1x *b){
	vec	t[8];
	
	vMULT4(t[0],t[1],t[2],t[3],t[4],t[5],t[6],a->v[0],a->v[1],a->v[2],a->v[3],b->v[0],b->v[1],b->v[2],b->v[3]);
	exapnd(t);
	reduce(t);

	c->v[0] = t[0]; c->v[1] = t[1]; c->v[2] = t[2]; c->v[3] = t[3];
}
*/
/*void gfe1xSq(gfe1x *c, gfe1x *a){
	vec	t[8];
	
	vSq4(t[0],t[1],t[2],t[3],t[4],t[5],t[6],a->v[0],a->v[1],a->v[2],a->v[3]);
	exapnd(t);
	reduce(t);

	c->v[0] = t[0]; c->v[1] = t[1]; c->v[2] = t[2]; c->v[3] = t[3];
}
*/

#endif

