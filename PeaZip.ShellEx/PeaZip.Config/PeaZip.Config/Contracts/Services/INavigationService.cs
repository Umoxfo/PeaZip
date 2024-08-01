using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Navigation;

namespace PeaZip.Config.Contracts.Services;

public interface INavigationService
{
    Frame Frame { get; set; }

    bool CanGoBack { get; }

    bool NavigateTo(Type pageType, object? parameter = null, NavigationTransitionInfo? transitionInfo = null, bool clearNavigation = false);
}
