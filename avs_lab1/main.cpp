#include <iostream>

using namespace std;

__int16 conv8to16(__int8* data)
{
    __int16* res;
    __asm
    {
        mov eax, data

        movd mm0, [eax]
        movd mm1, [eax]

        pxor mm2, mm2
        pcmpgtb mm2, mm0
        punpcklbw mm0, mm2
        pcmpgtb mm2, mm1
        punpckhbw mm1, mm2

        movd [ebx], mm0
        movd [ebx + 8], mm1

        mov res, ebx
        emms
    }
    return *res;
}

__int16 mmx_sum(__int16* a, __int16* b)
{
    __int16* res;
    __asm
    {
        mov esi, a
        mov edi, b

        movq mm0, [esi]
        movq mm1, [edi]
        paddsw mm0, mm1

        movq [ebx], mm0
        mov res, ebx
        emms
    }
    return *res;
}

__int16 mmx_sub(__int16* a, __int16* b)
{
    __int16* res;
    __asm
    {
        mov esi, a
        mov edi, b

        movq mm0, [esi]
        movq mm1, [edi]
        psubsw mm0, mm1

        movq [ebx], mm0
        mov res, ebx
        emms
    }
    return *res;
}

__int16 mmx_mul(__int16* a, __int16* b)
{
    __int16* res;
    __asm
    {
        mov esi, a
        mov edi, b
        movq mm0, [esi]
        movq mm1, [edi]

        pmulhw mm0, mm1
        movq mm2, [esi]
        pmullw mm1, mm2
        movq mm2, mm0
        movq mm3, mm1
        punpckhwd mm3, mm2
        punpcklwd mm1, mm0

        movq [ebx], mm1
        movq [ebx + 32], mm3
        mov res, ebx
        emms
    }
    return *res;
}

void task()
{
    __int8 a[8] = {1, 42, 100, 14, 57, 8, 23, 11};
    __int8 b[8] = {37, 10, 45, 97, 26, 88, 46, 50};
    __int16 c[8] = {54, 16, 129, 32, 95, 211, 11, 187};
    __int16 d[8] = {43, 45, 235, 80, 35, 60, 294, 6};

    for(int i = 0; i < 8; i++)
    {
        __int16 a16 = conv8to16(&a[i]);
        __int16 b16 = conv8to16(&b[i]);
        __int16 a_b = mmx_sum(&a16, &b16);
        __int16 ab_c = mmx_mul(&a_b, &c[i]);
        __int16 ans = mmx_sub(&ab_c, &d[i]);
        cout << ans << endl;
    }
}

int main() {

    task();
    return 0;
}
