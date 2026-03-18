#pragma once
#include "Observer.h"
#include "GameObject.h"
#include <vector>
#include <algorithm>

namespace dae
{
	 class Subject
    {
    public:
        void AddObserver(Observer* observer)
        {
            observers_.push_back(observer);
        }
        void RemoveObserver(Observer* observer)
        {
            std::erase(observers_, observer);
        }
    protected:
        void Notify(GameObject& entity, Event event)
        {
            for (auto* observer : observers_)
            {
                observer->OnNotify(entity, event);
            }
        }
    private:
        std::vector<Observer*> observers_;
    };
}