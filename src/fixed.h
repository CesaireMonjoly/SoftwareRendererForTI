#define FRACTION_BITS 6
#define FRACTION_SCALE (1 << FRACTION_BITS)

typedef int24_t fixed_point;

// Function to convert integer to fixed point
fixed_point int_to_fixed(int24_t integer_part) {
    return integer_part * FRACTION_SCALE;
}

// Function to convert fixed point to integer
int fixed_to_int(fixed_point fixed) {
    return (int) fixed / FRACTION_SCALE;
}

// Function to convert floating point to fixed point
fixed_point float_to_fixed(float floating_point) {
    return floating_point * FRACTION_SCALE;
}

fixed_point double_to_fixed(double value) {
    return (fixed_point) (value * FRACTION_SCALE);
}

// Function to convert fixed point to floating point
float fixed_to_float(fixed_point fixed) {
    return (float)fixed / FRACTION_SCALE;
}

// Function to convert fixed point to floating point
double fixed_to_double(fixed_point fixed) {
    return (double)fixed / FRACTION_SCALE;
}

// Function to match integer part with decimal part together
int24_t toFixedPoint(int integer_part, int decimalPart) {
    return (integer_part << FRACTION_BITS) + (decimalPart & ((1 << FRACTION_BITS) - 1));
}


// Basic arithmetic operations for fixed point numbers (using short name for readability, e.g. real name will be fixed_multiply)
fixed_point fmul(fixed_point a, fixed_point b) {
    return (fixed_point)((a * b) >> FRACTION_BITS);
}


fixed_point fdiv(fixed_point a, fixed_point b) {
    return ((long long)a << FRACTION_BITS) / b;
}