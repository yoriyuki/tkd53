#include <sstream>

#include "engine.hpp"

namespace lime {
namespace engine {

Engine::Engine(shared_ptr<ConverterInterface> converter)
  : converter_(converter) {
}

void Engine::Convert(const KkciString &input, TokenString *output) {
  converter_->Convert(input, output);
}

} // converter
} // lime
