#ifndef UNFOLD_VECTOR_FUNCS_H
#define UNFOLD_VECTOR_FUNCS_H

#include <immintrin.h>


#define MUL_UNFOLD_8(dest_vec, vecA, vecB)              \
    dest_vec##_a = _mm256_mul_pd(vecA##_a, vecB##_a);    \
    dest_vec##_b = _mm256_mul_pd(vecA##_b, vecB##_b);     \
    // dest_vec##_c = _mm256_mul_pd(vecA##_c, vecB##_c);      \
    // dest_vec##_d = _mm256_mul_pd(vecA##_d, vecB##_d);       \
    // dest_vec##_e = _mm256_mul_pd(vecA##_e, vecB##_e);        \
    // // dest_vec##_f = _mm256_mul_pd(vecA##_f, vecB##_f);         \
    // // dest_vec##_g = _mm256_mul_pd(vecA##_g, vecB##_g);          \
    // dest_vec##_h = _mm256_mul_pd(vecA##_h, vecB##_h)


#define SET1_UNFOLD_8(dest_vec, scalar)     \
    dest_vec##_a = _mm256_set1_pd(scalar);   \
    dest_vec##_b = _mm256_set1_pd(scalar);    \
    // dest_vec##_c = _mm256_set1_pd(scalar);     \
    // dest_vec##_d = _mm256_set1_pd(scalar);      \
    // dest_vec##_e = _mm256_set1_pd(scalar);       \
    // // dest_vec##_f = _mm256_set1_pd(scalar);        \
    // // dest_vec##_g = _mm256_set1_pd(scalar);         \
    // dest_vec##_h = _mm256_set1_pd(scalar)


#define SET_UNFOLD_8(dest_vec, scalar_0, scalar_1, scalar_2, scalar_3)      \
    dest_vec##_a = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);    \
    dest_vec##_b = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);     \
    // dest_vec##_c = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);      \
    // dest_vec##_d = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);       \
    // dest_vec##_e = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);        \
    // // dest_vec##_f = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);         \
    // // dest_vec##_g = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3);          \
    // dest_vec##_h = _mm256_set_pd(scalar_0, scalar_1, scalar_2, scalar_3)


#define ADD_UNFOLD_8(dest_vec, vecA, vecB)              \
    dest_vec##_a = _mm256_add_pd(vecA##_a, vecB##_a);    \
    dest_vec##_b = _mm256_add_pd(vecA##_b, vecB##_b);     \
    // dest_vec##_c = _mm256_add_pd(vecA##_c, vecB##_c);      \
    // dest_vec##_d = _mm256_add_pd(vecA##_d, vecB##_d);       \
    // dest_vec##_e = _mm256_add_pd(vecA##_e, vecB##_e);        \
    // // dest_vec##_f = _mm256_add_pd(vecA##_f, vecB##_f);         \
    // // dest_vec##_g = _mm256_add_pd(vecA##_g, vecB##_g);          \
    // dest_vec##_h = _mm256_add_pd(vecA##_h, vecB##_h)


#define SUB_UNFOLD_8(dest_vec, vecA, vecB)              \
    dest_vec##_a = _mm256_sub_pd(vecA##_a, vecB##_a);    \
    dest_vec##_b = _mm256_sub_pd(vecA##_b, vecB##_b);     \
    // dest_vec##_c = _mm256_sub_pd(vecA##_c, vecB##_c);      \
    // dest_vec##_d = _mm256_sub_pd(vecA##_d, vecB##_d);       \
    // dest_vec##_e = _mm256_sub_pd(vecA##_e, vecB##_e);        \
    // // dest_vec##_f = _mm256_sub_pd(vecA##_f, vecB##_f);         \
    // // dest_vec##_g = _mm256_sub_pd(vecA##_g, vecB##_g);          \
    // dest_vec##_h = _mm256_sub_pd(vecA##_h, vecB##_h)


#define MOVEMASK_UNFOLD_8(int_mask, vec_source)         \
int_mask##_a = _mm256_movemask_pd(vec_source##_a);       \
int_mask##_b = _mm256_movemask_pd(vec_source##_b);        \
// int_mask##_c = _mm256_movemask_pd(vec_source##_c);         \
// int_mask##_d = _mm256_movemask_pd(vec_source##_d);          \
// int_mask##_e = _mm256_movemask_pd(vec_source##_e);           \
// // int_mask##_f = _mm256_movemask_pd(vec_source##_f);            \
// // int_mask##_g = _mm256_movemask_pd(vec_source##_g);             \
// int_mask##_h = _mm256_movemask_pd(vec_source##_h)


#define GET_NORMAL_CMP_MASK(dest_vec, vec, ones)   \
    dest_vec##_a = _mm256_and_pd(vec##_a, ones);    \
    dest_vec##_b = _mm256_and_pd(vec##_b, ones);     \
    // dest_vec##_c = _mm256_and_pd(vec##_c, ones);      \
    // dest_vec##_d = _mm256_and_pd(vec##_d, ones);       \
    // dest_vec##_e = _mm256_and_pd(vec##_e, ones);        \
    // dest_vec##_f = _mm256_and_pd(vec##_f, ones);         \
    // dest_vec##_g = _mm256_and_pd(vec##_g, ones);          \
    // dest_vec##_h = _mm256_and_pd(vec##_h, ones)


#define CMP_UNFOLD_8(dest_vec, source_vec, cmp_vec, cmp_type)           \
    dest_vec##_a = _mm256_cmp_pd(source_vec##_a, cmp_vec, cmp_type);     \
    dest_vec##_b = _mm256_cmp_pd(source_vec##_b, cmp_vec, cmp_type);      \
    // dest_vec##_c = _mm256_cmp_pd(source_vec##_c, cmp_vec, cmp_type);       \
    // dest_vec##_d = _mm256_cmp_pd(source_vec##_d, cmp_vec, cmp_type);        \
    // dest_vec##_e = _mm256_cmp_pd(source_vec##_e, cmp_vec, cmp_type);         \
    // dest_vec##_f = _mm256_cmp_pd(source_vec##_f, cmp_vec, cmp_type);          \
    // dest_vec##_g = _mm256_cmp_pd(source_vec##_g, cmp_vec, cmp_type);           \
    // dest_vec##_h = _mm256_cmp_pd(source_vec##_h, cmp_vec, cmp_type)


#define MASK_AND(mask)  (mask##_a && mask##_b) // && mask##_c && mask##_d && mask##_e && mask##_f && mask##_g && mask##_h)


#define DRAW_VEC(it_mask_bits, iteration_num_pd, additives)                                                                                       \
for (size_t i = 0; i < 4; i++)                                                                                                                     \
{                                                                                                                                                   \
    ON_GRAPH_MODE(                                                                                                                                   \
    sf::Color pixel_color = SetPixelColor(iteration_num_pd[i], max_calc_iterations_num * (it_mask_bits && 1), DARK_TURQUOISE);                        \
                                                                                                                                                       \
    size_t num_in_vertex_array = (pixel_num_x + i + additives) + pixel_num_y * window_width;                                                            \
                                                                                                                                                         \
    vertex_array[num_in_vertex_array].position = Vector2f(pixel_num_x + i + additives, pixel_num_y);                                                      \
    vertex_array[num_in_vertex_array].color    = pixel_color;                                                                                              \
    );                                                                                                                                                      \
                                                                                                                                                             \
    it_mask_bits >>= 1;                                                                                                                                       \
}

#endif