#pragma once

namespace moss
{
    class CpuArm;

    class Debugger
    {
        public:
            Debugger(CpuArm *cpu);

            virtual void on_break();

        protected:
            CpuArm *_cpu;
    };
}
