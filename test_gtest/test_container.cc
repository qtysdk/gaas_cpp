#include "test_container.h"
#include <Python.h>
#include <string>

PyObject *createMyContainer() {
    const char *pythonScript = R"(
from testcontainers.mongodb import MongoDbContainer

class MyContainer:
    def __init__(self):
        self.mongo = MongoDbContainer('mongo:latest')
        self.db = None
        self.host = None

    def start(self):
        self.mongo.start()
        self.db = self.mongo.get_connection_client().test
        self.host = self.db.command('whatsmyuri').get('you')

    def stop(self):
        self.mongo.stop()
    )";
    PyRun_SimpleString(pythonScript);
    PyObject * pMyContainerClass = PyObject_GetAttrString(PyImport_AddModule("__main__"), "MyContainer");
    return PyObject_CallObject(pMyContainerClass, NULL);
}


TestContainer::TestContainer() {
    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    // Set up the Python environment
    PyRun_SimpleString("import sys\n"
                       "sys.path.append('.')\n");  // Add current directory to Python path

    // Create an instance of MyContainer and call its start() method
    pMyContainerInstance = createMyContainer();
    containerHost = PyObject_GetAttrString(pMyContainerInstance, "host");
    pStartMethod = PyObject_GetAttrString(pMyContainerInstance, "start");

    // Call the stop method on the MyContainer instance
    pStopMethod = PyObject_GetAttrString(pMyContainerInstance, "stop");
    startContainer();
}

std::string TestContainer::getClient() {
    PyObject * pResult = PyObject_GetAttrString(pMyContainerInstance, "host");
    const char *host = PyUnicode_AsUTF8(pResult);
    std::string result = host ? host : "";
    Py_DECREF(pResult);
    return result;
}

void TestContainer::stopContainer() const { PyObject_CallObject(pStopMethod, NULL); }

void TestContainer::startContainer() const { PyObject_CallObject(this->pStartMethod, NULL); }

TestContainer::~TestContainer() {
    stopContainer();

    // Clean up
    Py_DECREF(containerHost);
    Py_DECREF(pStopMethod);
    Py_DECREF(pStartMethod);
    Py_DECREF(pMyContainerInstance);

    // Note: never call it, it is hard to unload it properly
    // Py_Finalize();
}

