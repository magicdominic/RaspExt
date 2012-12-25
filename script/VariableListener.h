#ifndef VARIABLELISTENER_H
#define VARIABLELISTENER_H

class Variable;

class VariableListener
{
public:
    /**
     * @brief onVariableChanged gets called from the variable object, if the variable has changed
     * @param var the variable object which generated the event
     */
    virtual void onVariableChanged(Variable* var) = 0;
};

#endif // VARIABLELISTENER_H
