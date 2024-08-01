using Microsoft.UI.Xaml.Controls;

namespace PeaZip.Config.Contracts.Views;

public interface INavigation
{
    List<NavigationViewItem> GetNavigationViewItems();

    List<NavigationViewItem> GetNavigationViewItems(Type type);

    List<NavigationViewItem> GetNavigationViewItems(Type type, string title);

    NavigationViewItem GetCurrentNavigationViewItem();

    void SetCurrentNavigationViewItem(NavigationViewItem item);
}
