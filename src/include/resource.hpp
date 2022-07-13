#ifndef _FSPEC_RESOURCE_HPP
#define _FSPEC_RESOURCE_HPP

namespace fspec {
    class Resource {
        public:
            /**
             * @brief Destroy/cleanup this resource. Should be called in the owning class' destroy function.
             * 
             */
            inline virtual void destroy() {};
    };
}

#endif