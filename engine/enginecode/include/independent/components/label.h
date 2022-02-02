/* \file label.h */
#pragma once

namespace Engine
{
	class LabelComponent
	{
	public:
		LabelComponent(const char* pLabel) : label{ pLabel } {}
		const char* label;
	};
}