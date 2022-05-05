#include "engine_pch.h"
#include "rendering/rendererCommon.h"

namespace Engine
{
	std::shared_ptr<Texture> RendererCommon::defaultTexture = nullptr;
	SubTexture RendererCommon::defaultSubTexture;
	glm::vec4 RendererCommon::defaultTint = glm::vec4(1.0f);
	std::array<int32_t, 32> RendererCommon::textureUnits = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,26, 27, 28, 29, 30, 31 };


	uint32_t RendererCommon::pack(const glm::vec4& tint)
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
		uint32_t r = (static_cast<uint32_t>(tint.r * 255.f)) << 0;
		uint32_t g = (static_cast<uint32_t>(tint.g * 255.f)) << 8;
		uint32_t b = (static_cast<uint32_t>(tint.b * 255.f)) << 16;
		uint32_t a = (static_cast<uint32_t>(tint.a * 255.f)) << 24;

		result = (r | g | b | a);
#endif

		return result;
	} //!< function to pack tint data \param tint the data that will be packed
}
