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
concept Module = Evalable<T> && Tracable<T>;

template <typename T>
concept ClockableModule = Module<T> && Clockable<T>;

template<ClockableModule T>
class Wrap
{
private:
    VerilatedVcdC m_trace;
    int m_tick_count = 0;
public:
    T *vmodule;
    Wrap(const char* trace_name, T *m) : vmodule(m)
    {
        vmodule->trace(&m_trace, 99, 0);
        m_trace.open(trace_name);
    }
    ~Wrap()
    {  
        m_trace.close();
    }

    void tick() {
        vmodule->clk = 0;
        vmodule->eval();
        m_trace.dump(m_tick_count++);
        vmodule->clk = 1;
        vmodule->eval();
        m_trace.dump(m_tick_count++);
    }
};
#endif