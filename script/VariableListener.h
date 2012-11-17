#ifndef VARIABLELISTENER_H
#define VARIABLELISTENER_H

class Variable;

class VariableListener
{
public:
    virtual void onVariableChanged(Variable* var) = 0; // gets called, if a variable has changed its value
};

#endif // VARIABLELISTENER_H
