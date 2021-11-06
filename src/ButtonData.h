#ifndef PENDULUM_BUTTONDATA_H
#define PENDULUM_BUTTONDATA_H

#include "Hotkey.h"
#include "Slider.h"
#include <string>

template<typename ButtonId>
struct ButtonData
{
    ButtonId id;
    std::string name;
    std::optional<Hotkey> hotkey;

    ButtonData(ButtonId id, const std::string& name)
        : id(id)
        , name(name)
    {
    }

    ButtonData(
        ButtonId id,
        const std::string& name,
        const std::optional<Hotkey>& hotkey)
        : id(id)
        , name(name)
        , hotkey(hotkey)
    {
    }
};

template<typename SliderId>
struct SliderData
{
    SliderId id;
    std::string label;
    Slider::Range range;
    double initialValue;

    SliderData(
        SliderId id,
        const std::string& label,
        const Slider::Range& range,
        double initialValue)
        : id(id)
        , label(label)
        , range(range)
        , initialValue(initialValue)
    {
    }

    SliderData(
        SliderId id,
        const std::string& label,
        const Slider::Range& range,
        int initialValue)
        : id(id)
        , label(label)
        , range(range)
        , initialValue(static_cast<double>(initialValue))
    {
    }
};

#endif // PENDULUM_BUTTONDATA_H
