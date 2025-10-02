#ifndef MOCK_PROVIDER_H
#define MOCK_PROVIDER_H

#include "core/object/object.h"

namespace LLM {

class MockProvider {
public:
    MockProvider() {}
    ~MockProvider() {}

    String get_mock_completion(const String &context) {
        return String("// Mock completion based on context: ") + context.left(80);
    }
};

} // namespace LLM

#endif // MOCK_PROVIDER_H
