#include <Python.h>
#include <iostream>
//#include <time.h>
#include <ctime>
#include "udp.h"

using std::string;
using std::endl;
using std::cout;



//Arch Example Library Extension in C++ for Python 2.7

PyObject* version(PyObject* self, PyObject* args)
{
    return PyUnicode_FromString("Arch Library Version 1.0 by Filip (VA0)");
}

PyObject* udp_send(PyObject* self, PyObject* args)
{
    char* IP;
    char* msg;
    unsigned int PORT;


    //Parsing 3 arguments from Python
    if (!PyArg_ParseTuple(args, "sIs", &IP, &PORT, &msg)) //str uint str
    {
        return NULL;
    }

    if (strlen(IP) < 7) {
        PyErr_SetString(PyExc_ValueError, "IP musi miec przynajmniej 7 znakow!");
        return NULL;
    }
    
    if (strlen(msg) >= 1000) {
        PyErr_SetString(PyExc_ValueError, "Twoja komenda nie moze miec wiecej niz 1000 znakow!");
        return NULL;
    }

    //Launching our UDP Client and sending message
    char* output = _send(IP, PORT, msg);
    if(!output)
    {
        PyErr_SetString(PyExc_Exception, "Couldnt connect to UDP Socket at specified IP and PORT!\nCheck if udp server is up and make sure ports are open!");
        return NULL; //todo: add exception in python
    }
    
    //Getting output from server
    PyObject* data = PyUnicode_FromString(output);

    //Calling Python Methods in C++ (using this for demonstration)
    //PyObject *result = PyObject_CallMethod(data, "lower", NULL); //everything with lower characters
    PyObject *result = PyObject_CallMethod(data, "decode", "ss", "utf-8", "errors='ignore'");
    //PyObject *result = PyObject_CallMethod(data, "center", "i", 2424);  //data.center(2420) and prints output with 2420 spaces at the beggining


    //return PyUnicode_FromString(output);
    //return PyUnicode_DecodeUTF8(output, strlen(output)+1, NULL);
    return result;
}


PyObject* datetime(PyObject* self, PyObject* args)
{

     char *timeformat;

     if (!PyArg_ParseTuple(args, "s", &timeformat))
     {
        time_t time = std::time(NULL);
        tm * tm;
        char* strTime = ctime(&time);
        if (!strTime) return NULL;

        return PyUnicode_FromString(strTime);   
     }
       


    time_t time = std::time(NULL);
    tm tm;
    localtime_s(&tm, &time);
    char data[100];
    bool result = strftime(data, sizeof(data), timeformat, &tm);
    //if (!result) return NULL;

    return PyUnicode_FromString(data);

}



PyMethodDef Methods[] = 
{
    {"version", version, METH_NOARGS, "Check version build."},
    {"udp_send", udp_send, METH_VARARGS, "Send message to UDP Socket at specified IP and PORT."},
    {"time", datetime, METH_VARARGS, "Prints C++ strftime function result based on time format string or simplified ctime output."},
    {NULL, NULL, 0}
};

PyMODINIT_FUNC initarch(void)
{
    (void)Py_InitModule("arch", Methods);
}
