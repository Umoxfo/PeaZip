using Microsoft.UI.Xaml.Controls;

namespace PeaZip.Config.Contracts.Services;

public interface INavigationViewService
{
    IList<object>? MenuItems { get; }

    void Initialize(NavigationView navigationView);

    void UnregisterEvents();

    NavigationViewItem? GetSelectedItem(Type pageType);
}
