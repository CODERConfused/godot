#include "llm_manager.h"
#include "providers/mock_provider.h"

LLMManager *LLMManager::singleton = nullptr;

LLMManager::LLMManager() {
    singleton = this;
    provider = memnew(LLM::MockProvider());
}

LLMManager::~LLMManager() {
    memdelete(provider);
    singleton = nullptr;
}

LLMManager *LLMManager::get_singleton() {
    return singleton;
}

void LLMManager::request_completion(const String &context, Object *receiver, const String &callback_method) {
    ERR_FAIL_COND(!provider);
    // For now the mock provider will synchronously return a result via the receiver's method.
    String result = provider->get_mock_completion(context);
    if (receiver && callback_method != String()) {
        const Variant arg = result;
        receiver->call(callback_method, arg);
    }
}
