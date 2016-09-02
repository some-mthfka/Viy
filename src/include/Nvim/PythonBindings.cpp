#include <Nvim/PythonBindings.hpp>

#include <Nvim/Blinky.hpp> //for access to Blinky::Mode

#include <Nvim/ConversionUtilities.hpp>
#include <Nvim/Neovim.hpp>
#include <Nvim/DataHolder.hpp>

#include <thread> //for sleeping

#include <Python.h> //must be included last

/* Python bindings are provided here.
 * Two modules are exposed to Python: ui and messages_from_ui.
 *
 * As for the first module (ui), it ultimately provides API for nvim. 
 * The Python script redirects all calls to UI by using this API.
 *
 * messages_from_ui is used to communicate events from
 * UI to nvim. Such events include resizing, quitting
 * and input from user.
 *
 * The bindings could probably be templatized.
 *
 * Returning Py_None from functions somehow
 * caused an occasional
 * "Fatal Python error: deallocating None".
 * Returning a dummy PyLong eliminates the error
 * though. So, beware the Py_None! 
 */

//Initialized in runBridge and used by the bindings
nvim::BridgeData *bridgeData = nullptr;

static PyObject* ui_set_scroll_region(PyObject *self, PyObject *args)
{
    int32_t top, bottom, left, right;

    if(!PyArg_ParseTuple(args, "iiii", &top, &bottom, &left, &right))
        return NULL;

    bridgeData -> dataHolder -> setScrollRegion(top, bottom, left, right);

    return PyLong_FromLong(73);
}

static PyObject* ui_change_mode(PyObject *self, PyObject *args)
{
    int32_t mode;

    if(!PyArg_ParseTuple(args, "i", &mode))
        return NULL;

    if (mode == 0)
        bridgeData -> neovim -> changeMode(nvim::Blinky::Mode::NORMAL);
    if (mode == 1)
        bridgeData -> neovim -> changeMode(nvim::Blinky::Mode::INSERT);

    return PyLong_FromLong(73);
}

static PyObject* ui_update_fg(PyObject *self, PyObject *args)
{
    uint32_t color;

    if(!PyArg_ParseTuple(args, "k", &color))
        return NULL;

    bridgeData -> dataHolder -> setPrimaryForegroundColor(toColor(color));
    //auto c = toColor(color);
    //std::cout << "Setting fg: " << int(c.r) << " " << int(c.g) << " " << int(c.b) << std::endl;

    return PyLong_FromLong(73);
}

static PyObject* ui_update_bg(PyObject *self, PyObject *args)
{
    uint32_t color;

    if(!PyArg_ParseTuple(args, "k", &color))
        return NULL;

    bridgeData -> dataHolder -> setPrimaryBackgroundColor(toColor(color));

    //auto c = toColor(color);
    //std::cout << "Setting bg: " << int(c.r) << " " << int(c.g) << " " << int(c.b) << std::endl;

    return PyLong_FromLong(73);
}


static PyObject* ui_scroll(PyObject *self, PyObject *args)
{
    int32_t count;

    if(!PyArg_ParseTuple(args, "i", &count))
        return NULL;

    bridgeData -> dataHolder -> scroll(count);

    return PyLong_FromLong(73);
}

static PyObject* ui_eol_clear(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":eolClear"))
        return NULL;

    bridgeData -> dataHolder -> eolClear();

    return PyLong_FromLong(73);
}

static PyObject* ui_quit(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":exit"))
        return NULL;

    bridgeData -> neovim -> quit();

    return PyLong_FromLong(73);
}


static PyObject* ui_clear(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":clear"))
        return NULL;

    bridgeData -> dataHolder -> clear();

    return PyLong_FromLong(73);
    //return Py_None;
}

static PyObject* ui_schedule_redraw(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":scheduleRedraw"))
        return NULL;

    bridgeData -> neovim -> scheduleRedraw(true);

    return PyLong_FromLong(73);
    //return Py_None;
}

static PyObject* ui_lock_update_mutex(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":lockUpdateMutex"))
        return NULL;

    bridgeData -> updateMutex.lock();

    return PyLong_FromLong(73);
    //return Py_None;
}

static PyObject* ui_unlock_update_mutex(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":unlock update mutex"))
        return NULL;

    bridgeData -> updateMutex.unlock();

    return PyLong_FromLong(73);
    //return Py_None;
}

static PyObject* ui_highlight_set(PyObject *self, PyObject *args)
{
    int32_t id;     //id of attribute
    uint32_t value; //either color or bool

    if(!PyArg_ParseTuple(args, "ik", &id, &value))
        return NULL;

    bridgeData -> dataHolder -> highlightSet(toCellAttribute(id, value));

    return PyLong_FromLong(73);
}

static PyObject* ui_resize(PyObject *self, PyObject *args)
{
    int32_t rows, cols;

    if(!PyArg_ParseTuple(args, "ii", &rows, &cols))
        return NULL;

    //Since resizing cannot happen while
    //there is a draw operation, we schedule
    //a resize and wait until the mutex
    //gets unclocked. It is locked by
    //ScheduleResize and unlocked by
    //nvim::Neovim::updateCurrent.
    //We also unlock updateMutex, so
    //that updateCurrent could execute and
    //then relock it
    bridgeData -> neovim -> scheduleResize(rows, cols);
    bridgeData -> updateMutex.unlock();
    bridgeData -> resizeMutex.lock();
    bridgeData -> resizeMutex.unlock();
    bridgeData -> updateMutex.lock();

    return PyLong_FromLong(73);
}

static PyObject* ui_cursor_goto(PyObject *self, PyObject *args)
{
    int32_t row, col;

    if(!PyArg_ParseTuple(args, "ii", &row, &col))
        return NULL;

    bridgeData -> dataHolder -> cursorGoto(row, col);

    return PyLong_FromLong(73);
    //return Py_None;
}

static PyObject* ui_set_title(PyObject *self, PyObject *args)
{
    const char *title;

    if (!PyArg_ParseTuple(args, "s", &title))
        return NULL;

    bridgeData -> neovim -> setTitle(title);

    return PyLong_FromLong(73);
}

static PyObject* ui_put(PyObject *self, PyObject *args)
{
    //std::cout << "UI_PUT CALLED" << std::endl;
    const char *text;

    if (!PyArg_ParseTuple(args, "s", &text))
        return NULL;

    bridgeData -> dataHolder -> put(UTF8ToUTF32(text, strlen(text)));

    return PyLong_FromLong(73);
}

static PyObject* ui_switch_to_navigator(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":ui_switch_to_navigator"))
        return NULL;

    bridgeData -> switchToNavigatorFlag = true;

    return PyLong_FromLong(73);
}

static PyMethodDef UIMethods[] = {
    //...
    {"schedule_redraw", ui_schedule_redraw, METH_VARARGS,
     "Scedule redraw."},
    {"eol_clear", ui_eol_clear, METH_VARARGS,
     "Clear till the end of line."},
    {"clear", ui_clear, METH_VARARGS,
     "Clear current scroll region."},
    {"lock_update_mutex", ui_lock_update_mutex, METH_VARARGS,
     "Lock update mutex."},
    {"unlock_update_mutex", ui_unlock_update_mutex, METH_VARARGS,
     "Unlock update mutex."},
    {"put",  ui_put, METH_VARARGS,
     "Put string."},
    {"cursor_goto",  ui_cursor_goto, METH_VARARGS,
     "Cursor goto row and column."},
    {"set_scroll_region",  ui_set_scroll_region, METH_VARARGS,
     "Set scroll region."},
    {"scroll",  ui_scroll, METH_VARARGS,
     "Scroll by a number of lines."},
    {"change_mode",  ui_change_mode, METH_VARARGS,
     "Changes mode to normal or insert."},
    {"set_title",  ui_set_title, METH_VARARGS,
     "Sets the title."},
    {"update_fg",  ui_update_fg, METH_VARARGS,
     "Set primary foreground color."},
    {"update_bg",  ui_update_bg, METH_VARARGS,
     "Set primary background color."},
    {"highlight_set",  ui_highlight_set, METH_VARARGS,
     "Set current printing attributes."},
    {"resize",  ui_resize, METH_VARARGS,
     "Resize window to the given dimensions meausered in cells."},
    {"switch_to_navigator",  ui_switch_to_navigator, METH_VARARGS,
     "Switch to navigator."},
    {"quit",  ui_quit, METH_VARARGS,
     "Quit the program."},
    //...
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyObject* ui_messages_check_for_user_input(PyObject *self, PyObject *args)
{
    bridgeData -> nvimInputMutex.lock();
    auto send = (bridgeData -> userInput).toUtf8();
    (bridgeData -> userInput).clear();
    bridgeData -> nvimInputMutex.unlock();

    //WARNING
    //ACHTUNG
    //REINTERPRETO CASTO BULLSHITO DELETO
    return PyUnicode_FromString(reinterpret_cast<const char *>(send.c_str())); 
}

static PyObject* ui_messages_check_for_resize(PyObject *self, PyObject *args)
{
    int32_t rows = bridgeData -> newDimensionsRows,
            cols = bridgeData -> newDimensionsCols;

    PyObject* dimensions = Py_BuildValue("ii", cols, rows);

    bridgeData -> newDimensionsRows = -1;
    bridgeData -> newDimensionsCols = -1;

    //negative dimensions will be ignored by Python
    return dimensions; 
}

static PyObject* ui_messages_get_command_line_argument(PyObject *self, PyObject *args)
{
    //no mutexes needed, since command_line_argument is set only ince
    //before interpreter is run
    
    PyObject* arg = Py_BuildValue("s", bridgeData -> commandLineArgument.c_str());

    return arg; 
}

static PyObject* ui_messages_check_for_quit(PyObject *self, PyObject *args)
{
    auto result = PyLong_FromLong(0); //not quit

    if (bridgeData -> quitFlag) 
    {
        bridgeData -> quitFlag = false;
        result = PyLong_FromLong(1); //quit
    }

    return result;
}

static PyMethodDef MessagesFromUIMethods[] = {
    {"check_for_user_input",  ui_messages_check_for_user_input, METH_VARARGS,
     "Return utf-8 string containing input (can be empty)."},
    {"check_for_resize",  ui_messages_check_for_resize, METH_VARARGS,
     "Return dimensions. If not changed, return (-1, -1)"},
    {"check_for_quit",  ui_messages_check_for_quit, METH_VARARGS,
     "Return flag indicating a request to quit."},
    {"get_command_line_argument",  ui_messages_get_command_line_argument, METH_VARARGS,
     "Return flag indicating a request to quit."},
    //...
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef UIModule = {
   PyModuleDef_HEAD_INIT,
   "ui",   /* name of module */
   NULL,     /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   UIMethods
};

static PyObject* PyInit_UI(void)
{
    return PyModule_Create(&UIModule);
}

static struct PyModuleDef MessagesFromUIModule = {
   PyModuleDef_HEAD_INIT,
   "messages_from_ui",   /* name of module */
   NULL,     /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   MessagesFromUIMethods
};

static PyObject* PyInit_MessagesFromUI(void)
{
    return PyModule_Create(&MessagesFromUIModule);
}

void nvim::runBridge(BridgeData &data)
{
    bridgeData = &data;

    struct Interpreter
    {
        const char *scriptName = "launcher";
        PyObject *pName, *pModule;
        PyGILState_STATE gstate;

        Interpreter()
        {
            //register module
            PyImport_AppendInittab("ui", &PyInit_UI);
            PyImport_AppendInittab("messages_from_ui", &PyInit_MessagesFromUI);

            Py_Initialize();

            if (! PyEval_ThreadsInitialized())
                PyEval_InitThreads();

            gstate = PyGILState_Ensure();


            pName = PyUnicode_FromString(scriptName);
            pModule = PyImport_Import(pName);
         
            assert(pModule);
        }

        ~Interpreter()
        {
            Py_DECREF(pName);
            Py_DECREF(pModule);

            PyGILState_Release(gstate);

            Py_Finalize();
        }
    };

    Interpreter interpreter;

    Py_BEGIN_ALLOW_THREADS

    while(true)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Py_END_ALLOW_THREADS
}
