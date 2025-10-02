#include "core/object/class_db.h"
#include "llm_manager.h"

void register_llm_types() {
    CLASS_DB_REGISTER_CLASS(LLMManager);
}

void unregister_llm_types() {
}
