#pragma once

#include "../component.h"


namespace Kit
{

    class Components
    {
    private:
        vector<Component*>* components;

    public:
        explicit Components(vector<Component*>* components);

        ~Components();

    public:

        /**
         * @brief Sets the passed handler for each collection component
         */
        void addEventListener(string action, eventCallback event_callback);

        /**
         * @brief Performs the passed callback for each component in the collection,
         * passing the current first parameter to the callback.
         */
        void each(eachCallback each_callback);

        /**
         * @brief Returns the component by its serial number in the collection.
         */
        Component* at(size_t index);
        Component* operator[](size_t index);

        /**
         * @brief Returns the size of the collection.
         */
        size_t size();
    };

}