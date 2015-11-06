#include <boost/python.hpp>
#include <boost/noncopyable.hpp>

class pyinit : boost::noncopyable
{
public:
    pyinit(int sigs = 1) {
        assert(sigs == 0 || sigs == 1);
        Py_InitializeEx(sigs);
    }

    ~pyinit() {}

    bool isInitialized() {
        return Py_IsInitialized();
    }

    const char * version() {
        return Py_GetVersion();
    }
};

