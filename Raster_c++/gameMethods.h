#pragma once

#include "gameMethods.g.h"

namespace winrt::Rasterc::implementation
{
    struct gameMethods : gameMethodsT<gameMethods>
    {
        gameMethods() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::Rasterc::factory_implementation
{
    struct gameMethods : gameMethodsT<gameMethods, implementation::gameMethods>
    {
    };
}
