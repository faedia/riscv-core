#include <verilated_vcd_c.h>

#ifndef H_WRAP
#define H_WRAP

template<typename T>
concept Tracable = requires (T t, VerilatedVcdC trace) {
    { t.trace(&trace, 0, 0) };
};

template <typename T>
concept Evalable = requires (T t) {
    {t.eval()};
};

template <typename T>
concept Clockable = requires (T t) {
    {t.clk = 0};
};

template <typename T>
concept SynthModule = Evalable<T> && Tracable<T>; 

template <typename T>
concept ClockableModule = SynthModule<T> && Clockable<T>;

template<SynthModule T>
class UnclockedModule
{
protected:
    VerilatedVcdC m_trace;
    int m_tick_count = 0;
public:
    T m;
    UnclockedModule(const char* trace_name)
    {
        m.trace(&m_trace, 99, 0);
        m_trace.open(trace_name);
    }
    ~UnclockedModule()
    {  
        m_trace.close();
    }
    virtual void tick()
    {
        m.eval();
        m_trace.dump(m_tick_count++);
    }
};

template<ClockableModule T>
class ClockedModule : public UnclockedModule<T>
{
public:
    ClockedModule(const char* trace_name) : UnclockedModule<T>(trace_name) {}

    virtual void tick() override
    {
        this->m.clk = 0;
        this->m.eval();
        this->m_trace.dump(this->m_tick_count++);
        this->m.clk = 1;
        this->m.eval();
        this->m_trace.dump(this->m_tick_count++);
    }
};
#endif