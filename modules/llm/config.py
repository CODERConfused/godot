def can_build(env, platform):
    # Module can always be built. Keep dependencies minimal for the prototype.
    return True


def configure(env):
    # Nothing special to configure for now. Future providers can add opts.
    pass


def get_doc_classes():
    return ["LLMManager"]


def get_doc_path():
    return "doc_classes"
