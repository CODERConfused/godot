#ifndef LLM_MANAGER_H
#define LLM_MANAGER_H

#include "core/object/object.h"

class LLMProvider;

class LLMManager : public Object {
    GDCLASS(LLMManager, Object);

public:
    static LLMManager *get_singleton();

    LLMManager();
    ~LLMManager();

    // Request a completion from the active provider. The callback is a
    // MethodBind name on an Object (editor will use this to receive results).
    void request_completion(const String &context, Object *receiver, const String &callback_method);

private:
    static LLMManager *singleton;
    LLMProvider *provider;
};

#endif // LLM_MANAGER_H
