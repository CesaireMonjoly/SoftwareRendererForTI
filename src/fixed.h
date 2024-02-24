#define FRACTION_BITS 8
#define FRACTION_SCALE (1 << FRACTION_BITS)

// Define fixed point type
typedef int fixed_point;

// Function to convert integer to fixed point
fixed_point int_to_fixed(int integer_part) {
    return integer_part * FRACTION_SCALE;
}

// Function to convert fixed point to integer
int fixed_to_int(fixed_point fixed) {
    return fixed / FRACTION_SCALE;
}

// Function to convert floating point to fixed point
fixed_point float_to_fixed(float floating_point) {
    return floating_point * FRACTION_SCALE;
}

// Function to convert fixed point to floating point
float fixed_to_float(fixed_point fixed) {
    return (float)fixed / FRACTION_SCALE;
}

// Basic arithmetic operations for fixed point numbers
fixed_point fixed_multiply(fixed_point a, fixed_point b) {
    return ((long long)a * b) / FRACTION_SCALE;
}

fixed_point fixed_divide(fixed_point a, fixed_point b) {
    return ((long long)a * FRACTION_SCALE) / b;
}