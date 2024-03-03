#ifndef EVALTABLE_H
#define EVALTABLE_H
#include <clcengine2/checker/CheckerMetaInfo.h>

namespace CLCEngine {


    class EvalTable
    {
        public:
            EvalTable();
            double getCoefficient(Coordinates place);
        private:
            //double
    };
}
#endif // EVALTABLE_H
