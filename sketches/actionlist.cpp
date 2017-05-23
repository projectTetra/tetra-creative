#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;

class ProcessList;

class Process
{
public:
    ProcessList* engine;
    bool finished = false;
    std::unique_ptr<Process> next = nullptr;

    Process() = default;
    Process(const Process&) = delete;
    Process(Process&& from) = default;

    virtual void run() = 0;

    Process& andThen(std::unique_ptr<Process>&& next);

    template <class T, class... Args>
    T& andThen(Args&&... args)
    {
        return (T&)andThen(std::unique_ptr<Process>{new T{std::forward<Args>(args)...}});
    }
};

class ProcessList
{
public:
    Process& add(std::unique_ptr<Process>&& process);

    template <class T, class... Args>
    T& add(Args&&... args)
    {
        return (T&)add(std::unique_ptr<Process>{new T{std::forward<Args>(args)...}});
    }

    bool running() const;

    void run();
private:
    std::vector<std::unique_ptr<Process>> processes;
};

class EchoProc : public Process
{
public:
    EchoProc(string msg)
        : msg{msg}
    { }

    virtual void run() override
    {
        cout << msg << endl;
        this->finished = true;
    }
private:
    string msg;
};

int main()
{
    ProcessList engine{};

    engine
        .add<EchoProc>("hello")
        .andThen<EchoProc>("world")
        .andThen<EchoProc>("and another thing!");

    engine
        .add<EchoProc>("number")
        .andThen<EchoProc>("two");
        //.andThen<EchoProc>(engine, "turbo");


    while (engine.running())
    {
        engine.run();
    }

    return 0;
}

void
ProcessList::run()
{
    for (const auto& process : processes)
    {
        process->run();
    }

    // gather completed processes
    auto done = remove_if(begin(processes), end(processes), [](const auto& proc) { return proc->finished; });

    // grab all of the processes with next ptrs
    auto nextProcesses = vector<unique_ptr<Process>>{};
    for_each(done, end(processes), [&nextProcesses](const unique_ptr<Process>& proc)
    {
        if(proc->next)
        {
            nextProcesses.emplace_back(proc->next.release());
        }
    });

    // remove completed processes
    processes.erase(done, end(processes));

    // push the next processes
    for (auto& proc : nextProcesses)
    {
        this->add(move(proc));
    }
}

bool
ProcessList::running() const
{
    return !processes.empty();
}

Process&
ProcessList::add(unique_ptr<Process>&& process)
{
    processes.emplace_back(move(process));
    processes.back()->engine = this;
    return *processes.back();
}

Process&
Process::andThen(unique_ptr<Process>&& next)
{
    this->next = move(next);
    return *(this->next);
}

