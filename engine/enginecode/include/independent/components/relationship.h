/* \file relationship.h */
#pragma once

#include <entt/entt.hpp>
#include "core/application.h"

namespace Engine
{
	class RootComponent
	{
	public:
	};

	class RelationshipComponent
	{
	public:
		std::size_t children = 0;
		entt::entity first = entt::null;
		entt::entity next = entt::null;
		entt::entity parent = entt::null;
	};

	namespace HierarchySystem
	{
		void setChild(entt::entity parentEntity, entt::entity childEntity);
		void updateChildren();
	}
}