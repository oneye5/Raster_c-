#include "pch.h"
#include "gameMethods.h"
#if __has_include("gameMethods.g.cpp")
#include "gameMethods.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::Rasterc::implementation
{
    int32_t gameMethods::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void gameMethods::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void gameMethods::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
