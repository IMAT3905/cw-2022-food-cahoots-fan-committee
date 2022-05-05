#include <immintrin.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using vec4 = std::array<float, 4>;

vec4 add(vec4 a, vec4 b)
{
	vec4 result;
#ifdef __AVX2__
	__m128 SIMDa = _mm_load_ps(a.data());
	__m128 SIMDb = _mm_load_ps(b.data());
	__m128 SIMDresult = _mm_add_ps(SIMDa, SIMDb);

	_mm_store_ps(result.data(), SIMDresult);
#else
	result[0] = a[0] + b[0];
	result[1] = a[1] + b[1];
	result[2] = a[2] + b[2];
	result[3] = a[3] + b[3];
#endif

	return result;
}


uint32_t pack(const glm::vec4& tint)
{
	uint32_t result = 0;

	uint32_t r = (static_cast<uint32_t>(tint.r * 255.f)) << 0;
	uint32_t g = (static_cast<uint32_t>(tint.g * 255.f)) << 8;
	uint32_t b = (static_cast<uint32_t>(tint.b * 255.f)) << 16;
	uint32_t a = (static_cast<uint32_t>(tint.a * 255.f)) << 24;

	result = (r | g | b | a);
	return result;
}

uint32_t SIMDpack(const glm::vec4& tint)
{
	uint32_t result = 0;

#ifdef __AVX2__
	__m128 SIMDtint = _mm_load_ps(glm::value_ptr(tint));
	__m128 SIMDscalar = _mm_set_ps1(255.f);
	__m128 SIMDresult = _mm_mul_ps(SIMDtint, SIMDscalar);
	__m128i SIMDints = _mm_cvtps_epi32(SIMDresult);
	__m128i SIMDshift = _mm_setr_epi32(0, 8, 16, 24);
	__m128i SIMDshifted = _mm_sllv_epi32(SIMDints, SIMDshift);

	uint32_t rgba[4];
	_mm_store_si128((__m128i*)rgba, SIMDshifted);

	result = (rgba[0] | rgba[1] | rgba[2] | rgba[3]);
#else
	uint32_t result = 0;

	uint32_t r = (static_cast<uint32_t>(tint.r * 255.f)) << 0;
	uint32_t g = (static_cast<uint32_t>(tint.g * 255.f)) << 8;
	uint32_t b = (static_cast<uint32_t>(tint.b * 255.f)) << 16;
	uint32_t a = (static_cast<uint32_t>(tint.a * 255.f)) << 24;

	result = (r | g | b | a);
#endif

	return result;
}

int main()
{
	vec4 a = { 2, 4, 6, 8 };
	vec4 b = { 1, 2, 3, 4 };

	vec4 result = add(a, b);

	glm::vec4 tint(0.7, 0.6, 0.3, 1.0);
	auto packed = pack(tint);
	auto SIMDpacked = SIMDpack(tint);
	return 0;
}