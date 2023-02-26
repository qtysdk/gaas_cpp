#pragma once

#include <Python.h>
#include <string>

class TestContainer {
private:
    std::string host;
    PyObject *pStartMethod;
    PyObject *pMyContainerInstance;
    PyObject *pStopMethod;
    PyObject *containerHost;

    void startContainer() const;

    void stopContainer() const;

public:
    TestContainer();

    ~TestContainer();

    std::string getClient();

    std::string getHost();
};