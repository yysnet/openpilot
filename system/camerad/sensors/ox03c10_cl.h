#if SENSOR_ID == 2

#define BIT_DEPTH 12
#define BLACK_LVL 64
#define PWL_MAX 3530 // 17-bit cutoff
#define VIGNETTE_RSZ 1.0f

float ox_lut_func(int x_0) {
  int x = clamp(x_0 - BLACK_LVL, 0, PWL_MAX);
  float ret;
  if (x < 512) {
    ret = x * 5.94873e-8;
  } else if (512 <= x && x < 768) {
    ret = 3.0458e-05 + (x-512) * 1.19913e-7;
  } else if (768 <= x && x < 1536) {
    ret = 6.1154e-05 + (x-768) * 2.38493e-7;
  } else if (1536 <= x && x < 1792) {
    ret = 0.0002448 + (x-1536) * 9.56930e-7;
  } else if (1792 <= x && x < 2048) {
    ret = 0.00048977 + (x-1792) * 1.91441e-6;
  } else if (2048 <= x && x < 2304) {
    ret = 0.00097984 + (x-2048) * 3.82937e-6;
  } else if (2304 <= x && x < 2560) {
    ret = 0.0019601 + (x-2304) * 7.659055e-6;
  } else if (2560 <= x && x < 2816) {
    ret = 0.0039207 + (x-2560) * 1.525e-5;
  } else {
    ret = 0.0078421 + (exp((x-2816)/273.0) - 1) * 0.0092421;
  }
  return ret * 8.0;
}

float4 normalize_pv(int4 parsed, float vignette_factor) {
  // PWL
  float4 pv = {ox_lut_func(parsed.s0), ox_lut_func(parsed.s1), ox_lut_func(parsed.s2), ox_lut_func(parsed.s3)};
  return clamp(pv*vignette_factor, 0.0, 1.0);
}

float3 color_correct(float3 rgb) {
  float3 corrected = rgb.x * (float3)(1.5664815, -0.29808738, -0.03973474);
  corrected += rgb.y * (float3)(-0.48672447, 1.41914433, -0.40295248);
  corrected += rgb.z * (float3)(-0.07975703, -0.12105695, 1.44268722);
  return corrected;
}

float3 apply_gamma(float3 rgb, int expo_time) {
  return (10000 * rgb) / (1 + 9999 * rgb);
}

#endif
