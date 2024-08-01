using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace PeaZip.Config.Helpers;

// Helper class to set the navigation target for a NavigationViewItem.
//
// Usage in XAML:
// <NavigationViewItem x:Uid="Shell_Main" Icon="Document" helpers:NavigationHelper.NavigateTo="AppName.ViewModels.MainViewModel" />
//
// Usage in code:
// navigationViewItem.SetNavigateTo(typeof(MainViewModel).FullName);
public static class NavigationHelper
{
    public static readonly DependencyProperty NavigateToProperty =
        DependencyProperty.RegisterAttached("NavigateTo", typeof(string), typeof(NavigationHelper), new PropertyMetadata(null));

    public static string GetNavigateTo(this NavigationViewItem item) => (string)item.GetValue(NavigateToProperty);

    public static void SetNavigateTo(this NavigationViewItem item, string value) => item.SetValue(NavigateToProperty, value);
}
