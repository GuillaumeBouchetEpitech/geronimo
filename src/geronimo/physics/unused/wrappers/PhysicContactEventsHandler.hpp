
#include "../PhysicDefinitions.hpp"

#include <functional>

using ContactCallback = std::function<void(ContactEvent, PhysicContactData*)>;

namespace physicsContactEventsHandler {
void initialise(ContactCallback callback);
};
