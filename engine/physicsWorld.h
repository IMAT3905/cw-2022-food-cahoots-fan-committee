/* \file physics.h */
#pragma once

#include <systems/system.h>
#include <reactphysics3d/reactphysics3d.h>

namespace Engine
{
	class PhysicsSystem : public System
	{
	public:
		void start(SystemSignal init = SystemSignal::None, ...) override
		{
			m_world = m_rp3dCommon.createPhysicsWorld();
		}

		void stop(SystemSignal close = SystemSignal::None, ...) override
		{
			m_rp3dCommon.destroyPhysicsWorld(m_world);
		}

		rp3d::PhysicsWorld* m_world; //!< World
	private:
		rp3d::PhysicsCommon m_rp3dCommon;
	};
}