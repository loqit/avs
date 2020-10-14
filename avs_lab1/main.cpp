#include <iostream>

using namespace std;

void conv8to16(__int8* data, __int16* res)
{
    __asm
    {
        mov eax, data
        mov ebx, res

        movq mm0, [eax]
        movq mm1, [eax]

        pxor mm2, mm2
        pcmpgtb mm2, mm0
        punpcklbw mm0, mm2
        pcmpgtb mm2, mm1
        punpckhbw mm1, mm2

        movq [ebx], mm0
        movq [ebx + 8], mm1

        emms
    }
}

void mmx_sum(__int16* a, __int16* b, __int16* res)
{
    __asm
    {
        xor ebx, ebx
        xor esi, esi
        xor edi, edi
        mov esi, a
        mov edi, b
        mov ebx, res

        movq mm0, [esi]
        movq mm1, [edi]
        paddsw mm0, mm1

        movq mm2, [esi + 8]
        movq mm3, [edi + 8]
        paddsw mm2, mm3

        movq [ebx], mm0
        movq [ebx + 8], mm2
        emms
    }
}

void mmx_sub(__int16* a, __int16* b, __int16* res)
{
    __asm
    {
        xor ebx, ebx
        xor esi, esi
        xor edi, edi
        mov esi, a
        mov edi, b
        mov ebx, res

        movq mm0, [esi]
        movq mm1, [edi]
        psubsw mm0, mm1

        movq mm2, [esi + 8]
        movq mm3, [edi + 8]
        psubsw mm2, mm3

        movq [ebx], mm0
        movq [ebx + 8], mm2

        emms
    }
}

void mmx_mul(__int16* a, __int16* b, __int16* res)
{
    __asm
    {
        xor esi, esi
        xor edi, edi
        xor ebx, ebx
        mov esi, a
        mov edi, b
        mov ebx, res

        movq mm0, [esi]
        movq mm1, [edi]
        pmullw mm0, mm1

        movq mm2, [esi + 8]
        movq mm3, [edi + 8]
        pmullw mm2, mm3

        movq [ebx], mm0
        movq [ebx + 8], mm2

        emms
    }

}

int main() {

    __int8 a[8] = {1, 42, 100, 14, 57, 8, 23, 11};
    __int8 b[8] = {37, 10, 45, 97, 26, 88, 46, 50};
    __int16 c[8] = {54, 16, 129, 32, 95, 211, 11, 187};
    __int16 d[8] = {43, 45, 235, 80, 35, 60, 294, 6};

    __int16 a16[8], b16[8];
    conv8to16(a, a16);
    conv8to16(b, b16);

    __int16 a_b[8];
    mmx_sum(a16, b16, a_b);

    __int16 ab_c[8];
    mmx_mul(a_b, c, ab_c);

    __int16 res[8];
    mmx_sub(ab_c, d, res);

    for(int i = 0;i < 8; i++)
    {
        cout << res[i] << endl;
    }
    return 0;
}
