#ifndef EXCLUDE_SCRIPTING

#include "scripting/samplers-module.h"
#include "samplers/samplers.h"
#include "scripting/scripting-util.h"

using namespace chaiscript;
using namespace raytracer;
using namespace math;


namespace
{
    struct SamplerLibrary
    {
        Sampler single() const
        {
            return samplers::single();
        }
        Sampler random(const int& sample_count) const
        {
            return samplers::random(sample_count);
        }
        Sampler stratified(const int& n, const int& m) const
        {
            return samplers::stratified(n, m);
        }
        Sampler jittered(const int& n, const int& m) const
        {
            return samplers::jittered(n, m);
        }
        Sampler halfjittered(const int& n, const int& m) const
        {
            return samplers::halfjittered(n, m);
        }
        Sampler nrooks(const int& n) const
        {
            return samplers::nrooks(n);
        }
        Sampler multijittered(const int& n) const
        {
            return samplers::multijittered(n);
        }
    };
}

ModulePtr raytracer::scripting::_private_::create_samplers_module()
{
    auto module = std::make_shared<chaiscript::Module>();

    util::register_type<Sampler>(*module, "Sampler");

    auto sampler_library = std::make_shared<SamplerLibrary>();
    module->add_global_const(chaiscript::const_var(sampler_library), "Samplers");

#   define BIND(NAME)  module->add(fun(&SamplerLibrary::NAME), #NAME)
    BIND(single);
    BIND(random);
    BIND(stratified);
    BIND(jittered);
    BIND(halfjittered);
    BIND(nrooks);
    BIND(multijittered);
#   undef BIND

    return module;
}

#endif
