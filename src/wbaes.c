#include "wbaes.h"

u8 randbit()
{
    return cus_random() & 0x1;
}
shares *randshares()
{
    shares *s = (shares *)malloc(sizeof(shares));
    s->a = cus_random() & 0x1;
    s->b = cus_random() & 0x1;
    s->c = cus_random() & 0x1;
    return s;
}
//////////

////////// masking scheme
shares *encode(u8 x, u8 ra, u8 rb)
{
    shares *s = (shares *)malloc(sizeof(shares));;
    s->a = ra;
    s->b = rb;
    s->c = (ra & rb) ^ x;
    return s;
}

u8 decode(shares *s)
{
    return (s->a & s->b) ^ s->c;
}

shares *refresh(shares *m, shares *r)
{
    shares *s = (shares *)malloc(sizeof(shares));
    u8 ma = r->a & (m->b ^ r->c);
    u8 mb = r->b & (m->a ^ r->c);
    r->c = ma ^ mb ^ ((r->a ^ r->c) & (r->b ^ r->c)) ^ r->c;
    s->a = m->a ^ r->a;
    s->b = m->b ^ r->b;
    s->c = m->c ^ r->c;
    return s;
}

shares *evalxor(shares *m, shares *n, shares *rm, shares *rn)
{
    shares *s = (shares *)malloc(sizeof(shares));
    m = refresh(m, rm);
    n = refresh(n, rn);
    s->a = m->a ^ n->a;
    s->b = m->b ^ n->b;
    s->c = m->c ^ n->c ^ (m->a & n->b) ^ (m->b & n->a);
    return s;
}

shares *evaland(shares *m, shares *n, shares *rm, shares *rn)
{
    shares *s = (shares *)malloc(sizeof(shares));
    m = refresh(m, rm);
    n = refresh(n, rn);
    u8 ma = (m->b & n->c) ^ (rm->c & n->b);
    u8 md = (m->c & n->b) ^ (rn->c & m->b);
    s->a = (m->a & n->b) ^ rn->c;
    s->b = (m->b & n->a) ^ rm->c;
    s->c = (m->a & ma) ^ (n->a & md) ^ (rm->c & rn->c) ^ (m->c & n->c);
    return s;
}

shares *evalnot(shares *m)
{
    shares *s = (shares *)malloc(sizeof(shares));
    s->a = m->a;
    s->b = m->b;
    s->c = (~ (m->c)) & 0x1;
    return s;
}
/////////// masking sbox
void bs_wbsbox(shares *x[8])
{
    shares
        *y1,*y2,*y3,*y4,*y5,*y6,*y7,*y8,
        *y9,*y10,*y11,*y12,*y13,*y14,*y15,*y16,
        *y17,*y18,*y19,*y20,*y21;

    shares
        *t0,*t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,
        *t9,*t10,*t11,*t12,*t13,*t14,*t15,
        *t16,*t17,*t18,*t19,*t20,*t21,*t22,
        *t23,*t24,*t25,*t26,*t27,*t28,*t29,
        *t30,*t31,*t32,*t33,*t34,*t35,*t36,
        *t37,*t38,*t39,*t40,*t41,*t42,*t43,
        *t44,*t45,*t46,*t47,*t48,*t49,*t50,
        *t51,*t52,*t53,*t54,*t55,*t56,*t57,
        *t58,*t59,*t60,*t61,*t62,*t63,*t64,
        *t65,*t66,*t67;

    shares
        *z0,*z1,*z2,*z3,*z4,*z5,*z6,*z7,*z8,
        *z9,*z10,*z11,*z12,*z13,*z14,
        *z15,*z16,*z17;

    y14 = evalxor(x[3], x[5], randshares(), randshares());
    y13 = evalxor(x[0], x[6], randshares(), randshares());
    y9 = evalxor(x[0], x[3], randshares(), randshares());
    y8 = evalxor(x[0], x[5], randshares(), randshares());
    t0 = evalxor(x[1], x[2], randshares(), randshares());
    y1 = evalxor(t0, x[7], randshares(), randshares());
    y4 = evalxor(y1, x[3], randshares(), randshares());
    y12 = evalxor(y13, y14, randshares(), randshares());
    y2 = evalxor(y1, x[0], randshares(), randshares());
    y5 = evalxor(y1, x[6], randshares(), randshares());
    y3 = evalxor(y5, y8, randshares(), randshares());
    t1 = evalxor(x[4], y12, randshares(), randshares());
    y15 = evalxor(t1, x[5], randshares(), randshares());
    y20 = evalxor(t1, x[1], randshares(), randshares());
    y6 = evalxor(y15, x[7], randshares(), randshares());
    y10 = evalxor(y15, t0, randshares(), randshares());
    y11 = evalxor(y20, y9, randshares(), randshares());
    y7 = evalxor(x[7], y11, randshares(), randshares());
    y17 = evalxor(y10, y11, randshares(), randshares());
    y19 = evalxor(y10, y8, randshares(), randshares());
    y16 = evalxor(t0, y11, randshares(), randshares());
    y21 = evalxor(y13, y16, randshares(), randshares());
    y18 = evalxor(x[0], y16, randshares(), randshares());

    t2 = evaland(y12, y15, randshares(), randshares());
    t3 = evaland(y3, y6, randshares(), randshares());
    t4 = evalxor(t3, t2, randshares(), randshares());
    t5 = evaland(y4, x[7], randshares(), randshares());
    t6 = evalxor(t5, t2, randshares(), randshares());
    t7 = evaland(y13, y16, randshares(), randshares());
    t8 = evaland(y5, y1, randshares(), randshares());
    t9 = evalxor(t8, t7, randshares(), randshares());
    t10 = evaland(y2, y7, randshares(), randshares());
    t11 = evalxor(t10, t7, randshares(), randshares());
    t12 = evaland(y9, y11, randshares(), randshares());
    t13 = evaland(y14, y17, randshares(), randshares());
    t14 = evalxor(t13, t12, randshares(), randshares());
    t15 = evaland(y8, y10, randshares(), randshares());
    t16 = evalxor(t15, t12, randshares(), randshares());
    t17 = evalxor(t4, t14, randshares(), randshares());
    t18 = evalxor(t6,  t16, randshares(), randshares());
    t19 = evalxor(t9, t14, randshares(), randshares());
    t20 = evalxor(t11,  t16, randshares(), randshares());
    t21 = evalxor(t17, y20, randshares(), randshares());
    t22 = evalxor(t18, y19, randshares(), randshares());
    t23 = evalxor(t19, y21, randshares(), randshares());
    t24 = evalxor(t20, y18, randshares(), randshares());

    t25 = evalxor(t21, t22, randshares(), randshares());
    t26 = evaland(t21, t23, randshares(), randshares());
    t27 = evalxor(t24, t26, randshares(), randshares());
    t28 = evaland(t25, t27, randshares(), randshares());
    t29 = evalxor(t28, t22, randshares(), randshares());
    t30 = evalxor(t23, t24, randshares(), randshares());
    t31 = evalxor(t22, t26, randshares(), randshares());
    t32 = evaland(t31, t30, randshares(), randshares());
    t33 = evalxor(t32, t24, randshares(), randshares());
    t34 = evalxor(t23, t33, randshares(), randshares());
    t35 = evalxor(t27, t33, randshares(), randshares());
    t36 = evaland(t24, t35, randshares(), randshares());
    t37 = evalxor(t36, t34, randshares(), randshares());
    t38 = evalxor(t27, t36, randshares(), randshares());
    t39 = evaland(t29, t38, randshares(), randshares());
    t40 = evalxor(t25, t39, randshares(), randshares());

    t41 = evalxor(t40, t37, randshares(), randshares());
    t42 = evalxor(t29, t33, randshares(), randshares());
    t43 = evalxor(t29, t40, randshares(), randshares());
    t44 = evalxor(t33, t37, randshares(), randshares());
    t45 = evalxor(t42, t41, randshares(), randshares());
    z0 = evaland(t44, y15, randshares(), randshares());
    z1 = evaland(t37, y6, randshares(), randshares());
    z2 = evaland(t33, x[7], randshares(), randshares());
    z3 = evaland(t43, y16, randshares(), randshares());
    z4 = evaland(t40, y1, randshares(), randshares());
    z5 = evaland(t29, y7, randshares(), randshares());
    z6 = evaland(t42, y11, randshares(), randshares());
    z7 = evaland(t45, y17, randshares(), randshares());
    z8 = evaland(t41, y10, randshares(), randshares());
    z9 = evaland(t44, y12, randshares(), randshares());
    z10 = evaland(t37, y3, randshares(), randshares());
    z11 = evaland(t33, y4, randshares(), randshares());
    z12 = evaland(t43, y13, randshares(), randshares());
    z13 = evaland(t40, y5, randshares(), randshares());
    z14 = evaland(t29, y2, randshares(), randshares());
    z15 = evaland(t42, y9, randshares(), randshares());
    z16 = evaland(t45, y14, randshares(), randshares());
    z17 = evaland(t41, y8, randshares(), randshares());

    t46 = evalxor(z15, z16, randshares(), randshares());
    t47 = evalxor(z10, z11, randshares(), randshares());
    t48 = evalxor(z5, z13, randshares(), randshares());
    t49 = evalxor(z9, z10, randshares(), randshares());
    t50 = evalxor(z2, z12, randshares(), randshares());
    t51 = evalxor(z2, z5, randshares(), randshares());
    t52 = evalxor(z7, z8, randshares(), randshares());
    t53 = evalxor(z0, z3, randshares(), randshares());
    t54 = evalxor(z6, z7, randshares(), randshares());
    t55 = evalxor(z16, z17, randshares(), randshares());
    t56 = evalxor(z12, t48, randshares(), randshares());
    t57 = evalxor(t50, t53, randshares(), randshares());
    t58 = evalxor(z4, t46, randshares(), randshares());
    t59 = evalxor(z3, t54, randshares(), randshares());
    t60 = evalxor(t46, t57, randshares(), randshares());
    t61 = evalxor(z14, t57, randshares(), randshares());
    t62 = evalxor(t52, t58, randshares(), randshares());
    t63 = evalxor(t49, t58, randshares(), randshares());
    t64 = evalxor(z4, t59, randshares(), randshares());
    t65 = evalxor(t61, t62, randshares(), randshares());
    t66 = evalxor(z1, t63, randshares(), randshares());
    x[0] = evalxor(t59, t63, randshares(), randshares());
    x[6] = evalnot(evalxor(t56, t62, randshares(), randshares()));
    x[7] = evalnot(evalxor(t48, t60, randshares(), randshares()));
    t67 = evalxor(t64, t65, randshares(), randshares());
    x[3] = evalxor(t53, t66, randshares(), randshares());
    x[4] = evalxor(t51, t66, randshares(), randshares());
    x[5] = evalxor(t47, t65, randshares(), randshares());
    x[1] = evalnot(evalxor(t64, x[3], randshares(), randshares()));
    x[2] = evalnot(evalxor(t55, t67, randshares(), randshares()));
}
//////////// masking mc
void bs_wbmc(shares *x[32])
{
    shares *s[32];
    shares *t0, *t1, *t2, *t3, *t4;
    t0 = evalxor(x[1], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[9], randshares(), randshares());
    t2 = evalxor(t1, x[16], randshares(), randshares());
    s[0] = evalxor(t2, x[24], randshares(), randshares());

    t0 = evalxor(x[2], x[9], randshares(), randshares());
    t1 = evalxor(t0, x[10], randshares(), randshares());
    t2 = evalxor(t1, x[17], randshares(), randshares());
    s[1] = evalxor(t2, x[25], randshares(), randshares());

    t0 = evalxor(x[3], x[10], randshares(), randshares());
    t1 = evalxor(t0, x[11], randshares(), randshares());
    t2 = evalxor(t1, x[18], randshares(), randshares());
    s[2] = evalxor(t2, x[26], randshares(), randshares());

    t0 = evalxor(x[0], x[4], randshares(), randshares());
    t1 = evalxor(t0, x[8], randshares(), randshares());
    t2 = evalxor(t1, x[11], randshares(), randshares());
    t3 = evalxor(t2, x[12], randshares(), randshares());
    t4 = evalxor(t3, x[19], randshares(), randshares());
    s[3] = evalxor(t4, x[27], randshares(), randshares());

    t0 = evalxor(x[0], x[5], randshares(), randshares());
    t1 = evalxor(t0, x[8], randshares(), randshares());
    t2 = evalxor(t1, x[12], randshares(), randshares());
    t3 = evalxor(t2, x[13], randshares(), randshares());
    t4 = evalxor(t3, x[20], randshares(), randshares());
    s[4] = evalxor(t4, x[28], randshares(), randshares());

    t0 = evalxor(x[6], x[13], randshares(), randshares());
    t1 = evalxor(t0, x[14], randshares(), randshares());
    t2 = evalxor(t1, x[21], randshares(), randshares());
    s[5] = evalxor(t2, x[29], randshares(), randshares());

    t0 = evalxor(x[0], x[7], randshares(), randshares());
    t1 = evalxor(t0, x[8], randshares(), randshares());
    t2 = evalxor(t1, x[14], randshares(), randshares());
    t3 = evalxor(t2, x[15], randshares(), randshares());
    t4 = evalxor(t3, x[22], randshares(), randshares());
    s[6] = evalxor(t4, x[30], randshares(), randshares());

    t0 = evalxor(x[0], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[15], randshares(), randshares());
    t2 = evalxor(t1, x[23], randshares(), randshares());
    s[7] = evalxor(t2, x[31], randshares(), randshares());

    t0 = evalxor(x[0], x[9], randshares(), randshares());
    t1 = evalxor(t0, x[16], randshares(), randshares());
    t2 = evalxor(t1, x[17], randshares(), randshares());
    s[8] = evalxor(t2, x[24], randshares(), randshares());

    t0 = evalxor(x[1], x[10], randshares(), randshares());
    t1 = evalxor(t0, x[17], randshares(), randshares());
    t2 = evalxor(t1, x[18], randshares(), randshares());
    s[9] = evalxor(t2, x[25], randshares(), randshares());

    t0 = evalxor(x[2], x[11], randshares(), randshares());
    t1 = evalxor(t0, x[18], randshares(), randshares());
    t2 = evalxor(t1, x[19], randshares(), randshares());
    s[10] = evalxor(t2, x[26], randshares(), randshares());

    t0 = evalxor(x[3], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[12], randshares(), randshares());
    t2 = evalxor(t1, x[16], randshares(), randshares());
    t3 = evalxor(t2, x[19], randshares(), randshares());
    t4 = evalxor(t3, x[20], randshares(), randshares());
    s[11] = evalxor(t4, x[27], randshares(), randshares());

    t0 = evalxor(x[4], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[13], randshares(), randshares());
    t2 = evalxor(t1, x[16], randshares(), randshares());
    t3 = evalxor(t2, x[20], randshares(), randshares());
    t4 = evalxor(t3, x[21], randshares(), randshares());
    s[12] = evalxor(t4, x[28], randshares(), randshares());

    t0 = evalxor(x[5], x[14], randshares(), randshares());
    t1 = evalxor(t0, x[21], randshares(), randshares());
    t2 = evalxor(t1, x[22], randshares(), randshares());
    s[13] = evalxor(t2, x[29], randshares(), randshares());

    t0 = evalxor(x[6], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[15], randshares(), randshares());
    t2 = evalxor(t1, x[16], randshares(), randshares());
    t3 = evalxor(t2, x[22], randshares(), randshares());
    t4 = evalxor(t3, x[23], randshares(), randshares());
    s[14] = evalxor(t4, x[30], randshares(), randshares());

    t0 = evalxor(x[7], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[16], randshares(), randshares());
    t2 = evalxor(t1, x[23], randshares(), randshares());
    s[15] = evalxor(t2, x[31], randshares(), randshares());

    t0 = evalxor(x[0], x[8], randshares(), randshares());
    t1 = evalxor(t0, x[17], randshares(), randshares());
    t2 = evalxor(t1, x[24], randshares(), randshares());
    s[16] = evalxor(t2, x[25], randshares(), randshares());

    t0 = evalxor(x[1], x[9], randshares(), randshares());
    t1 = evalxor(t0, x[18], randshares(), randshares());
    t2 = evalxor(t1, x[25], randshares(), randshares());
    s[17] = evalxor(t2, x[26], randshares(), randshares());

    t0 = evalxor(x[2], x[10], randshares(), randshares());
    t1 = evalxor(t0, x[19], randshares(), randshares());
    t2 = evalxor(t1, x[26], randshares(), randshares());
    s[18] = evalxor(t2, x[27], randshares(), randshares());

    t0 = evalxor(x[3], x[11], randshares(), randshares());
    t1 = evalxor(t0, x[16], randshares(), randshares());
    t2 = evalxor(t1, x[20], randshares(), randshares());
    t3 = evalxor(t2, x[24], randshares(), randshares());
    t4 = evalxor(t3, x[27], randshares(), randshares());
    s[19] = evalxor(t4, x[28], randshares(), randshares());

    t0 = evalxor(x[4], x[12], randshares(), randshares());
    t1 = evalxor(t0, x[16], randshares(), randshares());
    t2 = evalxor(t1, x[21], randshares(), randshares());
    t3 = evalxor(t2, x[24], randshares(), randshares());
    t4 = evalxor(t3, x[28], randshares(), randshares());
    s[20] = evalxor(t4, x[29], randshares(), randshares());

    t0 = evalxor(x[5], x[13], randshares(), randshares());
    t1 = evalxor(t0, x[22], randshares(), randshares());
    t2 = evalxor(t1, x[29], randshares(), randshares());
    s[21] = evalxor(t2, x[30], randshares(), randshares());

    t0 = evalxor(x[6], x[14], randshares(), randshares());
    t1 = evalxor(t0, x[16], randshares(), randshares());
    t2 = evalxor(t1, x[23], randshares(), randshares());
    t3 = evalxor(t2, x[24], randshares(), randshares());
    t4 = evalxor(t3, x[30], randshares(), randshares());
    s[22] = evalxor(t4, x[31], randshares(), randshares());

    t0 = evalxor(x[7], x[15], randshares(), randshares());
    t1 = evalxor(t0, x[16], randshares(), randshares());
    t2 = evalxor(t1, x[24], randshares(), randshares());
    s[23] = evalxor(t2, x[31], randshares(), randshares());

    t0 = evalxor(x[0], x[1], randshares(), randshares());
    t1 = evalxor(t0, x[8], randshares(), randshares());
    t2 = evalxor(t1, x[16], randshares(), randshares());
    s[24] = evalxor(t2, x[25], randshares(), randshares());

    t0 = evalxor(x[1], x[2], randshares(), randshares());
    t1 = evalxor(t0, x[9], randshares(), randshares());
    t2 = evalxor(t1, x[17], randshares(), randshares());
    s[25] = evalxor(t2, x[26], randshares(), randshares());

    t0 = evalxor(x[2], x[3], randshares(), randshares());
    t1 = evalxor(t0, x[10], randshares(), randshares());
    t2 = evalxor(t1, x[18], randshares(), randshares());
    s[26] = evalxor(t2, x[27], randshares(), randshares());

    t0 = evalxor(x[0], x[3], randshares(), randshares());
    t1 = evalxor(t0, x[4], randshares(), randshares());
    t2 = evalxor(t1, x[11], randshares(), randshares());
    t3 = evalxor(t2, x[19], randshares(), randshares());
    t4 = evalxor(t3, x[24], randshares(), randshares());
    s[27] = evalxor(t4, x[28], randshares(), randshares());

    t0 = evalxor(x[0], x[4], randshares(), randshares());
    t1 = evalxor(t0, x[5], randshares(), randshares());
    t2 = evalxor(t1, x[12], randshares(), randshares());
    t3 = evalxor(t2, x[20], randshares(), randshares());
    t4 = evalxor(t3, x[24], randshares(), randshares());
    s[28] = evalxor(t4, x[29], randshares(), randshares());

    t0 = evalxor(x[5], x[6], randshares(), randshares());
    t1 = evalxor(t0, x[13], randshares(), randshares());
    t2 = evalxor(t1, x[21], randshares(), randshares());
    s[29] = evalxor(t2, x[30], randshares(), randshares());

    t0 = evalxor(x[0], x[6], randshares(), randshares());
    t1 = evalxor(t0, x[7], randshares(), randshares());
    t2 = evalxor(t1, x[14], randshares(), randshares());
    t3 = evalxor(t2, x[22], randshares(), randshares());
    t4 = evalxor(t3, x[24], randshares(), randshares());
    s[30] = evalxor(t4, x[31], randshares(), randshares());

    t0 = evalxor(x[0], x[7], randshares(), randshares());
    t1 = evalxor(t0, x[15], randshares(), randshares());
    t2 = evalxor(t1, x[23], randshares(), randshares());
    s[31] = evalxor(t2, x[24], randshares(), randshares());

    memmove(x, s, sizeof(s));
}
void genenkey(u8 key[16], shares *k[11][128])
{
    u8 kk[176];
    int i, j, r;
    u8 temp;
    expandKey(key, kk);
    for(i = 0; i < 11; i++)
    {
        for(j = 0; j < 16; j++)
        {
            temp = kk[i * 16 + j];
            for(r = 0; r < 8; r++)
            {
                if(temp & idM8[r]) k[i][j * 8 + r] = encode(1, randbit(), randbit());
                else k[i][j * 8 + r] = encode(0, randbit(), randbit());
            }
        }
    }
}
void encode_ptx(u8 in[16], shares *x[128])
{
    int j, r;
    u8 temp;
    for(j = 0; j < 16; j++)
    {
        temp = in[j];
        for(r = 0; r < 8; r++)
        {
            if(temp & idM8[r]) x[j * 8 + r] = encode(1, randbit(), randbit());
            else x[j * 8 + r] = encode(0, randbit(), randbit());
        }
    }
    u32 *t = (u32 *)in;
    InitRandom(*t ^ *(t + 1) ^ *(t + 2) ^ *(t + 3));
}

void decode_ctx(shares *x[128], u8 out[16])
{
    int j;
    memset(out, 0, sizeof(u8) * 16);
    for(j = 0; j < 128; j++)
    {
        if(decode(x[j])) out[j / 8] ^= idM8[j % 8];
    }
}
void AddRoundKey(shares *x[128], shares *k[128])
{
    int i;
    for(i = 0; i < 128; i++)
    {
        x[i] = evalxor(x[i], k[i], randshares(), randshares());
    }
}
void SubBytes(shares *x[128])
{
    int i;
    for(i = 0; i < 16; i++)
    {
        bs_wbsbox(x + i * 8);
    }
}
void ShiftRows(shares *x[128])
{
    bs_wbsr(x);
}
void MixColumns(shares *x[128])
{
    bs_wbmc(x);
    bs_wbmc(x + 32);
    bs_wbmc(x + 64);
    bs_wbmc(x + 96);
}
void wbaes(shares *x[128], shares *k[11][128], shares *y[128])
{
    int r;
    memmove(y, x, sizeof(shares*) * 128);
    AddRoundKey(y, k[0]);
    for(r = 1; r < 10; r++)
    {
        SubBytes(y);
        ShiftRows(y);
        MixColumns(y);
        AddRoundKey(y, k[r]);
    }
    SubBytes(y);
    ShiftRows(y);
    AddRoundKey(y, k[10]);
}
//////////// masking shiftrows
void bs_wbsr(shares *x[128])
{
    shares *s[128];
    int i;
    for(i = 0; i < 8; i++)
    {
        s[i] = x[i];
        s[32 + i] = x[32 + i];
        s[64 + i] = x[64 + i];
        s[96 + i] = x[96 + i];

        s[8 + i] = x[40 + i];
        s[40 + i] = x[72 + i];
        s[72 + i] = x[104 + i];
        s[104 + i] = x[8 + i];

        s[16 + i] = x[80 + i];
        s[48 + i] = x[112 + i];
        s[80 + i] = x[16 + i];
        s[112 + i] = x[48 + i];

        s[24 + i] = x[120 + i];
        s[56 + i] = x[24 + i];
        s[88 + i] = x[56 + i];
        s[120 + i] = x[88 + i];
    }
    memmove(x, s, sizeof(s));
}