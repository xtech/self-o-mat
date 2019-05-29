#ifndef SELF_O_MAT_ILOGICCONTROLLER_H
#define SELF_O_MAT_ILOGICCONTROLLER_H

namespace selfomat {
    namespace logic {
        class ILogicController {
        public:
            virtual void trigger() = 0;
            virtual void acceptAgreement() = 0;
            virtual void cancelPrint() = 0;
            virtual void stop() = 0;
        };
    }
}

#endif