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
			score += 1;
			scene.scores[id] = score;
			Log::info(scene.scores[id]);
		}
	}
}

