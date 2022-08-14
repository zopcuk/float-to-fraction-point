#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define fractional_bits 5
#define fixed_type_bits 16
#define DEG_2_RAD (M_PI / 180)

typedef int16_t fixed_type;
typedef int32_t expand_type;

fixed_type float_to_fixed(float inp);
float fixed_to_float(fixed_type inp);
fixed_type fixed_mult(fixed_type inp_1, fixed_type inp_2);
fixed_type fixed_add(fixed_type inp_1, fixed_type inp_2);

int main()
{
    float X1 = 3.5;
    float Y1 = -2.0;
    float trans_X1 = 1.5;
    float trans_Y1 = 1.5;
    float rot_angle = 30;
    float rot_X1 =cos(rot_angle*DEG_2_RAD);
    float rot_Y1 =sin(rot_angle*DEG_2_RAD);
    float X2, Y2;
    fixed_type fixed_X, fixed_Y;


    /*float fp_test_X, fp_test_Y;
    fp_test_X = X1+trans_X1;
    fp_test_Y = Y1+trans_Y1;
    fp_test_X = fp_test_X*rot_X1 - fp_test_Y*rot_Y1;
    fp_test_Y = fp_test_X*rot_Y1 + fp_test_Y*rot_X1;
    printf("float point result: (%f, %f)\n", fp_test_X, fp_test_Y);*/

    //VECTORS
    fixed_type original_vector[] = {float_to_fixed(X1), float_to_fixed(Y1)};
    fixed_type translation_vector[] = {float_to_fixed(trans_X1), float_to_fixed(trans_Y1)};
    fixed_type rotation_vector[] = {float_to_fixed(rot_X1), float_to_fixed(rot_Y1)};

    //TRANSLATION // x' = x + Tx // y' = y + Ty
    fixed_X = fixed_add(original_vector[0], translation_vector[0]);
    fixed_Y = fixed_add(original_vector[1], translation_vector[1]);

    //ROTATION // x' = xcosA - ysinA // y' = xsinA + ycosA
    fixed_X = fixed_add(fixed_mult(fixed_X, rotation_vector[0]), -fixed_mult(fixed_Y, rotation_vector[1]));
    fixed_Y = fixed_add(fixed_mult(fixed_X, rotation_vector[1]), fixed_mult(fixed_Y, rotation_vector[0]));

    //RESULT
    X2 = fixed_to_float(fixed_X);
    Y2 = fixed_to_float(fixed_Y);

    //OUTPUT
    printf("Original vector = (%f, %f)\n", X1, Y1);
    printf("Translation vector = (%f, %f)\n", trans_X1, trans_Y1);
    printf("Rotation angle = %f degree\n", rot_angle);
    printf("Result vector = (%f, %f)", X2, Y2);

    return 0;
}

fixed_type float_to_fixed(float inp)
{
    return (fixed_type)(inp * (1 << fractional_bits));
}

float fixed_to_float(fixed_type inp)
{
    return ((float)inp) / (1 << fractional_bits);
}

fixed_type fixed_mult(fixed_type inp_1, fixed_type inp_2)
{
    fixed_type inp_1_sign = inp_1 >> (fixed_type_bits - 1);
    fixed_type inp_2_sign = inp_2 >> (fixed_type_bits - 1);
    fixed_type mult = (fixed_type)(((expand_type)inp_1 * (expand_type)inp_2) >> fractional_bits);
    fixed_type mult_sign = mult >> (fixed_type_bits - 1);

    if ((inp_1_sign != inp_2_sign && mult_sign == -1) || (inp_1_sign == 0 && inp_2_sign == 0 && mult_sign == 0) || (inp_1_sign == -1 && inp_2_sign == -1 && mult_sign == 0))
    {
        return mult;
    }
    else if ((inp_1_sign != inp_2_sign) && mult_sign == 0)
    {
        return (1 << (fixed_type_bits - 1));
    }
    else
    {
        return ((1 << (fixed_type_bits - 2)) - 1 + (1 << (fixed_type_bits - 2)));
    }
}

fixed_type fixed_add(fixed_type inp_1, fixed_type inp_2)
{
    fixed_type inp_1_sign = inp_1 >> (fixed_type_bits - 1);
    fixed_type inp_2_sign = inp_2 >> (fixed_type_bits - 1);
    fixed_type add = inp_1 + inp_2;
    fixed_type add_sign = add >> (fixed_type_bits - 1);

    if (inp_1_sign != inp_2_sign)
    {
        return add;
    }
    else if (add_sign == inp_1_sign)
    {
        return add;
    }
    else if (add_sign == -1)
    {
        return ((1 << (fixed_type_bits - 2)) - 1 + (1 << (fixed_type_bits - 2)));
    }
    else if (add_sign == 0)
    {
        return (1 << (fixed_type_bits - 1));
    }
}




