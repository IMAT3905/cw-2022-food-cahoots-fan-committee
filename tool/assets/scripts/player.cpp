#include "player.h"
#include "include/independent/systems/log.h"

namespace Engine {
	void PlayerScript::onCreate() {

	}

	void PlayerScript::onDestroy() {

	}

	void PlayerScript::onUpdate(float timestep) {

	}

	void PlayerScript::onKeyPress(KeyPressedEvent& e) {
		if (e.getKeyCode() == key) {
			scene.numselected[id] = 1;
		}
	}
}

