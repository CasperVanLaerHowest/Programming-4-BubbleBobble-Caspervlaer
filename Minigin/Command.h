#pragma once
class Command
{
public:
    virtual void Execute(bool notfirstExecute) = 0;
	virtual ~Command() {};
};
