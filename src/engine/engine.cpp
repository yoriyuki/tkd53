#include <sstream>

#include "engine.hpp"

namespace lime {
namespace engine {

Engine::Engine(shared_ptr<AbstractConverter> converter)
  : converter_(converter) {
}

void Engine::Convert(const KkciString &input, Segments *output) {
  converter_->Convert(input, kBOS, kBOS, output);
}

} // converter
} // lime
