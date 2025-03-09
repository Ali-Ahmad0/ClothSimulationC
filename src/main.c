#include "engine/engine.h"

int main() {
	Engine engine = EngineCreate();
	EngineUpdate(&engine);

	return 0;
}
